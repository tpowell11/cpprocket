#include <iostream>
#include <fstream>
#include <sstream>
#include "json.hpp" //nlohmann::json
#include "HTTPRequest.hpp" //https://github.com/elnormous/HTTPRequest
using namespace nlohmann;
json getGeo(float lat, float lon){
    std::ostringstream ss;
    ss << "api.openweathermap.org/data/2.5/weather?lat={" << lat << "}&lon={" << lon << "}&appid={}";
    http::Request r{ss.str()};
}