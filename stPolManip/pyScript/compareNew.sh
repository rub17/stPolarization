#!/bin/bash

plotdir=plots/

tree0name="singleTop"
tree1name="singleTop"
tree2name="singleTop"
tree3name="singleTop"

weight0="(top.id==6)"
weight1="(top.id==6)"
weight2="(top.id==6)"
weight3="(top.id==6)"

opt0="norm hist e"
opt1="norm e same"
opt2="norm hist e same"
opt3="norm e same"

nvar=1000

var0=("cosTheta>>h(32,-1,1)"
      "cosThetaStar>>h(32,-1,1)"
      "Phi>>h(32,0,6.28)"   
      "PhiStar>>h(32,0,6.28)"

      "cosThetaZ>>h(20,-1,1)"          
      "cosThetaX>>h(20,-1,1)"          
      "cosThetaY>>h(20,-1,1)"          
                                  
      "top.pt>>h(40,0,200)"           
      "top.eta>>h(40,-6,6)"           
      "top.phi>>h(40,-3.14,3.14)"    
      "top.mass>>h(80,150,200)"      
                                        
      "W.pt>>h(40,0,200)"            
      "W.eta>>h(40,-6,6)"            
      "W.phi>>h(40,-3.14,3.14)"      
      "W.mass>>h(80,60,100)"         
                                        
      "bottom.pt>>h(40,0,200)"       
      "bottom.eta>>h(40,-6,6)"       
      "bottom.phi>>h(40,-3.14,3.14)" 
      "bottom.mass>>h(40,0,40)"      
                                        
      "lepton.pt>>h(60,0,120)"       
      "lepton.eta>>h(40,-6,6)"       
      "lepton.phi>>h(40,-3.14,3.14)" 
      "lepton.mass>>h(40,0,40)"      
 
      "neutrino.pt>>h(60,0,120)"       
      "neutrino.eta>>h(40,-6,6)"       
      "neutrino.phi>>h(40,-3.14,3.14)" 
      "neutrino.mass>>h(40,0,40)"      
                                        
      "WT.pt>>h(50,0,100)"           
      "WT.eta>>h(40,-6,6)"           
      "WT.phi>>h(40,-3.14,3.14)"     
      "WT.mass>>h(80,60,100)"        
                                        
      "leptonW.pt>>h(60,0,120)"       
      "leptonW.eta>>h(40,-6,6)"       
      "leptonW.phi>>h(40,-3.14,3.14)" 
      "leptonW.mass>>h(40,0,40)"      
     )
var1=("cosTheta"
      "cosThetaStar"
      "Phi"   
      "PhiStar"

      "cosThetaZ"          
      "cosThetaX"          
      "cosThetaY"          
                                  
      "top.pt"           
      "top.eta"           
      "top.phi"    
      "top.mass"      
                                        
      "W.pt"            
      "W.eta"            
      "W.phi"      
      "W.mass"         
                                        
      "bottom.pt"       
      "bottom.eta"       
      "bottom.phi" 
      "bottom.mass"      
                                        
      "lepton.pt"       
      "lepton.eta"       
      "lepton.phi" 
      "lepton.mass"      
 
      "neutrino.pt"       
      "neutrino.eta"       
      "neutrino.phi" 
      "neutrino.mass"      
                                        
      "WT.pt"           
      "WT.eta"           
      "WT.phi"     
      "WT.mass"        
                                        
      "leptonW.pt"       
      "leptonW.eta"       
      "leptonW.phi" 
      "leptonW.mass"      
     )
