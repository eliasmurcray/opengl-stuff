CFLAGS = -std=c89 -Ofast -Wall -Wextra -Iinclude/ -Ivendors/
OUT = build

ifeq ($(OS),Windows_NT)
    LIBS = `pkg-config glfw3 glew --cflags --libs` -lopengl32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        LIBS = `pkg-config glfw3 glew --cflags --libs` -framework OpenGL
    else ifeq ($(UNAME_S),Linux)
        LIBS = `pkg-config glfw3 glew --cflags --libs` -lGL
    else
        $(error Unsupported platform: $(UNAME_S))
    endif
endif

all:
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) -o $(OUT)/main $(wildcard src/*.c) $(LIBS)

clean:
	rm -rf $(OUT)

.PHONY: all clean
