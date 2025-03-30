all:
	g++ main.cpp grim.cpp buffer.cpp -o .build/main.out -lncurses

run:
	.build/main.out