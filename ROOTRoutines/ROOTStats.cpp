#include<iostream>
#include<fstream>
#include<vector>
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

    
    TH1F* histos = new TH1F("", "", 100, 0, 100);;
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



