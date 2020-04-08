#include "protosInput.h"

std::istream& operator>> (std::istream& is, protosInput::substrate &p){
    is >> p.particleId >> p.color >> p.flow >> p.pZ;
    return is;
}

std::istream& operator>> (std::istream& is, protosInput::product &p){
    is >> p.particleId >> p.color >> p.flow >> p.pX >> p.pY >> p.pZ;
    return is;
}

std::istream& operator>> (std::istream& is, protosInput &st){
    is >> st.eventNumber >> st.weight >> st.eventQ;
    is >> st.qg[0];
    is >> st.qg[1];
    for (int i = 0; i < 5; i++) {
        is >> st.bblvq[i];
    }
    return is;
}

//Constructor
protosInput::protosInput(){};
//Destructor
protosInput::~protosInput(){};
//Copy
//protosInput::protosInput(const protosInput &right);

double protosInput::particleMass(int id){
    switch (id) {
        case 1:
        case -1:
            return 0.0048;
        case 2:
        case -2:
            return 0.0023;
        case 3:
        case -3:
            return 0.095;
        case 4:
        case -4:
            return 1.29;
        case 5:
        case -5:
            return 4.18;
        case 6:
        case -6:
            return 172.44;
        case 11:
        case -11:
            return 0.000511;
        case 13:
        case -13:
            return 0.10566;
        case 15:
        case -15:
            return 1.7768;
    }
    return 1;
}

TLorentzVector protosInput::getParticle(int mcNum){
    TLorentzVector p;
    for (unsigned int i=0; i < sizeof(qg); i++) {
        if (mcNum == qg[i].particleId) {
            p.SetPxPyPzE(0,0,qg[i].pZ,
                         sqrt(pow(particleMass(qg[i].particleId),2)
                              + pow(qg[i].pZ,2)));
            return p;
        }
    }
    for (unsigned int i=0; i < sizeof(bblvq); i++) {
        if (mcNum == bblvq[i].particleId) {
            p.SetPxPyPzE(bblvq[i].pX,
                         bblvq[i].pY,
                         bblvq[i].pZ,
                         sqrt(pow(bblvq[i].pX,2)
                              +pow(bblvq[i].pY,2)
                              +pow(bblvq[i].pZ,2)
                              +pow(particleMass(bblvq[i].particleId),2)));
            return p;
        }
    }
    throw std::runtime_error ("The acquired particle number " + std::to_string(mcNum) + "is not found.");
}
                         
TLorentzVector protosInput::getParticle(int mcNum1, int mcNum2){
    TLorentzVector p;
    for (unsigned int i=0; i < sizeof(qg); i++) {
        if ((mcNum1 == qg[i].particleId) || (mcNum2 == qg[i].particleId)) {
            p.SetPxPyPzE(0,0,qg[i].pZ,
                         sqrt(pow(particleMass(qg[i].particleId),2)
                              + pow(qg[i].pZ,2)));
            return p;
        }
    }
    for (unsigned int i=0; i < sizeof(bblvq); i++) {
        if ((mcNum1 == bblvq[i].particleId) || (mcNum2 == bblvq[i].particleId))  {
            p.SetPxPyPzE(bblvq[i].pX,
                         bblvq[i].pY,
                         bblvq[i].pZ,
                         sqrt(pow(bblvq[i].pX,2)
                              +pow(bblvq[i].pY,2)
                              +pow(bblvq[i].pZ,2)
                              +pow(particleMass(bblvq[i].particleId),2)));
            return p;
        }
    }
    throw std::runtime_error ("The acquired particle number " + std::to_string(mcNum1) + " or " + std::to_string(mcNum2) +  "is not found.");
}

TLorentzVector protosInput::getBeam(){//We choose quark to be the beam particle.
    TLorentzVector Beam;
    if (abs(qg[0].particleId)!= 21 ) {
        Beam.SetPxPyPzE(0,0,qg[0].pZ,sqrt(pow(particleMass(qg[0].particleId),2)
                                             + pow(qg[0].pZ,2)));
    }else{
        Beam.SetPxPyPzE(0,0,qg[1].pZ,sqrt(pow(particleMass(qg[1].particleId),2)
                                             + pow(qg[1].pZ,2)));
    }

    return Beam;
}

TLorentzVector protosInput::getNeutrino(){
    TLorentzVector neutrino;
    neutrino.SetPxPyPzE(bblvq[0].pX,
                        bblvq[0].pY,
                        bblvq[0].pZ,
                        sqrt(pow(bblvq[0].pX,2)
                             + pow(bblvq[0].pY,2)
                             + pow(bblvq[0].pZ,2)));
    return neutrino;
}

