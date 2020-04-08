import os,sys
from optparse import OptionParser
import ROOT
import math
import time
import multiprocessing
#import AtlasStyle
# define colors
operatorList = [">","<","==",">=","<=","!="]
BLUE = '\033[94m'
GREEN = '\033[92m'
BOLD = '\033[1m'
WARNING = '\033[93m'
ERROR = '\033[91m'
ENDC = '\033[0m'
Red = '\033[91m'
Cyan = '\033[96m'
White = '\033[97m'
Yellow = '\033[93m'
Magenta = '\033[95m'
Grey = '\033[90m'
Black = '\033[90m'
Default = '\033[99m'

#=====================================================================
# Variables to Plot:
#=====================================================================
angleList = ["cosTheta","cosThetaStar","cosThetaX","cosThetaY","cosThetaZ"]
phiList = ["Phi","PhiStar"]
beamList = ["beam1","beam2"]
#particleList = ["top","W","bottom","antibottom","specQuark","lepton","neutrino","WT","leptonW"]
particleList = ["top","lepton", "neutrino"]
leafList = ["id","pt","eta","phi","mass"]
leafList2 = ["id","pz","mass"]
#=====================================================================
# Plotting Configurations:
#=====================================================================
#fileList = ["/eos/user/r/rubi/shares/Nello/12M/PolManipNG.root","/eos/user/r/rubi/shares/Nello/12M/PolManipGenSM.root","/eos/user/r/rubi/shares/Nello/12M/PolManipPzplus1.root","/eos/user/r/rubi/shares/Nello/12M/PolManipPzminus1.root"]
fileList = ["../build/Debug/st13TeVSTD_10k.root","../build/Debug/st13TeVSTD_10k2.root","../build/Debug/st13TeVSTD_20k.root"]
treeList = ["singleTop","singleTop","singleTop","singleTop"]
nameList = ["NG","GenSM","Pzplus1","Pzminus1"]
colorList = [ROOT.kBlue,ROOT.kPink,ROOT.kGreen,ROOT.kBlack]
optionList = ["","*HSAME","*HSAME","*HSAME"]
cutList = ["top.id>0","top.id>0&&lepton.pt>10","top.id==6","top.id<0"]
#=====================================================================
#
#=====================================================================
def addLegend(legend,hist1,hist2,*histMore):
    legend.AddEntry(hist1,hist1.GetName().split("_")[0])
    legend.AddEntry(hist2,hist2.GetName().split("_")[0])
    for hist in histMore:
        legend.AddEntry(hist,hist.GetName().split("_")[0])

def jobMulti(stPlot,q):
    stPlot.dumpInfo()
    q.put(stPlot)

class stPlot:
    
    def __init__(self,file,fileName,treeName,color,drawOption,cut):
        self.rootFile = ROOT.TFile.Open(file)
        self.fileName = fileName
        self.treeName = treeName
        self.tree = self.rootFile.Get(self.treeName)
        self.color = color
        self.drawOption = drawOption
        self.dataList = []
        self.histList = []
        self.cut = cut
##==========
## analyze the cut condition:
##==========
        self.condList = []
        self.cutBranchList = []
        self.cutLeafList = []
        self.cutOPList = []
        self.cutValueList = []
        if self.cut.find("&&"):
            count = self.cut.count("&&")
            for i in range(0,count + 1):
                self.condList.append(self.cut.split("&&")[i])
        else:
            self.condList.append(self.cut)
        for i in range(0,len(self.condList)):
            whichOP = None
            for index,op in enumerate(operatorList):
                if (self.condList[i].find(op)>0):
                    whichOP = index
                    break
            self.cutOPList.append(operatorList[whichOP])
            self.cutBranchList.append(self.condList[i].split(".")[0])
            rest_str = self.condList[i].split(".")[1]
            self.cutLeafList.append( rest_str.split(self.cutOPList[i])[0] )
            self.cutValueList.append( rest_str.split(self.cutOPList[i])[1] )
