//
//  stPolManip.cpp
//
//
//  Created by Runyu on 1/22/18.
//
#include "stPolManip.h"

//Constructor: Use multiple to replace int gen=0 case.
//
stPolManip::stPolManip(std::string filename, int gen, int Nevents, double polX, double polY,double polZ):hfile("stPolarization.root","RECREATE","Generated Decay products of Top."),inStream(filename),    ofs("protos.events",std::ofstream::out),_gen(gen),e(device()),MAXTHROW(0.02),flatTheta(0.0,M_PI),flatPhi(0,2*M_PI),flatY(0.0,MAXTHROW),flatOne(0.0,1.0),_Nevents(Nevents),cosTheta(0.0),cosThetaStar(0.0),phi(0.0),phiStar(0.0){
    tree = new TTree("singleTop","Angles of such polarization.");
    _pol.push_back(polX);
    _pol.push_back(polY);
    _pol.push_back(polZ);
    std::string branchName[] = {"cosTheta","cosThetaStar","Phi","PhiStar","cosThetaX","cosThetaY","cosThetaZ","Event","beam1","beam2","top","W","bottom","antibottom","specQuark","lepton","neutrino","WT","leptonW","ProdAngZMF"};
    std::string branchParms[] = {"eventNumber/i:weight/F:eventQ/F:eventS/F","id/I:color/I:flow/I:pz/F:mass/F", "id/I:color/I:flow/I:pt/F:eta/F:phi/F:mass/F"};
    tree->Branch(branchName[0].c_str(),&o_cosTheta,"cosTheta/F");
    tree->Branch(branchName[1].c_str(),&o_cosThetaStar,"cosThetaStar/F");
    tree->Branch(branchName[2].c_str(),&o_Phi,"Phi/F");
    tree->Branch(branchName[3].c_str(),&o_PhiStar,"PhiStar/F");
    tree->Branch(branchName[4].c_str(),&o_cosThetaX,"cosThetaX/F");
    tree->Branch(branchName[5].c_str(),&o_cosThetaY,"cosThetaY/F");
    tree->Branch(branchName[6].c_str(),&o_cosThetaZ,"cosThetaZ/F");
    tree->Branch(branchName[7].c_str(),&o_eventInfo,branchParms[0].c_str());
    tree->Branch(branchName[8].c_str(),&o_beam1,branchParms[1].c_str());
    tree->Branch(branchName[9].c_str(),&o_beam2,branchParms[1].c_str());
    tree->Branch(branchName[10].c_str(),&o_top,branchParms[2].c_str());
    tree->Branch(branchName[11].c_str(),&o_W,branchParms[2].c_str());
    tree->Branch(branchName[12].c_str(),&o_bottom,branchParms[2].c_str());
    tree->Branch(branchName[13].c_str(),&o_antibottom,branchParms[2].c_str());
    tree->Branch(branchName[14].c_str(),&o_specQuark,branchParms[2].c_str());
    tree->Branch(branchName[15].c_str(),&o_lepton,branchParms[2].c_str());
    tree->Branch(branchName[16].c_str(),&o_neutrino,branchParms[2].c_str());
    tree->Branch(branchName[17].c_str(),&o_WT,branchParms[2].c_str());
    tree->Branch(branchName[18].c_str(),&o_leptonW,branchParms[2].c_str());
    tree->Branch(branchName[19].c_str(),&o_ProdAngZMF,"ProdAngZMF/F");
};

//Destructor:
stPolManip::~stPolManip(){};

//READ from PROTOS_INPUT
void stPolManip::ReadProtos(){
    if (inStream.is_open()) {
        std::cout << "Reading the event file as fast as I can..." << std::endl;
        //Getting the file length.
        inStream.seekg (0,inStream.end);
        unsigned long long int fileLength = inStream.tellg();
        inStream.seekg (0,inStream.beg);
        while (!inStream.eof()) {
            int long long currPosition = inStream.tellg();
            protosInput a;
            inStream >> a;
            sT.push_back(a);
            if (currPosition % 10000 == 0 ) {
                int maxLoading = 30;
                float progress = currPosition/ float(fileLength);
                int cur = std::ceil(progress  * maxLoading);
                std::cout << std::fixed << std::setprecision(2)
                          << "\r Reading:   [" << std::string(cur, '*')
                          << std::string(maxLoading + 1 - cur, ' ') << "] "
                          << progress * 100 << "%" << std::flush;
            }
        }
        std::cout << std::endl;
    }else{
        std::cout
        << "The event file is not found!\n"
        << "Quitting..."
        << std::endl;
    }
};

