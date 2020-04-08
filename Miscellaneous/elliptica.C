//
// ATLAS Style, based on a style file from BaBar
//

#include <iostream>
#include "AtlasStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TArc.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1.h"
#include "RooWorkspace.h"
#include "TGraph.h"
#include "TF1.h"
#include "TEllipse.h"

void SetAtlasStyle ()
{
  static TStyle* atlasStyle = 0;
  std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;
  if ( atlasStyle==0 ) atlasStyle = AtlasStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();
}

TStyle* AtlasStyle() 
{
  TStyle *atlasStyle = new TStyle("ATLAS","Atlas style");
  // use plain black on white colors
  Int_t icol=0; // WHITE
  atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetFrameFillColor(icol);
  atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetCanvasColor(icol);
  atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetStatColor(icol);
  //atlasStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  atlasStyle->SetPaperSize(20,26);

  // set margin sizes
  atlasStyle->SetPadTopMargin(0.05);
  atlasStyle->SetPadRightMargin(0.05);
  atlasStyle->SetPadBottomMargin(0.16);
  atlasStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  atlasStyle->SetTitleXOffset(1.4);
  atlasStyle->SetTitleYOffset(1.4);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  atlasStyle->SetTextFont(font);

  atlasStyle->SetTextSize(tsize);
  atlasStyle->SetLabelFont(font,"x");
  atlasStyle->SetTitleFont(font,"x");
  atlasStyle->SetLabelFont(font,"y");
  atlasStyle->SetTitleFont(font,"y");
  atlasStyle->SetLabelFont(font,"z");
  atlasStyle->SetTitleFont(font,"z");
  
  atlasStyle->SetLabelSize(tsize,"x");
  atlasStyle->SetTitleSize(tsize,"x");
  atlasStyle->SetLabelSize(tsize,"y");
  atlasStyle->SetTitleSize(tsize,"y");
  atlasStyle->SetLabelSize(tsize,"z");
  atlasStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  atlasStyle->SetMarkerStyle(20);
  atlasStyle->SetMarkerSize(1.2);
  atlasStyle->SetHistLineWidth(2.);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars (as recommended in ATLAS figure guidelines)
  atlasStyle->SetErrorX(0.0001);
  // get rid of error bar caps
  atlasStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  atlasStyle->SetOptTitle(0);
  //atlasStyle->SetOptStat(1111);
  atlasStyle->SetOptStat(0);
  //atlasStyle->SetOptFit(1111);
  atlasStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  atlasStyle->SetPadTickX(1);
  atlasStyle->SetPadTickY(1);

  return atlasStyle;

}

void elliptica()
{
  SetAtlasStyle();
  
  TCanvas *c1 = new TCanvas("c1");
  c1->SetCanvasSize(600,600);
  c1->Range(-2,-2,2,2);
  gPad->DrawFrame(-1.2,-1.2,1.2,1.2);
  c1->SetFixedAspectRatio();
// =========================================================================

  // ATLAS label
  // TLatex *   tex = new TLatex(0.6,0.88,"#bf{#it{ATLAS}} Internal");
  // TLatex *   tex = new TLatex(0.6,0.88,"#bf{#it{ATLAS}} Preliminary");
  TLatex *   tex = new TLatex(0.2,0.9,"#bf{#it{ATLAS}} Internal");
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetLineWidth(2);
  tex->SetTextSize(0.04);
  tex->Draw();

  TLatex *tex2 = new TLatex(0.2, 0.86, "#sqrt{s} = 13 TeV, 140.5 fb^{-1}");
  tex2->SetNDC();
  tex2->SetTextFont(42);
  tex2->SetLineWidth(2);
  tex2->SetTextSize(0.03);
  tex2->Draw();

  // =======================================================================

  //TH1 *frame = c1->DrawFrame(-2,-2,2,2);
  //frame->SetXTitle("Pz");
  //frame->SetYTitle("Px");
  //c1->Update();
  TArc *arc = new TArc(0,0,1);
  arc->SetLineColor(kBlack);
  arc->SetLineWidth(3);
  arc->SetFillColorAlpha(0,0);
  arc->Draw("A");
  TEllipse *el_t_sys = new TEllipse(0.90,0,0.217,0.10163,0,360,28.3382);
  el_t_sys->Draw("SAME");
  el_t_sys->SetFillColorAlpha(kGreen,0);
  el_t_sys->SetLineColor(kBlue);
  el_t_sys->SetLineWidth(2);
  TEllipse *el_t_stat = new TEllipse(0.90,0,0.04276,0.021,0,360,32.044);
  el_t_stat->SetFillColor(kOrange);
  el_t_stat->SetLineColor(kOrange);
  el_t_stat->SetLineWidth(2);
  el_t_stat->Draw("SAME");
  TMarker *m_t = new TMarker(0.90,0,34);
  m_t->Draw("SAME");
  
  TEllipse *el_tb_sys = new TEllipse(-0.87,-0.04,0.3421,0.12784,0,360,40.5332);
  el_tb_sys->SetFillColorAlpha(kGreen,0);
  el_tb_sys->SetLineColor(kBlue);
  el_tb_sys->SetLineWidth(2);
  el_tb_sys->Draw("SAME");
  
  TEllipse *el_tb_stat = new TEllipse(-0.87,-0.04,0.0714,0.02552,0,360,40.5332);
  el_tb_stat->SetFillColor(kOrange);
  el_tb_stat->SetLineColor(kOrange);
  el_tb_stat->SetLineWidth(2);
  el_tb_stat->Draw("SAME");

  TMarker *m_tb = new TMarker(-0.87,-0.04,34);
  m_tb->Draw("SAME");

  TLatex *textb = new TLatex(0.2, 0.5, "anti-top");
  textb->SetNDC();
  textb->SetTextFont(42);
  textb->SetLineWidth(2);
  textb->SetTextSize(0.03);
  textb->Draw();

  TLatex *text = new TLatex(0.7, 0.5, "top");
  text->SetNDC();
  text->SetTextFont(42);
  text->SetLineWidth(2);
  text->SetTextSize(0.03);
  text->Draw();

  TLatex *textAs = new TLatex(0.2, 0.82, "#bf{#it{Asimov Fit}}");
  textAs->SetNDC();
  textAs->SetTextFont(42);
  textAs->SetLineWidth(2);
  textAs->SetTextSize(0.03);
  textAs->Draw();
  
  //Legend ========================
  TLegend *mylegend = new TLegend(0.85, 0.1, 0.95, 0.25);
  mylegend->SetFillColor(0);
  mylegend->SetBorderSize(0);
  mylegend->SetTextSize(0.02);
  mylegend->SetTextFont(42);
  mylegend->AddEntry(m_t, "best Fit", "p");
  mylegend->AddEntry(el_t_stat, "stat. only 68%CL", "l");
  mylegend->AddEntry(el_t_sys, "stat.+syst. 68%CL ", "l");
  mylegend->Draw(); 

}
