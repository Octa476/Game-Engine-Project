BUILD_DIR = build

# Regula implicită (rulați doar 'make') va compila proiectul
all: build_project

# Rulează configurarea CMake doar dacă folderul build nu există
configure:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR); \
	fi

# Compilează proiectul prin CMake
build_project: configure
	cmake --build $(BUILD_DIR)

# Regula ta preferată: compilează (dacă e nevoie) și rulează direct programul
run: build_project
	./$(BUILD_DIR)/main

# Curăță fișierele temporare și build-ul
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all configure build_project run clean

