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
//#include "materialmanager.hpp" //materials management
//#include "preset.hpp"
long double PI = 3.14159265358979323846L;
using namespace nlohmann;


namespace rocket {
    struct nosecone{
        int gen;
        float param, base, slength, sbase, wall;
        nosecone(){}
        nosecone(nosecone *n){
            gen = n->gen;
            param = n->param;
            base = n->base;
            slength = n->slength;
            sbase = n->sbase;
            wall = n->wall;
        }
    };
    struct fins{
        int typ;
        std::vector<std::vector<float>> points;
        fins(){}
        fins(fins *f){
            typ = f->typ;
            points = f->points;
        }
    };
    struct tube{
        float wall;
        tube(){}
        tube(tube *t){
            wall = t->wall;
        }
    };
    // typedef nosecone nosecone_t;
    // typedef fins fins_t;
    // typedef tube tube_t;
    //typedef std::variant<nosecone, fins, tube> prop_t;
    union prop_t {
        nosecone n;
        fins f; 
        tube t;
        prop_t () {};
        ~prop_t () {};
    };

    class Component {
        private:
            float length;
        public:
            char T;
            float MaxDia, position, finish, volume;
            std::string name;
            prop_t props;
            bool isExposed = true;
            Component(){}
            Component(const Component&){}
            void show(){
                std::cout << "Component" << "\n";
                std::cout << "\t" << "Name: " << name << "\n";
                //std::cout << "\t" << "Material Name: " << m.name << "\n";
                std::cout << "\t" << "Finish: " << finish << "\n";
            }
            void setLength(float v){
                length = v;
            }
            float getLength(){
                return length;
            }
            void setType(char t){
                T = t;
                if(t == 'n'){
                    std::cout << "init nosecone" << "\n";
                    nosecone n;                                                                                                 
                    T = 'n';
                    props.n = n;
                } else if (t == 't'){
                    std::cout << "init tube" << "\n";
                    tube t;
                    T = 't';
                    props.t = t;
                } else if (t == 'f'){
                    std::cout << "init fins" << "\n";
                    fins f;
                    T = 'f';
                    props.f = f;
                }
            }
            float mass(){
                //calculate mass of component
                //!nyi
                //return m.props.density * volume;
                return 0.0f;
            }           
            json get_json(){
                //TODO commit volume to file
                //returns the json for given component
                json j;
                j["T"] = T;
                std::cout << T << "\n";
                j["length"] = length;
                std::cout << length << "\n";
                j["isExposed"] = isExposed;
                j["diam"] = MaxDia;
                j["position"] = position;
                j["name"] = name;
                j["finish"] = finish;
                if (T == 'n'){
                    std::cout << "saving Nosecone\n";
                    nosecone n;
                    n = props.n;
                    j["props"]["base"] = n.base;
                    j["props"]["gen"] = n.gen;
                    j["props"]["param"] = n.param;
                    j["props"]["sbase"] = n.sbase;
                    j["props"]["slength"] = n.slength;
                    j["props"]["wall"] = n.wall;
                } else if (T == 'f'){
                    std::cout << "saving Fins\n";
                    fins f = props.f;
                } else if (T == 't'){
                    std::cout << "saving Bodytube\n";
                    tube* t;
                    //*t = std::get<tube>(props);
                    *t = props.t;
                    j["props"]["wall"] = t->wall;
                    delete t;
                }
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
            //Rocket(){}
            //file constructor
            Rocket(std::string filename){
                std::cout << "Loading from file "<<filename<<"\n";
                std::ifstream file(filename);
                json j = json::parse(file); // load file as json
                for(auto i : j["components"]){
                    Component *c = new Component;
                    c->T = i["T"].get<char>();
                    //c->setLength() = i["length"].get<float>();
                    c->isExposed = i["isExposed"].get<bool>();
                    c->MaxDia = i["diam"].get<float>();
                    c->position = i["position"].get<float>();
                    c->name = i["name"].get<std::string>();
                    c->finish = i["finish"].get<int>();
                    prop_t k;
                    if(c->T == 'n'){
                        //nosecone
                        c->props.n.base = i["props"]["base"].get<float>();
                        c->props.n.gen = i["props"]["gen"].get<int>();
                        c->props.n.param = i["props"]["param"].get<float>();
                        c->props.n.sbase = i["props"]["sbase"].get<float>();
                        c->props.n.slength = i["props"]["slength"].get<float>();
                        c->props.n.wall = i["props"]["wall"].get<float>();
                    } else if (c->T == 't'){
                        //bodytube
                        c->props.t.wall = i["props"]["wall"].get<float>();
                    } else if (c->T == 'f'){
                        //fins
                        //k.f.typ = 0;
                    } else {
                        continue;
                    }
                    addComponent(c);
                    delete c;
                }
                file.close();
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
            //update the total fields & check for errors
            void update(){
                Tlength,Tmass, MXdiam = 0;
                //float lastdiam;
                //std::vector<float> diams;
                for(auto i : components){
                    Numcomponents ++;
                    Tlength = Tlength + i.getLength(); //accumulate lengths
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
                for(auto i:components){
                //for(int i=0; i<components.size();i++){
                    std::cout << i.getLength() << "\n";
                    //std::cout << i.get_json().dump(4) << "\n";
                    j["components"].push_back(i.get_json());
                    //std::cout << components[i].length;
                    //std::cout << components[i].get_json().dump(4) << "\n";
                    //j["components"].push_back(components[i].get_json());
                }
                std::cout << j.dump(4) << "\n";
                //file << j.dump(4) << std::endl;
                file.close(); 
            }

    };
}
#endif