CXX=emcc
all: test.html

test.html:
	$(CXX) -std=c++1z -stdlib=libc++ -O3 *.cpp -o $@ --preload-file test.simp --preload-file teapot.obj -s ALLOW_MEMORY_GROWTH=1
