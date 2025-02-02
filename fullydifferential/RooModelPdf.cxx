/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooModelPdf.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooModelPdf) 

 RooModelPdf::RooModelPdf(const char *name, const char *title, 
                        RooAbsReal& _theta,
                        RooAbsReal& _phi,
                        RooAbsReal& _thetastar,
                        RooAbsReal& _phistar,
                        RooAbsReal& _mt,
                        RooAbsReal& _mW,
                        RooAbsReal& _q,
                        RooAbsReal& _px,
                        RooAbsReal& _py,
                        RooAbsReal& _pz,
                        RooAbsReal& _lambda) :
   RooAbsPdf(name,title), 
   theta("theta","theta",this,_theta),
   phi("phi","phi",this,_phi),
   thetastar("thetastar","thetastar",this,_thetastar),
   phistar("phistar","phistar",this,_phistar),
   mt("mt","mt",this,_mt),
   mW("mW","mW",this,_mW),
   q("q","q",this,_q),
   px("px","px",this,_px),
   py("py","py",this,_py),
   pz("pz","pz",this,_pz),
   lambda("lambda","lambda",this,_lambda)
 { 
 } 


 RooModelPdf::RooModelPdf(const RooModelPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   theta("theta",this,other.theta),
   phi("phi",this,other.phi),
   thetastar("thetastar",this,other.thetastar),
   phistar("phistar",this,other.phistar),
   mt("mt",this,other.mt),
   mW("mW",this,other.mW),
   q("q",this,other.q),
   px("px",this,other.px),
   py("py",this,other.py),
   pz("pz",this,other.pz),
   lambda("lambda",this,other.lambda)
 { 
 } 



 Double_t RooModelPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return (1/(2*mt*mW)) * ( -mt*mW*(2*pow(1-(pow(mW,2)-2*mt*q)/pow(mt,2),2)*pow(-1+lambda*cos(thetastar),2)+pow((pow(mt,2)-pow(mW,2)-2*mt*q)/pow(mW,2),2)*pow(sin(thetastar),2))*(-1+pz*cos(theta)+px*cos(phi)*sin(theta)+py*sin(theta)*sin(phi)) +                                                                                                       mt*mW*(2*pow(1-(pow(mW,2)+2*mt*q)/pow(mt,2),2)*pow( 1+lambda*cos(thetastar),2)+pow((pow(mt,2)-pow(mW,2)+2*mt*q)/pow(mW,2),2)*pow(sin(thetastar),2))*( 1+pz*cos(theta)+px*cos(phi)*sin(theta)+py*sin(theta)*sin(phi)) +                                                                                                       8*(pow(mt,2)-pow(mW,2)-2*mt*q)*lambda*cos(phistar)*sin(thetastar)*(-pz*sin(theta)+cos(theta)*(px*cos(phi)+py*sin(phi))) +                                                                                                                                                                                                    8*(pow(mt,2)-pow(mW,2)-2*mt*q)*lambda*sin(thetastar)*(py*cos(phi)-px*sin(phi))*sin(phistar) ) ; 
 } 



