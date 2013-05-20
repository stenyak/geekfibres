UNAME := $(shell uname)
#Opciones OS-specific
ifeq ("$(OS)", "Windows_NT")

PLATFORM := WIN32
TYPE := CYGWIN
EXE_EXT := .exe
BASE_INCLUDE_DIR := /cygwin/usr/include
BASE_LIBRARY_DIR := /cygwin/usr/lib
LD_FLAGS:= -mno-cygwin -mwindows
LIBRARIES_WIN32 := mingw32
endif

ifeq ($(UNAME), Linux)
PLATFOM := LINUX
TYPE :=
EXE_EXT := 
BASE_INCLUDE_DIR := /usr/include
BASE_LIBRARY_DIR := /usr/lib
LD_FLAGS:= 
LIBRARIES_LINUX := .
endif


#Opciones del compilador
DEFINES := $(PLATFORM) $(TYPE)
TARGET := geekfibres$(EXE_EXT)
LD := g++
CC := gcc
CXX := g++
CFLAGS := -c -Wall -g -O3
CXXFLAGS := $(CFLAGS)

#Definicion de rutas a cabeceras y librerias
INCLUDE_DIRS := \
    . \
    src \
    $(BASE_INCLUDE_DIR)/SDL \
    $(BASE_INCLUDE_DIR) \

LIBRARIES := \
    SDL \
    SDLmain \
    SDL_mixer \
    SDL_ttf \
	SDL_image \
    $(LIBRARIES_$(PLATFORM))

SOURCES := \
	src/tools/leveldata.cpp \
    src/tools/normalrand.cpp \
    src/tools/config.cpp \
    src/world/tile.cpp \
    src/tools/sonido.cpp \
    src/actors/animacion.cpp \
    src/actors/disparo.cpp \
    src/actors/characterbuilder.cpp \
    src/physics/boundingbox.cpp \
    src/physics/avance.cpp \
    src/world/mundo.cpp \
    src/physics/motorfisico.cpp \
    src/tools/listaimagenes.cpp \
    src/actors/personaje.cpp \
    src/actors/protagonista.cpp \
    src/actors/malo.cpp \
    src/tools/musicsystem.cpp \
    src/gui/palette.cpp \
    src/juego.cpp \
    src/editor.cpp \
    src/main.cpp 

OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

LIBRARY_DIRS := $(LIBRARY_DIRS)

#Ordenes de compilacion
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(patsubst %,-D%,$(DEFINES)) $(patsubst %,-I%,$(INCLUDE_DIRS)) -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) $(patsubst %,-D%,$(DEFINES)) $(patsubst %,-I%,$(INCLUDE_DIRS)) -o $@ $<


#Opciones de make
.PHONY: all
all: 	$(TARGET)

.PHONY: clean
clean:
	@echo    "----------------------------------------------"
	rm -f $(TARGET) $(OBJECTS)
	@echo    "----------------------------------------------"

.PHONY: clobber
clobber : clean
	@echo    "----------------------------------------------"
#	rm -f log*.txt //poner aki ordenes para borrar tonterias de logs y etc...

.PHONY: run
run:
	./geekfibres

.PHONY: tags
tags:
	ctags --c-types=+px **/*.[ch]pp

$(TARGET) :$(OBJECTS)
	$(LD) $(LD_FLAGS) $(patsubst %,-L%,$(LIBRARY_DIRS)) -o $@ $^ $(patsubst %,-l%,$(LIBRARIES)) 
	@chmod +x $@
	@echo    "----------------------------------------------"
	@echo -n "Geekfibres compilado satisfactoriamente!: "
	@ls -Falh $(TARGET) |cut -c 39-42
	@echo    "----------------------------------------------"

