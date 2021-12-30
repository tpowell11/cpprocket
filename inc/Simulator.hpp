#include <vector>
#include <string>
#include <functional>
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
        Svec Fd;
        Svec PropMass;
    };
    struct Wind{
        float v;
        float d;
    };
    struct SimAtmosphericConfig{
        std::function<float(float)> rho;
        Wind w; 
    };
    struct SimConfig{
        float dt;
        float g=9.81;
        int maxIT, maxT;
    };
    class Simulator{
        private:
            SimResultVectors R;
            SimAtmosphericConfig A;
            rocket::Rocket Ro;
        public:
            Simulator(rocket::Rocket Ro, SimAtmosphericConfig A){
                this->Ro = Ro;
                this->A = A;
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
                for (auto i : R.Fd) {
                    j["Fd"].push_back(i);
                }
                for (auto i : R.PropMass) {
                    j["PropMass"].push_back(i);
                }
                std::ofstream file(name, std::ofstream::out);
                file << j.dump(4) << std::endl;
                file.close();

            }
            void RunEuler(SimConfig conf) {
                //run Euler simulation
                float t, Ft, Fd, Fn, A, dV, V, dX, X; //currents
                float lV, lA, lMm, lX; //lasts
                float M = this->Ro.mass();
                float Mm = this->Ro.Motor.Mp;
                float Cd = this->Ro.Cd;
                while(t<=conf.maxT){
                    // thrust force peicewise
                    if (t <= this->Ro.Motor.Tb) {
                        Ft=this->Ro.Motor.Isp/this->Ro.Motor.Tb;
                    } else {
                        Ft=0;
                    }
                    float k = pow((float)PI*(float)0.5*Cd*(conf.g/2),2);
                    Fd = k*lV; //calc. force of drag
                    Mm = lMm-(this->Ro.Motor.Mp/this->Ro.Motor.Tb)*conf.dt; //calc. motor mass
                    Fn = Ft+Fd-(M+Mm)*conf.g; //calc. net force
                    A = Fn/(M+Mm);
                    dV = conf.dt*A;
                    V = lV + dV;
                    dX = conf.dt*V;
                    X = lX + dX;
                    //DATA MANAGMENT
                    this->R.T.push_back(t);
                    this->R.A.push_back(A);
                    this->R.V.push_back(V);
                    this->R.H.push_back(X);
                    this->R.Fd.push_back(Fd);
                    this->R.PropMass.push_back(Mm);
                    //INCRMENTS FOR NEXT LOOP
                    t = conf.dt + t; //inc. time
                }
                
            }
    };
} // namespace Simulator
