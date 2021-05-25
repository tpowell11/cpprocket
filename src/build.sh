g++ --version | head -n 1 
# grab the last version number from file
#lastnum=cat ../dat/version.txt
lastnum=$(<../dat/version.txt)
newnum=$((lastnum +1))
fname="rocket$newnum.o"
rm ../bin/rocket$lastnum.o
g++ -std=c++17 -o ../bin/$fname -B . main.cpp