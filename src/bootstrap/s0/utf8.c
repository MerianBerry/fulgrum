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

int powi(int x, int y) {
  int o = x;
  for (int i = 0; i < y-1; ++i)
    o *= x;
  return o;
}

int utf8_charsize(u_char c) {
  if (c >= 0xf0 && c <= 0xf4)
    return 4;
  else if (c >= 0xe0)
    return 3;
  else if (c >= 0xc2)
    return 2;
  return 1;
}

long utf8_strlen(const char *str) {
  if (!str)
    return -1;
  long s = 0;
  for (long i = 0; i < STR_MAX; ++s) {
    if (!str[i])
      break;
    int us = utf8_charsize(((u_char*)str)[i]);
    i += us;
  }
  return s;
}

long utf8_actual(const char *str, long off) {
  if (!str)
    return -1;
  const long l1 = utf8_strlen(str);
  off = MIN(l1-1, off);
  long s = 0;
  for (long i = 0; i < off; ++i) {
    s += utf8_charsize(str[s]);
  }
  return s;
}

int utf8_encode(int utf) {
  int out = 0;
  // 4 byte
  if (utf > 0xffff) {
    u_char one = 30<<3 | utf>>18&0x7;
    u_char two = 2<<6 | utf>>12&0x3f;
    u_char tre = 2<<6 | utf>>6&0x3f;
    u_char qua = 2<<6 | utf&0x3f;
    out = one | two<<8 | tre<<16 | qua<<24;
  } // 3 byte
  else if (utf > 0x7ff) {
    u_char one = 14<<4 | utf>>12&0xf;
    u_char two = 2<<6 | utf>>6&0x3f;
    u_char tre = 2<<6 | utf&0x3f;
    out = one | two<<8 | tre << 16;
  } // 2 byte
  else if (utf > 0x7f) {
    u_char one = 6<<5 | utf>>6&0x1f;
    u_char two = 2<<6 | utf&0x3f;
    out = one | two << 8;
  } // 1 byte
  else {
    return utf;
  }
  return out;
}

int utf8_decode(int utf8) {
  int out = 0;
  // Is ASCII?
  if (!utf8>>7)
    return utf8;
  int c = utf8_charsize(*(u_char*)&utf8);
  for (int i = c-1; i >= 1; --i) {
    out |= (utf8>>i*8&0x3f)<<((i-1)*6);
  }
  out |= (utf8&(powi(2,8-c-1)-1)) << (c-1)*6;
  return out;
}

int utf8_swap(int utf8) {
  int out = (utf8&0xff)<<24 | (utf8>>8&0xff)<<16
    | (utf8>>16&0xff)<<8 | (utf8>>24&0xff);
}

int utf8_literal(int utf) {
  #if 1
  int out = 0;
  int _i = 0;
  for (int i = sizeof(int)-1; i >=0; --i) {
    u_char c = utf>>i*8&0xff;
    if (c) {
      out |= c<<_i*8;
      ++_i;
    }
  }
  return out;
  #else
  int o = utf8_swap(utf);
  while (!(o&0xff)) {
    o >>= 8;
  }
  return o;
  #endif
}

char *utf8_tostring(int utf8) {
  int l = utf8_charsize(*(u_char*)&utf8);
  char *str = malloc(l+1);
  memset(str, 0, l+1);
  memcpy(str, &utf8, l);
  return str;
}