void stPolManip::RecordProtos(unsigned int i){ //USED IN A LOOP < sizeof(sT)
    o_eventInfo.eventNumber = sT[i].eventNumber;
    o_eventInfo.weight = sT[i].weight;
    o_eventInfo.eventQ = sT[i].eventQ;
    
    pBeam = sT[i].getBeam(); //Never modified.
    gluon = sT[i].getGluon();
    o_beam1.id = sT[i].qg[0].particleId;
    o_beam1.color = sT[i].qg[0].color;
    o_beam1.flow = sT[i].qg[0].flow;
    o_beam1.pz = sT[i].qg[0].pZ;
    o_beam1.mass = sT[i].particleMass(sT[i].qg[0].particleId);
    o_beam2.id = sT[i].qg[1].particleId;
    o_beam2.color = sT[i].qg[1].color;
    o_beam2.flow = sT[i].qg[1].flow;
    o_beam2.pz = sT[i].qg[1].pZ;
    o_beam2.mass = sT[i].particleMass(sT[i].qg[1].particleId);
    
    if (sT[i].bblvq[0].particleId > 0) { //Write in top/antitop
        o_top.id = 6;
    }else{
        o_top.id = -6;
    }
    if (sT[i].bblvq[0].particleId > 0) {
        o_W.id = 24;
        o_WT.id = 24;
    }else{
        o_W.id = -24;
        o_WT.id = -24;
    }
    
    neutrino = sT[i].getNeutrino(); //Modified.
    o_neutrino.id = sT[i].bblvq[0].particleId;
    o_neutrino.color = sT[i].bblvq[0].color;
    o_neutrino.flow = sT[i].bblvq[0].flow;
    
    lepton = sT[i].getLepton(); //Modified.
    o_lepton.id = sT[i].bblvq[1].particleId;
    o_lepton.color = sT[i].bblvq[1].color;
    o_lepton.flow = sT[i].bblvq[1].flow;
    o_leptonW.id = sT[i].bblvq[1].particleId;
    o_leptonW.color = sT[i].bblvq[1].color;
    o_leptonW.flow = sT[i].bblvq[1].flow;
    
    bottom = sT[i].getBottom(); //Modified.
    o_bottom.id = sT[i].bblvq[2].particleId;
    o_bottom.color = sT[i].bblvq[2].color;
    o_bottom.flow = sT[i].bblvq[2].flow;
    
    antiBottom = sT[i].getAntiBottom();//Never Modified
    o_antibottom.id = sT[i].bblvq[3].particleId;
    o_antibottom.color = sT[i].bblvq[3].color;
    o_antibottom.flow = sT[i].bblvq[3].flow;
    o_antibottom.pt = antiBottom.Pt();
    o_antibottom.eta = antiBottom.Eta();
    o_antibottom.phi = antiBottom.Phi();
    o_antibottom.mass = antiBottom.M();
    
    specQuark = sT[i].getSpecQuark(); //Never modified.
    o_specQuark.id = sT[i].bblvq[4].particleId;
    o_specQuark.color = sT[i].bblvq[4].color;
    o_specQuark.flow = sT[i].bblvq[4].flow;
    o_specQuark.pt = specQuark.Pt();
    o_specQuark.eta = specQuark.Eta();
    o_specQuark.phi = specQuark.Phi();
    o_specQuark.mass = specQuark.M();
    
    // FORCING POLARIZAITON IN PRODUCTION
    Wprod = sT[i].getInitquark() - specQuark; // q -> q_s + Wprod
};

