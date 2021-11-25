// Calculation program for model rocketry (c) Tom Powell
// THIS PROGRAM USES M.K.S. UNITS 

#include <stdio.h>
#include <iostream>
#include <signal.h> //sigint handler
#include <iomanip> //for std::setw
#include <variant>
#include <string>
#include "../inc/rocket.hpp"

void on_sigint(int sig){
    // save to temp.json on control-c
    exit(0);
}

namespace cui{
//* simple prompted input
    template<typename t>
    inline t const promptInput(std::string prompt){
        std::variant<t,char> inp;
        std::cout << prompt;
        std::cin >> std::get<t>(inp);
        return std::get<t>(inp);
    }
//* print help from file
    void printHelp(){
        std::ifstream f;
        f.open("help.txt");
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
        "[3]Fins"
        "\ncomponents]";
        int inp = cui::promptInput<int>(s);
        return inp;
    }


}

int main(int argc, char* argv[]) {
    // for(int i; i <= argc; i++){
    //     std::cout << argv[i] << "\n";
    //     std::cout << argc << "\n";
    //     if(argc = 1){

    //     }
    //     if(argv[i] == "-f"){
            
    //     }

    // }
    signal(SIGINT, on_sigint); //run on_sigint on SIGINT
    rocket::Rocket r;
    rocket::Component c;
    c.name = "nosecone";
    c.length = 45.7;
    c.setType('n');
    r.addComponent(c);
    rocket::Component cc;
    cc.name = "fins";
    cc.setType('f');
    r.addComponent(cc);
    while(1){
        std::string inp = cui::promptInput<std::string>("]");
        if(inp.at(0) == 'c' || inp.at(0) == 'C'){
            char choice = cui::promptInput<char>("Component Type:\n");
            rocket::Component *c = new rocket::Component;
            if (choice == 'b' || 'B'){
                c->setType('b');
                std::cout << "Selected Bodytube\n";
            } else if (choice == 'n' || 'N'){
                c->setType('n');
                std::cout << "Selected Nosecone\n";
            } else if (choice == 'f' || 'F'){
                c->setType('f');
                std::cout << "Selected Fins\n";
            }
        } else if(inp.at(0) == 'l' || inp.at(0) == 'L'){
            r.show();
        } else if(inp.at(0) == 'h' || inp.at(0) == 'H'){
            cui::printHelp();
        } else if(inp.at(0) == 'q' || inp.at(0) == 'Q'){
            //mat::save(mat::data);
            exit(0);
        } else if(inp.at(0) == 'v' || inp.at(0) == 'V'){
            std::cout << "Version 0.0.";
            cui::printBuild();
        } else if(inp.at(0) == 's' || inp.at(0) == 'S'){
            r.save("q.json");
        }
    }
}