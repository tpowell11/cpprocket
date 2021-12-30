#include "../inc/materialmanager.hpp"
#include <iostream>

int main() {
    mat::matManager MM("../dat/materials.json");
    std::cout << MM.getMatfStr("balsa").density << "\n";
}