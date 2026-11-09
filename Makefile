BUILD_DIR = build
EXEX = engine

all: build_project

configure:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		cmake -S . -B $(BUILD_DIR); \
	fi

build_project: configure
	cmake --build $(BUILD_DIR)

run: build_project
	./$(BUILD_DIR)/engine

clean:
	cmake --build build --target clean

.PHONY: all configure build_project run clean

