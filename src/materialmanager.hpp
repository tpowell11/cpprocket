#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "json.hpp"
#include "errors.hpp"
using namespace nlohmann;
//json data; //! find workaround to this
namespace mat{
    namespace{
        json load(std::string name){
            std::ifstream f(name);
            json j = json::parse(f);
            return j;
            f.close();
        }
        json data = load("../dat/materials.json"); //load materials
    }
    void save(std::string name){
        std::ofstream o(name);
        o << std::setw(4) << data << std::endl;
    }
//* unions & structs
    struct Material{
        float density; //normal or linear density, in kg/m3 or kg/m respectively
        float strength; //unit depends on @char type, see docs
    };
//* Structural materials

    class material{
        public:
            struct Material props;
            std::string name;
            material(std::string Name){
                std::string name = Name;
                props.density=data["struct"][Name]["density"].get<float>();
                props.strength=data["struct"][Name]["strength"].get<float>();
            }
            material()=default;
            ~material()=default;
            float getMass(float amount){
                return props.density * amount;
            }
    };
}