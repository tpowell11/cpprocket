g++ --version | head -n 1 
# grab the last version number from file
lastnum=$(cat "../dat/version.txt")
newnum=$((lastnum +1)) #increment
echo "Build number $newnum"
fname="rocket$newnum.o"
fp="../bin/rocket$lastnum.o"
rm $fp
g++ -std=c++17 -o ../bin/$fname -B . main.cpp # compile under new filename
# if the compile was successful, remove old file
if test -f "../bin/rocket$newnum.o"; then
    echo $newnum > ../dat/version.txt #update the version file
fi