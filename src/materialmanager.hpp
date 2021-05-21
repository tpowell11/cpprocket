#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "json.hpp"
#include "errors.hpp"
using namespace nlohmann;
namespace mat{
//* unions & structs
    union TENSILE{
        float s;
        bool n;
    };
    union COMPRESSIVE{
        float s;
        bool n;
    };
    struct Material{
        char type; //l indicates linear density, n indicates normal
        float density; //normal or linear density, in kg/m3 or kg/m respectively
        COMPRESSIVE compressiveStrength; //R_s, in megapascals
        TENSILE ultimateTensile; //point where a material breaks, megapascals
    };
//* File managment functions
    inline json LoadMaterialsDb(){ //! why does this need to be inline
        //*loads the json materials database
        std::fstream i("../dat/materials.json");
        json j;
        i >> j;
        return j;
    }
    //takes a material and adds it to global materials dataset
    template<class t>
    void writeMaterial(t m, json j){
        switch(m.props.type){
            case 's':
                j["struct"][m.name];
                j["struct"][m.name]["density"] = m.props.density;
                j["struct"][m.name]["Rs"] = m.props.compressiveStrength.s;
            case 'p':
                j["plane"][m.name];
                j["plane"][m.name]["density"];
            case 'l':
                j["line"][m.name];
                j["line"][m.name]["density"] = m.props.density;
                j["line"][m.name]["ultimatestrength"] = m.props.ultimateTensile.s;
        }
    }
    void saveMaterialsFile(json j){
        std::fstream i("../dat/materials.json");
        i << std::setw(4) << j;
    }
    // void addMaterial(StructureMaterial m){
    //     data["struct"][m.name];
    //     data["struct"][m.name]["density"] = m.density
    // }
    json data = LoadMaterialsDb();
//* Structural materials

    class StructureMaterial{
        public:
            StructureMaterial(std::string Name){
                std::string name = Name;
                props.density=data["struct"][Name]["density"];
                props.compressiveStrength.s=data["struct"][Name]["Rs"];
                props.type='s';
                props.ultimateTensile.n=false;
            }
            StructureMaterial()=default;
            struct Material props; 
            std::string name;
    };
    StructureMaterial getMaterialS(std::string name){
        std::cout << name;
        if(name.at(0) == 's' || name.at(0) == 'S'){
            std::string n = name.substr(0,1); //trim the structual specifier
            return StructureMaterial(n);
        } else {
            throw err::MaterialNotFound();
        }
    }

//* Linear materials
    class LineMaterial{
        public:
            LineMaterial(std::string Name){
                std::string name = Name;
                props.density=data["line"][Name]["density"];
                props.type = 'l';
                props.ultimateTensile.s = data["line"][Name]["ultimatestrength"];
                props.compressiveStrength.n=false;
            }
            LineMaterial()=default;
            struct Material props;
    };
    LineMaterial getMaterialL(std::string name){
        if(name.at(0) == 'l' || name.at(0)){
            std::string n = name.substr(0,1);
            return LineMaterial(n);
        }
    }
}