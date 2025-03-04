CC = gcc
CFLAGS = -Wno-everything -I/opt/homebrew/include/SDL2
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf -lm

SRC = main.c visualizer.c algorithms.c ui.c graph.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = algo

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
	rm -f $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)
