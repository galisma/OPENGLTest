# Compilador y banderas
CXX = g++
CC = gcc
CXXFLAGS = -Ilib/glad/include -Ilib/GLFW/include -Isrc  # Clave: -Isrc para tus headers
CFLAGS = $(CXXFLAGS)
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.d
LDFLAGS = -Llib/GLFW/obj
LIBS = -lglfw3 -lGL -ldl -lpthread -lm

# Directorios
BUILD_DIR = build/obj
BIN_DIR = build/bin
SRC_DIR = src

# Fuentes y objetos
SOURCES_C = $(wildcard $(SRC_DIR)/*.c)
SOURCES_CPP = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS_C = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES_C))
OBJECTS_CPP = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES_CPP))
TARGET = $(BIN_DIR)/main

all: create-dirs $(TARGET)

$(TARGET): $(OBJECTS_C) $(OBJECTS_CPP)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)

# Regla para .cpp (con dependencias de headers en src/)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Regla para .c (glad, con headers en src/)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

create-dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Incluir archivos .d generados (para detectar cambios en headers)
-include $(wildcard $(BUILD_DIR)/*.d)

run: all
	GDK_BACKEND=wayland _GLFW_WAYLAND=1 GTK_THEME=Adwaita:dark ./$(TARGET)

clean:
	rm -rf build

.PHONY: all create-dirs run clean