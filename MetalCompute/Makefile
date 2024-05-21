
all: program

program: metallib gpu

gpu: hellogpu.cpp
	clang++ -std=c++20 hellogpu.cpp -o hellogpu.out -I ../metal-cpp -framework Metal -framework MetalKit -framework Foundation

metallib: default.metal
	xcrun -sdk macosx metal -c default.metal -o default.air
	xcrun -sdk macosx metallib default.air -o default.metallib

clean:
	rm -f hellogpu default.air default.metallib


.PHONY: all clean