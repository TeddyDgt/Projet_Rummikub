CC = gcc
CFLAGS = -Wextra -Wall

CONSOLE_SRC = $(filter-out main_opengl.c audio.c,$(wildcard *.c))
CONSOLE_OBJ = $(CONSOLE_SRC:.c=.o)

GUI_SRC = graphics/main_opengl.c graphics/platform/gui_platform.c graphics/gfx/renderer2d.c \
          $(wildcard graphics/ui/*.c) Draw.c Players.c Table.c GameLoop.c Combinaisons.c graphics/audio.c

EXEC = main

THIRD_PARTY = ../third_party
GLFW_DIR = $(THIRD_PARTY)/glfw
GLFW_BUILD = $(GLFW_DIR)/build
GLFW_LIB = $(GLFW_BUILD)/src/libglfw3.a
GLFW_DLL = ../glfw3.dll
GLFW_IMP = $(GLFW_DIR)/lib-mingw/libglfw3dll.a
GLFW_LIB_DEP =
CMAKE ?= cmake
GLFW_CMAKE_GEN ?=
GLFW_CMAKE_FLAGS = -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_EXAMPLES=OFF
CFLAGS += -Igraphics -I$(THIRD_PARTY)/miniaudio

ifeq ($(OS),Windows_NT)
GUI_EXEC = rummikub_gui.exe
GLFW_CMAKE_GEN ?= MinGW Makefiles
ifneq ($(wildcard $(GLFW_DLL)),)
ifneq ($(wildcard $(GLFW_IMP)),)
CFLAGS += -I$(GLFW_DIR)/include
CFLAGS += -DGLFW_DLL
PLATFORM_LIBS = -L$(GLFW_DIR)/lib-mingw -lglfw3dll -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm
GLFW_LIB_DEP = $(GLFW_IMP)
else
$(error libglfw3dll.a manquant: place-le dans third_party/glfw/lib-mingw/)
endif
else ifneq ($(wildcard $(GLFW_DIR)/CMakeLists.txt),)
USE_BUNDLED_GLFW = 1
CFLAGS += -I$(GLFW_DIR)/include
GLFW_CMAKE_FLAGS += -DGLFW_BUILD_SHARED_LIBS=OFF
PLATFORM_LIBS = $(GLFW_LIB) -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm
GLFW_LIB_DEP = $(GLFW_LIB)
else
PLATFORM_LIBS = -lglfw3 -lopengl32 -lgdi32 -lwinmm
endif
else
GUI_EXEC = rummikub_gui
ifneq ($(wildcard $(GLFW_DIR)/CMakeLists.txt),)
USE_BUNDLED_GLFW = 1
CFLAGS += -I$(GLFW_DIR)/include
GLFW_CMAKE_FLAGS += -DGLFW_BUILD_SHARED_LIBS=OFF -DGLFW_BUILD_WAYLAND=ON -DGLFW_BUILD_X11=ON
PLATFORM_LIBS = $(GLFW_LIB) -lGL -lm -ldl -lpthread -lX11 -lXrandr -lXi
GLFW_LIB_DEP = $(GLFW_LIB)
else
PKG_CFLAGS := $(shell pkg-config --cflags glfw3 2>/dev/null)
PKG_LIBS := $(shell pkg-config --libs glfw3 2>/dev/null)
ifneq ($(strip $(PKG_CFLAGS)),)
CFLAGS += $(PKG_CFLAGS)
endif
ifneq ($(strip $(PKG_LIBS)),)
PLATFORM_LIBS = $(PKG_LIBS)
else
PLATFORM_LIBS = -lglfw -lGL -lm -ldl -lpthread -lX11 -lXrandr -lXi
endif
endif
endif

all: gui

gui: $(GUI_EXEC)

console: $(EXEC)

$(EXEC): $(CONSOLE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(GUI_EXEC): $(GUI_SRC) $(GLFW_LIB_DEP)
	$(CC) $(CFLAGS) -I. -o $@ $^ $(PLATFORM_LIBS)

$(GLFW_LIB):
ifneq ($(USE_BUNDLED_GLFW),)
	@$(CMAKE) --version > /dev/null 2>&1 || (echo "CMake requis pour compiler GLFW." && exit 1)
	$(CMAKE) -S $(GLFW_DIR) -B $(GLFW_BUILD) $(if $(GLFW_CMAKE_GEN),-G "$(GLFW_CMAKE_GEN)",) $(GLFW_CMAKE_FLAGS)
	$(CMAKE) --build $(GLFW_BUILD) --config Release
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC) $(GUI_EXEC) *.txt

re: clean all
