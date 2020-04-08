///
///  Making 2/3 dimensional plots of p.d.f.s and datasets
///
/// Nello Bruscino nello.bruscino@cern.ch

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooConstVar.h"
#include "RooGenericPdf.h"
#include "RooProdPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "RooPlot.h"
#include "RooClassFactory.h"
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include <TSystem.h>

using namespace RooFit;

bool checkpolarization(float &Px, float &Py, float &Pz) {
  int prov=(fabs(Px)>1.)+(fabs(Py)>1.)+(fabs(Pz)>1.); // something wring
  if(prov>1) { 
    std::cout<<"ERROR: provided polarization exceeds 1."<<std::endl; 
    return false; 
  }

  return true;
}

void ndimpdf(float Px=0., float Py=0., float Pz=0., bool top=true)
{
  if(!checkpolarization(Px,Py,Pz)) exit(1);

  std::string part;
  if(top) part = "top";
  else    part = "antitop";
  std::cout<<"\033[41;1;37m "<<part<<" : P = ("<<Px<<", "<<Py<<", "<<Pz<<") \033[0m"<<std::endl<<std::endl;

  //if(Px==1.) Px=0.99;
  //if(Py==1.) Py=0.99;

  gStyle->SetOptStat(0);

  TString outpath = "./Output/";
  gSystem->Exec("mkdir -p "+outpath) ;
  string Savename = "distr_"+part+"_Px"+to_string(Px).substr(0,3)+"_Py"+to_string(Py).substr(0,3)+"_Pz"+to_string(Pz).substr(0,3);


  // C r e a t e   4 D   m o d e l   a n d   d a t a s e t
  // -----------------------------------------------------
  
  double tol = 0.01;
  // Create observables for W in the top rest frame
  RooRealVar theta("theta","#theta",0.+tol,TMath::Pi()-tol);
  RooRealVar phi("phi","#phi",0.+tol,2*TMath::Pi()-tol);
  
  // Create observables for charged lepton in the W rest frame
  RooRealVar thetastar("thetastar","#theta*",0.+tol,TMath::Pi()-tol);
  RooRealVar phistar("phistar","#phi*",0.+tol,2*TMath::Pi()-tol);

  // Create paramenters
  RooRealVar mt("mt","m_t",173.); // GeV
  RooRealVar mW("mW","m_W",82.); // GeV
  //RooRealVar q("q","q",40.); // W momentum in the top rest fram, expressed in GeV
  RooFormulaVar q("q","sqrt(pow(pow(@0,2)+pow(@1,2),2)-4*pow(@0,2)*pow(@1,2))/(2*@0)",RooArgList(mt,mW)); // W momentum in the top rest fram, expressed in GeV
  RooRealVar lambda("lambda","#lambda",1.); // +1 for top and -1 for antitop
  RooRealVar px("px","P_x",Px); // polarization
  RooRealVar py("py","P_y",Py); // polarization
  RooRealVar pz("pz","P_z",Pz); // polarization

  // Coefficients    (pow(mt,2)-pow(mW,2)-2*mt*q)/(pow(mt,2)+pow(mW,2)) + pow(mt,2)/(2*pow(mW,2))*(pow(mt,2)-pow(mW,2)-2*mt*q)/(pow(mt,2)+pow(mW,2)) + (pow(mt,2)-pow(mW,2)+2*mt*q)/(pow(mt,2)+pow(mW,2)) + pow(mt,2)/(2*pow(mW,2))*(pow(mt,2)-pow(mW,2)-2*mt*q)/(pow(mt,2)+pow(mW,2))
  RooFormulaVar a0("a0","(pow(@0,2)-pow(@1,2)-2*@0*@2)/pow(@0,2)",RooArgList(mt,mW,q));
  RooFormulaVar a3("a3","(pow(@0,2)-pow(@1,2)+2*@0*@2)/pow(@0,2)",RooArgList(mt,mW,q));
  RooFormulaVar a2("a2","pow(@0,2)/(2*pow(@1,2))*@2",RooArgList(mt,mW,a0));
  RooFormulaVar a1("a1","pow(@0,2)/(2*pow(@1,2))*@2",RooArgList(mt,mW,a3));
  RooFormulaVar a4("a4","@0/(sqrt(2)*@1)*@2*cos(@3)",RooArgList(mt,mW,a0,phistar));
  RooFormulaVar a5("a5","@0/(sqrt(2)*@1)*@2*cos(@3)",RooArgList(mt,mW,a3,phistar));
  RooFormulaVar a6("a6","@0/(sqrt(2)*@1)*@2*(-sin(@3))",RooArgList(mt,mW,a0,phistar));
  RooFormulaVar a7("a7","@0/(sqrt(2)*@1)*@2*(-sin(@3))",RooArgList(mt,mW,a3,phistar));
  RooFormulaVar NORM("NORM","@0+@1+@2+@3",RooArgList(a0,a1,a2,a3));
  
  // if(fabs(px.getVal())==1.) px = 0.99*px.getVal()/fabs(px.getVal());
  // else if(px.getVal()==0.)  px = 0.01;
  // if(fabs(py.getVal())==1.) py = 0.99*py.getVal()/fabs(py.getVal());
  // else if(py.getVal()==0.)  py = 0.01;
  // if(fabs(pz.getVal())==1.) pz = 0.99*pz.getVal()/fabs(pz.getVal());
  // else if(pz.getVal()==0.)  pz = 0.01;
 
  RooArgSet argset(theta,phi,thetastar,phistar,mt,mW,q); argset.add(px); argset.add(py); argset.add(pz); argset.add(lambda);
  argset.add(a0); argset.add(a1); argset.add(a2); argset.add(a3); argset.add(a4); argset.add(a5); argset.add(a6); argset.add(a7); argset.add(NORM);
  RooAbsPdf *model;
  if(top) {
    lambda = 1.;
    model = RooClassFactory::makePdfInstance(Form("pdf_%s",part.c_str()), "model",
  					     "abs(  3*sin(theta)*sin(thetastar) / (64*pow(TMath::Pi(),2)*NORM) *                                                                                                                                                                                                                                                                 ( (a0*pow(1+lambda*cos(thetastar),2) + 2*a1*pow(sin(thetastar),2)) * (1+pz*cos(theta)+px*cos(phi)*sin(theta)+py*sin(theta)*sin(phi)) +                                                                                                                                                                                         (2*a2*pow(sin(thetastar),2) + a3*pow(1-lambda*cos(thetastar),2)) * (1-pz*cos(theta)+px*cos(phi)*sin(theta)+py*sin(theta)*sin(phi)) +                                                                                                                                                                                         2*sqrt(2)*lambda*(a4*(1+lambda*cos(thetastar)) + a5*(1-lambda*cos(thetastar))) * sin(thetastar) * (-pz*sin(theta)+cos(theta)*(px*cos(phi)+py*sin(phi))) +                                                                                                                                                                    2*sqrt(2)*lambda*(a6*(1+lambda*cos(thetastar)) + a7*(1-lambda*cos(thetastar))) * sin(thetastar) * (px*sin(phi)-py*cos(phi)) )  )", argset);   
  }
  else {
    lambda = -1.;
    model = RooClassFactory::makePdfInstance(Form("pdf_%s",part.c_str()),"model",
					     "abs(  3*sin(theta)*sin(thetastar) / (64*pow(TMath::Pi(),2)*NORM) *                                                                                                                                                                                                                                                                 ( (a3*pow(1+lambda*cos(thetastar),2) + 2*a2*pow(sin(thetastar),2)) * (1+pz*cos(theta)+px*cos(phi)*sin(theta)+py*sin(theta)*sin(phi)) +                                                                                                                                                                                         (2*a1*pow(sin(thetastar),2) + a0*pow(1-lambda*cos(thetastar),2)) * (1-pz*cos(theta)+px*cos(phi)*sin(theta)+py*sin(theta)*sin(phi)) +                                                                                                                                                                                         2*sqrt(2)*lambda*(a5*(1+lambda*cos(thetastar)) + a4*(1-lambda*cos(thetastar))) * sin(thetastar) * (-pz*sin(theta)+cos(theta)*(px*cos(phi)+py*sin(phi))) +                                                                                                                                                                    2*sqrt(2)*lambda*(a7*(1+lambda*cos(thetastar)) + a6*(1-lambda*cos(thetastar))) * sin(thetastar) * (px*sin(phi)-py*cos(phi)) )  )", argset);   
  }
  
  // RooArgSet integSet(theta,phi,thetastar,phistar), anaIntSet;
  // double integr=0;
  // int intcode = model->getAnalyticalIntegral(integSet, anaIntSet);
  // if(intcode!=0) integr = model->analyticalIntegral(intcode);
  // std::cout<<"intcode = "<<intcode<<std::endl;
  // std::cout<<"integr  = "<<integr <<std::endl;
  
  // Sample dataset from gauss(x,y)
  // RooDataSet* data     = model->generate(RooArgSet(theta,phi)        ,10000);
  // RooDataSet* datastar = model->generate(RooArgSet(thetastar,phistar),10000);

  // Silent mode for message service
  //RooMsgService::instance().setSilentMode(kTRUE);
  //for (Int_t i=0 ; i<RooMsgService::instance().numStreams() ; i++)if (RooMsgService::instance().getStream(i).minLevel<RooFit::FATAL) RooMsgService::instance().setStreamStatus(i,kFALSE) ;
  // Sample dataset from gauss(x,y)
  RooDataSet* datafull     = model->generate(RooArgSet(theta,phi,thetastar,phistar)        ,1000000);
  //  Reset message service to default stream configuration
  RooMsgService::instance().reset() ;
  

  // M a k e   4 D   p l o t s   o f   d a t a   a n d   m o d e l
  // -------------------------------------------------------------
  // Create and fill ROOT 2D histogram (8x8x8 bins) with contents of dataset
  TH1* hh_data     = datafull->createHistogram("theta,phi"        ,theta    ,Binning(20),YVar(phi    ,Binning(20)));
  TH1* hh_datastar = datafull->createHistogram("thetastar,phistar",thetastar,Binning(20),YVar(phistar,Binning(20)));
  //TH2F* hh_data      = new TH2F("theta,phi","theta,phi",20,0.,TMath::Pi(),20,0.,2*TMath::Pi());		     
  //TH2F* hh_datastar  = new TH2F("thetastar,phistar","thetastar,phistar",20,0.,TMath::Pi(),20,0.,2*TMath::Pi());
  TH2F* hh_data2     = new TH2F("costheta,phi","costheta,phi",20,-1.,1.,20,0.,2*TMath::Pi());
  TH2F* hh_datastar2 = new TH2F("costhetastar,phistar","costhetastar,phistar",20,-1.,1.,20,0.,2*TMath::Pi());
  for(int i=0; i<datafull->numEntries();++i) {
    //hh_data     ->Fill(((RooRealVar*)datafull->get(i)->find("theta"))    ->getVal(),((RooRealVar*)datafull->get(i)->find("phi"))    ->getVal());
    //hh_datastar ->Fill(((RooRealVar*)datafull->get(i)->find("thetastar"))->getVal(),((RooRealVar*)datafull->get(i)->find("phistar"))->getVal());
    // f(costheta) d(costheta) = f(theta) (-sin(theta) d(theta)
    hh_data2    ->Fill(cos(((RooRealVar*)datafull->get(i)->find("theta"))    ->getVal()),((RooRealVar*)datafull->get(i)->find("phi"))    ->getVal()); //,sin(((RooRealVar*)datafull->get(i)->find("theta"))    ->getVal()));
    hh_datastar2->Fill(cos(((RooRealVar*)datafull->get(i)->find("thetastar"))->getVal()),((RooRealVar*)datafull->get(i)->find("phistar"))->getVal()); //,sin(((RooRealVar*)datafull->get(i)->find("thetastar"))->getVal()));
  }

  // Create and fill ROOT 2D histogram (20x20x20 bins) with sampling of pdf
  TH1* hh_pdf     = model->createProjection(RooArgSet(thetastar,phistar))->createHistogram("(theta,phi) pdf"        ,theta    ,Binning(20),YVar(phi    ,Binning(20)));
  TH1* hh_pdfstar = model->createProjection(RooArgSet(theta    ,phi    ))->createHistogram("(thetastar,phistar) pdf",thetastar,Binning(20),YVar(phistar,Binning(20)));

  hh_data    ->SetLineColor(kBlue) ;
  hh_datastar->SetLineColor(kBlue) ;  
  hh_pdf     ->SetLineColor(kRed) ;
  hh_pdfstar ->SetLineColor(kRed) ;

  TH1* hh_data_theta     = (TH1*)((TH2*)hh_data)    ->ProjectionX();
  TH1* hh_data_phi       = (TH1*)((TH2*)hh_data)    ->ProjectionY();
  TH1* hh_data_thetastar = (TH1*)((TH2*)hh_datastar)->ProjectionX();
  TH1* hh_data_phistar   = (TH1*)((TH2*)hh_datastar)->ProjectionY();
  TH1* hh_pdf_theta      = (TH1*)((TH2*)hh_pdf)     ->ProjectionX();
  TH1* hh_pdf_phi        = (TH1*)((TH2*)hh_pdf)     ->ProjectionY();
  TH1* hh_pdf_thetastar  = (TH1*)((TH2*)hh_pdfstar) ->ProjectionX();
  TH1* hh_pdf_phistar    = (TH1*)((TH2*)hh_pdfstar) ->ProjectionY();
  hh_pdf_theta     ->SetMinimum(0.);
  hh_pdf_phi       ->SetMinimum(0.);
  hh_pdf_thetastar ->SetMinimum(0.);
  hh_pdf_phistar   ->SetMinimum(0.); 
  hh_pdf_theta     ->SetMaximum(hh_pdf_theta    ->GetMaximum()*1.5);
  hh_pdf_phi       ->SetMaximum(hh_pdf_phi      ->GetMaximum()*1.5);
  hh_pdf_thetastar ->SetMaximum(hh_pdf_thetastar->GetMaximum()*1.5);
  hh_pdf_phistar   ->SetMaximum(hh_pdf_phistar  ->GetMaximum()*1.5); 

  TCanvas* c1 = new TCanvas("fullydiff","fully differential xsec",1600,800) ;
  c1->Divide(4,2) ;
  c1->cd(1) ; gPad->SetLeftMargin(0.15) ; hh_data         ->GetZaxis()->SetTitleOffset(1.4) ; hh_data         ->Draw("lego") ; hh_pdf     ->Draw("surf same") ; 
  c1->cd(2) ; gPad->SetLeftMargin(0.20) ; hh_pdf          ->GetZaxis()->SetTitleOffset(2.5) ; hh_pdf          ->Draw("colz") ;
  c1->cd(3) ; gPad->SetLeftMargin(0.20) ; hh_pdf_theta    ->GetZaxis()->SetTitleOffset(1.4) ; hh_pdf_theta    ->Draw("c") ; hh_data_theta->DrawNormalized("hist same") ;
  c1->cd(4) ; gPad->SetLeftMargin(0.20) ; hh_pdf_phi      ->GetZaxis()->SetTitleOffset(1.4) ; hh_pdf_phi      ->Draw("c") ; hh_data_phi  ->DrawNormalized("hist same") ;
  c1->cd(5) ; gPad->SetLeftMargin(0.15) ; hh_datastar     ->GetZaxis()->SetTitleOffset(1.4) ; hh_datastar     ->Draw("lego") ; hh_pdfstar ->Draw("surf same") ;
  c1->cd(6) ; gPad->SetLeftMargin(0.15) ; hh_pdfstar      ->GetZaxis()->SetTitleOffset(2.5) ; hh_pdfstar      ->Draw("colz") ;  // lego surf box cont3 colz
  c1->cd(7) ; gPad->SetLeftMargin(0.20) ; hh_pdf_thetastar->GetZaxis()->SetTitleOffset(1.4) ; hh_pdf_thetastar->Draw("c") ; hh_data_thetastar->DrawNormalized("hist same") ;
  c1->cd(8) ; gPad->SetLeftMargin(0.20) ; hh_pdf_phistar  ->GetZaxis()->SetTitleOffset(1.4) ; hh_pdf_phistar  ->Draw("c") ; hh_data_phistar  ->DrawNormalized("hist same") ;

  c1->Update();
  std::string dummy = "";
  
  dummy = Form("%s%s.png",outpath.Data(),Savename.c_str());//png";eps
  std::cout<<"dummy : "<<dummy<<std::endl;
  c1->SaveAs(dummy.c_str());
  dummy = Form("%s%s.pdf",outpath.Data(),Savename.c_str());//png";eps
  c1->SaveAs(dummy.c_str());

  //delete c1;  

  // COSIN PLOTS
  hh_data2    ->SetLineColor(kBlue) ;
  hh_datastar2->SetLineColor(kBlue) ;  
  hh_data2    ->GetXaxis()->SetTitle("cos(#theta)");   hh_data2    ->GetYaxis()->SetTitle("#phi");  hh_data2    ->GetZaxis()->SetTitle("Events");
  hh_datastar2->GetXaxis()->SetTitle("cos(#theta*)");  hh_datastar2->GetYaxis()->SetTitle("#phi*"); hh_datastar2->GetZaxis()->SetTitle("Events");

  TH1* hh_data2_theta     = (TH1*)((TH2*)hh_data2)    ->ProjectionX();
  TH1* hh_data2_phi       = (TH1*)((TH2*)hh_data2)    ->ProjectionY();
  TH1* hh_data2_thetastar = (TH1*)((TH2*)hh_datastar2)->ProjectionX();
  TH1* hh_data2_phistar   = (TH1*)((TH2*)hh_datastar2)->ProjectionY();
  hh_data2_theta     ->SetMinimum(0.);
  hh_data2_phi       ->SetMinimum(0.);
  hh_data2_thetastar ->SetMinimum(0.);
  hh_data2_phistar   ->SetMinimum(0.); 
  hh_data2_theta     ->SetMaximum(hh_data2_theta    ->GetMaximum()*1.5);
  hh_data2_phi       ->SetMaximum(hh_data2_phi      ->GetMaximum()*1.5);
  hh_data2_thetastar ->SetMaximum(hh_data2_thetastar->GetMaximum()*1.5);
  hh_data2_phistar   ->SetMaximum(hh_data2_phistar  ->GetMaximum()*1.5); 

  TCanvas* c2 = new TCanvas("costheta","costheta",1600,800) ;
  c2->Divide(4,2) ;
  c2->cd(1) ; gPad->SetLeftMargin(0.15) ; hh_data2        ->GetZaxis()->SetTitleOffset(1.4) ; hh_data2        ->Draw("lego") ;
  c2->cd(2) ; gPad->SetLeftMargin(0.20) ; hh_data2        ->GetZaxis()->SetTitleOffset(2.5) ; hh_data2        ->Draw("colz") ;
  c2->cd(3) ; gPad->SetLeftMargin(0.20) ; hh_data2_theta   ->GetZaxis()->SetTitleOffset(1.4) ; hh_data2_theta   ->Draw("hist c") ;
  c2->cd(4) ; gPad->SetLeftMargin(0.20) ; hh_data2_phi     ->GetZaxis()->SetTitleOffset(1.4) ; hh_data2_phi     ->Draw("hist c") ;
  c2->cd(5) ; gPad->SetLeftMargin(0.15) ; hh_datastar2    ->GetZaxis()->SetTitleOffset(1.4) ; hh_datastar2    ->Draw("lego") ;
  c2->cd(6) ; gPad->SetLeftMargin(0.15) ; hh_datastar2    ->GetZaxis()->SetTitleOffset(2.5) ; hh_datastar     ->Draw("colz") ;
  c2->cd(7) ; gPad->SetLeftMargin(0.20) ; hh_data2_thetastar->GetZaxis()->SetTitleOffset(1.4) ; hh_data2_thetastar->Draw("hist c") ;
  c2->cd(8) ; gPad->SetLeftMargin(0.20) ; hh_data2_phistar  ->GetZaxis()->SetTitleOffset(1.4) ; hh_data2_phistar  ->Draw("hist c") ;

  c2->Update();
  
  dummy = Form("%s%s_cosin.png",outpath.Data(),Savename.c_str());//png";eps
  std::cout<<"dummy : "<<dummy<<std::endl;
  c2->SaveAs(dummy.c_str());
  dummy = Form("%s%s_cosin.pdf",outpath.Data(),Savename.c_str());//png";eps
  c2->SaveAs(dummy.c_str());
  
  //delete c2;  



  /*
  //direct generation! NOt working....
  RooArgSet generateVars(theta,phi,thetastar,phistar), directVars;
  int gencode = model->getGenerator(generateVars, directVars);
  std::cout<<"gencode = "<<gencode<<std::endl;

  TH2F* hh_data3     = new TH2F("costheta,phi new","costheta,phi new",20,-1.,1.,20,0.,2*TMath::Pi());
  TH2F* hh_datastar3 = new TH2F("costhetastar,phistar new","costhetastar,phistar new",20,-1.,1.,20,0.,2*TMath::Pi());

  for(int i=0; i<datafull->numEntries(); ++i) {
    model->generateEvent(gencode);
    hh_data3    ->Fill(cos(((RooRealVar*)directVars.find("theta"))    ->getVal()),((RooRealVar*)directVars.find("phi"))    ->getVal());
    hh_datastar3->Fill(cos(((RooRealVar*)directVars.find("thetastar"))->getVal()),((RooRealVar*)directVars.find("phistar"))->getVal());
}
  TH1* hh_data3_theta     = (TH1*)((TH2*)hh_data3)    ->ProjectionX();
  TH1* hh_data3_phi       = (TH1*)((TH2*)hh_data3)    ->ProjectionY();
  TH1* hh_data3_thetastar = (TH1*)((TH2*)hh_datastar3)->ProjectionX();
  TH1* hh_data3_phistar   = (TH1*)((TH2*)hh_datastar3)->ProjectionY();

  hh_data3_theta     ->SetMinimum(0.);
  hh_data3_phi       ->SetMinimum(0.);
  hh_data3_thetastar ->SetMinimum(0.);
  hh_data3_phistar   ->SetMinimum(0.); 
  hh_data3_theta     ->SetMaximum(hh_data3_theta    ->GetMaximum()*1.5);
  hh_data3_phi       ->SetMaximum(hh_data3_phi      ->GetMaximum()*1.5);
  hh_data3_thetastar ->SetMaximum(hh_data3_thetastar->GetMaximum()*1.5);
  hh_data3_phistar   ->SetMaximum(hh_data3_phistar  ->GetMaximum()*1.5); 

  TCanvas* c3 = new TCanvas("costheta new","costhetanew",1600,800) ;
  c3->Divide(4,2) ;
  c3->cd(1) ; gPad->SetLeftMargin(0.15) ; hh_data3        ->GetZaxis()->SetTitleOffset(1.4) ; hh_data3        ->Draw("lego") ;
  c3->cd(2) ; gPad->SetLeftMargin(0.20) ; hh_data3        ->GetZaxis()->SetTitleOffset(2.5) ; hh_data3        ->Draw("colz") ;
  c3->cd(3) ; gPad->SetLeftMargin(0.20) ; hh_data3_theta   ->GetZaxis()->SetTitleOffset(1.4) ; hh_data3_theta   ->Draw("c") ;
  c3->cd(4) ; gPad->SetLeftMargin(0.20) ; hh_data3_phi     ->GetZaxis()->SetTitleOffset(1.4) ; hh_data3_phi     ->Draw("c") ;
  c3->cd(5) ; gPad->SetLeftMargin(0.15) ; hh_datastar3    ->GetZaxis()->SetTitleOffset(1.4) ; hh_datastar3    ->Draw("lego") ;
  c3->cd(6) ; gPad->SetLeftMargin(0.15) ; hh_datastar3    ->GetZaxis()->SetTitleOffset(2.5) ; hh_datastar     ->Draw("colz") ;
  c3->cd(7) ; gPad->SetLeftMargin(0.20) ; hh_data3_thetastar->GetZaxis()->SetTitleOffset(1.4) ; hh_data3_thetastar->Draw("c") ;
  c3->cd(8) ; gPad->SetLeftMargin(0.20) ; hh_data3_phistar  ->GetZaxis()->SetTitleOffset(1.4) ; hh_data3_phistar  ->Draw("c") ;

  c2->Update();
  
  dummy = Form("%s%s_cosin.png",outpath.Data(),Savename.c_str());//png";eps
  std::cout<<"dummy : "<<dummy<<std::endl;
  c2->SaveAs(dummy.c_str());
  dummy = Form("%s%s_cosin.pdf",outpath.Data(),Savename.c_str());//png";eps
  c2->SaveAs(dummy.c_str());
  
  delete c3;
  */

}
