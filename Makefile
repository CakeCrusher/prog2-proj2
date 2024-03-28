build:
	g++ -std=c++11 -o project2.out src/*.cpp
compile:
	g++ -std=c++11 src/*.cpp
tasks: build
	./project2.out output/task11.tga input/circles.tga multiply input/layer1.tga
	./project2.out output/task12.tga input/layer1.tga flip
	./project2.out output/task13.tga input/layer1.tga subtract input/layer2.tga