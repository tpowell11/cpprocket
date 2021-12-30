#ifndef ROCKET
#define ROCKET
#include <vector>
#include <variant>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stddef.h>
#include <unistd.h> //file username stamping
#include <ctime> //file time stamping
#include "json.hpp" //json
#include "errors.hpp" //project errors
#include "materialmanager.hpp" //materials management
//#include "preset.hpp"
long double PI = 3.14159265358979323846L;
using namespace nlohmann;


namespace rocket {
    struct motor {
        std::string code;
        float Isp, Mp, Tb; 
    };
    struct nosecone{
        //parameters for a nosecone
        int gen;
        float param, base, slength, sbase, wall;
        void Zero(){
            this->param=(float)0;
            this->base=(float)0;
            this->gen=(float)0;
            this->slength=(float)0;
            this->sbase=(float)0;
            this->wall=(float)0;
        }
    };
    struct fins{
        int typ;
        float thickness;
        std::vector<std::tuple<float,float>> P;
        void Zero(){
            this->typ = (float)0;
            this->thickness = (float)0;
            for(auto i : this->P){
                i = std::make_tuple((float)0.0,(float)0.0);
            }
        }
    };
    struct tube{
        float innerDiameter;
        void Zero(){
            innerDiameter = 0.0;
        }
    };
    typedef nosecone nosecone_t;
    typedef fins fins_t;
    typedef tube tube_t;
    typedef std::variant<nosecone, fins, tube> prop_t;
    class Component {
        private:
            char T = 0;
            mat::matManager MM;
        public:
            float length, MaxDia, position, finish, volume = 0; //universal properties
            std::string name;
            mat::material m;
            prop_t props;
            bool isExposed = true; 
            Component(mat::material Material, std::string Name, float finish=0, bool isExposed=true){
                //shift incoming values to correct fields
                m = Material;
                name = Name;
                finish = finish;
            }
            Component(){
                this->Zero();
            };
            void show(){
                std::cout << "Component" << "\n";
                std::cout << "\t" << "Name: " << name << "\n";
                std::cout << "\t" << "Material Name: " << m.name << "\n";
                std::cout << "\t" << "Finish: " << finish << "\n";
            }
            void setType(char t){
                //ensures that changing the type identifier changes the contents of @props. 
                T = t;
                if (this->T == 'n')
                {
                    nosecone N;
                    N.Zero();
                    this->props = N;
                } else if (this->T == 'f')
                {
                    fins F;
                    F.Zero();
                    this->props=F;
                }
            }
            char getType(){
                return T;
            }
            float mass(){
                //calculate mass of component
                //!nyi
                return m.props.density * volume;
            }
            json get_json(){
                //TODO commit volume to file
                //returns the json for given component
                json j;
                j["T"] = T;
                j["length"] = length;
                j["isExposed"] = isExposed;
                j["diam"] = MaxDia;
                j["position"] = position;
                j["name"] = name;
                j["finish"] = finish;
                if (T == 'n'){
                    nosecone n = std::get<nosecone>(props);
                    j["props"]["base"] = n.base;
                    j["props"]["gen"] = n.gen;
                    j["props"]["param"] = n.param;
                    j["props"]["sbase"] = n.sbase;
                    j["props"]["slength"] = n.slength;
                    j["props"]["wall"] = n.wall;
                } else if (T == 'f'){
                    fins f = std::get<fins>(props);
                    j["props"]["typ"] = f.typ;
                    j["props"]["thickness"] = f.thickness;
                    for(auto i : f.P){
                        //loops through each tuple in points
                        j["props"]["points"] = i;
                    }
                } else if (T == 'b'){
                    tube t = std::get<tube>(props);
                    j["props"]["ID"] = t.innerDiameter;
                }
                return j;
            }
            void Zero(){
                this->length=(float)0;
                this->MaxDia=(float)0;
                this->name="";
                this->position=(float)0;
                this->volume=(float)0;
                this->finish=(float)0;
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
            motor Motor;
            float Cd;
            //file constructor
            Rocket(std::string filename){
                std::ifstream file(filename);
                json j = json::parse(file); // load file as json
                for(auto i : j["components"]){
                    Component *c = new Component;
                    //c->getType() = i["T"].get<char>();
                    c->length = i["length"].get<float>();
                    c->isExposed = i["isExposed"].get<bool>();
                    c->MaxDia = i["diam"].get<float>();
                    c->position = i["position"].get<float>();
                    c->name = i["name"].get<std::string>();
                    c->finish = i["finish"].get<int>();
                    if(c->getType() == 'n'){
                        //nosecone
                        nosecone_t n;
                        n.base = i["props"]["base"].get<float>();
                        n.gen = i["props"]["gen"].get<int>();
                        n.param = i["props"]["param"].get<float>();
                        n.sbase = i["props"]["sbase"].get<float>();
                        n.slength = i["props"]["slength"].get<float>();
                        n.wall= i["props"]["wall"].get<float>();
                        c->props = n;
                    }
                    addComponent(c);
                    delete c;
                }
                file.close();
            }
            std::string getName() {
                return this->name;
            }
            //adds components to the vector
            void addComponent(Component part){
                components.push_back(part);
                //part.show();
            }
            void addComponent(Component* part){
                components.push_back(*part);
            }
            void show(){
                for(auto i : components){
                    std::cout << i.name << "\n";
                }
            }
            //calculate total constant mass of the rocket
            float mass(){
                float* sum;
                for(auto i : this->components){
                    *sum += i.mass();
                }
                return *sum;
                delete sum;
            }
            //update the total fields & check for errors
            void update(){
                Tlength,Tmass, MXdiam = 0;
                float lastdiam;
                //std::vector<float> diams;
                for(auto i : components){
                    Numcomponents ++;
                    Tlength = Tlength + i.length; //accumulate lengths
                    Tmass = Tmass + i.mass(); //accumulate masses
                    if(i.isExposed == true){
                        MXdiam = i.MaxDia;
                    }
                }
            }
            //save to json file of filename
            void save(std::string filename){
                std::ofstream file(filename, std::ofstream::out); //output file
                json j;
                j["timestamp"] = (int)std::time(0);
                j["filename"] = filename;
                for(auto i : components){
                    //std::cout << i.get_json() << "\n";
                    j["components"].push_back(i.get_json());
                }
                std::cout << j.dump(4) << "\n";
                file << j.dump(4) << std::endl;
                file.close(); 
            }

    };
}
#endif