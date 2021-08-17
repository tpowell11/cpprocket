
#include <stdio.h>
#include <iostream>
#include <signal.h> //sigint handler
#include <iomanip> //for std::setw
#include "rocket.hpp"
//#include "HTTPRequest.hpp"
#include "weatherapi.hpp"
int main() {
    // try{
    //     // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
    //     http::Request request{"api.openweathermap.org/data/2.5/weather?lat={60}&lon={80}&appid={6bebae05507a8e23ad8061613f1b2274}"};

    //     // send a get request
    //     const auto response = request.send("GET");
    //     std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; // print the result
    // } catch (const std::exception& e){
    //     std::cerr << "Request failed, error: " << e.what() << '\n';
    // }
    nlohmann::json j;
    j = getGeo(40.2, 45.3);
    std::cout << j.dump(4)<< "\n";
}