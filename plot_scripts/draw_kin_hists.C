#define file_path "../output/ROOT/kin_results.root"
#include "root_headers.h"

void draw_kin_hists(){
    TFile * file = TFile::Open(file_path);
    TTree * tree;
    file->GetObject("kin_tree", tree);
    auto c1 = new TCanvas("c1", "c1", 600, 600);
    gStyle->SetOptStat(0);

    c1->SetWindowSize(600 + (600 - c1->GetWw()), 600 + (600 - c1->GetWh()));

    TH2D * xy_hist = new TH2D("xy_hist","vector radial scatter plot", 40, -1.2, 1.2, 40, -1.2, 1.2);
    
    tree->Draw("y:x >> xy_hist", "nuebar_energy > 2", "goff");

    
    xy_hist->Draw("");
    xy_hist->GetXaxis()->SetTitle("x");
    xy_hist->GetYaxis()->SetTitle("y");
    c1->Print((out_dir + "xy_plot.png").c_str());


    TH2D * en_hist = new TH2D("en_hist","cos(#theta) vs. e^{+} energy", 50, 0, 100, 20, -1, -0.5);
    TProfile * en_prof = new TProfile("en_prof","cos(#theta) vs. e^{+} energy", 20, 0, 100, -1, 1);

    tree->Draw("cosTheta:ebar_energy >> en_hist", "", "goff");
    //tree->Draw("cosTheta:ebar_energy >> en_prof", "", "goff");
    tree->Draw("ebar_cos:nuebar_energy >> en_prof", "", "goff");


    auto c2 = new TCanvas("c2", "c2", 800, 600);

    
    en_hist->Draw("colz");
    en_hist->GetXaxis()->SetTitle("e^{+} energy [MeV]");
    en_hist->GetYaxis()->SetTitle("cos(#theta)");
    c2->Print((out_dir + "en_plot.png").c_str());
    
    auto c3 = new TCanvas("c3", "c3", 800, 600);

    en_prof->GetXaxis()->SetTitle("e^{+} energy [MeV]");
    en_prof->SetMaximum(0.2);
    en_prof->SetMinimum(-0.2);
    en_prof->GetYaxis()->SetTitle("cos(#theta)");
    en_prof->Draw("");
    c3->Print((out_dir + "en_prof.png").c_str());

    //en_hist->Draw("prof");


    //tree->Draw("y:x >> xy_hist", "", "goff");
    //xy_hist->Draw("");


    // plot distribution of radial  
    auto c4 = new TCanvas("c4", "c4", 800, 600);
    TH1D * r_hist = new TH1D("r_hist", "x component of unit vector", 25, -0.5, 0.5);
    tree->Draw("x>>r_hist", "", "goff");
    r_hist->Draw();
    c4->Print((out_dir + "ux.png").c_str());

    TH1D * theta_hist = new TH1D("theta_hist", "cosTheta", 25, -1, -0.5);
    tree->Draw("cosTheta>>theta_hist", "", "goff");
    theta_hist->Draw();
    c4->Print((out_dir + "cosTheta.png").c_str());

    


}

