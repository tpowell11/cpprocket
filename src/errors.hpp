#ifndef ERRORS
#define ERRORS
#include <exception>
namespace err{
    class NoDiameter:public std::exception{
        const char* what() const throw(){
            return "Passed component does not have a diameter or has a diameter of 0";
        }
    };
    class NoMaterial:public std::exception{
        const char* what() const throw(){
            return "Passed components does not have a material assigned";
        }
    };
    class NotYetImplemented:public std::exception{
        const char* what() const throw(){
            return "This item is not yet implemented";
        }
    };
    class MaterialNotFound:public std::exception{
        const char* what() const throw(){
            return "The supplied mataerial name is not assigned in dat/materials.jsonc";
        }
    };
}
#endif