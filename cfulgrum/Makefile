
#===================USR SETTINGS===================#

override MAKEFLAGS=$(MFLAGS) --

PLAT= guess

#==================BUILD SETTINGS==================#

MKCLIFLAGS= "--no-print-directory"
UNAME = uname
WHICH= which
SOURCES= $(wildcard *.c) hydrogen/hydrogen.c
OBJECTS= $(patsubst %.c, %.o, $(SOURCES))
ALL= all
CFLIB= 
CFE= 

CC= 
CFLAGS= 
LIBS= 

$(if $(shell which clang-format), $(eval CFMT=clang-format), $(info clang-format isnt present and will not format code))

#==================TARGET SETTINGS=================#

target: $(PLAT)

guess:
	@echo Guessing `$(UNAME)`
	$(MAKE) $(MKCLIFLAGS) ALL=$(ALL) `$(UNAME)`

Linux linux:
	$(MAKE) $(MKCLIFLAGS) $(ALL) PLAT=linux CC=gcc CFLAGS="-g -std=c11" LIBS="-lm" CFLIB="libcfulgrum.a" CFE="cfulgrum"

mingw:
	$(MAKE) $(MKCLIFLAGS) $(ALL) PLAT=mingw CC="x86_64-w64-mingw32-gcc" CFLAGS="-g -std=c11" LIBS="-lm" CFLIB="cfulgrum.lib" CFE="cfulgrum.exe"

#win32:
#	$(MAKE) $(MKCLIFLAGS) all PLAT=win32 WHICH=where CC=cl

#=======================BUILD======================#

all: format $(OBJECTS) cfulgrum.o
	ar r $(CFLIB) $(OBJECTS)
	$(CC) $(LDFLAGS) cfulgrum.o -o $(CFE) -L . -lcfulgrum $(LIBS)

echo:
	@echo "PLAT         =" $(PLAT)
	@echo "UNAME        =" $(UNAME)
	@echo "WHICH        =" $(WHICH)
	@echo "MKCLIFLAGS   =" $(MKCLIFLAGS)
	@echo "SOURCES      =" $(SOURCES)
	@echo "OBJECTS      =" $(OBJECTS)
	@echo "ALL          =" $(ALL)
	@echo "CFLIB         =" $(CFLIB)
	@echo "CC           =" $(CC)
	@echo "CFLAGS       =" $(CFLAGS)
	@echo "LIBS         =" $(LIBS)

format: $(SOURCES)
	$(if $(shell $(WHICH) clang-format), $(shell clang-format -i -style=file $(SOURCES)))

clean:
	rm -f *.o *.a hydrogen/*.o

cfulgrum.o: cfulgrum.c include/fulgrum.h
fuerror.o: fuerror.c fuerror.h include/fulgrum.h
flex.o: flex.c flex.h include/fulgrum.h
fulgrum.o: fulgrum.c include/fulgrum.h
furn.o: furn.c furn.h include/fulgrum.h
ftc.o: ftc.c ftc.h
#fuseminfo.o: fuseminfo.c fuseminfo.h include/fulgrum.h
#futype.o: futype.c futype.h include/fulgrum.h
hydrogen/hydrogen.o: hydrogen/hydrogen.c hydrogen/hydrogen.h
#end of file