# Universal MakeFile v1.4
# Compatible with Code::blocks

# Declaration of variables
CC = g++
DB = gdb
RM = rm
MD = mkdir
CP = cp
RC = windres
MAKE = mingw32-make
BUILD = Debug
FILE =
BULLET = "* "
BULLET2 = ""
TAB = "  "

DEBUG = true
PRECOMPILED_HEADERS = true
COMPILE_STATIC_LIB = false
COPY_INCLUDE_HEADERS = false

# Black        0;30     Dark Gray     1;30
# Red          0;31     Light Red     1;31
# Green        0;32     Light Green   1;32
# Brown/Orange 0;33     Yellow        1;33
# Blue         0;34     Light Blue    1;34
# Purple       0;35     Light Purple  1;35
# Cyan         0;36     Light Cyan    1;36
# Light Gray   0;37     White         1;37

CRED 		= "\033[1;31m"
CLBLUE 		= "\033[1;34m"
CYELLOW 	= "\033[1;33m"
CGREEN 		= "\033[0;32m"
CLGREEN 	= "\033[1;32m"
CGREY 		= "\033[1;30m"
CLGREY 		= "\033[0;37m"
CORANGE 	= "\033[0;33m"
CPURPLE 	= "\033[0;35m"
CLPURPLE 	= "\033[1;35m"
CCYAN 		= "\033[0;36m"
CLCYAN 		= "\033[1;36m"
CEND 		= "\033[0m"

ifeq ($(BUILD),Debug)
	DEBUG = true
else
	DEBUG = false
endif

#The Target Binary Program
ROOT_DIR = "$(CURDIR)"
THIS_DIR = "$(shell basename "$(CURDIR)")"
#TARGET = $(THIS_DIR).exe
TARGET = Game.exe
LIBTARGET = libshushao.dll
STATICLIB = libshushaos.a
CORES = 8

#PATHS
BASE_LIBS	= ../../mingw-dev-libs

SRCDIR	= .
ENGINE	= engine
GAME	= game
RESDIR	= res
SRCEXT	= cpp
OBJEXT	= o
RESEXT	= res
RCEXT	= rc
ASEXT	= s
GCHDIR  = pch
GCHEXT  = h.gch

BUILDDIR = obj/$(BUILD)
TARGETDIR = bin/$(BUILD)
PRECOMPILEDDIR = $(ENGINE)/$(GCHDIR)
TARGETLIBDIR = lib
HEADERSDIR = se

#Single file comp
SINGLEFILE = $(subst $(GAME), $(SRCDIR)/$(GAME)/, $(subst $(ENGINE), $(SRCDIR)/$(ENGINE)/, $(FILE)))
SINGLEOBJ = $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SINGLEFILE:.$(SRCEXT)=.$(OBJEXT)))
SINGLEASSEMBLY = $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SINGLEFILE:.$(SRCEXT)=.$(ASEXT)))

#Flags, Libraries and Includes
# -fexceptions
DEPFLAGS =
COMFLAGS = -MMD -MP -std=c++17 -DGLEW_STATIC -fmax-errors=10 -Wfatal-errors -DDEBUG=$(DEBUG) -x c++
GCHFLAGS = -std=c++17 -DGLEW_STATIC -fmax-errors=10 -Wfatal-errors -DDEBUG=$(DEBUG) -x c++-header
LNKFLAGS =
SHAREDFLAGS = -shared
ARFLAGS = rcs

SDLLIBS  = -L$(BASE_LIBS)/SDL2/lib -L$(BASE_LIBS)/SDL2_image/lib -L$(BASE_LIBS)/SDL2_mixer/lib
LIBDIRS	 = -L$(BASE_LIBS)/boost/stage/lib -L$(BASE_LIBS)/glew/lib -L$(BASE_LIBS)/freetype/lib -L$(BASE_LIBS)/Box2D/lib $(SDLLIBS)
LIB 	 = -lboost_context -lboost_coroutine -lglew32 -lmingw32 -lopengl32 -lgdi32 -lglu32 -lfreetype -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lBox2D
INCDIRS  = -I$(BASE_LIBS)/boost -I$(BASE_LIBS)/glew/include -I$(BASE_LIBS)/glew/include -I$(BASE_LIBS)/glm -I$(BASE_LIBS)/freetype/include -I$(BASE_LIBS)/SDL2/include/SDL2 -I$(BASE_LIBS)/SDL2_image/include/SDL2 -I$(BASE_LIBS)/SDL2_mixer/include/SDL2 -I$(BASE_LIBS)/Box2D/include

#Condizioni
ifeq ($(DEBUG),true)
	COMFLAGS += -Wall -g
	GCHFLAGS += -Wall -g
else
	COMFLAGS += -O2
	GCHFLAGS += -O2
	LNKFLAGS += -s -mwindows
endif

SUBDIRS  = $(wildcard */)
SOURCES  = $(wildcard $(SRCDIR)/$(ENGINE)/*.$(SRCEXT)) $(wildcard $(SRCDIR)/$(ENGINE)/**/*.$(SRCEXT))
SOURCES += $(wildcard $(SRCDIR)/$(GAME)/*.$(SRCEXT)) $(wildcard $(SRCDIR)/$(GAME)/**/*.$(SRCEXT))
RCSOURCES = $(wildcard $(SRCDIR)/$(GAME)/*.$(RCEXT)) $(wildcard $(SRCDIR)/$(GAME)/**/*.$(RCEXT))
TOPRECOMP = $(wildcard $(SRCDIR)/$(ENGINE)/$(GCHDIR)/*.h)

ENGINESOURCES = $(wildcard $(SRCDIR)/$(ENGINE)/*.$(SRCEXT)) $(wildcard $(SRCDIR)/$(ENGINE)/**/*.$(SRCEXT))
ENGINEOBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(ENGINESOURCES:.$(SRCEXT)=.$(OBJEXT)))

#per compilare le risorse dell'engine nell'exe (ora le metto nella dll)
#RCSOURCES += $(wildcard $(SRCDIR)/$(ENGINE)/*.$(RCEXT)) $(wildcard $(SRCDIR)/$(ENGINE)/**/*.$(RCEXT))

OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
RCFILES = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(RCSOURCES:.$(RCEXT)=.$(RESEXT)))
PRECOMP = $(patsubst $(SRCDIR)/$(ENGINE)/$(GCHDIR)/%,$(PRECOMPILEDDIR)/%,$(TOPRECOMP:.h=.$(GCHEXT)))
DEPENDS = $(OBJECTS:.o=.d)

ENGINE_RCFILE = engine/resources/resources.$(RCEXT)
ENGINE_RESFILE = engine/resources/resources.$(RESEXT)
GAME_RESFILE = game/resources/resources.$(RESEXT)

N_SOURCES := $(words $(SOURCES))
N_RCSOURCES := $(words $(RCSOURCES))


