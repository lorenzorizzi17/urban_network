#include<iostream>
#include<fstream>
#include<vector>
#include<math.h>
#include"TGraph.h"

void DisplayHisto(std::string s) {

    gROOT->SetStyle("Plain");
    gStyle->SetPalette(57);
    gStyle->SetOptTitle(0);
    gStyle->SetStatBorderSize(4);
    gStyle->SetStatX(0.9);
    gStyle->SetStatY(0.9);
    gStyle->SetStatW(0.5); 
    gStyle->SetStatH(0.2);
    
    TH1F* histos = new TH1F("", "", 100, 0, 2000);;
    std::string name = "Histogram ";;
    char const *histName = name.c_str();
    histos->SetNameTitle(histName,"Flux in: 9");
    histos->SetFillColor(kBlue);
    histos->SetLineColor(kBlack);
    histos->SetMarkerStyle(kFullCircle);
    histos->SetMarkerSize(0.8);
    histos->GetYaxis()->SetTitleOffset(1.2);
    histos->GetXaxis()->SetTitleSize(0.04);
    histos->GetYaxis()->SetTitleSize(0.04);
    histos->StatOverflows(true);
    histos->SetLabelSize(0.04);

    std::ifstream myfile(s, std::ifstream::in);
    while(myfile.good()){
        double x = 0;
        myfile >> x;
        histos->Fill(x);
    }
    myfile.close();

    histos->Draw();
}


void processGeneralGraph(){

    int const N = 24;
    double Ns[N] = {1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300,3400, 3500, 3600, 3700, 3800};
    double errNs[N];
    double p[N];
    double errp[N];

    for (int ix = 0; ix < N; ix++){
        std::ifstream myfile;
        myfile.open("/home/lrizzi17/UrbanNetwork/data/general/T10000/general_T10000_N" + std::to_string(int(Ns[ix])) + ".txt");
        int tot_t = 0;
        int tot_f = 0;
        while(myfile.good()){
            int t, f;
            std::string placeholder1, placeholder2;
            myfile >> placeholder1 >> t >> placeholder2 >> f;
            tot_t += t;
            tot_f += f;
        }
        myfile.close();
        int tot = tot_t+tot_f;
        p[ix] = (double(tot_t)/tot);
        errp[ix] = std::sqrt(double(tot_t))/tot;
        errNs[ix] = 0;
    }

    //std::ifstream myfile("./data/general_T10000.txt", std::ifstream::in);
    TGraphErrors* gr = new TGraphErrors(N, Ns, p, errNs, errp);
    gr->SetMarkerStyle(21);
    gr->SetMarkerSize(0.8);
    gr->SetMarkerColor(kRed);
    gr->SetLineColor(kRed);
    gr->SetTitle("Gridlocks");
    gr->GetXaxis()->SetTitle("Traffic load (N)");
    gr->GetYaxis()->SetTitle("Percentage of gridlocks");
    gr->Draw("AP");
}

void DisplayGraph(){
    // read a graph from a file

    TGraphErrors* gr = new TGraphErrors("./ROOTRoutines/general_T10000.txt", "%lg %lg %lg %lg");
    gr->SetMarkerStyle(21);
    gr->SetMarkerSize(0.8);
    gr->SetMarkerColor(kRed);
    gr->SetLineColor(kRed);
    gr->SetTitle("General");
    gr->GetXaxis()->SetTitle("X");
    gr->GetYaxis()->SetTitle("Y");
    gr->Draw("AP");
}



