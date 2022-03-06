all:
	g++ -std=c++11 -O3 -g test.cpp task.cpp -o test
clean:
	rm ./test
