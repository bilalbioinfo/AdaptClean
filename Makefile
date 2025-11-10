all: AdaptClean

AdaptClean: AdaptClean.cpp
	@mkdir -p bin/
	g++ -std=c++17 -O2 -Wall AdaptClean.cpp -o bin/AdaptClean -lz
	@echo "Done. Built bin/AdaptClean"

clean:
	rm -f bin/AdaptClean
