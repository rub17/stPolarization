#include "AtlasStyle.C"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include <TSystem.h>
#include "TFile.h"
#include "TTreeReader.h"
#include <TLegend.h>
#include <TLatex.h>

void Plot(){
    gROOT->Reset();
    SetAtlasStyle();
    TString FilePath = "/Users/runyu/Documents/Git/CERN/stPolarization/rootFile/unwSample/";
    std::vector<TString> FileName = {"stZ.root","stZ0.root","stX.root","stX0.root","stY.root","stY0.root"};
    std::vector<TH1F*> histCosTheta;
    std::vector<TH1F*> histPhi;
    std::vector<TH1F*> histCosThetaStar;
    std::vector<TH1F*> histPhiStar;
    std::vector<TH1F*> histCosThetaZ;
    std::vector<TH1F*> histCosThetaX;
    std::vector<TH1F*> histCosThetaY;
    
    for (unsigned int i=0; i<FileName.size(); i++) {
        auto sTFile = TFile::Open(FilePath+FileName[i]);
        TTree *sTTree = nullptr;
        TTreeReader theReader("singleTop",sTFile);
        TTreeReaderValue<Float_t> eventNum(theReader, "Event.eventNumber");
        TTreeReaderValue<Float_t> _weight(theReader, "Event.weight");
        TTreeReaderValue<Float_t> _cosTheta(theReader,"fourangles.cos_Theta");
        TTreeReaderValue<Float_t> _phi(theReader,"fourangles.Phi_");
        TTreeReaderValue<Float_t> _cosThetaStar(theReader,"fourangles.cos_Theta_Star");
        TTreeReaderValue<Float_t> _phiStar(theReader,"fourangles.Phi_Star");
        TTreeReaderValue<Float_t> _top(theReader,"_top.id");

        TH1F* h1 = new TH1F("h1","CosTheta", 30,-1,1);
        TH1F* hPhi = new TH1F("hPhi","Phi", 30,0,2*M_PI);
        TH1F* h3 = new TH1F("h3","CosThetaStar", 25,-1,1);
        TH1F* h4 = new TH1F("h4","PhiStar", 25,0,2*M_PI);
        TH1F* hX =(TH1F*) sTFile->Get("hCosThetaX")->Clone("hX");
        TH1F* hY =(TH1F*) sTFile->Get("hCosThetaY")->Clone("hY");
        TH1F* hZ =(TH1F*) sTFile->Get("hCosThetaZ")->Clone("hZ");
        int counter = 0;
        while(theReader.Next()){
            auto event = *eventNum;
            auto weight = *_weight;
            auto topid = *_top;
            if (topid > 0) { //Producing only top plots
                Float_t cosTheta = *_cosTheta;
                h1->Fill(cosTheta,weight);
                Float_t phi = *_phi;
                hPhi->Fill(phi,weight);
                Float_t cosThetaStar = *_cosThetaStar;
                h3->Fill(cosThetaStar,weight);
                Float_t phiStar = *_phiStar;
                h4->Fill(phiStar,weight);
                counter++;
            }
        }
        std::cout << counter << std::endl;
        TCanvas *c1 = new TCanvas("c1","CosTheta",0.,0.,800,600);
        h1->Scale(1/h1->Integral());
        h1->SetMinimum(0.);
        h1->SetMaximum(0.07);
        h1->GetXaxis()->SetTitle("cos(#theta)");
        h1->GetYaxis()->SetTitle("Arbitrary Units");
        h1->Draw("Norm");
        c1->SaveAs("./plots/cosTheta"+FileName[i]+".pdf");
        
        TCanvas *c2 = new TCanvas("c2","Phi",0.,0.,800,600);
        hPhi->Scale(1/hPhi->Integral());
        hPhi->SetMinimum(0.);
        hPhi->GetXaxis()->SetTitle("#phi");
        hPhi->GetYaxis()->SetTitle("Arbitrary Units");
        hPhi->Draw("Norm");
        c2->SaveAs("./plots/phi"+FileName[i]+".pdf");

        TCanvas *c3 = new TCanvas("c3","CosThetaStar",0.,0.,800,600);
        h3->Scale(1/h3->Integral());
        h3->SetMinimum(0.);
        h3->SetMaximum(0.07);
        h3->GetXaxis()->SetTitle("cos(#theta*)");
        h3->GetYaxis()->SetTitle("Arbitrary Units");
        h3->Draw("Norm");
        c3->SaveAs("./plots/cosThetaStar"+FileName[i]+".pdf");

        TCanvas *c4 = new TCanvas("c4","PhiStar",0.,0.,800,600);
        h4->Scale(1/h4->Integral());
        h4->SetMinimum(0.);
        h4->GetXaxis()->SetTitle("#phi*");
        h4->GetYaxis()->SetTitle("Arbitrary Units");
        h4->Draw("Norm");
        c4->SaveAs("./plots/phiStar"+FileName[i]+".pdf");

        TCanvas *c5 = new TCanvas("c5","CosThetaX",0.,0.,800,600);
        hX->Scale(1/hX->Integral());
        hX->SetMinimum(0.);
        hX->Draw("Norm");
        TCanvas *c6 = new TCanvas("c6","CosThetaY",0.,0.,800,600);
        hY->Scale(1/hY->Integral());
        hY->SetMinimum(0.);
        hY->Draw("Norm");
        TCanvas *c7 = new TCanvas("c7","CosThetaZ",0.,0.,800,600);
        hZ->Scale(1/hZ->Integral());
        hZ->SetMinimum(0.);
        hZ->Draw("Norm");
        
        histCosTheta.push_back(h1);
        histPhi.push_back(hPhi);
        histCosThetaStar.push_back(h3);
        histPhiStar.push_back(h4);
        histCosThetaX.push_back(hX);
        histCosThetaY.push_back(hY);
        histCosThetaZ.push_back(hZ);
    }
 
    TCanvas *c8 = new TCanvas("c8","Z-Observable",0.,0.,800,600);
    histCosThetaZ[0]->SetMarkerSize(1);
    histCosThetaZ[0]->SetMarkerStyle(26);
    histCosThetaZ[0]->SetMarkerColor(kPink);
    histCosThetaZ[0]->SetLineColor(kOrange-1);
    histCosThetaZ[0]->GetXaxis()->SetTitle("Cos(#theta_{z})");
    histCosThetaZ[0]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosThetaZ[0]->Rebin();
    histCosThetaZ[0]->Draw("Norm");
    histCosThetaZ[1]->SetMarkerStyle(32);
    histCosThetaZ[1]->SetMarkerSize(1);
    histCosThetaZ[1]->SetMarkerColor(kViolet+2);
    histCosThetaZ[1]->SetLineColor(kBlue-7);
    histCosThetaZ[1]->Rebin();
    histCosThetaZ[1]->Draw("SAME*Norm");
    histCosThetaZ[6]->Rebin();
    histCosThetaZ[6]->Draw("SAME*Norm");
    auto* legend = new TLegend(0.5, 0.8, .65, .9);
    legend->AddEntry(histCosThetaZ[0], "P = +Pz", "P");
    //legend->AddEntry((TObject*)0, "", "");
    legend->AddEntry(histCosThetaZ[1], "P = -Pz", "P");
    //legend->AddEntry((TObject*)0, "", "");
    legend->AddEntry(histCosThetaZ[6], "Standard Model" , "P");
    legend->Draw();
    c8->SaveAs("./plots/CosThetaZ.pdf");
    
    TCanvas *c9 = new TCanvas("c9","X-Observable",0.,0.,800,600);
    histCosThetaX[2]->SetMarkerSize(1);
    histCosThetaX[2]->SetMarkerStyle(26);
    histCosThetaX[2]->SetMarkerColor(kPink);
    histCosThetaX[2]->SetLineColor(kOrange-1);
    histCosThetaX[2]->GetXaxis()->SetTitle("Cos(#theta_{x})");
    histCosThetaX[2]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosThetaX[2]->Rebin();
    histCosThetaX[2]->Draw();
    histCosThetaX[3]->SetMarkerStyle(32);
    histCosThetaX[3]->SetMarkerSize(1);
    histCosThetaX[3]->SetMarkerColor(kViolet+2);
    histCosThetaX[3]->SetLineColor(kBlue-7);
    histCosThetaX[3]->Rebin();
    histCosThetaX[3]->Draw("SAME*Norm");
    histCosThetaX[6]->Rebin();
    histCosThetaX[6]->Draw("SAME*Norm");
    auto* legend1 = new TLegend(0.5, 0.8, .65, .9);
    legend1->AddEntry(histCosThetaX[2], "P = +Px", "P");
    legend1->AddEntry(histCosThetaX[3], "P = -Px", "P");
    legend1->AddEntry(histCosThetaX[6], "Standard Model" , "P");
    legend1->Draw();
    c9->SaveAs("./plots/CosThetaX.pdf");
    
    TCanvas *c10 = new TCanvas("c10","Y-Observable",0.,0.,800,600);
    histCosThetaY[4]->SetMarkerSize(1);
    histCosThetaY[4]->SetMarkerStyle(26);
    histCosThetaY[4]->SetMarkerColor(kPink);
    histCosThetaY[4]->SetLineColor(kOrange-1);
    histCosThetaY[4]->GetXaxis()->SetTitle("Cos(#theta_{y})");
    histCosThetaY[4]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosThetaY[4]->Rebin();
    histCosThetaY[4]->Draw();
    histCosThetaY[5]->SetMarkerStyle(32);
    histCosThetaY[5]->SetMarkerSize(1);
    histCosThetaY[5]->SetMarkerColor(kViolet+2);
    histCosThetaY[5]->SetLineColor(kBlue-7);
    histCosThetaY[5]->Rebin();
    histCosThetaY[5]->Draw("SAME*Norm");
    histCosThetaY[6]->Rebin();
    histCosThetaY[6]->Draw("SAME*Norm");
    auto* legend2 = new TLegend(0.5, 0.8, .65, .9);
    legend2->AddEntry(histCosThetaY[4], "P = +Py", "P");
    //legend2->AddEntry((TObject*)0, "", "");
    legend2->AddEntry(histCosThetaY[5], "P = -Py", "P");
    //legend2->AddEntry((TObject*)0, "", "");
    legend2->AddEntry(histCosThetaY[6], "Standard Model" , "P");
    legend2->Draw();
    c10->SaveAs("./plots/CosThetaY.pdf");

    TCanvas *c11 = new TCanvas("c11","cosTheta_Z/-Z",0.,0.,800,600);
    histCosTheta[0]->SetMarkerSize(1);
    histCosTheta[0]->SetMarkerStyle(26);
    histCosTheta[0]->SetMarkerColor(kPink);
    histCosTheta[0]->SetLineColor(kOrange-1);
    histCosTheta[0]->GetXaxis()->SetTitle("Cos(#theta)");
    histCosTheta[0]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosTheta[0]->Draw();
    histCosTheta[1]->SetMarkerStyle(32);
    histCosTheta[1]->SetMarkerSize(1);
    histCosTheta[1]->SetMarkerColor(kViolet+2);
    histCosTheta[1]->SetLineColor(kBlue-7);
    histCosTheta[1]->Draw("SAME*Norm");
    auto* legend3 = new TLegend(0.5, 0.8, .65, .9);
    legend3->AddEntry(histCosTheta[0], "P = +Pz", "P");
    legend3->AddEntry(histCosTheta[1], "P = -Pz", "P");
    legend3->Draw();
    c11->SaveAs("./plots/CosThetaZ_Z0.pdf");

    TCanvas *c12 = new TCanvas("c12","cosTheta_X/-X",0.,0.,800,600);
    histCosTheta[2]->SetMarkerSize(1);
    histCosTheta[2]->SetMarkerStyle(26);
    histCosTheta[2]->SetMarkerColor(kPink);
    histCosTheta[2]->SetLineColor(kOrange-1);
    histCosTheta[2]->GetXaxis()->SetTitle("Cos(#theta)");
    histCosTheta[2]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosTheta[2]->Draw();
    histCosTheta[3]->SetMarkerStyle(32);
    histCosTheta[3]->SetMarkerSize(1);
    histCosTheta[3]->SetMarkerColor(kViolet+2);
    histCosTheta[3]->SetLineColor(kBlue-7);
    histCosTheta[3]->Draw("SAME*Norm");
    auto* legend4 = new TLegend(0.5, 0.8, .65, .9);
    legend4->AddEntry(histCosTheta[2], "P = +Px", "P");
    legend4->AddEntry(histCosTheta[3], "P = -Px", "P");
    legend4->Draw();
    c12->SaveAs("./plots/CosThetaX_X0.pdf");
    
    TCanvas *c13 = new TCanvas("c13","cosTheta_Y/-Y",0.,0.,800,600);
    histCosTheta[4]->SetMarkerSize(1);
    histCosTheta[4]->SetMarkerStyle(26);
    histCosTheta[4]->SetMarkerColor(kPink);
    histCosTheta[4]->SetLineColor(kOrange-1);
    histCosTheta[4]->GetXaxis()->SetTitle("Cos(#theta)");
    histCosTheta[4]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosTheta[4]->Draw();
    histCosTheta[5]->SetMarkerStyle(32);
    histCosTheta[5]->SetMarkerSize(1);
    histCosTheta[5]->SetMarkerColor(kViolet+2);
    histCosTheta[5]->SetLineColor(kBlue-7);
    histCosTheta[5]->Draw("SAME*Norm");
    auto* legend5 = new TLegend(0.5, 0.8, .65, .9);
    legend5->AddEntry(histCosTheta[4], "P = +Py", "P");
    legend5->AddEntry(histCosTheta[5], "P = -Py", "P");
    legend5->Draw();
    c13->SaveAs("./plots/CosThetaY_Y0.pdf");
    
    TCanvas *c14 = new TCanvas("c14","cosTheta_STD/Pro",0.,0.,800,600);
    histCosTheta[6]->SetMarkerSize(1);
    histCosTheta[6]->SetMarkerStyle(26);
    histCosTheta[6]->SetMarkerColor(kPink);
    histCosTheta[6]->SetLineColor(kOrange-1);
    histCosTheta[6]->GetXaxis()->SetTitle("Cos(#theta)");
    histCosTheta[6]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosTheta[6]->Draw("Norm");
    histCosTheta[7]->SetMarkerStyle(32);
    histCosTheta[7]->SetMarkerSize(1);
    histCosTheta[7]->SetMarkerColor(kViolet+2);
    histCosTheta[7]->SetLineColor(kBlue-7);
    histCosTheta[7]->Draw("SAME*Norm");
    auto* legend6 = new TLegend(0.5, 0.8, .65, .9);
    legend6->AddEntry(histCosTheta[6], "Pz = 0.9", "P");
    legend6->AddEntry(histCosTheta[7], "Protos Sample", "P");
    legend6->Draw();
    c14->SaveAs("./plots/CosThetaSTD_Pro.pdf");
    
    TCanvas *c15 = new TCanvas("c15","cosThetaStar_STD/Pro",0.,0.,800,600);
    histCosThetaStar[6]->SetMarkerSize(1);
    histCosThetaStar[6]->SetMarkerStyle(26);
    histCosThetaStar[6]->SetMarkerColor(kPink);
    histCosThetaStar[6]->SetLineColor(kOrange-1);
    histCosThetaStar[6]->GetXaxis()->SetTitle("Cos(#theta)");
    histCosThetaStar[6]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosThetaStar[6]->Draw("Norm");
    histCosThetaStar[7]->SetMarkerStyle(32);
    histCosThetaStar[7]->SetMarkerSize(1);
    histCosThetaStar[7]->SetMarkerColor(kViolet+2);
    histCosThetaStar[7]->SetLineColor(kBlue-7);
    histCosThetaStar[7]->Draw("SAME*Norm");
    auto* legend7 = new TLegend(0.5, 0.8, .65, .9);
    legend7->AddEntry(histCosThetaStar[6], "Pz = 0.9", "P");
    legend7->AddEntry(histCosThetaStar[7], "Protos Sample", "P");
    legend7->Draw();
    c15->SaveAs("./plots/CosThetaStarSTD_Pro.pdf");
    
    TCanvas *c16 = new TCanvas("c16","Phi_STD/Pro",0.,0.,800,600);
    histPhi[6]->SetMarkerSize(1);
    histPhi[6]->SetMarkerStyle(26);
    histPhi[6]->SetMarkerColor(kPink);
    histPhi[6]->SetLineColor(kOrange-1);
    histPhi[6]->GetXaxis()->SetTitle("#phi");
    histPhi[6]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhi[6]->Draw("Norm");
    histPhi[7]->SetMarkerStyle(32);
    histPhi[7]->SetMarkerSize(1);
    histPhi[7]->SetMarkerColor(kViolet+2);
    histPhi[7]->SetLineColor(kBlue-7);
    histPhi[7]->Draw("SAME*Norm");
    auto* legend8 = new TLegend(0.5, 0.8, .65, .9);
    legend8->AddEntry(histPhi[6], "Pz = 0.9", "P");
    legend8->AddEntry(histPhi[7], "Protos Sample", "P");
    legend8->Draw();
    c16->SaveAs("./plots/histPhiSTD_Pro.pdf");
    
    TCanvas *c17 = new TCanvas("c17","PhiStar_STD/Pro",0.,0.,800,600);
    histPhiStar[6]->SetMarkerSize(1);
    histPhiStar[6]->SetMarkerStyle(26);
    histPhiStar[6]->SetMarkerColor(kPink);
    histPhiStar[6]->SetLineColor(kOrange-1);
    histPhiStar[6]->GetXaxis()->SetTitle("#phi *)");
    histPhiStar[6]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhiStar[6]->Draw("Norm");
    histPhiStar[7]->SetMarkerStyle(32);
    histPhiStar[7]->SetMarkerSize(1);
    histPhiStar[7]->SetMarkerColor(kViolet+2);
    histPhiStar[7]->SetLineColor(kBlue-7);
    histPhiStar[7]->Draw("SAME*Norm");
    auto* legend9 = new TLegend(0.5, 0.8, .65, .9);
    legend9->AddEntry(histPhiStar[6], "Pz = 0.9", "P");
    legend9->AddEntry(histPhiStar[7], "Protos Sample", "P");
    legend9->Draw();
    c17->SaveAs("./plots/histPhiStarSTD_Pro.pdf");
    
    TCanvas *c18 = new TCanvas("c18","Phi_Z/-Z",0.,0.,800,600);
    histPhi[0]->SetMarkerSize(1);
    histPhi[0]->SetMarkerStyle(26);
    histPhi[0]->SetMarkerColor(kPink);
    histPhi[0]->SetLineColor(kOrange-1);
    histPhi[0]->GetXaxis()->SetTitle("#phi");
    histPhi[0]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhi[0]->Draw();
    histPhi[1]->SetMarkerStyle(32);
    histPhi[1]->SetMarkerSize(1);
    histPhi[1]->SetMarkerColor(kViolet+2);
    histPhi[1]->SetLineColor(kBlue-7);
    histPhi[1]->Draw("SAME*Norm");
    auto* legend10 = new TLegend(0.5, 0.2, .65, .3);
    legend10->AddEntry(histPhi[0], "P = +Pz", "P");
    legend10->AddEntry(histPhi[1], "P = -Pz", "P");
    legend10->Draw();
    c18->SaveAs("./plots/PhiZ_Z0.pdf");
    
    TCanvas *c19 = new TCanvas("c19","Phi_X/-X",0.,0.,800,600);
    histPhi[2]->SetMarkerSize(1);
    histPhi[2]->SetMarkerStyle(26);
    histPhi[2]->SetMarkerColor(kPink);
    histPhi[2]->SetLineColor(kOrange-1);
    histPhi[2]->GetXaxis()->SetTitle("#phi");
    histPhi[2]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhi[2]->Draw();
    histPhi[3]->SetMarkerStyle(32);
    histPhi[3]->SetMarkerSize(1);
    histPhi[3]->SetMarkerColor(kViolet+2);
    histPhi[3]->SetLineColor(kBlue-7);
    histPhi[3]->Draw("SAME*Norm");
    auto* legend11 = new TLegend(0.5, 0.2, .65, .3);
    legend11->AddEntry(histPhi[2], "P = +Px", "P");
    legend11->AddEntry(histPhi[3], "P = -Px", "P");
    legend11->Draw();
    c19->SaveAs("./plots/PhiX_X0.pdf");
    
    TCanvas *c20 = new TCanvas("c20","Phi_Y/-Y",0.,0.,800,600);
    histPhi[4]->SetMarkerSize(1);
    histPhi[4]->SetMarkerStyle(26);
    histPhi[4]->SetMarkerColor(kPink);
    histPhi[4]->SetLineColor(kOrange-1);
    histPhi[4]->GetXaxis()->SetTitle("#phi");
    histPhi[4]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhi[4]->Draw();
    histPhi[5]->SetMarkerStyle(32);
    histPhi[5]->SetMarkerSize(1);
    histPhi[5]->SetMarkerColor(kViolet+2);
    histPhi[5]->SetLineColor(kBlue-7);
    histPhi[5]->Draw("SAME*Norm");
    auto* legend12 = new TLegend(0.5, 0.2, .65, .3);
    legend12->AddEntry(histPhi[4], "P = +Py", "P");
    legend12->AddEntry(histPhi[5], "P = -Py", "P");
    legend12->Draw();
    c20->SaveAs("./plots/PhiY_Y0.pdf");
    
    TCanvas *c211 = new TCanvas("c211","cosThetaStar_Z/-Z",0.,0.,800,600);
    histCosThetaStar[0]->SetMarkerSize(1);
    histCosThetaStar[0]->SetMarkerStyle(26);
    histCosThetaStar[0]->SetMarkerColor(kPink);
    histCosThetaStar[0]->SetLineColor(kOrange-1);
    histCosThetaStar[0]->GetXaxis()->SetTitle("cos(#theta *)");
    histCosThetaStar[0]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosThetaStar[0]->Draw();
    histCosThetaStar[1]->SetMarkerStyle(32);
    histCosThetaStar[1]->SetMarkerSize(1);
    histCosThetaStar[1]->SetMarkerColor(kViolet+2);
    histCosThetaStar[1]->SetLineColor(kBlue-7);
    histCosThetaStar[1]->Draw("SAME*Norm");
    auto* legend133 = new TLegend(0.5, 0.2, .65, .3);
    legend133->AddEntry(histCosThetaStar[0], "P = +Pz", "P");
    legend133->AddEntry(histCosThetaStar[1], "P = -Pz", "P");
    legend133->Draw();
    c211->SaveAs("./plots/cosThetaStarZ_Z0.pdf");
    
    TCanvas *c21 = new TCanvas("c21","cosThetaStar_X/-X",0.,0.,800,600);
    histCosThetaStar[2]->SetMarkerSize(1);
    histCosThetaStar[2]->SetMarkerStyle(26);
    histCosThetaStar[2]->SetMarkerColor(kPink);
    histCosThetaStar[2]->SetLineColor(kOrange-1);
    histCosThetaStar[2]->GetXaxis()->SetTitle("cos(#theta *)");
    histCosThetaStar[2]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosThetaStar[2]->Draw();
    histCosThetaStar[3]->SetMarkerStyle(32);
    histCosThetaStar[3]->SetMarkerSize(1);
    histCosThetaStar[3]->SetMarkerColor(kViolet+2);
    histCosThetaStar[3]->SetLineColor(kBlue-7);
    histCosThetaStar[3]->Draw("SAME*Norm");
    auto* legend14 = new TLegend(0.5, 0.2, .65, .3);
    legend14->AddEntry(histCosThetaStar[2], "P = +Px", "P");
    legend14->AddEntry(histCosThetaStar[3], "P = -Px", "P");
    legend14->Draw();
    c21->SaveAs("./plots/cosThetaStarX_X0.pdf");
    
    TCanvas *c22 = new TCanvas("c22","cosThetaStar_Y/-Y",0.,0.,800,600);
    histCosThetaStar[4]->SetMarkerSize(1);
    histCosThetaStar[4]->SetMarkerStyle(26);
    histCosThetaStar[4]->SetMarkerColor(kPink);
    histCosThetaStar[4]->SetLineColor(kOrange-1);
    histCosThetaStar[4]->GetXaxis()->SetTitle("cos(#theta *)");
    histCosThetaStar[4]->GetYaxis()->SetTitle("Arbitrary Units");
    histCosThetaStar[4]->Draw();
    histCosThetaStar[5]->SetMarkerStyle(32);
    histCosThetaStar[5]->SetMarkerSize(1);
    histCosThetaStar[5]->SetMarkerColor(kViolet+2);
    histCosThetaStar[5]->SetLineColor(kBlue-7);
    histCosThetaStar[5]->Draw("SAME*Norm");
    auto* legend15 = new TLegend(0.5, 0.2, .65, .3);
    legend15->AddEntry(histCosThetaStar[4], "P = +Py", "P");
    legend15->AddEntry(histCosThetaStar[5], "P = -Py", "P");
    legend15->Draw();
    c22->SaveAs("./plots/cosThetaStarY_Y0.pdf");

    TCanvas *c23 = new TCanvas("c23","PhiStar_Z/-Z",0.,0.,800,600);
    histPhiStar[0]->SetMarkerSize(1);
    histPhiStar[0]->SetMarkerStyle(26);
    histPhiStar[0]->SetMarkerColor(kPink);
    histPhiStar[0]->SetLineColor(kOrange-1);
    histPhiStar[0]->GetXaxis()->SetTitle("#phi *");
    histPhiStar[0]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhiStar[0]->Draw();
    histPhiStar[1]->SetMarkerStyle(32);
    histPhiStar[1]->SetMarkerSize(1);
    histPhiStar[1]->SetMarkerColor(kViolet+2);
    histPhiStar[1]->SetLineColor(kBlue-7);
    histPhiStar[1]->Draw("SAME*Norm");
    auto* legend16 = new TLegend(0.5, 0.2, .65, .3);
    legend16->AddEntry(histPhiStar[0], "P = +Pz", "P");
    legend16->AddEntry(histPhiStar[1], "P = -Pz", "P");
    legend16->Draw();
    c23->SaveAs("./plots/PhiStarZ_Z0.pdf");
    
    TCanvas *c24 = new TCanvas("c24","PhiStar_X/-X",0.,0.,800,600);
    histPhiStar[2]->SetMarkerSize(1);
    histPhiStar[2]->SetMarkerStyle(26);
    histPhiStar[2]->SetMarkerColor(kPink);
    histPhiStar[2]->SetLineColor(kOrange-1);
    histPhiStar[2]->GetXaxis()->SetTitle("#phi *");
    histPhiStar[2]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhiStar[2]->Draw();
    histPhiStar[3]->SetMarkerStyle(32);
    histPhiStar[3]->SetMarkerSize(1);
    histPhiStar[3]->SetMarkerColor(kViolet+2);
    histPhiStar[3]->SetLineColor(kBlue-7);
    histPhiStar[3]->Draw("SAME*Norm");
    auto* legend17 = new TLegend(0.5, 0.2, .65, .3);
    legend17->AddEntry(histPhiStar[2], "P = +Px", "P");
    legend17->AddEntry(histPhiStar[3], "P = -Px", "P");
    legend17->Draw();
    c24->SaveAs("./plots/PhiStarX_X0.pdf");
    
    TCanvas *c25 = new TCanvas("c25","PhiStar_Y/-Y",0.,0.,800,600);
    histPhiStar[4]->SetMarkerSize(1);
    histPhiStar[4]->SetMarkerStyle(26);
    histPhiStar[4]->SetMarkerColor(kPink);
    histPhiStar[4]->SetLineColor(kOrange-1);
    histPhiStar[4]->GetXaxis()->SetTitle("#phi *");
    histPhiStar[4]->GetYaxis()->SetTitle("Arbitrary Units");
    histPhiStar[4]->Draw();
    histPhiStar[5]->SetMarkerStyle(32);
    histPhiStar[5]->SetMarkerSize(1);
    histPhiStar[5]->SetMarkerColor(kViolet+2);
    histPhiStar[5]->SetLineColor(kBlue-7);
    histPhiStar[5]->Draw("SAME*Norm");
    auto* legend18 = new TLegend(0.5, 0.2, .65, .3);
    legend18->AddEntry(histPhiStar[4], "P = +Px", "P");
    legend18->AddEntry(histPhiStar[5], "P = -Px", "P");
    legend18->Draw();
    c25->SaveAs("./plots/PhiStarY_Y0.pdf");
    
    
//        TH1F* h2 =(TH1F*) sTFile->Get("hPhi")->Clone("h2"+FileName[i]);
//        h2->Scale(1/h2->Integral());
//        histPhi.push_back(h2);
//        TH1F* h3 =(TH1F*) sTFile->Get("hCosThetaStar")->Clone("h3"+FileName[i]);
//        h3->Scale(1/h3->Integral());
//        histCosThetaStar.push_back(h3);
//        TH1F* h4 =(TH1F*) sTFile->Get("hPhiStar")->Clone("h4"+FileName[i]);
//        h4->Scale(1/h4->Integral());
//        h4->SetMinimum(0);
    
    
//    TCanvas *c2 = new TCanvas("c2","Canvas",0.,0.,800,600);
//    h4->Scale(1/h4->Integral());
//    h4->Draw();

//        histPhiStar.push_back(h4);
//    }
//    histCosThetaZ[0]->Draw();
//    histCosThetaZ[1]->Draw("SAME");
//    histCosThetaZ.back()->Draw("SAME");
    
}
