#include "include/common.h"

#if !defined(MAX)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#if !defined(MIN)
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
#if !defined(CLAMP)
#define CLAMP(x, y, z) MAX(y, MIN(z, x))
#endif

#pragma region "AVL"

int avl_height(avl_node_t *base)
{
	if (!base) // If base is NULL, return no height
		return 0;
	int count = 1;

	int left = 0;
	int right = 0;
	// Find left
	if (!!base->left)
	{
		left = avl_height(base->left);
	}
	// Find right
	if (!!base->right)
	{
		right = avl_height(base->right);
	}
	count += MAX(left, right);
	return count;
}

int avl_findbalance(avl_node_t *base)
{
	int balance = 0;

	if (!base)
		return 0;
	balance += avl_height(base->right);
	balance -= avl_height(base->left);

	return balance;
}

avl_node_t **avl_fside(avl_node_t *parent,
	avl_node_t *target)
{
	if (parent->left == target)
		return &parent->left;
	else if (parent->right == target)
		return &parent->right;
	return NULL;
}

void avl_leftrot(avl_node_t *X,
	avl_node_t *Z)
{
	avl_node_t *DC = Z->left;
	if (DC)
		DC->parent = X;
	X->right = DC;
	Z->parent = X->parent;
	Z->left = X;
	if (X->parent)
		*avl_fside(X->parent, X) = Z;
	X->parent = Z;
}

void avl_rightrot(avl_node_t *X,
	avl_node_t *Z)
{
	avl_node_t *DC = Z->right;
	if (DC)
		DC->parent = X;
	X->left = DC;
	Z->parent = X->parent;
	Z->right = X;
	if (X->parent)
		*avl_fside(X->parent, X) = Z;
	X->parent = Z;
}

void avl_balance(avl_tree_t *bintree,
	avl_node_t *base)
{
	avl_node_t *X = base->parent;
	char rootswitch = X == bintree->root;
	while(X) {
		rootswitch = X == bintree->root;
		int balance = avl_findbalance(X);
		if (balance <= -2) // Left child
		{
			avl_node_t *Z = X->left;
			int bal2 = avl_findbalance(Z);
			if (bal2 <= 0) // Right rotation
			{
				avl_rightrot(X, Z);
			} else // Left Right rotation
			{
				avl_node_t *Y = Z->right;
				avl_leftrot(Z, Y);
				avl_rightrot(X, Y);
			}
			break;
		} else if (balance >= 2)
		{
			avl_node_t *Z = X->right;
			int bal2 = avl_findbalance(Z);
			if (bal2 > 0) // Left rotation
			{
				avl_leftrot(X, Z);
			} else // Right Left rotation
			{
				avl_node_t *Y = Z->left;
				avl_rightrot(Z, Y);
				avl_leftrot(X, Y);
			}
			break;
		}
		if (X == X->parent)
			break;
		X = X->parent;
	}
	if (rootswitch && X)
		bintree->root = X->parent;
	if (!!bintree->root->parent)
		bintree->root = bintree->root->parent;
}

void avl_create(avl_tree_t *bintree,
	avl_node_t **target, avl_node_t *parent,
	const char * key, void * mem)
{
	*target = (avl_node_t *)malloc(sizeof(avl_node_t));
	memset(*target, 0, sizeof(avl_node_t));

	(*target)->mem = mem;

	int keysize = strlen(key)+1;
	(*target)->key = (char*)malloc(keysize);
	memset((*target)->key, 0, keysize);
	memcpy((*target)->key, key, keysize-1);

	(*target)->parent = parent;
	bintree->size += 1;
	avl_balance(bintree, *target);
}

avl_node_t *avl_find(avl_tree_t *bintree,
	const char * key)
{
	if (!bintree)
		return NULL;
	avl_node_t *itr = bintree->root;
	while(!!itr && !!itr->key) {
		int cmp = strcmp(itr->key, key);
		if (cmp == 0) {
			return itr;
		} else if (cmp < 0) {
			if (!!itr->left) {
				itr = itr->left;
				continue;
			}
		} else // cmp is > 0
		{
			if (!!itr->right) {
				itr = itr->right;
				continue;
			}
		}
		break;
	}
	return NULL;
}

void avl_append(avl_tree_t *bintree,
	const char * key, void * mem)
{
	if (!bintree->root) {
		avl_create(bintree, &bintree->root, NULL, key, mem);
		return;
	}
	avl_node_t *itr = bintree->root;
	int _itr = 0;
	while (!!itr) {
		if (_itr > 1000)
			return;
		_itr += 1;
		int cmp = strcmp(itr->key, key);
		if (cmp == 0) {
			itr->mem = mem;
			return;
		} else if (cmp < 0) {
			if (!!itr->left) {
				itr = itr->left;
				continue;
			}
			avl_create(bintree, &itr->left, itr, key, mem);
			return;
		} else // cmp is > 0
		{
			if (!!itr->right) {
				itr = itr->right;
				continue;
			}
			avl_create(bintree, &itr->right, itr, key, mem);
			return;
		}
	}
}

void avl_destroysubtree(avl_tree_t *tree,
	avl_node_t * root)
{
	if (!root)
		return;
	free(root->key);
	avl_destroysubtree(tree, root->left);
	avl_destroysubtree(tree, root->right);
	if (tree->size > 0)
		--tree->size;
	free(root);
}

void avl_relocate(avl_tree_t *tree, avl_node_t *root)
{
	if (!root)
		return;
	root->parent = NULL;
	avl_append(tree, root->key, root->mem);
	--tree->size;
	avl_relocate(tree, root->left);
	avl_relocate(tree, root->right);
	free(root->key);
	free(root);
}

void avl_destroynode(avl_tree_t *tree, avl_node_t *node)
{
	if (!node)
		return;
	avl_node_t **n = NULL;
	if (node->parent)
		n = avl_fside(node->parent, node);
	else
		tree->root = NULL;
	if (n)
		(*n) = NULL;
	avl_relocate(tree, node->left);
	avl_relocate(tree, node->right);
	free(node->key);
	free(node);
	--tree->size;
	if (!tree->size)
		tree->root = NULL;
}

void avl_clear(avl_tree_t *bintree)
{
	avl_destroysubtree(bintree, bintree->root);
	bintree->root = NULL;
	bintree->size = 0;
}

void avl_free(avl_tree_t **bintree)
{
	avl_destroysubtree(*bintree, (*bintree)->root);
	(*bintree)->root = NULL;
	(*bintree)->size = 0;
	free((*bintree));
	*bintree = NULL;
}

void avl_freeX(avl_tree_t *bintree)
{
	avl_destroysubtree(bintree, bintree->root);
	bintree->root = NULL;
	bintree->size = 0;
	free(bintree);
}

avl_tree_t *avl_newtree()
{
	avl_tree_t *ptr = (avl_tree_t*)malloc(sizeof(avl_tree_t));
	return ptr;
}

#pragma endregion "AVL"
