// stPolManip.h
//
// 1.15.2018
// Runyu Bi
//
// Package to generate polarized tops
//

#ifndef stPolManip_H
#define stPolManip_H

#include <iostream>
#include <string>
#include <math.h> 
#include <cmath>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <random>
#include <thread>
#include <ctime>
// Root analysis:
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TFile.h"
#include "TProfile.h"
#include "TTree.h"
//#pragma link C++ class vector<TH1F>*
//#include "AtlasStyle.C"
//input formats:
#include "protosInput.h"

typedef struct {Float_t cos_Theta,Phi_,cos_Theta_Star,Phi_Star;} FOURANGLES;
typedef struct {Float_t cos_Theta_X,cos_Theta_Y,cos_Theta_Z;} XYZ;
typedef struct {Int_t eventNumber; Float_t weight, eventQ, eventS;} EVENTINFO;
typedef struct {Int_t id,color,flow; Float_t pt,eta,phi,mass;} PARTICLE;
typedef struct {Int_t id,color,flow; Float_t pz,mass;} BEAM;

class stPolManip
{
    public:
    
        //Default Constructor
        stPolManip(std::string filename, int gen,int Nevents, double polX, double polY,double polZ);

        //Copy Constructor:
        stPolManip(const stPolManip &right);

        //Destructor:
        virtual ~stPolManip();
        void ReadProtos();
        void RecordProtos(unsigned int i);
        void generate();
        void generate_weight();
        void compute();
        void WriteIn();
        void OutputProtos();
        unsigned int sampleSize();
        //The Formula to generate angles
        double pdf(double theta, double phi, double thetaStar, double phiStar, double q, double mt, double mW, int topID);
        //  void rootOutput();
        double getCosTheta (TLorentzVector restParticle, TLorentzVector particleOne,TLorentzVector particleTwo);
        double getPhi (TLorentzVector restParticle, TLorentzVector particle,TVector3 x_axis, TVector3 y_axis,TVector3 z_axis);
        TVector3 getBooster(TVector3 p, TVector3 pstar, double gamma);
    
        //ROOT ANALYSIS:
        TFile hfile;
        TTree *tree;
        std::ofstream ofs;

    private:
    
        std::ifstream inStream;
        int _gen; //generation: 0,1,2
        std::vector<double> _pol;
        std::vector<protosInput> sT; //FOR PROTOS
        //***for Generation***:
        std::random_device device;
        std::mt19937 e;
        double MAXTHROW;
        std::uniform_real_distribution<double> flatTheta;
        std::uniform_real_distribution<double> flatPhi;
        std::uniform_real_distribution<double> flatY;
        std::uniform_real_distribution<double> flatOne;
        unsigned int _Nevents;
        double _q, _mt, _mW;
        //int _topID;
        size_t NPROC; //FOR PARALLELIZE
        std::vector<std::thread> tVector;
        //***
        //output for rootFile
        EVENTINFO o_eventInfo;
        Float_t o_cosTheta,o_cosThetaStar,o_Phi,o_PhiStar,o_cosThetaX, o_cosThetaY, o_cosThetaZ,o_ProdAngZMF;
        PARTICLE o_top, o_W, o_bottom, o_antibottom, o_specQuark, o_lepton, o_neutrino, o_WT,o_leptonW;
        BEAM o_beam1, o_beam2;
        TLorentzVector pBeam,pBeamT,pBeamW,ZMF,gluon,top,topZMF,bTwoZMF,W,WT,bottom,bottomT,bottomW,antiBottom,specQuark,specQuarkT,specQuarkW,lepton,leptonT,leptonW,neutrino,neutrinoT,neutrinoW,buffer,bottominit,Wprod,WprodT;
        Float_t cosTheta,cosThetaStar,phi,phiStar,cosThetaX,cosThetaY,cosThetaZ,pW,pL,pB,pN,pWprod,cosThetaprod,phiprod,ProdAngZMF;
        //Coordinates:
        TVector3 T,N,q; //X,-Y,Z in top rest frame.
        TVector3 Tprime,Nprime,qprime; //X,-Y,Z in W rest frame.
        TLorentzVector XB,YB;

};

#endif
