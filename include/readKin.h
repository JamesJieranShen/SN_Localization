#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

#define IBD -93
#define NUEBAR -12
#define EBAR -11
#define PROTON 2212
#define NEUTRON 2112

#define e0_n 939.565    // rest mass of neutron
#define e0_e 0.511      // rest mass of electron

#define XDIR 0
#define YDIR 1
#define ZDIR 2

// kinfile event begin/end
#define KIN_BEGIN   "$ begin"
#define KIN_END     "$ end"


using namespace std;

struct kin_ibd_event{
    /**
     * @brief Record the important information in a simulated event, from the .kin files
     *      Data is given in a reference frame where the incoming neutrino has direction -z
     *      Theta   angle between detection vector and +z
     */

    double nuebar_en;       // neutrino energy
    double ebar_en;         // positron energy
    double n_en;            // neutron energy

    double p_n;             // momentum of neutron
    double p_ebar;          // momentum of positron

    double ebar_dir[3];     // positron, x, y, z; normalized
    double n_dir[3];        // neutron , x, y, z; normalized
    double u_dir[3];        // momentum vector, normalized

    double p_total;         // total momentum of positron and neutron

    
    
    kin_ibd_event(std::ifstream&);
    kin_ibd_event(double nuebar_en, double ebar_en, double n_en, double ex, double ey, double ez, double nx, double ny, double nz){
        this->nuebar_en = nuebar_en;
        this->ebar_en = ebar_en;
        this->n_en = n_en;

        ebar_dir[XDIR] = ex;
        ebar_dir[YDIR] = ey;
        ebar_dir[ZDIR] = ez;

        n_dir[XDIR]    = nx;
        n_dir[YDIR]    = ny;
        n_dir[ZDIR]    = nz;
        initialize();
    }


    double momentumFromEnergy(double energy, double E0){
        return sqrt(pow(energy, 2)-pow(E0, 2));

    }

    void initialize(){
        // re-written to get sum of momentum
        p_n = momentumFromEnergy(n_en, e0_n);
        p_ebar = momentumFromEnergy(ebar_en, e0_e);

        double norm_factor = 0;


        for(int i = 0; i < 3; i++){
            u_dir[i] = n_dir[i] * p_n + ebar_dir[i] * p_ebar;
            norm_factor += u_dir[i] * u_dir[i]; // norm is effectively overall momentum squared
        }

        this->p_total = sqrt(norm_factor);

        /* // u = n-ebar
        for(int i = 0; i < 3; i++){
            u_dir[i] = n_dir[i] - ebar_dir[i];
            // calculate normalization along the way...
            norm_factor += u_dir[i] * u_dir[i];
        }*/

        // normalize;
        for(int i = 0; i < 3; i++){
            u_dir[i] /= sqrt(norm_factor);
        } 
    }
    

    double getCosTheta(){
        return u_dir[ZDIR];
    }

    
    double getTheta(){
    // angle away from the incoming angle of -PI
        //double cosAngle = getCosAngle();
        return acos(getCosTheta());
    }

    double getPhi(){
        return atan(u_dir[YDIR] / u_dir[XDIR]);
    }

    double getAngle(){
        return abs(M_PI - getTheta());
    }

    double getCosEbar(){
        return -ebar_dir[ZDIR];
    }


    string toString(){
        ostringstream ret;
        ret << "IBD\tEnergy:\t" << nuebar_en << "\t theta:\t" << getTheta();
        return ret.str();
    }
};
