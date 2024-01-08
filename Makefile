
#===================USR SETTINGS===================#

override MAKEFLAGS=$(MFLAGS) --

PLAT= guess

#==================BUILD SETTINGS==================#

UNAME = uname
MKCLIFLAGS= "--no-print-directory"
PLATS= guess linux mingw win32
CFE= 
CFLIB= 

#==================TARGET SETTINGS=================#

target: $(PLAT)

guess:
	$(MAKE) $(MKCLIFLAGS) `$(UNAME)`

Linux linux:
	$(MAKE) $(MKCLIFLAGS) all PLAT=linux CFE="cfulgrum" CFLIB="libcfulgrum.a"

mingw:
	$(MAKE) $(MKCLIFLAGS) all PLAT=mingw CFE="cfulgrum.exe" CFLIB="cfulgrum.lib"

#win32:
#	$(MAKE) $(MKCLIFLAGS) all PLAT=win32 WHICH=where CC=cl

#=======================BUILD======================#

all: builddir cfulgrum
	
builddir:
	mkdir -p build

cfulgrum:
	$(MAKE) -C cfulgrum $(MKCLIFLAGS) PLAT="$(PLAT)" ALL=echo
	$(MAKE) -C cfulgrum $(MKCLIFLAGS) PLAT="$(PLAT)"
	mv cfulgrum/$(CFE) build/$(CFE) || echo 
	mv cfulgrum/$(CFLIB) build/$(CFLIB) || echo 

clean: 
	rm -rf build
	$(MAKE) -C cfulgrum $(MKCLIFLAGS) clean

.PHONY: cfulgrum