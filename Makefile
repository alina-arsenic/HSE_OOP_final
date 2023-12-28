all: clean build start

build:
	g++ *.cpp *.h
	rm -rf *.gch

start:
	./a.out

style:
	cppcheck --language=c++ *.cpp *.h
	clang-format -n *.cpp *.h

clean:
	rm -rf a.out *.gch