TLorentzVector protosInput::getLepton(){
    TLorentzVector lepton;
    lepton.SetPxPyPzE(bblvq[1].pX,
                      bblvq[1].pY,
                      bblvq[1].pZ,
                      sqrt(pow(bblvq[1].pX,2)
                           +pow(bblvq[1].pY,2)
                           +pow(bblvq[1].pZ,2)
                           +pow(particleMass(bblvq[1].particleId),2)));
    return lepton;
}

TLorentzVector protosInput::getBottom(){
    TLorentzVector bottom;
    bottom.SetPxPyPzE(bblvq[2].pX,
                      bblvq[2].pY,
                      bblvq[2].pZ,
                      sqrt(pow(bblvq[2].pX,2)
                           +pow(bblvq[2].pY,2)
                           +pow(bblvq[2].pZ,2)
                           +pow(particleMass(bblvq[2].particleId),2)));
    return bottom;
}

TLorentzVector protosInput::getAntiBottom(){
    TLorentzVector antiBottom;
    antiBottom.SetPxPyPzE(bblvq[3].pX,
                      bblvq[3].pY,
                      bblvq[3].pZ,
                      sqrt(pow(bblvq[3].pX,2)
                           +pow(bblvq[3].pY,2)
                           +pow(bblvq[3].pZ,2)
                           +pow(particleMass(bblvq[3].particleId),2)));
    return antiBottom;
}

TLorentzVector protosInput::getSpecQuark(){
    TLorentzVector specQuark;
    specQuark.SetPxPyPzE(bblvq[4].pX,
                         bblvq[4].pY,
                         bblvq[4].pZ,
                         sqrt(pow(bblvq[4].pX,2)
                              +pow(bblvq[4].pY,2)
                              +pow(bblvq[4].pZ,2)
                              +pow(particleMass(bblvq[4].particleId),2)));
    return specQuark;
}

TLorentzVector protosInput::getGluon(){
    TLorentzVector gluon;

    for(int i=0; i<(sizeof(qg)/sizeof(qg[0])); ++i)
      if(fabs(qg[i].particleId)==21)
	gluon.SetPxPyPzE(0.,
                      0.,
                      qg[i].pZ,
                      sqrt(pow(qg[i].pZ,2)));
    return gluon;
}

TLorentzVector protosInput::getInitquark(){
    TLorentzVector initquark;
    for(int i=0; i<(sizeof(qg)/sizeof(qg[0])); ++i)
      if(fabs(qg[i].particleId)!=21)
	initquark.SetPxPyPzE(0.,
                      0.,
                      qg[i].pZ,
                      sqrt(pow(qg[i].pZ,2)
                           +pow(particleMass(qg[i].particleId),2)));
    return initquark;
}

TVector3 protosInput::getBooster(TVector3 p, TVector3 pstar, double gamma) {
  float u2  = p.Mag2();
  float us2 = pstar.Mag2();
  float uus = p.Dot(pstar);
  float square = sqrt(fabs(1-4*u2+2*uus*(-1+gamma)+gamma+pow(uus,2)*(1+gamma)));
  // p.Print();
  // pstar.Print();
  // std::cout<<u2<<std::endl;
  // std::cout<<us2<<std::endl;
  // std::cout<<uus<<std::endl;
  // std::cout<<gamma<<std::endl;
  // std::cout<<square<<std::endl;
  float booster_x = (  p .X() * ( (1+uus)*(1+gamma) + sqrt(1+gamma)*square ) +
		    pstar.X() * ( (1-uus)*(1+gamma) + sqrt(1+gamma)*square - 2*u2*(1+gamma))  ) / (2*gamma*(u2+uus));
  float booster_y = (  p .Y() * ( (1+uus)*(1+gamma) + sqrt(1+gamma)*square ) + 
		    pstar.Y() * ( (1-uus)*(1+gamma) + sqrt(1+gamma)*square - 2*u2*(1+gamma))  ) / (2*gamma*(u2+uus));
  float booster_z = (  p .Z() * ( (1+uus)*(1+gamma) + sqrt(1+gamma)*square ) + 
  		    pstar.Z() * ( (1-uus)*(1+gamma) + sqrt(1+gamma)*square - 2*u2*(1+gamma))  ) / (2*gamma*(u2+uus));

  //int signz = (booster_z!=0)? booster_z/abs(booster_z) : 1.; // get the sign if the value is !=0
  //booster_z = signz*sqrt((1-1/pow(gamma,2)) - pow(booster_x,2) - pow(booster_y,2));
  
  TVector3 booster(booster_x,booster_y,booster_z);

  return booster;
}