//PDF FUNCTION:
double stPolManip::pdf(double theta, double phi, double thetaStar, double phiStar, double q, double mt, double mW, int topID){
    double px(_pol[0]);
    double py(_pol[1]);
    double pz(_pol[2]);
    if(_gen<0) { // SM generation at 13 TeV
      if(topID>0) {
	px = 0.;
	py = 0.;
	pz = 0.88;
      }
      else {
	px = -0.11;
	py = 0.;
	pz = -0.85;
      }
    }
    TVector3 P(px,py,pz);
    TVector3 uL(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
    TVector3 uT(cos(theta)*cos(phi),cos(theta)*sin(phi),-sin(theta));
    TVector3 uN(sin(phi),-cos(phi),0);
    double PuL = P.Dot(uL);
    double PuT = P.Dot(uT);
    double PuN = P.Dot(uN);
    double lambda(1);
    //parameters definition:
    double a12   = (mt*mt-mW*mW-2*mt*q)/(mt*mt);
    double a0m2  = (mt*mt/2/mW/mW)*(mt*mt-mW*mW+2*mt*q)/(mt*mt);
    double a02   = (mt*mt/2/mW/mW)*(mt*mt-mW*mW-2*mt*q)/(mt*mt);
    double am1m2 = (mt*mt-mW*mW+2*mt*q)/(mt*mt);
    double a4 = (mt/mW/sqrt(2))*a12*cos(phiStar);
    double a5 = (mt/mW/sqrt(2))*am1m2*cos(phiStar);
    double a6 = (mt/mW/sqrt(2))*a12*(-sin(phiStar));
    double a7 = (mt/mW/sqrt(2))*am1m2*(-sin(phiStar));

    // // higher order terms in mb
    // double mb = 4.2;
    // a12  += (mb*mb)/(mt*mt);
    // a0m2 += (mb*mb/2/mW/mW)*(mb*mb-2*mt*mt-mW*mW-2*mt*q)/(mt*mt);
    // a02  += (mb*mb/2/mW/mW)*(mb*mb-2*mt*mt-mW*mW+2*mt*q)/(mt*mt);
    // am1m2+= (mb*mb)/(mt*mt);
    
    if(topID<0) {//+1 for tops and -1 for anti-tops.
        lambda = -1;
        std::swap(a12,am1m2);
        std::swap(a0m2,a02);
        std::swap(a4,a5);
        std::swap(a6,a7);
    }
    
    double constant = 3./pow(8.*TMath::Pi(),2);
    double fancyN = a12+a02+a0m2+am1m2;
    double output = // abs( // nasty trick!
                    constant/fancyN * sin(theta) * sin(thetaStar) *
                    ( (a12*pow(1+lambda*cos(thetaStar),2)+2*a0m2*sin(thetaStar)*sin(thetaStar)) * (1+PuL)
                     +(2*a02*sin(thetaStar)*sin(thetaStar)+am1m2*pow(1-lambda*cos(thetaStar),2))*(1-PuL)
                     +lambda*2*sqrt(2)*(a4*(1+lambda*cos(thetaStar))+a5*(1-lambda*cos(thetaStar)))*sin(thetaStar)*PuT
                     +lambda*2*sqrt(2)*(a6*(1+lambda*cos(thetaStar))+a7*(1-lambda*cos(thetaStar)))*sin(thetaStar)*PuN ) ; // );
    return output;
};
//Generate:
void stPolManip::generate(){
    unsigned int pickNumber(1);
    std::vector<Float_t> _cosTheta;
    std::vector<Float_t> _phi;
    std::vector<Float_t> _cosThetaStar;
    std::vector<Float_t> _phiStar;
    for(unsigned int i=0;i<_Nevents;i++){
        Float_t thetaRan = flatTheta(e);
        Float_t phiRan = flatPhi(e);
        Float_t thetaSRan = flatTheta(e);
        Float_t phiSRan = flatPhi(e);
        Float_t y = flatY(e);
    //THROWAWAY METHOD
        if  (y<pdf(thetaRan, phiRan, thetaSRan, phiSRan,_q,_mt,_mW,o_top.id)){
            _cosTheta.push_back(cos(thetaRan));
            _phi.push_back(phiRan);
            _cosThetaStar.push_back(cos(thetaSRan));
            _phiStar.push_back(phiSRan);
        }else if (pdf(thetaRan, phiRan, thetaSRan, phiSRan,_q,_mt,_mW,o_top.id)>MAXTHROW) {
            throw std::runtime_error ("Function value exceeds max random number -> "+std::to_string(pdf(thetaRan, phiRan, thetaSRan, phiSRan,_q,_mt,_mW,o_top.id)));
        }else if (pdf(thetaRan, phiRan, thetaSRan, phiSRan,_q,_mt,_mW,o_top.id)< -0.000001) {
            throw std::runtime_error ("The PDF function became less than 0! -> "+std::to_string(pdf(thetaRan, phiRan, thetaSRan, phiSRan,_q,_mt,_mW,o_top.id)));
        }
    }
//Select:
    pickNumber = _cosTheta.size() * flatOne(e);
    if(pickNumber==0){
        pickNumber = 1;
    };
    cosTheta = _cosTheta[pickNumber-1];
    phi = _phi[pickNumber-1];
    cosThetaStar = _cosThetaStar[pickNumber-1];
    phiStar = _phiStar[pickNumber-1];
};

void stPolManip::generate_weight(){
    double theta = flatTheta(e);
    cosTheta = cos(theta);
    phi = flatPhi(e);
    double thetaS = flatTheta(e);
    cosThetaStar = cos(thetaS);
    phiStar = flatPhi(e);
    o_eventInfo.weight = pdf(theta, phi, thetaS, phiStar,_q,_mt,_mW,o_top.id);
};

void stPolManip::compute(){
    top = neutrino + bottom + lepton;    
    _mt = top.M();
    ZMF = top + specQuark;
    o_eventInfo.eventS = ZMF.M();
    //ZMF FRAME:
    buffer = top;
    buffer.Boost(-ZMF.BoostVector());
    topZMF = buffer;
    buffer = gluon - antiBottom;
    buffer.Boost(-ZMF.BoostVector());
    bTwoZMF = buffer;
    ProdAngZMF = cos(topZMF.Angle(bTwoZMF.Vect()));
    
    // FORCING POLARIZAITON IN PRODUCTION
    buffer = Wprod;
    buffer.Boost(-top.BoostVector());
    WprodT = buffer;
    pWprod = WprodT.P();

    // GETTING SPECQUARK AND BEAM INFO IN THE REST FRAMES
    buffer = specQuark;
    buffer.Boost(-top.BoostVector());
    specQuarkT = buffer;
    buffer = pBeam;
    buffer.Boost(-top.BoostVector());
    pBeamT = buffer;

    //Construct q,N,T coord for PRODUCTION:
    q = specQuarkT.Vect(); //Z-axis defined.
    q.SetMag(1);
    N = pBeamT.Vect().Cross(q);  //-Y-axis defined. N = st X q.
    N.SetMag(1);
    T = q.Cross(N); //X-axis defined. T = q X N.
    T.SetMag(1);
    
    // Generate 4 angles for PRODUCTION:
    if(abs(_gen)==2) {
        _q = pWprod;
        //Generator Geninit(10000,pWprod,top.M(),80.385,_top.id); // using a 2-D pdf would speed up a lot!!!  consider W real when we need it in the generation!! quite hursh assumption!
        stPolManip::generate_weight();
        cosThetaprod = cosTheta;
        phiprod = phi;

        TVector3 tempprod = sin(acos(cosThetaprod))*cos(phiprod)*(T) + sin(acos(cosThetaprod))*sin(phiprod)*(-N) + cosThetaprod*(q);
        WprodT.SetVectM(pWprod*tempprod, Wprod.M());

        if(Wprod.M2()<=0.) {
          Wprod .SetVectM(Wprod .Vect(),80.385);
          WprodT.SetVectM(WprodT.Vect(),80.385);
        }
        TVector3 vtop = getBooster(Wprod.BoostVector(), WprodT.BoostVector(), top.Gamma()); // beta1, beta2, gamma
        //vtop.Print();
        //top.Print();
        top.SetTheta(vtop.Theta()); top.SetPhi(vtop.Phi());
        //top.Print();
    }else{
        cosThetaprod = cos(WprodT.Angle(q));
        phiprod      = getPhi(top,Wprod,T,-N,q);
    }
    //Boost W, bottom spectator quark and beam into the top rest frame;
    W = neutrino + lepton; // W = N + lepton ; only wants its abs momentum.
    _mW = W.M(); //Mass of W boson.
    buffer = W;
    buffer.Boost(-top.BoostVector());
    WT = buffer;
    pW = WT.P();
    buffer = bottom;
    buffer.Boost(-top.BoostVector());
    pB = buffer.P();
    bottomT = buffer;
        
    buffer = lepton;
    buffer.Boost(-top.BoostVector());
    leptonT = buffer;
    buffer = neutrino;
    buffer.Boost(-top.BoostVector());
    neutrinoT = buffer;
        
    buffer = specQuark;
    buffer.Boost(-top.BoostVector());
    specQuarkT = buffer;
    buffer = pBeam;
    buffer.Boost(-top.BoostVector());
    pBeamT = buffer;

     //Construct q,N,T coord:
    q = specQuarkT.Vect(); //Z-axis defined.
    q.SetMag(1);
    N = pBeamT.Vect().Cross(q);  //-Y-axis defined. N = st X q.
    N.SetMag(1);
    T = q.Cross(N); //X-axis defined. T = q X N.
    T.SetMag(1);        
    XB.SetVectM(T, 0);
    YB.SetVectM(-N, 0);

    //Boost spectator, bottom, lepton and neutrino into W; record their momentums:
    buffer = specQuarkT;
    buffer.Boost(-WT.BoostVector());
    specQuarkW = buffer;
    
    buffer = pBeamT;
    buffer.Boost(-WT.BoostVector());
    pBeamW = buffer;

    buffer = bottomT;
    buffer.Boost(-WT.BoostVector());
    bottomW = buffer;
        
    buffer = leptonT;
    buffer.Boost(-WT.BoostVector());
    leptonW = buffer;
    pL = leptonW.P();
        
    buffer = neutrinoT;
    buffer.Boost(-WT.BoostVector());
    pN = buffer.P();

    // Generate 4 angles:
    // Use can choose if generation is turned ON or OFF.
    if(abs(_gen)>0) { //YES Generation
        _q = pW;
        stPolManip::generate();
    }
    else {
        cosTheta = cos(WT.Angle(q));
        phi      = getPhi(top,W,T,-N,q);
    }
    //Apply the formula to recreate WT:
    TVector3 temp = sin(acos(cosTheta))*cos(phi)*(T) + sin(acos(cosTheta))*sin(phi)*(-N) + cosTheta*(q);
    //pW = sqrt(pow(top.M2()+WT.M2(),2) - 4*top.M2()*WT.M2()) / (2*top.M()); // value according to the energy conservation, rely on Generator for the moment
    WT.SetVectM(pW*temp, W.M());
    bottomT.SetVectM(-pB*temp, bottom.M());
 
    //Boost again spectator, bottom, lepton and neutrino into new W:
    buffer = specQuarkT;
    buffer.Boost(-WT.BoostVector());
    specQuarkW = buffer;
    
    buffer = bottomT;
    buffer.Boost(-WT.BoostVector());
    bottomW = buffer;
    
    //construct q',N',T' coord:
    qprime = -bottomW.Vect(); //Z-axis defined.
    qprime.SetMag(1);
    //XB.Boost(-WT.BoostVector());
    //YB.Boost(-WT.BoostVector());
    Nprime = -(XB.Vect().Cross(YB.Vect())).Cross(qprime);
            //(problematic definition -> now it works!) Nprime = -specQuarkW.Vect().Cross(qprime);
    Nprime.SetMag(1);
    Tprime = qprime.Cross(Nprime); //X-axis defined. T = q X N.
    Tprime.SetMag(1);

    if(abs(_gen)==0) { //NO Generation
        cosThetaStar = cos(leptonW.Angle(qprime));
        phiStar = getPhi(WT,leptonT,Tprime,-Nprime,qprime);
    }
    //Apply the formula to recreate lepton and neutrino:
    temp = sin(acos(cosThetaStar))*cos(phiStar)*(Tprime) + sin(acos(cosThetaStar))*sin(phiStar)*(-Nprime) + cosThetaStar*(qprime);
    //pL = sqrt(pow(WT.M2()+leptonW.M2(),2) - 4*WT.M2()*leptonT.M2()) / 2*WT.M()); // value according to the energy conservation, rely on Generator for the moment
    leptonW.SetVectM(pL*temp, lepton.M());
    neutrinoW.SetVectM(-pN*temp, neutrino.M());
    
    //Boost everything back into lab frame. Record everything:
    buffer = WT;
    buffer.Boost(top.BoostVector());
    W = buffer; //Modified W acquired.

    buffer = bottomT;
    buffer.Boost(top.BoostVector());
    bottom = buffer; //Modified B acquired.

    buffer = leptonW;
    buffer.Boost(WT.BoostVector());
    leptonT = buffer;
    buffer.Boost(top.BoostVector());
    lepton = buffer; //Modified lepton acquired.
        
    buffer = neutrinoW;
    buffer.Boost(WT.BoostVector());
    neutrinoT = buffer;
    buffer.Boost(top.BoostVector());
    neutrino = buffer;//Modified neutrino acquired.

    //Get the thetaXYZ angles:
    cosThetaX = cos(leptonT.Angle(T));
    cosThetaY = cos(leptonT.Angle(-N));
    cosThetaZ = cos(leptonT.Angle(q));
};
 
void stPolManip::WriteIn(){ //Modified particles and computed information (observables) are recorded here.
    o_top.pt = top.Pt();
    o_top.eta = top.Eta();
    o_top.phi = top.Phi();
    o_top.mass = top.M();

    o_W.pt = W.Pt();
    o_W.eta = W.Eta();
    o_W.phi = W.Phi();
    o_W.mass = W.M();

    o_bottom.pt = bottom.Pt();
    o_bottom.eta = bottom.Eta();
    o_bottom.phi = bottom.Phi();
    o_bottom.mass = bottom.M();

    o_lepton.pt = lepton.Pt();
    o_lepton.eta = lepton.Eta();
    o_lepton.phi = lepton.Phi();
    o_lepton.mass = lepton.M();

    o_neutrino.pt = neutrino.Pt();
    o_neutrino.eta= neutrino.Eta();
    o_neutrino.phi = neutrino.Phi();
    o_neutrino.mass = neutrino.M();

    o_WT.pt = WT.Pt();
    o_WT.eta = WT.Eta();
    o_WT.phi = WT.Phi();
    o_WT.mass = WT.M();
    
    o_leptonW.pt = leptonW.Pt();
    o_leptonW.eta = leptonW.Eta();
    o_leptonW.phi = leptonW.Phi();
    o_leptonW.mass = leptonW.M();

    o_cosTheta = cosTheta;
    o_Phi = phi;
    o_cosThetaStar = cosThetaStar;
    o_PhiStar = phiStar;
    o_ProdAngZMF = ProdAngZMF;

    o_cosThetaX = cosThetaX;
    o_cosThetaY = cosThetaY;
    o_cosThetaZ = cosThetaZ;
    
    tree->Fill();
};

void stPolManip::OutputProtos(){
    ofs << "\t" << o_eventInfo.eventNumber << " " << std::setprecision(8) << std::scientific << o_eventInfo.weight << "  " <<  o_eventInfo.eventQ << std::endl;
    ofs << std::setw(3) << std::right << o_beam1.id << "  " << std::right << o_beam1.color << "  " << std::right << o_beam1.flow << "  "  << std::right << std::setprecision(8) << std::scientific << o_beam1.pz << std::endl;
    ofs << std::setw(3) << o_beam2.id << "  " << o_beam2.color << "  " << o_beam2.flow << "  " << std::setprecision(8) << std::scientific << o_beam2.pz << std::endl;
    ofs << std::setw(3) << o_neutrino.id << "  " << o_neutrino.color << "  " << o_neutrino.flow << "  " << std::right << std::setprecision(8) << neutrino.Px() << "  " << std::right << std::right << std::setprecision(8) << neutrino.Py() << "  " << std::right <<  std::right << std::setprecision(8) << neutrino.Pz() << std::endl;
    ofs << std::setw(3) << o_lepton.id << "  " << o_lepton.color << "  " << o_lepton.flow << "  " << lepton.Px() << "  " << lepton.Py() << "  " << lepton.Pz() << std::endl;
    ofs << std::setw(3) << o_bottom.id << "  " << o_bottom.color << "  " << o_bottom.flow << "  " << bottom.Px() << "  " << bottom.Py() << "  " << bottom.Pz() << std::endl;
    ofs << std::setw(3) << o_antibottom.id << "  " << o_antibottom.color << "  " << o_antibottom.flow << "  " << antiBottom.Px() << "  " << antiBottom.Py() << "  " << antiBottom.Pz() << std::endl;
    ofs << std::setw(3) << o_specQuark.id << "  " << o_specQuark.color << "  " << o_specQuark.flow << "  " << specQuark.Px() << "  " << specQuark.Py() << "  " << specQuark.Pz() << std::endl;
}

unsigned int stPolManip::sampleSize(){
    return sT.size();
}

double stPolManip::getPhi(TLorentzVector restParticle, TLorentzVector particle,TVector3 x_axis, TVector3 y_axis,TVector3 z_axis){//Due to the relatively complex setup, we will need x-axis in the right frame to be provided for the routine; in other words, it does not boost the last argument input. Double check if the x-axis is boosted in the frame that's described in the first input.
    TLorentzVector restFrame = restParticle;
    TLorentzVector p = particle;
    TVector3 x = x_axis;
    TVector3 y = y_axis;
    TVector3 z = z_axis;
    TVector3 pVect;

    p.Boost(-restFrame.BoostVector());
    pVect = p.Vect();
    pVect = pVect - pVect.Dot(z)*z;
    double pVect_X = pVect.Dot(x);
    double pVect_Y = pVect.Dot(y);
    double Angle = atan2(pVect_Y,pVect_X);
    
    if (Angle < 0) {
        Angle = Angle + 2*M_PI;
    }
    return Angle;
};

TVector3 stPolManip::getBooster(TVector3 p, TVector3 pstar, double gamma) {
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
 

