#include <iostream>
#include <fstream>      // std::ofstream
#include <vector>
#include <cmath>
#include <TMath.h>
#include <TH1F.h>
#include <TTree.h>
#include <TCut.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLatex.h>
#include <TLegend.h>
#include <sstream>
#include <algorithm>
//#include "/Users/nellobruscino/atlasrootstyle/AtlasStyle.C"
#include <TSystem.h>
#include <TStyle.h>
#include <TROOT.h>
#include <iomanip>      // std::setprecision

#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"

using namespace RooFit;

// statistical threshold to smooth or not a low statistic process (instead of removing it)
bool doSmooth=false; int stat_threshold = 400;
// very general settings
double Luminosity = 36074.6;       // v22=28012.86  pb-1
bool blinded=true;                // to lock/unlock the "OnlyMC" vector and the CUTFLOW

bool sort_fn (float i,float j) { return (i>j); }

// This function returns true if the first pair is "more" than the second one according to some metric.
bool pairCompare(const std::pair<string,double>& firstElem, const std::pair<string,double>& secondElem) { return (firstElem.second > secondElem.second); }

void ATLASLabel(Double_t x,Double_t y,const char* text,Color_t color) {
  Double_t tsize=0.035;
  TLatex lex;
  lex.SetTextSize(tsize);
  lex.SetNDC();
  lex.SetTextFont(72);
  lex.SetTextColor(color);
  // double delx = 0.1;
  double delx = 3.4*tsize;

  lex.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p;
    p.SetNDC();
    p.SetTextSize(tsize);
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+delx,y,text);
  }
}

void myText(Double_t x,Double_t y,Color_t color=1,Double_t size=0.035, const char *text="") {

  TLatex lex;
  lex.SetTextSize(size);
  lex.SetNDC();
  lex.SetTextColor(color);
  lex.DrawLatex(x,y,text);
}

void Coutter(const std::vector<string> &invec){
  for (unsigned int i=0;i<invec.size();i++){
    cout<<i<<"/"<<invec.size()<<"  value : "<<invec.at(i)<<endl;
  }
  cout<<endl;
}

// _____________________REWEIGHT PLOT 1D ____________________________________________________________________
void reweight_plot(TH1* t_plot,
                   double total_weight = 14996900 ,     // To normalize the distribution to 1
                   double kfactor      = 1.1994   ,
                   double xsec         = 114.49   ,
                   double luminosity   = 20281    )        // because luminosity in fb-1 while the x-sec is in pb 
{
  // t_plot->Sumw2();
  if(total_weight!=0) {
    double norm = (luminosity*xsec*kfactor)/total_weight;
    t_plot->Scale(norm);
  }
  else std::cout<<"WARNING: reweight_plot() --> total_weight = 0"<<std::endl;
}

// _____________________PLOTS WITH UNDERFLOWS AND OVERFLOWS _________________________________________________                                               
void addUOFlow(TH1* histo) {
  // Add undeflow and overflow in the first and last bin.                                                                                                       

  TH1* histoOld = (TH1*) histo -> Clone();
  histo -> Reset();
  int NBin = histoOld->GetNbinsX();

  // UNDERFLOW
  double err = 0;
  double sum = histoOld -> IntegralAndError(0, 1, err);
  histo -> SetBinContent(1, sum);
  histo -> SetBinError(1, err);

  for (int iBin = 2; iBin < NBin; iBin++) {
    err = 0;
    sum = histoOld -> IntegralAndError(iBin, iBin, err);
    histo -> SetBinContent(iBin, sum);
    histo -> SetBinError(iBin, err);
  }

  // OVERFLOW
  err = 0;
  sum = histoOld -> IntegralAndError(NBin, NBin+1, err);
  histo -> SetBinContent(NBin, sum);
  histo -> SetBinError(NBin, err);

  histo->SetEntries(histoOld->GetEntries());  // to take memory of the number of entries

  delete histoOld;
}


/************************************************************************************/
/***********************          PERFORM FIT            ****************************/
/************************************************************************************/
enum SELECTION_options {all, // inclusive region
			cut1   // cut1 applieD
                       };

