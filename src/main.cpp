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
//* reusable prompt strings

    const char* constMethodPrompt = "Select Construction Method:\n"
    "[1]Preset (dat/presets.json)\n"
    "[2]Parameter input\n"
    "]";
    const char* matMethodPrompt = "Choose material selection method:\n"
    "[1]Existing (dat/materials.json)\n"
    "[2]Create new\n]";
    const char* presetPrompt = "Enter Preset Name\n]";
//* selection dialogs

    int selectComponentType(){
        int inp;
        const char* s = 
        "Select component type to add\n"
        "[1]Nosecone\n"
        "[2]Bodytube\n"
        "[3]Switch to internal components\n]";
        std::cout << s;
        std::cin >> inp;
        return inp;
    }
//* component construction dialogs

    //builds a nosecone
    void addNosecone(rocket::Rocket rocket){
        int inp = promptInput<int>(constMethodPrompt);
        std::string pName;
        rocket::Nosecone cone;
        switch(inp){
            case 1:
                cone.loadPreset(cui::promptInput<char*>("Preset name\n]"));
                break;
            case 2:
                std::cout << "Enter Generator type:\n"
                "[1]Conical\n"
                "[2]Ogive\n"
                "[3]Ellipsoid\n"
                "[4]Haack Series\n"
                "[5]Power Series\n]";
                std::cin >> cone.generator;
                cone.gLength = cui::promptInput<float>("Enter Length of Curved Portion:\n]");
                cone.gDiameter = cui::promptInput<float>("Enter base diameter of curved portion:\n]");
                cone.gParameter = cui::promptInput<float>("Enter the shape parameter for the generator:\n]");
                cone.isShoulder = cui::promptInput<float>("Does the nosecone have a shoulder 0: No,1: Yes\n]");
                if(cone.isShoulder){
                    cone.sLength = cui::promptInput<float>("Enter length of shoulder:\n]");
                    cone.sDiameter = cui::promptInput<float>("Enter diameter of shoulder:\n]");
                }
                int matMethod = cui::promptInput<int>(cui::matMethodPrompt);
                switch(matMethod){
                    case 1: 
                        cone.material = mat::getMaterialS(cui::promptInput<std::string>("Enter material name:\n]"));
                    case 2:
                        cone.material.name = cui::promptInput<std::string>("Enter material name:\n]");
                        cone.material.props.density = cui::promptInput<float>("Enter density:\n]");
                        cone.material.props.compressiveStrength.s = cui::promptInput<float>("Compressive strength (megapascals):\n]");
                }
                mat::writeMaterial<mat::StructureMaterial>(cone.material, mat::data); //send new material to data
                cone.name = cui::promptInput<std::string>("Component name (default 'Nosecone')\n]");
        }
        rocket.addComponent(cone);
    }

    //builds a bodytube
    void addBodytube(rocket::Rocket r){
        int inp = promptInput<int>(constMethodPrompt);
        rocket::Bodytube tube;
        switch(inp){
            case 1:
                char pName[64];
                std::cin >> pName;
                tube.loadPreset(pName);
            case 2:
                tube.length = cui::promptInput<float>("Enter length of tube:\n]");
                tube.innerdiameter = cui::promptInput<float>("Enter inner diameter of tube:\n]");
                tube.outerdiameter = cui::promptInput<float>("Enter outer diameter of tube:\n]");   
        }

    }

}



int main(int argc, char* argv[]) {
    //setup
    // mat::StructureMaterial mat = mat::StructureMaterial("balsa");
    // rocket::Component test(mat,"yeet");
    // rocket::Bodytube testt("T70-H","templatetube");
    // rocket::Nosecone cone("test","templatecone");
    rocket::Rocket rock;
    // rock.AddComponent(cone);
    // rock.AddComponent(test);
    // rock.AddComponent(testt);
    // rock.printComponents();
    //application loop
    std::cout << "Enter command, h for help, q to quit\n";
    while(1){
        std::string inp = cui::promptInput<std::string>("]");
        if(inp.at(0) == 'c' || inp.at(0) == 'C'){
            switch(cui::selectComponentType()){
                case 1:
                    cui::addNosecone(rock);
                case 2:
                    break;
                case 3:
                    break;
            }
        } else if(inp.at(0) == 'l' || inp.at(0) == 'L'){
            //rock.printComponents();
        } else if(inp.at(0) == 'h' || inp.at(0) == 'H'){
            cui::printHelp();
        
        } else if(inp.at(0) == 'q' || inp.at(0) == 'Q'){
            mat::saveMaterialsFile(mat::data);
            exit(0);
        }
    }
}