CFLAGS = -std=c99 -Ofast -Wall -Wextra -Iinclude/ -Ivendors/
LIBS = `pkg-config glfw3 glew --cflags --libs` -framework OpenGL
OUT = build

all:
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) -o $(OUT)/main $(wildcard src/*.c) $(LIBS)

clean:
	rm -rf $(OUT)

.PHONY: all clean
