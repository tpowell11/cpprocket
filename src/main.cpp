// Calculation program for model rocketry (c) Tom Powell
// THIS PROGRAM USES M.K.S. UNITS 

#include <stdio.h>
#include <iostream>
#include <signal.h> //sigint handler
#include <iomanip> //for std::setw
#include "../inc/rocket.hpp"

void on_sigint(int sig){
    // save to temp.json on control-c
    exit(0);
}

namespace cui{
    //simple prompted input
    const char* f = 0;
    template<typename t>
    inline t const promptInput(const char* prompt = f){
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
        "[3]Switch to internal components\ncomponents]";
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
    c.finish = 4.0f;
    std::cout << (float)c.finish << "\n";
    c.name = (std::string)"first";
    //c.length = 34.0f;
    c.setLength(45.0f);
    c.setType('n');
    //std::get<0>(c.props).base=100; //set nosecone wall of first to 100
    r.addComponent(c);

    // std::cout<<"2" << "\n";
    // rocket::Component cc = rocket::Component();
    // cc.length = 45.0f;
    // cc.MaxDia = 4.0f;
    // cc.position = 4.0f;
    // cc.name = "second";
    // cc.setType('t'); 
    // //r.addComponent(cc);

    // std::cout << "3" << "\n";
    // rocket::Component ccc = rocket::Component();
    // ccc.length = 556.0f;
    // ccc.setType('t');
    // //std::get<2>(ccc.props).wall=300;
    // r.addComponent(ccc);

    // std::cout << "4" << "\n";
    // rocket::Component cccc = rocket::Component();
    // cccc.length = 44543.0f;
    // cccc.setType('n');
    // //std::get<0>(ccc.props).base=346;
    // r.addComponent(cccc);

    std::cout << "Enter command, h for help, q to quit\n";
    std::cout << "Version 0.0.b"; 
    cui::printBuild();
    while(1){
        std::string inp = cui::promptInput<std::string>("]");
        if(inp.at(0) == 'c' || inp.at(0) == 'C'){
            
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
            r.save("TEST.json");
        }
    }
}