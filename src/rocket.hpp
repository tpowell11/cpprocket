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

struct finish
{
    float roughnessum;
    int id;
    std::string name;
};
class Finish {
    private:
        std::fstream finishfile;
    public:
    finish F;
        Finish(int id){
            // finishfile.open("../dat/finish.json");
            // json j = json::parse(finishfile);
            // F.id = id;
            // F.name = j[(const char)id]["name"];
            // F.roughnessum = j[(const char)id]["r"];
            //finishfile.close()
        }

};


namespace rocket {

    class Component {
        private:
            char T;
        public:
            float length, mass, MaxDia, position;
            Component(mat::StructureMaterial Material, std::string Name, int FinishID=0, bool isExposed=true){
                //shift incoming values to correct fields
                material = Material;
                name = Name;
                finishID = FinishID;
            }
            Component(std::string name){

            }
            Component()=default;
            std::string name;
            mat::StructureMaterial material;
            bool isExposed;
            int finishID;
            void show(){
                std::cout << "Component" << "\n";
                std::cout << "\t" << "Name: " << name << "\n";
                std::cout << "\t" << "Material Name: " << material.name << "\n";
                std::cout << "\t" << "Finish: " << finishID << "\n";
            }
            json get_json(){
                //returns the json for given component
                json j;
                j["T"] = T;
                j["length"] = length;
                j["isExposed"] = isExposed;
                j["mass"] = mass;
                j["diam"] = MaxDia;
                j["position"] = position;
                j["name"] = name;
                j["finish"] = finishID;

                return j;
            }
    };
    //cout overload for components
    std::ostream &operator<<(std::ostream &os, Component const &m){
        os << m.name << "\n";
        return os;
    }
    class Rocket {
        //! Add method to remove components.
        private: 
            std::vector<Component> components;
            std::string name;
            float Tmass, Tlength, MXdiam;
            int Numcomponents;
        public:
            Rocket()=default;
            //file constructor
            Rocket(std::string filename){
                std::ifstream file(filename);
                json j = json::parse(file);
            }
            //adds components to the vector
            void addComponent(Component part){
                components.push_back(part);
                //part.show();
            }
            void show(){
                for(auto i : components){
                    std::cout << i.name << "\n";
                }
            }
            //update the total fields & check for errors
            void updateSelf(){
                Tlength,Tmass, MXdiam = 0;
                float lastdiam;
                //std::vector<float> diams;
                for(auto i : components){
                    Numcomponents ++;
                    Tlength = Tlength + i.length; //accumulate lengths
                    Tmass = Tmass + i.mass; //accumulate masses
                    if(i.isExposed == true){
                        MXdiam = i.MaxDia;
                    }
                }
            }
            //save to json file of filename
            void save(std::string filename){
                std::cout << "dkjh";
                std::ofstream file(filename, std::ofstream::out); //output file
                json j;
                std::vector<json> c;
                for(auto i : components){
                    c.push_back(i.get_json());
                }
                file << j.dump(4) << std::endl;
                file.close(); 
            }

    };
}
#endif