var2=("cosTheta"
      "cosThetaStar"
      "Phi"   
      "PhiStar"

      "cosThetaZ"          
      "cosThetaX"          
      "cosThetaY"          
                                  
      "top.pt"           
      "top.eta"           
      "top.phi"    
      "top.mass"      
                                        
      "W.pt"            
      "W.eta"            
      "W.phi"      
      "W.mass"         
                                        
      "bottom.pt"       
      "bottom.eta"       
      "bottom.phi" 
      "bottom.mass"      
                                        
      "lepton.pt"       
      "lepton.eta"       
      "lepton.phi" 
      "lepton.mass"      
 
      "neutrino.pt"       
      "neutrino.eta"       
      "neutrino.phi" 
      "neutrino.mass"      
                                        
      "WT.pt"           
      "WT.eta"           
      "WT.phi"     
      "WT.mass"        
                                        
      "leptonW.pt"       
      "leptonW.eta"       
      "leptonW.phi" 
      "leptonW.mass"      
     )
var3=("cosTheta"
      "cosThetaStar"
      "Phi"   
      "PhiStar"

      "cosThetaZ"          
      "cosThetaX"          
      "cosThetaY"          
                                  
      "top.pt"           
      "top.eta"           
      "top.phi"    
      "top.mass"      
                                        
      "W.pt"            
      "W.eta"            
      "W.phi"      
      "W.mass"         
                                        
      "bottom.pt"       
      "bottom.eta"       
      "bottom.phi" 
      "bottom.mass"      
                                        
      "lepton.pt"       
      "lepton.eta"       
      "lepton.phi" 
      "lepton.mass"      
 
      "neutrino.pt"       
      "neutrino.eta"       
      "neutrino.phi" 
      "neutrino.mass"      
                                        
      "WT.pt"           
      "WT.eta"           
      "WT.phi"     
      "WT.mass"        
                                        
      "leptonW.pt"       
      "leptonW.eta"       
      "leptonW.phi" 
      "leptonW.mass"      
     )
name=("cosTheta"
      "cosThetaStar"
      "Phi"   
      "PhiStar"

      "cosThetaZ"          
      "cosThetaX"          
      "cosThetaY"          
                                  
      "top_pt"           
      "top_eta"           
      "top_phi"    
      "top_mass"      
                                        
      "W_pt"            
      "W_eta"            
      "W_phi"      
      "W_mass"         
                                       
      "bottom_pt"       
      "bottom_eta"       
      "bottom_phi" 
      "bottom_mass"      
                                       
      "lepton_pt"       
      "lepton_eta"       
      "lepton_phi" 
      "lepton_mass"      
 
      "neutrino_pt"       
      "neutrino_eta"       
      "neutrino_phi" 
      "neutrino_mass"      
                                       
      "WT_pt"           
      "WT_eta"           
      "WT_phi"     
      "WT_mass"        
                                       
      "leptonW_pt"       
      "leptonW_eta"       
      "leptonW_phi" 
      "leptonW_mass"
     )
mkdir -p $plotdir

