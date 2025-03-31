DIR_NAME := build

all:
	@echo "Checking if $(DIR_NAME) exists..."
	@if [ ! -d "$(DIR_NAME)" ]; then \
		echo "Directory $(DIR_NAME) does not exist. Creating..."; \
		mkdir -p "$(DIR_NAME)"; \
	else \
		echo "Directory $(DIR_NAME) exists."; \
	fi
	@echo "Building GRIM..."
	g++ main.cpp grim.cpp buffer.cpp -o build/grim -lncurses
	@echo "Done."

run:
	build/main.out