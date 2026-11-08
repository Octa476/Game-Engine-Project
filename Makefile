BUILD_DIR = build

all: build_project

configure:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR); \
	fi

build_project: configure
	cmake --build $(BUILD_DIR)

run: build_project
	./$(BUILD_DIR)/main

clean:
	cmake --build build --target clean

.PHONY: all configure build_project run clean

