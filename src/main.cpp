// Calculation program for model rocketry (c) Tom Powell
// THIS PROGRAM USES M.K.S. UNITS 

#include <stdio.h>
#include <iomanip> //for std::setw
#include "rocket.hpp"
//#include "materialmanager.hpp"
namespace cui{
    //simple prompted input
    template<typename t>
    inline t const promptInput(const char* prompt){
        std::cout << prompt;
        t inp; 
        std::cin >> inp;
        return inp;
    }
//* print help from file
    void printHelp(){
        std::ifstream f("help.txt");
        if(f.is_open()){
            std::cout << f.rdbuf() << "\n";
        }
    }
//* print version number
    void printBuild(){
        std::ifstream f("../dat/version.txt");
        if(f.is_open()){
            std::cout << f.rdbuf() << "\n";
        }
    }
//* reusable prompt strings

    const char* constMethodPrompt = "Select Construction Method:\n"
    "[1]Preset (dat/presets.json)\n"
    "[2]Parameter input\n"
    "c]";
    const char* matMethodPrompt = "Choose material selection method:\n"
    "[1]Existing (dat/materials.json)\n"
    "[2]Create new\nmaterial]";
    const char* presetPrompt = "Enter Preset Name\nmaterial]";
//* selection dialogs

    int selectComponentType(){
        const char* s = 
        "Select component type to add\n"
        "[1]Nosecone\n"
        "[2]Bodytube\n"
        "[3]Switch to internal components\ncomponents]";
        int inp = cui::promptInput<int>(s);
        return inp;
    }
//* component construction dialogs

    //builds a nosecone
    void addNoseo(rocket::Rocket r){
        int inp = promptInput<int>(constMethodPrompt);
        std::string pName;
        rocket::Nosecone o;
        switch(inp){
            case 1:
                o.loadPreset(cui::promptInput<char*>("Preset name\nnosecone]"));
                break;
            case 2:
                std::cout << "Enter Generator type:\n"
                "[1]Conical\n"
                "[2]Ogive\n"
                "[3]Ellipsoid\n"
                "[4]Haack Series\n"
                "[5]Power Series\nnosecone]";
                std::cin >> o.generator;
                o.gLength = cui::promptInput<float>("Enter Length of Curved Portion:\nnosecone]");
                o.gDiameter = cui::promptInput<float>("Enter base diameter of curved portion:\nnosecone]");
                o.gParameter = cui::promptInput<float>("Enter the shape parameter for the generator:\nnosecone]");
                o.isShoulder = cui::promptInput<float>("Does the noseo have a shoulder 0: No,1: Yes\nnosecone]");
                if(o.isShoulder){
                    o.sLength = cui::promptInput<float>("Enter length of shoulder:\nn->shoulder]");
                    o.sDiameter = cui::promptInput<float>("Enter diameter of shoulder:\nn->shoulder]");
                }
                int matMethod = cui::promptInput<int>(cui::matMethodPrompt);
                switch(matMethod){
                    case 1: 
                        while (1){
                            try {
                                o.material = mat::getMaterialS(cui::promptInput<std::string>("Enter material name:\nn->material]"));
                                break;
                            } catch (const std::exception& e){
                                std::cout << "\nMaterial not found.\n";
                            }
                        }
                    case 2:
                        o.material.name = cui::promptInput<std::string>("Enter material name:\n]");
                        o.material.props.density = cui::promptInput<float>("Enter density:\n]");
                        o.material.props.compressiveStrength.s = cui::promptInput<float>("Compressive strength (megapascals):\n]");
                }
                mat::writeMaterial<mat::StructureMaterial>(o.material, mat::data); //send new material to data
                o.name = cui::promptInput<std::string>("Component name (default 'Noseo')\n]");
        }
        r.appendComponent<rocket::Nosecone>(o);
    }

    //builds a body
    void addBodyo(rocket::Rocket r){
        int inp = promptInput<int>(constMethodPrompt);
        rocket::Bodytube o;
        switch(inp){
            case 1:
                char pName[64];
                std::cin >> pName;
                o.loadPreset(pName);
            case 2:
                o.length = cui::promptInput<float>("Enter length of o:\nbodytube]");
                o.innerdiameter = cui::promptInput<float>("Enter inner diameter of obodytubedyo]");
                o.outerdiameter = cui::promptInput<float>("Enter outer diameter of obodytubedyo]");
                int matMethod = cui::promptInput<int>(cui::matMethodPrompt);
                switch(matMethod){
                    case 1:
                        o.material = mat::getMaterialS(cui::promptInput<std::string>("Enter material name:\nb->material]"));
                    case 2:
                        o.material.name = cui::promptInput<std::string>("Enter material name:\n]");
                        o.material.props.density = cui::promptInput<float>("Enter density:\n]");
                        o.material.props.compressiveStrength.s = cui::promptInput<float>("Compressive strength (megapascals):\n]");
                }  
        }
        r.appendComponent<rocket::Bodytube>(o);

    }

}



int main(int argc, char* argv[]) {
    //setup
    // mat::StructureMaterial mat = mat::StructureMaterial("balsa");
    // rocket::Component test(mat,"yeet");
    rocket::Bodytube testt("T70-H","templatecone");
    rocket::Nosecone cone("test","templatetube");
    rocket::Rocket rock;
    rock.appendComponent(cone);
    // rock.AddComponent(test);
    rock.appendComponent(testt);
    // rock.printComponents();
    //application loop
    std::cout << "Enter command, h for help, q to quit\n";
    std::cout << "Version 0.0."; 
    cui::printBuild();
    //std::cout << "\n"; 
    while(1){
        std::string inp = cui::promptInput<std::string>("]");
        if(inp.at(0) == 'c' || inp.at(0) == 'C'){
            switch(cui::selectComponentType()){
                case 1:
                    cui::addNoseo(rock);
                case 2:
                    break;
                case 3:
                    break;
            }
        } else if(inp.at(0) == 'l' || inp.at(0) == 'L'){
            rock.show();
        } else if(inp.at(0) == 'h' || inp.at(0) == 'H'){
            cui::printHelp();
        } else if(inp.at(0) == 'q' || inp.at(0) == 'Q'){
            mat::saveMaterialsFile(mat::data);
            exit(0);
        } else if(inp.at(0) == 'v' || inp.at(0) == 'V'){
            std::cout << "Version 0.0.";
            cui::printBuild();
        }
    }
}