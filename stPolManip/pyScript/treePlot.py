import ROOT
import os,sys

file = ROOT.TFile("../build/Debug/5kSM.root")
f = ROOT.TFile.Open("test.root","recreate")
tree = file.Get("singleTop")
h1 = ROOT.TH1F('','',20,-10,10)
tree.Draw("top.id>>h1","","*H")
h1.Write()
f.Write()
f.Close()