root -l -b $* <<EOF
((TTree*)_file0->Get("$tree0name"))->SetLineColor(kBlue);   ((TTree*)_file0->Get("$tree0name"))->SetMarkerColor(kBlue);   ((TTree*)_file0->Get("$tree0name"))->SetFillStyle(0); ((TTree*)_file0->Get("$tree0name"))->SetFillColorAlpha(0,1);
((TTree*)_file1->Get("$tree1name"))->SetLineColor(kRed);    ((TTree*)_file1->Get("$tree1name"))->SetMarkerColor(kRed);    ((TTree*)_file1->Get("$tree1name"))->SetFillStyle(0); ((TTree*)_file1->Get("$tree1name"))->SetFillColorAlpha(0,1); 
((TTree*)_file2->Get("$tree2name"))->SetLineColor(kCyan);   ((TTree*)_file2->Get("$tree2name"))->SetMarkerColor(kCyan);   ((TTree*)_file2->Get("$tree2name"))->SetFillStyle(0); ((TTree*)_file2->Get("$tree2name"))->SetFillColorAlpha(0,1);
((TTree*)_file3->Get("$tree3name"))->SetLineColor(kMagenta);((TTree*)_file3->Get("$tree3name"))->SetMarkerColor(kMagenta);((TTree*)_file3->Get("$tree3name"))->SetFillStyle(0); ((TTree*)_file3->Get("$tree3name"))->SetFillColorAlpha(0,1); 
TCanvas c;
TLegend *leg = new TLegend(0.78, 0.60, 0.78+.2, 0.60+.15, NULL, "brNDC");
((TTree*)_file0->Get("$tree0name"))->Draw("1>>h0","1","$opt0"); leg->AddEntry((TH1F*)gROOT->FindObject("h0"),string(_file0->GetName()).substr(0,string(_file0->GetName()).find(".")).substr(string(_file0->GetName()).find_last_of("/")+1).c_str(),"ELP");
((TTree*)_file1->Get("$tree1name"))->Draw("1>>h1","1","$opt1"); leg->AddEntry((TH1F*)gROOT->FindObject("h1"),string(_file1->GetName()).substr(0,string(_file1->GetName()).find(".")).substr(string(_file1->GetName()).find_last_of("/")+1).c_str(),"ELP");
((TTree*)_file2->Get("$tree2name"))->Draw("1>>h2","1","$opt2"); leg->AddEntry((TH1F*)gROOT->FindObject("h2"),string(_file2->GetName()).substr(0,string(_file2->GetName()).find(".")).substr(string(_file2->GetName()).find_last_of("/")+1).c_str(),"ELP");
((TTree*)_file3->Get("$tree3name"))->Draw("1>>h3","1","$opt3"); leg->AddEntry((TH1F*)gROOT->FindObject("h3"),string(_file3->GetName()).substr(0,string(_file3->GetName()).find(".")).substr(string(_file3->GetName()).find_last_of("/")+1).c_str(),"ELP");

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[0]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[0]}"); ((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[0]}","$weight1","$opt1");
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[0]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[0]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[0]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[1]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[1]}"); ((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[1]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[1]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[1]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[1]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[2]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[2]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[2]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[2]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[2]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[2]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[3]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[3]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[3]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[3]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[3]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[3]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[4]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[4]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[4]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[4]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[4]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[4]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[5]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[5]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[5]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[5]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[5]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[5]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[6]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[6]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[6]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[6]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[6]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[6]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[7]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[7]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[7]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[7]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[7]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[7]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[8]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[8]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[8]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[8]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[8]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[8]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[9]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[9]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[9]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[9]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[9]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[9]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[10]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[10]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[10]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[10]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[10]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[10]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[11]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[11]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[11]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[11]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[11]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[11]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[12]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[12]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[12]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[12]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[12]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[12]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[13]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[13]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[13]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[13]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[13]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[13]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[14]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[14]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[14]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[14]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[14]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[14]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[15]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[15]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[15]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[15]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[15]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[15]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[16]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[16]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[16]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[16]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[16]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[16]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[17]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[17]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[17]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[17]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[17]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[17]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[18]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[18]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[18]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[18]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[18]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[18]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[19]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[19]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[19]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[19]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[19]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[19]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[20]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[20]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[20]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[20]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[20]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[20]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[21]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[21]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[21]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[21]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[21]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[21]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[22]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[22]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[22]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[22]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[22]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[22]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[23]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[23]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[23]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[23]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[23]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[23]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[24]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[24]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[24]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[24]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[24]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[24]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[25]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[25]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[25]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[25]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[25]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[25]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[26]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[26]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[26]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[26]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[26]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[26]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[27]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[27]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[27]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[27]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[27]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[27]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[28]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[28]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[28]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[28]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[28]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[28]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[29]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[29]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[29]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[29]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[29]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[29]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[30]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[30]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[30]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[30]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[30]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[30]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[31]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[31]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[31]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[31]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[31]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[31]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[32]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[32]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[32]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[32]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[32]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[32]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[33]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[33]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[33]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[33]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[33]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[33]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[34]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[34]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[34]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[34]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[34]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[34]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[35]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[35]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[35]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[35]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[35]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[35]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[36]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[36]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[36]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[36]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[36]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[36]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[37]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[37]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[37]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[37]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[37]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[37]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[38]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[38]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[38]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[38]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[38]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[38]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[39]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[39]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[39]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[39]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[39]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[39]}.png"); 

