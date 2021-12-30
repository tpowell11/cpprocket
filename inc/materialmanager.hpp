#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"
#include "errors.hpp"
using namespace nlohmann;
namespace mat{
    struct Material {
        std::string name;
        float density;
        float strength;
    };
    class matManager {
        private:
        std::vector<std::string> ValidNames;
        json data;
        public:
        matManager(std::string path){
            std::ifstream f(path);
            data=json::parse(f);
            f.close();
            for (auto i : data.items()){
                std::cout << i.key() << "\n";
            }
        }
        Material getMatfStr(std::string name){
            Material m;
            if (*std::find(ValidNames.begin(), ValidNames.end(),name) == name){
                m.name = name;
                m.density = data[name]["density"];
                m.strength = data[name]["strength"];
                return m; 
            } else {
                throw("m001");
                std::cerr << "m001: invalid material name\n";
            }
            //delete m;
        }
        void dumpJ(int indent){
            std::cout << data.dump(indent) << "\n";
        }
    };
}