void templatefit(string analysis="top", SELECTION_options selection=all){

  //Event weight definitions
  string extrapath="";
  string Cuts     ="";
  string weight  ="1";
  string RunYear ="1"; // for choosing Data 2015 2016
  int nlep = 0;
  if     (analysis=="top")       { Cuts += RunYear+" && _top.id==6  && "; nlep=1;   weight += "*1"; }
  else if(analysis=="antitop")   { Cuts += RunYear+" && _top.id==-6 && "; nlep=1;   weight += "*1"; }
  else { cout<<"----> NO lepton requirement applied! Please choose an analysis...."<<std::endl; exit(1); }

  if     (selection==all) {
    extrapath="all";
    Cuts+="1";
  }
  else if(selection==cut1) {
    extrapath="cut1";
    if(analysis=="top")      
      Cuts+="(lep_ID_0!=-lep_ID_1||(Mll01>12e3&&abs(Mll01-91.2e3)>10e3)) && (lep_ID_0!=-lep_ID_2||(Mll02>12e3&&abs(Mll02-91.2e3)>10e3)) ";
    else if(analysis=="test") 
      Cuts+="1";
    else { cout<<"----> NO lepton requirement applied! Please choose an analysis...."<<std::endl; exit(1); }
  }
  else { cout<<"please type the correct CUTS_option: all,cut1,... "<<endl; exit(1); }
  
  cout<<endl<<"Cuts   : "<<Cuts  <<endl<<endl;
  cout<<endl<<"weight : "<<weight<<endl<<endl;
  cout<<endl<<"RunYear: "<<RunYear<<endl<<endl;

  // create folder
  TString path = "./Output_"+analysis+"/";
  gSystem->Exec("mkdir -p "+path) ;
  path += extrapath+"/";
  gSystem->Exec("mkdir -p "+path) ;
  cout<< "path ---------------------------->>>  : " <<path <<endl;
  //TString FilesPath = "/eos/atlas/user/n/nbruscin/ttHmultilepton/";
  TString FilesPath     = "~/singletop/data/sample22Jan/";
  TString FilesPath2     = "~/singletop/data/hadded/";
  TString FilesPathData = FilesPath;
  // FilesPath     += "MC/";
  // FilesPathData += "Data/";

  gStyle->SetOptStat(0);
  //SetAtlasStyle();

  //New files createtree
  TFile *Datafile;
  TFile *st_SM_file;
  TFile *st_Pzplus_file;
  TFile *st_Pzminus_file;
  
  //Input files loading
  Datafile           = new TFile(FilesPathData+"stPolarizationOG.root"   ,"OPEN");
  st_SM_file         = new TFile(FilesPath2+"stPolarizationOG.root"      ,"OPEN");
  st_Pzplus_file     = new TFile(FilesPath+"stPolarizationZ.root"        ,"OPEN");
  st_Pzminus_file    = new TFile(FilesPath+"stPolarizationZ0.root"       ,"OPEN");

  //Creating vector of trees, one for each region  
  //--ttH--
  gROOT->cd();
  std::cout<<"applying Cuts..."<<std::endl;
  TTree* Datatree             = ((TTree*) Datafile       ->Get("singleTop"))->CopyTree(Cuts.c_str());
  TTree* st_SM_tree           = ((TTree*) st_SM_file     ->Get("singleTop"))->CopyTree(Cuts.c_str());
  TTree* st_Pzplus_tree       = ((TTree*) st_Pzplus_file ->Get("singleTop"))->CopyTree(Cuts.c_str());
  TTree* st_Pzminus_tree      = ((TTree*) st_Pzminus_file->Get("singleTop"))->CopyTree(Cuts.c_str());

  std::vector<string> Savename;
  std::vector<string> Variables;
  vector<bool> doExtracut; std::vector<string> Extracut;
  int nbin=0; std::vector<int> Nbin;     std::vector<bool> IsFixedbins; vector<double*> xbins;
  double * array;
   std::vector<bool> OnlyMC;
  double minbin=0; std::vector<double> Minbin;
  double maxbin=0; std::vector<double> Maxbin;
  std::vector<string> XTitle;
  double ev_per_gev=0; std::vector<string> YTitle;
  vector<bool> doLogScaleX; vector<bool> doLogScaleY;
  vector<bool> doUnderOverFlows;

  //Building the vectors for the histograms definitions
  ///////////////////////////////////
  // MU
  Savename.push_back("cos_Theta_Z");
  //Variables.push_back("_cosThetaZ");
  Variables.push_back("fourangles.cos_Theta");
  doExtracut.push_back(false); Extracut.push_back("");
  nbin=8     ; Nbin.push_back(nbin);
  minbin=-1.  ; Minbin.push_back(minbin);
  maxbin=1.   ; Maxbin.push_back(maxbin);
  OnlyMC.push_back(blinded);
  IsFixedbins.push_back(true);
  array = new double[nbin+1];
  xbins.push_back(array);
  XTitle.push_back("cos(#theta_{z})");
  ev_per_gev=(maxbin-minbin)/nbin; YTitle.push_back(Form("events / %.0f",ev_per_gev));
  doLogScaleX.push_back(false); doLogScaleY.push_back(false);
  doUnderOverFlows.push_back(true);
  
  
  int NVariable = Variables.size();
  //NVariable = 3;   // just to test the production, running on XX variable only
  int FirstVariable = 0;  // just to test the production, ruunning from the XX variable
  
  std::cout<< " Variables.size()         : " << Variables.size()         << std::endl;
  std::cout<< " Savename.size()          : " << Savename.size()          << std::endl;
  std::cout<< " doExtracut.size()        : " << doExtracut.size()        << std::endl;
  std::cout<< " Extracut.size()          : " << Extracut.size()          << std::endl;
  std::cout<< " Nbin.size()              : " << Nbin.size()              << std::endl;
  std::cout<< " IsFixedbins.size()       : " << IsFixedbins.size()       << std::endl;
  std::cout<< " Minbin.size()            : " << Minbin.size()            << std::endl;
  std::cout<< " Maxbin.size()            : " << Maxbin.size()            << std::endl;
  std::cout<< " XTitle.size()            : " << XTitle.size()            << std::endl;
  std::cout<< " YTitle.size()            : " << YTitle.size()            << std::endl;
  std::cout<< " doLogScaleX.size()       : " << doLogScaleX.size()       << std::endl;
  std::cout<< " doLogScaleY.size()       : " << doLogScaleY.size()       << std::endl;
  std::cout<< " doUnderOverFlows.size()  : " << doUnderOverFlows.size()  << std::endl;

  Color_t color[9] = {kRed, kBlue+2, kYellow-3, kAzure-4, kRed-1, kGreen+1, kBlack, kRed+3, kOrange+1};

  //one histo for each variable 
  std::vector<TH1F*> Data(NVariable);
  std::vector<TH1F*> st_SM     (NVariable);
  std::vector<TH1F*> st_Pzplus (NVariable);
  std::vector<TH1F*> st_Pzminus(NVariable);

  // std::vector<RooDataSet*> Dataset(NVariable);
  // std::vector<RooDataSet*> st_SMset(NVariable);

  string weight_forMC =""; 
  string weight_forData =""; 

  //Filling of histograms
  for(int i=FirstVariable;i<NVariable;i++){ // NVariable
    cout<<"nvar : "<<i<<endl;
    if( doExtracut[i] ) { weight_forMC = weight + "*(" + Extracut[i] + ")"; weight_forData ="(" + Extracut[i] + ")"; }
    else                { weight_forMC = weight; weight_forData = "1."; }
    RooRealVar var(Variables[i].c_str(),Variables[i].c_str(), Minbin.at(i), Maxbin.at(i));

    //--Data--
    std::ostringstream  nhistoData; nhistoData <<"Data_V_"<<i;
    if(IsFixedbins[i]) Data[i] = new TH1F(nhistoData.str().c_str(),"",Nbin.at(i),Minbin.at(i),Maxbin.at(i));
    else Data[i] = new TH1F(nhistoData.str().c_str(),"",Nbin.at(i),xbins[i]);
    Data[i]->Sumw2();
    if( !OnlyMC[i] ) {
      Datatree->Draw(Form("%s>>Data_V_%i",Variables[i].c_str(),i),weight_forData.c_str());
    }
    //Data[i]->Print("all");
    if( doUnderOverFlows[i] ) addUOFlow(Data[i]);
    // //--Dataset--
    // std::ostringstream  nDataset; nhistoData <<"Dataset_V_"<<i;
    // Dataset[i] = new RooDataSet(nDataset.str().c_str(),"",Datatree,RooArgSet(var),weight_forData.c_str());
    // //Dataset[i]->binnedClone()->dump2();

    //--st_SM--
    std::ostringstream  nhistost_SM; nhistost_SM <<"st_SM_V_"<<i;
    if(IsFixedbins[i]) st_SM[i] = new TH1F(nhistost_SM.str().c_str(),"",Nbin.at(i),Minbin.at(i),Maxbin.at(i));
    else st_SM[i] = new TH1F(nhistost_SM.str().c_str(),"",Nbin.at(i),xbins[i]);
    st_SM[i]->Sumw2();
    st_SM_tree->Draw(Form("%s>>st_SM_V_%i",Variables[i].c_str(),i),weight_forMC.c_str());
    reweight_plot(st_SM[i], 1., 1., 1., Luminosity);  // histo, n_events, k-factor, xsec, Luminosity (pb-1)
    //st_SM[i]->Print("all");
    if( doUnderOverFlows[i] ) addUOFlow(st_SM[i]);
    // //--st_SMset--
    // std::ostringstream  nst_SMset; nst_SMset <<"st_SMset_V_"<<i;
    // st_SMset[i] = new RooDataSet(nst_SMset.str().c_str(),"",st_SM_tree,RooArgSet(var),weight_forMC.c_str());
    // //st_SMset[i]->binnedClone()->dump2();

    //--st_Pzplus--
    std::ostringstream  nhistost_Pzplus; nhistost_Pzplus <<"st_Pzplus_V_"<<i;
    if(IsFixedbins[i]) st_Pzplus[i] = new TH1F(nhistost_Pzplus.str().c_str(),"",Nbin.at(i),Minbin.at(i),Maxbin.at(i));
    else st_Pzplus[i] = new TH1F(nhistost_Pzplus.str().c_str(),"",Nbin.at(i),xbins[i]);
    st_Pzplus[i]->Sumw2();
    st_Pzplus_tree->Draw(Form("%s>>st_Pzplus_V_%i",Variables[i].c_str(),i),weight_forMC.c_str());
    reweight_plot(st_Pzplus[i], 1., 1., 1., Luminosity);  // histo, n_events, k-factor, xsec, Luminosity (pb-1)
    //st_Pzplus[i]->Print("all");
    if( doUnderOverFlows[i] ) addUOFlow(st_Pzplus[i]);

    //--st_Pzminus--
    std::ostringstream  nhistost_Pzminus; nhistost_Pzminus <<"st_Pzminus_V_"<<i;
    if(IsFixedbins[i]) st_Pzminus[i] = new TH1F(nhistost_Pzminus.str().c_str(),"",Nbin.at(i),Minbin.at(i),Maxbin.at(i));
    else st_Pzminus[i] = new TH1F(nhistost_Pzminus.str().c_str(),"",Nbin.at(i),xbins[i]);
    st_Pzminus[i]->Sumw2();
    st_Pzminus_tree->Draw(Form("%s>>st_Pzminus_V_%i",Variables[i].c_str(),i),weight_forMC.c_str());
    reweight_plot(st_Pzminus[i], 1., 1., 1., Luminosity);  // histo, n_events, k-factor, xsec, Luminosity (pb-1)
    //st_Pzminus[i]->Print("all");
    if( doUnderOverFlows[i] ) addUOFlow(st_Pzminus[i]);

  }

 

  TCanvas * cOUT1 = new TCanvas("OUT1", "OUT2",800,800);
  Double_t xl1=0.65, yl1=0.75, xl2=xl1+.2, yl2=yl1+.15;
  TLegend *leg;


  for(int Idx =FirstVariable;Idx <NVariable;Idx++){
    cout<<"Idx : "<<Idx<<endl;

    // trick: smooth processes with low statistics
    double integr=0., integr2=0.;
    if(st_SM[Idx]      ->GetEntries()<stat_threshold &&  st_SM[Idx]     ->GetEntries()!=0 && doSmooth) 
      { integr=st_SM[Idx]->Integral(0,st_SM[Idx]->GetXaxis()->GetNbins()+1);   
	st_SM[Idx]->Smooth(); integr2=st_SM[Idx]->Integral(0,st_SM[Idx]->GetXaxis()->GetNbins()+1);   
	if(integr2!=0) st_SM[Idx]->Scale(integr/integr2); }
    if(st_Pzplus[Idx]      ->GetEntries()<stat_threshold &&  st_Pzplus[Idx]     ->GetEntries()!=0 && doSmooth) 
      { integr=st_Pzplus[Idx]->Integral(0,st_Pzplus[Idx]->GetXaxis()->GetNbins()+1);   
	st_Pzplus[Idx]->Smooth(); integr2=st_Pzplus[Idx]->Integral(0,st_Pzplus[Idx]->GetXaxis()->GetNbins()+1);   
	if(integr2!=0) st_Pzplus[Idx]->Scale(integr/integr2); }
    if(st_Pzminus[Idx]      ->GetEntries()<stat_threshold &&  st_Pzminus[Idx]     ->GetEntries()!=0 && doSmooth) 
      { integr=st_Pzminus[Idx]->Integral(0,st_Pzminus[Idx]->GetXaxis()->GetNbins()+1);   
	st_Pzminus[Idx]->Smooth(); integr2=st_Pzminus[Idx]->Integral(0,st_Pzminus[Idx]->GetXaxis()->GetNbins()+1);   
	if(integr2!=0) st_Pzminus[Idx]->Scale(integr/integr2); }


    Double_t err = 0.;
    cout<<"Data[Idx]->GetEntries()        : "<<Data[Idx]->GetEntries()<<endl;
    cout<<"Data[Idx]->Integral()          : "<<Data[Idx]->Integral()<<endl;
    cout<<"Data[Idx]->Integral(all bins)  : "<<Data[Idx]->IntegralAndError(0,Data[Idx]->GetXaxis()->GetNbins()+1,err)<<" +/- "; cout<<err<<endl;
    // Data[Idx]->Print("all");
    err = 0.;
    cout<<"st_SM[Idx]->GetEntries()        : "<<st_SM[Idx]->GetEntries()<<endl;
    cout<<"st_SM[Idx]->Integral()          : "<<st_SM[Idx]->Integral()<<endl;
    cout<<"st_SM[Idx]->Integral(all bins)  : "<<st_SM[Idx]->IntegralAndError(0,st_SM[Idx]->GetXaxis()->GetNbins()+1,err)<<" +/- "; cout<<err<<endl;
    if(st_SM[Idx]->Integral()!=0.) st_SM[Idx]->Scale(1/st_SM[Idx]->IntegralAndError(0,st_SM[Idx]->GetXaxis()->GetNbins()+1,err));
    // st_SM[Idx]->Print("all");
    err = 0.;
    cout<<"st_Pzplus[Idx]->GetEntries()        : "<<st_Pzplus[Idx]->GetEntries()<<endl;
    cout<<"st_Pzplus[Idx]->Integral()          : "<<st_Pzplus[Idx]->Integral()<<endl;
    cout<<"st_Pzplus[Idx]->Integral(all bins)  : "<<st_Pzplus[Idx]->IntegralAndError(0,st_Pzplus[Idx]->GetXaxis()->GetNbins()+1,err)<<" +/- "; cout<<err<<endl;
    if(st_Pzplus[Idx]->Integral()!=0.) st_Pzplus[Idx]->Scale(1/st_Pzplus[Idx]->IntegralAndError(0,st_Pzplus[Idx]->GetXaxis()->GetNbins()+1,err));
    // st_Pzplus[Idx]->Print("all");
    err = 0.;
    cout<<"st_Pzminus[Idx]->GetEntries()        : "<<st_Pzminus[Idx]->GetEntries()<<endl;
    cout<<"st_Pzminus[Idx]->Integral()          : "<<st_Pzminus[Idx]->Integral()<<endl;
    cout<<"st_Pzminus[Idx]->Integral(all bins)  : "<<st_Pzminus[Idx]->IntegralAndError(0,st_Pzminus[Idx]->GetXaxis()->GetNbins()+1,err)<<" +/- "; cout<<err<<endl;
    if(st_Pzminus[Idx]->Integral()!=0.) st_Pzminus[Idx]->Scale(1/st_Pzminus[Idx]->IntegralAndError(0,st_Pzminus[Idx]->GetXaxis()->GetNbins()+1,err));
    // st_Pzminus[Idx]->Print("all");

    cout<<"---"<<endl;
    
    st_SM[Idx]->GetYaxis()->SetTitle("%/bin");
    st_SM[Idx]->GetYaxis()->SetTitleOffset(1.2);
    st_SM[Idx]->GetYaxis()->SetLabelSize(0.035);    

    st_SM[Idx]->GetXaxis()->SetLabelColor(0); // makes the x axis numbers transparent in the top plot ;) 
    
    Data[Idx]->SetMarkerColor(1);
    Data[Idx]->SetLineColor(1);
    int icolor=0;
    st_SM[Idx]->SetLineColor  (color[icolor]);
    st_SM[Idx]->SetLineWidth  (4);
    st_SM[Idx]->SetMarkerColor(color[icolor]);
    st_SM[Idx]->SetFillColor  (0);
    icolor++;      
    st_Pzplus[Idx]->SetLineColor  (color[icolor]);
    st_Pzplus[Idx]->SetLineWidth  (4);
    st_Pzplus[Idx]->SetMarkerColor(color[icolor]);
    st_Pzplus[Idx]->SetFillColor  (0);
    icolor++;      
    st_Pzminus[Idx]->SetLineColor  (color[icolor]);
    st_Pzminus[Idx]->SetLineWidth  (4);
    st_Pzminus[Idx]->SetMarkerColor(color[icolor]);
    st_Pzminus[Idx]->SetFillColor  (0);
    icolor++;      
    
    //----
    leg = new TLegend(xl1,yl1,xl2,yl2,NULL,"brNDC");
    TString legoption = "ELP";
    leg->AddEntry(st_SM[Idx]     ,"single-top SM"    ,legoption); // "F"
    leg->AddEntry(st_Pzplus[Idx] ,"single-top P_z=+1",legoption); // "F"
    leg->AddEntry(st_Pzminus[Idx],"single-top P_z=-1",legoption); // "F"

    leg->SetBorderSize(0);  
    leg->SetTextSize(0.035); 
    leg->SetLineColor(0); 
    leg->SetLineStyle(1); 
    leg->SetLineWidth(1); 
    leg->SetFillColor(0); 
    leg->SetFillStyle(0);


    //--------------------------------------//
    // PERFORMIG THE FIT
    // Pz = (Nplus - Nminus) / (Nplus + Nminus) = fplus - fminus
    // Nplus+Nminus = Ntot
    //--------------------------------------//
    RooRealVar var(Variables[Idx].c_str(),Variables[Idx].c_str(), Minbin.at(Idx), Maxbin.at(Idx));

    RooRealVar Ntot("Ntot","Ntot", OnlyMC[Idx]? st_SM[Idx]->Integral() : Data[Idx]->Integral() ); // = Nplus + Nminus
    //RooRealVar Ntot("Ntot","Ntot", OnlyMC[Idx]? st_SMset[Idx]->sumEntries() : Dataset[Idx]->sumEntries() ); // = Nplus + Nminus
    // RooFormulaVar datasum("datasum","@0+@1",RooArgList(Ntot ,Ntot2));  // all data events
    std::cout<<"------->Ntot = "<<Ntot.getVal()<<std::endl;
    double Pz_init = 0.; // defined as fplus - fminus
    double fplus_init = 0.5;

    RooRealVar Pz("Pz","Pz", Pz_init, -1., 1.);
    RooRealVar Nplus("Nplus","Nplus", fplus_init*Ntot.getVal(), 0., Ntot.getVal());
    RooFormulaVar Nminus("Nminus","@0-@1*@2",RooArgList(Nplus,Pz,Ntot));
 
    /******** MODELS & DATA ******/
    // data
    RooDataHist data("data","data",var,Import(OnlyMC[Idx]? *st_SM[Idx] : *Data[Idx])); 
    cout<<"-----> data.numEntries() = "<<data.numEntries() <<endl;
    //PDFs
    RooDataHist h_st_SM     ("h_st_SM"     ,"h_st_SM"     ,var,st_SM     [Idx]);
    RooDataHist h_st_Pzplus ("h_st_Pzplus" ,"h_st_Pzplus" ,var,st_Pzplus [Idx]);
    RooDataHist h_st_Pzminus("h_st_Pzminus","h_st_Pzminus",var,st_Pzminus[Idx]);
    RooHistPdf PDF_st_SM     ("PDF_st_SM"     ,"PDF_st_SM"     ,var,h_st_SM     );
    RooHistPdf PDF_st_Pzplus ("PDF_st_Pzplus" ,"PDF_st_Pzplus" ,var,h_st_Pzplus ); // The histogram distribution is explicitly normalized by RooHistPdf
    RooHistPdf PDF_st_Pzminus("PDF_st_Pzminus","PDF_st_Pzminus",var,h_st_Pzminus);
    // model 
    RooAddPdf model("model","model",RooArgList(PDF_st_Pzplus,PDF_st_Pzminus),RooArgList(Nplus,Nminus));
    
    // // RooDataHist to RooDataSet
    // RooRealVar wgt   ("wgt"   ,"wgt"   ,1,0,10000);
    // RooDataSet* d = model.generate(RooArgSet(var),data.numEntries()); 
    // // Convert d into unbinned weighted datasets 
    // d->addColumn(wgt);
    // RooDataSet* dataset = (RooDataSet*)d->emptyClone() ; delete d;
    // // for (Int_t i=0 ; i<data.numEntries() ; i++) { 
    // //   data.get(i);  // point everything to the current bit (expecially the weight!!)
    // //   //cout<<data.weight()<<"  "<<data.weightError()<<"  "<<data.weightError(RooAbsData::ErrorType::SumW2)<<"  "<<data.weightError(RooAbsData::ErrorType::None)<<endl;
    // //   dataset->add(*data.get(i),data.weight(),data.weightError(RooAbsData::ErrorType::SumW2)); }
    // for (Int_t i=0 ; i<st_SM[Idx]-> ; i++) { 
    //   data.get(i);  // point everything to the current bit (expecially the weight!!)
    //   //cout<<data.weight()<<"  "<<data.weightError()<<"  "<<data.weightError(RooAbsData::ErrorType::SumW2)<<"  "<<data.weightError(RooAbsData::ErrorType::None)<<endl;
    //   dataset->add(*data.get(i),data.weight(),data.weightError(RooAbsData::ErrorType::SumW2)); }
    // //data.dump2();
    // //dataset->binnedClone()->dump2();
    //RooDataSet* dataset = (RooDataSet*)(OnlyMC[Idx]? st_SMset[Idx] : Dataset[Idx]);
    
    //PDF_st_Pzplus.Print();
    //PDF_st_Pzminus.Print();
    
    // Create  index  category  and  join  samples
    // Define category to distinguish cosThetaZ and cosThetaZ samples events (not sure it'll be the final procedure)
    RooCategory sample("sample","sample") ;
    sample.defineType("cosThetaZ") ;
    //sample.defineType("cosThetaX") ;
    // Construct combined dataset in (x,sample)
    // RooDataSet combData("combData","combined data",var,Index(sample),Import("cosThetaZ",*dataset),Import("cosThetaX",*dataset2));
    // RooDataSet combData("combData","combined data",RooArgSet(var,wgt),Index(sample),Import("cosThetaZ",*dataset),Import("cosThetaX",*dataset2),WeightVar(wgt));
    //RooDataSet combData("combData","combined data",RooArgSet(var,wgt),Index(sample),Import("cosThetaZ",*dataset),WeightVar("wgt"));
    //RooDataSet combData("combData","combined data",RooArgSet(var),Index(sample),Import("cosThetaZ",*dataset));

    // Construct a simultaneous pdf  in (var,sample). Construct a simultaneous pdf using category sample as index
    RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
    // Associate model with the categories
    simPdf.addPdf(model,"cosThetaZ") ;
    //simPdf.addPdf(model2,"cosThetaX") ;


    // Perform simultaneous fit of model to data and model2 to data2
    //RooFitResult *fitres = simPdf.fitTo(combData,Extended(kTRUE),Hesse(kFALSE),Minos(kFALSE),PrintLevel(-1),Save(kTRUE),Range(Minbin.at(Idx), Maxbin.at(Idx))); // PrintLevel=-1(everything suppressed) to 3
    RooFitResult *fitres = model.fitTo(data,SumW2Error(OnlyMC[Idx]),PrintLevel(-1),Save(kTRUE),Range(Minbin.at(Idx), Maxbin.at(Idx)));
    //fitres->Print("v");

    // print results on a tex file
    // model.getParameters(var)->printLatex(OutputFile(Form("%s%s.tex",path.Data(),Savename[Idx].c_str())), Format("NE")) ;

    TString paramfile       = Form("%s%s.tex",path.Data(),Savename[Idx].c_str());
    cout<<" paramfile      : " <<paramfile      << endl;
    ofstream fout(paramfile);
    cout<<" Pz     = " <<Pz.getVal()    <<" +/- "<<Pz.getError()<<endl;
    fout<<" Pz     = " <<Pz.getVal()    <<" +/- "<<Pz.getError()<<endl;
    cout<<" Nplus       = " <<Nplus.getVal()      <<" +/- "<<Nplus.getError()<<endl;
    fout<<" Nplus       = " <<Nplus.getVal()      <<" +/- "<<Nplus.getError()<<endl;
    cout<<" Nminus   = " <<Nminus.getVal()  <<" +/- "<<Nminus.getPropagatedError(*fitres)<<endl;
    fout<<" Nminus   = " <<Nminus.getVal()  <<" +/- "<<Nminus.getPropagatedError(*fitres)<<endl;


    TPad *pad1  = new TPad("pad1", "pad1", 0., 0., 1.0, 1.0); // 0,0.3525,1,1);
    if( doLogScaleX[Idx] ) { 
      pad1->SetLogx(1); 
    }
    if( doLogScaleY[Idx] ) pad1->SetLogy(1);

    pad1->SetBottomMargin(0.10);
    pad1->SetLeftMargin(0.14);
    pad1->SetRightMargin(0.075);
    cOUT1->cd();
    pad1->Draw();

    pad1->cd();

    RooPlot* frame1 = var.frame(Title((Savename[Idx]+" sample").c_str()));

    // Plot all data tagged as cosThetaZ sample
    // combData.plotOn(frame1,Cut("sample==sample::cosThetaZ"),MarkerSize(1),MarkerStyle(24),LineWidth(1),Binning(Nbin[Idx]));
    // // Plot "cosThetaZ" slice of simultaneous pdf. 
    // // NBL You _must_ project the sample index category with data using ProjWData as a RooSimultaneous makes no prediction on the shape in the index category and can thus not be integrated
    // simPdf.plotOn(frame1,Slice(sample,"cosThetaZ"),Components("PDF_st_Pzplus" ),ProjWData(sample,combData),LineStyle(kDashed),LineColor(color[0]),LineWidth(5)); // blue
    // simPdf.plotOn(frame1,Slice(sample,"cosThetaZ"),Components("PDF_st_Pzminus"),ProjWData(sample,combData),LineStyle(kDashed),LineColor(color[1]),LineWidth(5)); // red
    // simPdf.plotOn(frame1,Slice(sample,"cosThetaZ"),ProjWData(sample,combData),LineColor(kBlack),LineWidth(1));

    data.plotOn(frame1,MarkerSize(1),MarkerStyle(24),LineWidth(1));
    model.plotOn(frame1,Components("PDF_st_Pzplus" ),LineStyle(kDashed),LineColor(color[0]),LineWidth(2));//blue
    model.plotOn(frame1,Components("PDF_st_Pzminus"),LineStyle(kDashed),LineColor(color[1]),LineWidth(2));//red
    model.plotOn(frame1,LineStyle(1), LineColor(kBlack),LineWidth(1));
    //model.createHistogram("histo_model",var)->Print("all");

    frame1->GetXaxis()->SetTitle(XTitle[Idx].c_str());
    // frame->GetXaxis()->SetTitleSize(0.12);
    frame1->GetXaxis()->SetTitleOffset(0.75);
    frame1->GetXaxis()->SetLabelSize(0.035);
    // frame->GetYaxis()->SetTitle(YTitle[Idx].c_str());
    frame1->GetYaxis()->SetTitleOffset(1.2);
    frame1->GetYaxis()->SetLabelSize(0.035);    

    frame1->SetMaximum(frame1->GetMaximum()*1.2);
    frame1->SetMinimum(0.);

    frame1->Draw("") ;
    cOUT1->Update();
    

    // chi-square/d.o.f.
    cout<<" chi/dof = " <<frame1->chiSquare(5)<<endl;
    fout<<" chi/dof = " <<frame1->chiSquare(5)<<endl;
    cout<<" edm = " <<fitres->edm()<<endl;
    fout<<" edm = " <<fitres->edm()<<endl;
    fout.close();

    leg->SetBorderSize(0);  
    leg->SetTextSize(0.035); 
    leg->SetLineColor(0);   // transparent legend
    leg->SetLineStyle(1); 
    leg->SetLineWidth(1); 
    leg->SetFillColor(0); 
    leg->SetFillStyle(0);   // transparent legend

    cOUT1->cd();
    pad1->cd();
    int nlines=0;
    ATLASLabel(0.17,0.90,"Internal",1); ++nlines;
    if     (analysis=="top" ) { myText(0.17, 0.90-nlines*0.05, 1, 0.033, Form("single-top template fit  %s",extrapath.c_str())); ++nlines; }
    else if(analysis=="test") { myText(0.17, 0.90-nlines*0.05, 1, 0.033, "test"                                               ); ++nlines; }
    else {;}
    //myText(0.17, 0.90-nlines*0.05, 1       , 0.033, Form("#int L dt = %.1f pb^{-1}, #sqrt{s} = 13 TeV",Luminosity)); ++nlines;
    // myText(0.15, 0.90, 1,Form("Region %s b%s",jet_mult.c_str(),promo_bin.c_str()));
    //myText(0.17, 0.90-nlines*0.05, 1       , 0.033, "#mu+jets channel"); ++nlines;
    myText(0.17, 0.90-nlines*0.05, 1       , 0.033, Form("%s",Savename[Idx].c_str())); ++nlines;
    myText(0.17, 0.90-nlines*0.05, kBlack  , 0.033, Form("P_{z} = %.2f #pm %.2f",Pz.getVal(),Pz.getError())); ++nlines;
    myText(0.17, 0.90-nlines*0.05, color[0], 0.033, Form("N_{+} = %.2f #pm %.2f",Nplus.getVal(),Nplus.getError())); ++nlines;
    myText(0.17, 0.90-nlines*0.05, color[1], 0.033, Form("N_{-} = %.2f #pm %.2f",Nminus.getVal(),Nminus.getPropagatedError(*fitres))); ++nlines;
    myText(0.17, 0.90-nlines*0.05, 1       , 0.033, Form("#chi^{2}/d.o.f. = %.2f",frame1->chiSquare(5))); ++nlines;
    pad1->Update();

    cOUT1->cd();
    cOUT1->Update();


    std::string dummy   = "";
    dummy = Form("%s%s",path.Data(),Savename[Idx].c_str());//png";eps
    std::cout<<"dummy : "<<dummy<<std::endl;
    cOUT1->SaveAs((dummy+".png").c_str());
    cOUT1->SaveAs((dummy+".pdf").c_str());


    delete fitres;
    delete frame1;

    delete pad1;

    delete leg;

    delete cOUT1;
  } // end variables loop

  cout<<"\n DONE \n";
}
