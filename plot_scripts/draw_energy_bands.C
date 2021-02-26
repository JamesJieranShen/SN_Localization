#define file_path "../output/ROOT/kin_results.root"
#include "root_headers.h"

void draw_energy_bands(){
    TFile * file = TFile::Open(file_path);
    TTree * tree;
    file->GetObject("kin_tree", tree);
    TCanvas* c;    
    TH1D * hist;
    gStyle->SetOptStat(0);
    
    char en_condition[50];
    char outpath[200];

    double en_low, en_high;
    for(int i = 0;  i < 10; i++){
        en_low = i * 10;
        en_high = (i+1) * 10;

        sprintf(en_condition, "energy > %.1f && energy < %.1f", en_low, en_high);
        sprintf(outpath, (out_dir+"%.1f, %.1f MeV.png").c_str(), en_low, en_high);


        hist = new TH1D("cos_hist","cos(#theta)", 20, 0, 1);
        c = new TCanvas("c", "c", 600, 600);
        c->SetWindowSize(600 + (600 - c->GetWw()), 600 + (600 - c->GetWh()));
        tree->Draw("sqrt(x*x + y*y) >> cos_hist", en_condition, "goff");
        hist->Draw();
        hist->Print();
        c->Print(outpath);

    }

    


}

