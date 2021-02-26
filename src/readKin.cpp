#include "../include/readKin.h"

// load root libraries!
#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TProfile.h"

//#define kin_path "data/ibd/ibdConstAlongEne_FromZ1_10000.kin"
#define kin_path "data/ibd/ibdConstAlongEne1_FromZ1_10000.kin"
#define out_path "output/ROOT/kin_results.root"

#define EN_TH 2

// Read the Kin files, make them into event structs
kin_ibd_event::kin_ibd_event(std::ifstream &kinfile){
    if(!kinfile.good()){
        cerr << "BAD INPUT FILE!" << endl;
        return;
    }
    // begin
    string line;
    getline(kinfile, line);
    if(line != KIN_BEGIN) 
        cerr << "Wrong kin format" << endl;
    // skip nuance and vertex line;
    getline(kinfile, line);
    getline(kinfile, line);
    
    // read track lines
    char label[50];
    int pdg_n;
    double energy;
    double x, y, z;
    int simulated;

    getline(kinfile, line);
    while(line != KIN_END){
        sscanf(line.c_str(), "$ %s %d %le %le %le %le %d", 
                label, &pdg_n, &energy, &x, &y, &z, &simulated);
        if(pdg_n == NUEBAR){
            this->nuebar_en = energy;
        }
        else if(pdg_n == PROTON){} // don't care about protons..

        else if(pdg_n == EBAR && simulated == 0){
            this->ebar_en = energy;

            ebar_dir[XDIR] = x;
            ebar_dir[YDIR] = y;
            ebar_dir[ZDIR] = z;
            //cerr << z << endl;
        }

        else if(pdg_n == NEUTRON && simulated == 0){
            this->n_en = energy;

            n_dir[XDIR] = x;
            n_dir[YDIR] = y;
            n_dir[ZDIR] = z;
        }

        getline(kinfile, line);

    }// endwhile
    initialize();
    //cerr<< p_total - nuebar_en <<endl;
    //cerr << (n_en) << endl;

}



int main(){
    TFile * outfile = new TFile(out_path, "recreate", "Energy Angular Distribution");
    TTree * tree = new TTree("kin_tree","kin file tree");

    //set up tree
    Float_t nuebar_energy;
    Float_t ebar_energy;
    Float_t ebar_cos;   // deflection angle of the positron
    Float_t ex, ey, ez; // direction of positron
    Float_t nx, ny, nz; // direction of neutron
    

    tree->Branch("nuebar_energy", &nuebar_energy, "nuebar_energy/F");
    tree->Branch("ebar_energy", &ebar_energy, "ebar_energy/F");

    tree->Branch("ebar_cos", &ebar_cos, "ebar_cos/F");


    tree->Branch("ex", &ex, "ex/F");
    tree->Branch("ey", &ey, "ey/F");
    tree->Branch("ez", &ez, "ez/F");

    tree->Branch("nx", &nx, "nx/F");
    tree->Branch("ny", &ny, "ny/F");
    tree->Branch("nz", &nz, "nz/F");



    ifstream kinfile(kin_path);

    int nEvents = 0;

    while(kinfile.peek()!=EOF){
        
        kin_ibd_event current(kinfile);
        nuebar_energy = current.nuebar_en;

        ebar_energy = current.ebar_en;
        ebar_cos = current.getCosEbar();

        if(nuebar_energy < EN_TH) continue;

        ex = current.ebar_dir[XDIR];
        ey = current.ebar_dir[YDIR];
        ez = current.ebar_dir[ZDIR];

        nx = current.n_dir[XDIR];
        ny = current.n_dir[YDIR];
        nz = current.n_dir[ZDIR];

        nEvents++;
        tree->Fill();
    }

    cout << "Total Events: " << nEvents << endl;

    tree->Write();
    outfile->Close();
    return 0;
    
}
