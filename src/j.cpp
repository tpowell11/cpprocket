#include <fstream>
#include <iostream>
#include "json.hpp"
using namespace nlohmann;
int main(){
    //*loads the json materials database
    std::ifstream i("../dat/materials.jsonc");
    json j;
    i >> j;
    std::cout << j;
}