##==========
##
##==========
        self.createHistograms()

    def createHistograms(self):
        for angle in angleList:
            hist = ROOT.TH1F(self.fileName+"_"+angle,angle,50,-1,1)
            self.histList.append(hist)
        for angle in phiList:
            hist = ROOT.TH1F(self.fileName+"_"+angle,angle,50,0,2*math.pi)
            self.histList.append(hist)
        for beam in beamList:
            for leaf in leafList2:
                hist = ROOT.TH1F(self.fileName+"_"+beam+"_"+leaf,beam+"_"+leaf,50,0,150)
                self.histList.append(hist)
        for particle in particleList:
            for leaf in leafList:
                hist = ROOT.TH1F(self.fileName+"_"+particle+"_"+leaf,particle+"_"+leaf,50,0,150)
                self.histList.append(hist)
                
    def dumpInfo(self):
        passEvents = 0
        for index,event in enumerate(self.tree):
            varList = []
            passCut = True
            for i,cond in enumerate(self.condList):
                if not ( eval(str( self.tree.GetBranch(self.cutBranchList[i]).GetLeaf(self.cutLeafList[i]).GetValue() )+self.cutOPList[i]+self.cutValueList[i] ) ):  #string operator to real math operator
                    passCut = False
                    break
            if (passCut):
                for j,angle in enumerate(angleList):
                    var = {"Name":None, "value":None}
                    var["Name"] = angle
                    var["value"] = event.GetBranch(angle).GetLeaf(angle).GetValue()
                    self.histList[j].Fill(var["value"])
                    varList.append(var)
                for j,angle in enumerate(phiList):
                    var = {"Name":None, "value":None}
                    var["Name"] = angle
                    var["value"] = event.GetBranch(angle).GetLeaf(angle).GetValue()
                    self.histList[len(angleList)+j].Fill(var["value"])
                    varList.append(var)
                for j,beam in enumerate(beamList):
                    var = {"Name":None, "id":None, "pz":None, "mass":None }
                    var["Name"] = beam
                    for k,leaf in enumerate(leafList2):
                        var[leaf] = event.GetBranch(beam).GetLeaf(leaf).GetValue()
                        self.histList[len(angleList)+len(phiList)+j*3+k].Fill(var[leaf])
                    varList.append(var)
                for j,particle in enumerate(particleList):
                    var = {"Name":None, "id":None, "pt":None, "eta":None, "phi":None, "mass":None }
                    var["Name"] = particle
                    for k,leaf in enumerate(leafList):
                        var[leaf] = event.GetBranch(particle).GetLeaf(leaf).GetValue()
                        self.histList[len(angleList)+len(phiList)+len(beamList)*3+j*5+k].Fill(var[leaf])
                    varList.append(var)
                passEvents += 1
                #self.dataList.append(varList)
            if (index % 1000 ==0):
#                for i,cond in enumerate(self.condList):
#                    print str( self.tree.GetBranch(self.cutBranchList[i]).GetLeaf(self.cutLeafList[i]).GetValue() )+self.cutOPList[i]+self.cutValueList[i]
#                    print eval(str( self.tree.GetBranch(self.cutBranchList[i]).GetLeaf(self.cutLeafList[i]).GetValue() )+self.cutOPList[i]+self.cutValueList[i] )
                #print BLUE + "Processed " + str(index) + " events." + ENDC
                print BLUE + "Passed " + str(passEvents) + " events." + ENDC

    def beautifyHist(self,hist):
        norm = hist.GetEntries()
        hist.Scale(1/norm)
        hist.SetStats(ROOT.kFALSE)
        hist.SetMinimum(0.)
        hist.GetXaxis().SetTitle(hist.GetName())
        hist.SetLineColor(self.color)
        hist.SetMarkerColor(self.color)
        #hist.SetMarkerSize(0.8)
        #hist.SetMarkerStyle(21)
            
    def drawHist(self,hist):
        self.beautifyHist(hist)
        hist.Draw(self.drawOption)

def main():
    start_time = time.time()
    items = []
    procs = []
    results = []
    q = []
    for i in range(0,len(fileList)):
        items.append(stPlot(fileList[i],nameList[i],treeList[i],colorList[i],optionList[i],cutList[i]))
        q.append(multiprocessing.Queue())

    for i,item in enumerate(items):
        p = multiprocessing.Process(target=jobMulti, args=(item,q[i])) #Pickle into Queue
        procs.append(p)
        p.start()
    for i,queue in enumerate(q):
        results.append(queue.get())
    for proc in procs:
        proc.join()

    c1 = ROOT.TCanvas()
    for i in range(0,len(results[0].histList)):
        histName = results[0].histList[i].GetName()
        legend = ROOT.TLegend(0.8,0.5,0.95,0.6)
        for result in results:
            histogram = result.histList[i]
            result.drawHist(histogram)
            legend.AddEntry(histogram,histogram.GetName().split("_")[0])
        legend.Draw()
        #c1.SaveAs("/eos/user/r/rubi/pythonPlots/"+histName+ ".png")
        c1.SaveAs("pythonPlots/"+histName+ ".png")
    print (Magenta + "--- Plotting done! ---  " + ENDC)
    print (Magenta + "--- %s seconds taken.---" %(time.time() - start_time) + ENDC)

if __name__ == '__main__':
    main()
