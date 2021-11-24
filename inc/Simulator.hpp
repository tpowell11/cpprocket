#include <vector>
#include <string>
#include "rocket.hpp"
#include "json.hpp"
using namespace nlohmann;
namespace Simulator
{
    typedef std::vector<float> Svec;
    struct SimResult {
        float apogee;
        float Tmax;
        float Vmax;
        float Amax;
    }; 
    struct SimResultVectors{
    //these must always be the same length or else 
    //file saving will break
        Svec T;
        Svec A;
        Svec V;
        Svec H;
        Svec dCd;
        Svec PropMass;
    };
    class Simulator{
        private:
            SimResultVectors R;
        public:
            Simulator(rocket::Rocket R, char type){
            }
            void save(std::string name){
            //saves the simulation result vectors to a json file
                json j;
                for (auto i : R.T){
                    j["T"].push_back(i);
                }
                for (auto i : R.A) {
                    j["A"].push_back(i);
                }
                for (auto i : R.V){
                    j["V"].push_back(i);
                }
                for (auto i : R.H) {
                    j["H"].push_back(i);
                }
                for (auto i : R.dCd) {
                    j["dCd"].push_back(i);
                }
                for (auto i : R.PropMass) {
                    j["PropMass"].push_back(i);
                }
                std::ofstream file(name, std::ofstream::out);
                file << j.dump(4) << std::endl;
                file.close();
            }
    };
} // namespace Simulator
