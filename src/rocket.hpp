#ifndef ROCKET
#define ROCKET
#include <vector>
#include <variant>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <stddef.h>
//#include <exception.h>
#include "json.hpp" //json
#include "errors.hpp" //project errors
#include "materialmanager.hpp" //materials management
//#include "preset.hpp"
long double PI = 3.14159265358979323846L;
using namespace nlohmann;


// namespace load{
//     json getSection(std::string sectionName){
//         std::cout << sectionName << "|\n";
//         std::fstream file("../dat/presets.json");
//         json j;
//         std::cout << j[sectionName] << "|\n";
//         return json::parse(j[sectionName]);
//     }
// }

namespace rocket {

    class Component {
        public:
            Component(mat::StructureMaterial Material, std::string Name, int FinishNumber=0, bool isExposed=true){
                //shift incoming values to correct fields
                material = Material;
                name = Name;
                finishNumber = FinishNumber;
            }
            Component()=default;
            float mass, position;
            std::string name;
            mat::StructureMaterial material;
            bool isExposed;
            int finishNumber;
            void show(){
                std::cout << "Component" << "\n";
                std::cout << "\t" << "Name:" << name << "\n";
                std::cout << "\t" << "Material Name:" << material.name << "\n";
            }
    };
    //cout overload for components
    std::ostream &operator<<(std::ostream &os, Component const &m){
        os << m.name << "\n";
        return os;
    }

    class Nosecone: public Component{
        public:
            Nosecone(std::string Name, mat::StructureMaterial Material){
                material = Material;
                name = Name;
            }
            Nosecone(std::string presetName, std::string Name = ""){
                name = Name;
                loadPreset(Name);
            }
            Nosecone()=default;
            void loadPreset(std::string presetname){
                std::cout << presetname << "\n";
                std::ifstream i("../dat/presets.json");
                json j;
                i >> j;
                //std::cout << j["nosecones"][presetname]["generator"] << "\n";
                generator = j["nosecones"][presetname]["generator"];
                gLength = j["nosecones"][presetname]["gLength"];
                gDiameter = j["nosecones"][presetname]["gBaseRadius"];
                isShoulder = j["nosecones"][presetname]["isShoulder"];
                sLength = j["nosecones"][presetname]["shoulderLength"];
                sDiameter = j["nosecones"][presetname]["shoulderDiameter"];
                isHollow = j["nosecones"][presetname]["isHollow"];

            }
            void show(){
                std::cout << "Nosecone" << "\n";
                std::cout << "\t" << "Name: " << name << "\n";
                std::cout << "\t" << "Material Name: " << material.name << "\n";
                std::cout << "\t" << "Diameter: " << gDiameter << "\n";
                std::cout << "\t" << "Generator: " << generator << "\n";
            }
            int generator;
            float gParameter;
            bool isHollow;
            bool isShoulder;
            float sLength;
            float sDiameter;
            float gLength;
            float gDiameter;
            mat::StructureMaterial material;
    };
    // represents a bodytube
    class Bodytube: public Component{
        public:
            Bodytube(mat::StructureMaterial Material, std::string Name="Bodytube",
            float Innerdiameter=0.0, float Outerdiameter=0.0, float Length=0.0){
               name = Name;
               material = Material;
               innerdiameter = Innerdiameter;
               outerdiameter = Outerdiameter;
               length = Length;
            }
            Bodytube(std::string presetName, std::string Name = "Bodytube"){
                name = Name;
                std::ifstream i("../dat/presets.json");
                json j;
                i >> j;
                innerdiameter = j["bodytubes"][presetName]["innerdiameter"];
                outerdiameter = j["bodytubes"][presetName]["outerdiameter"];
                length = j["bodytubes"][presetName]["length"];
                
                //*why cant the json get be directly in the function call?
                std::string matname = j["bodytubes"][presetName]["material"];
                // std::cout << matname <<"\n";
                // material = mat::getMaterialS(matname);
                //heatresistant = j["bodytubes"][presetName]["isheatresistant"];
            }
            Bodytube()=default;
            void loadPreset(char* presetName){
                std::ifstream i("../dat/presets.json");
                json j;
                i >> j;
                innerdiameter = j["bodytubes"][presetName]["innerdiameter"];
                outerdiameter = j["bodytubes"][presetName]["outerdiameter"];
                length = j["bodytubes"][presetName]["length"];
            }
            float innerdiameter;
            float outerdiameter;
            float length;
            bool heatresistant;
    };
    
    // union parts{
    //     Component C();
    //     Nosecone N();
    //     Bodytube B();
    // };
    //container for components
    class Rocket {
        //! Add method to remove components.
        public:
            Rocket()=default;
            //file constructor
            Rocket(std::string filename){

            }
            //adds any component to the components vector
            void addComponent(std::variant<Component,Bodytube,Nosecone> part){
                components.push_back(part);
                //part.show();
            }

            template <typename t>
            void appendComponent(t part){
                std::variant<Component,Bodytube,Nosecone> v;
                v = part;
                components.push_back(v);
                //Components.push_back()
                part.show();
                std::cout << "\n" << components.size() << "\n";
                std::cout << std::get<t>(v) << "\n";
            }
            // void pushComponent(Nosecone n){
            //     union parts v;
            //     v.N() = n;
            //     Components.push_back(v);
            // }
            //!really auful way to do this
            void show(){
                std::cout << "Size:" << components.size() << "\n";
                //std::cout << "Sizec:" << Components.size() << "\n";
                // for(size_t i=0; i <= components.size(); i++){
                //     while(1){
                //         try{
                //             std::get<0>(components[i]).show();
                //         } catch(const std::exception& e) {
                //             std::get<1>(components[i]).show();
                //         } catch(...) {
                //             std::get<2>(components[i]).show();
                //         }
                //     }
                // }
            }


            //this line requires a compiler with c++17 support
            std::vector<std::variant<Component, Bodytube, Nosecone>> components;
            //std::vector<union parts>Components;
    };
    // std::ostream &operator<<(std::ostream &os, Rocket const &m){
    //     for(size_t i = 0; i < m.components.size();++i){
    //         std::cout << m.components[i] << "\n";
    //     }
    // }
//typedef std::variant<Component, Nosecone, Bodytube> AnyComponent;
}
#endif