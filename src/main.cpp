// Calculation program for model rocketry (c) Tom Powell
// THIS PROGRAM USES M.K.S. UNITS 

#include <stdio.h>
#include <iostream>
#include <signal.h> //sigint handler
#include <iomanip> //for std::setw
#include <variant>
#include <string>
#include "../inc/rocket.hpp"
#include "../inc/cui.hpp"

void on_sigint(int sig){
    // save to temp.json on control-c
    exit(0);
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