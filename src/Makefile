CC = gcc
CFLAGS = -Wextra -Wall

CONSOLE_SRC = $(filter-out main_opengl.c,$(wildcard *.c))
CONSOLE_OBJ = $(CONSOLE_SRC:.c=.o)

GUI_SRC = main_opengl.c platform/gui_platform.c gfx/renderer2d.c \
          $(wildcard ui/*.c) Draw.c Players.c Table.c GameLoop.c Combinaisons.c

EXEC = main
GUI_EXEC = rummikub_gui.exe

all: gui

gui: $(GUI_EXEC)

console: $(EXEC)

$(EXEC): $(CONSOLE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(GUI_EXEC): $(GUI_SRC)
	$(CC) $(CFLAGS) -I. -Iplatform -Igfx -o $@ $^ -lglfw3 -lopengl32 -lgdi32 -lwinmm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC) $(GUI_EXEC) *.txt

re: clean all
