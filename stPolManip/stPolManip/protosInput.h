// protosInput.h
//
// 1.15.2018
// Runyu Bi
//
// Class to read Protos Iutput
//

#ifndef protosInput_H
#define protosInput_H
#include "TLorentzVector.h"
#include <iostream>

class protosInput
{
    public:
        struct substrate{
            int particleId;
            int color;
            int flow;
            double pZ;
        };
        struct product{
            int particleId;
            int color;
            int flow;
            double pX;
            double pY;
            double pZ;
        };
//    struct Wboson{//from spectator quark in top.
//        double momentum;
//        double cosTheta;
//        double phi;
//    };
//
//    struct Leptons{//from -b in W.
//        double momentum;
//        double cosThetaStar;
//        double phiStar;
//    };
        protosInput();
//       protosInput(const protosInput &right);
        virtual ~protosInput();
        int eventNumber;
        double weight;
        double eventQ;
        substrate qg[2];
        product bblvq[5];
        friend std::istream& operator>> (std::istream& is, substrate &p) ;
        friend std::istream& operator>> (std::istream& is, product &p) ;
        friend std::istream& operator>> (std::istream& is, protosInput &st) ;
        double particleMass(int id);
        TLorentzVector getParticle(int mcNum);
        TLorentzVector getParticle(int mcNum1, int mcNum2);
        TLorentzVector getBeam();
        TLorentzVector getNeutrino();
        TLorentzVector getLepton();
        TLorentzVector getBottom();
        TLorentzVector getAntiBottom();
        TLorentzVector getSpecQuark();
        TLorentzVector getGluon();
        TLorentzVector getInitquark();
        TVector3 getBooster(TVector3 p, TVector3 pstar, double gamma);
};


#endif