((TTree*)_file0->Get("$tree0name"))->Draw("${var0[40]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[40]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[40]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[40]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[40]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[40]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[41]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[41]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[41]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[41]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[41]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[41]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[42]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[42]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[42]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[42]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[42]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[42]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[43]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[43]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[43]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[43]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[43]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[43]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[44]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[44]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[44]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[44]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[44]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[44]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[45]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[45]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[45]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[45]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[45]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[45]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[46]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[46]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[46]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[46]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[46]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[46]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[47]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[47]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[47]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[47]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[47]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[47]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[48]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[48]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[48]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[48]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[48]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[48]}.png"); 
						  
((TTree*)_file0->Get("$tree0name"))->Draw("${var0[49]}","$weight0","$opt0"); ((TH1F*)gROOT->FindObject("h"))->GetYaxis()->SetRangeUser(0.,0.1); ((TH1F*)gROOT->FindObject("h"))->GetXaxis()->SetTitle("${name[49]}");((TH1F*)gROOT->FindObject("h"))->SetStats(0);
((TTree*)_file1->Get("$tree1name"))->Draw("${var1[49]}","$weight1","$opt1"); 
((TTree*)_file2->Get("$tree2name"))->Draw("${var2[49]}","$weight2","$opt2"); 
((TTree*)_file3->Get("$tree3name"))->Draw("${var3[49]}","$weight3","$opt3"); leg->Draw(); c.SaveAs("plots/${name[49]}.png"); 





// TCanvas c32; ((TTree*)_file0->Get("$tree0name"))->Draw("cosTheta:cosThetaStar>>h32(32,-1,1,32,-1,1)","$weight0","norm box"); 
// ((TTree*)_file1->Get("$tree1name"))->Draw("cosTheta:cosThetaStar","$weight1","norm box same"); c32.SaveAs("plots/cosTheta_vscosThetaStar.png");

// TCanvas c25; ((TTree*)_file0->Get("$tree0name"))->Draw("cosTheta:cosThetaStar>>h25(32,-1.,1.,32,-1.,1.)","$weight0","norm colz"); c25.SaveAs("plots/cosTheta_vscosThetaStar_0.png");
// TCanvas c26; ((TTree*)_file1->Get("$tree1name"))->Draw("cosTheta:cosThetaStar>>h26(32,-1.,1.,32,-1.,1.)","$weight1","norm colz"); c26.SaveAs("plots/cosTheta_vscosThetaStar_1.png"); 
//
// TCanvas c27; ((TTree*)_file0->Get("$tree0name"))->Draw("cosThetaStar:PhiStar>>h27(32,0.,6.28,32,-1.,1.)","$weight0","norm box"); 
// ((TTree*)_file1->Get("$tree1name"))->Draw("cosThetaStar:PhiStar","$weight1","norm box same"); c27.SaveAs("plots/cosThetaStar_vs_PhiStar.png");
//
// TCanvas c28; ((TTree*)_file0->Get("$tree0name"))->Draw("cosThetaStar:PhiStar>>h28(32,0.,6.28,32,-1.,1.)","$weight0","norm colz"); c28.SaveAs("plots/cosThetaStar_vs_PhiStar_0.png");
// TCanvas c29; ((TTree*)_file1->Get("$tree1name"))->Draw("cosThetaStar:PhiStar>>h29(32,0.,6.28,32,-1.,1.)","$weight1","norm colz"); c29.SaveAs("plots/cosThetaStar_vs_PhiStar_1.png"); 



.qqqq
EOF

