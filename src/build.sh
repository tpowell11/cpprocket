g++ --version | head -n 1 
rm ../bin/rocket.o
g++ -std=c++17 -o ../bin/rocket.o -B . main.cpp