# Source and target
SRCS = main.cpp CS3113/cs3113.cpp

TARGET = raylib_app

# OS detection (macOS = Darwin, Windows via MinGW = MINGW*)
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# Default values
CXX = g++
CXXFLAGS = -std=c++11

# Raylib configuration using pkg-config, with Homebrew fallbacks for macOS
RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS = $(shell pkg-config --libs raylib 2>/dev/null)

ifeq ($(UNAME_S), Darwin)
    ifeq ($(UNAME_M), x86_64)
        ARCH_FLAG = -arch x86_64
    else ifeq ($(UNAME_M), arm64)
        ARCH_FLAG = -arch arm64
    else
        ARCH_FLAG =
    endif

    ifeq ($(strip $(RAYLIB_CFLAGS)),)
        RAYLIB_CFLAGS = -I/opt/homebrew/include -I/usr/local/include
    endif
    ifeq ($(strip $(RAYLIB_LIBS)),)
        RAYLIB_LIBS = -L/opt/homebrew/lib -L/usr/local/lib -lraylib
    endif

    CXXFLAGS += $(ARCH_FLAG) $(RAYLIB_CFLAGS)
    LIBS = $(RAYLIB_LIBS) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    EXEC = ./$(TARGET)
else ifneq (,$(findstring MINGW,$(UNAME_S)))
    # Windows configuration (assumes raylib in C:/raylib)
    CXXFLAGS += -IC:/raylib/include
    LIBS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET := $(TARGET).exe
    EXEC = ./$(TARGET)
else
    # Linux/WSL fallback
    CXXFLAGS +=
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    EXEC = ./$(TARGET)
endif

# Build rule
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Clean rule
clean:
	@if [ -f "$(TARGET)" ]; then rm -f $(TARGET); fi
	@if [ -f "$(TARGET).exe" ]; then rm -f $(TARGET).exe; fi

# Run rule
run: $(TARGET)
	$(EXEC)
