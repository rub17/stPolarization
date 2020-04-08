#include <vector>
void ptCheck(){
    TFile f1("stSTD.root");
    TFile f2("sampleRef.root");
    TTree *gen = (TTree*)f1.Get("singleTop");
    TTree *protos = (TTree*)f2.Get("singleTop");
    std::vector<TString> particle = {"bottom", "W", "lepton"};
    TCanvas *c1 = new TCanvas("c1",particle[0],1500,600);
    TCanvas *c2 = new TCanvas("c2",particle[1],1500,600);
    TCanvas *c3 = new TCanvas("c3",particle[2],1500,600);
    std::vector<TCanvas*> c = {c1,c2,c3};
    TLegend *legend1 = new TLegend(0.5, 0.8, .65, .9);
    TLegend *legend2 = new TLegend(0.5, 0.8, .65, .9);
    TLegend *legend3 = new TLegend(0.5, 0.8, .65, .9);
    std::vector<TLegend*> legd = {legend1,legend2,legend3};

    for (int i=0; i < particle.size(); i++) {
        c[i]->Divide(3,1);
        c[i]->cd(1);
        gen->Draw("_"+particle[i]+".pt","","NORM");
        //gen->Draw("_"+particle[i]+".pt","Event.weight","NORM SAME");
        protos->Draw("_"+particle[i]+".pt","Event.weight","NORM SAME HIST");
        c[i]->cd(2);
        gen->Draw("_"+particle[i]+".eta","","NORM");
        protos->Draw("_"+particle[i]+".eta","Event.weight","NORM SAME HIST");
        c[i]->cd(3);
        gen->Draw("_"+particle[i]+".phi","","NORM");
        protos->Draw("_"+particle[i]+".phi","Event.weight","NORM SAME HIST");
        c[i]->Show();
    }
    

}
