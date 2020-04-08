from ROOT import TFile, TLorentzVector
from decimal import Decimal
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-f", "--file", dest="filePath", default="",
                      help="choose the input root file", metavar="FL")
(options, args) = parser.parse_args()

global filePath
filePath = options.filePath
if filePath == '':
    print 'Needs path to input root file!'
    sys.exit()

rootFile = TFile.Open(filePath)
tree = rootFile.Get("singleTop")
txtOut = open("fromROOT.events", "w")

for event in tree:
    eventNumber = int(event.GetBranch("Event").GetLeaf("eventNumber").GetValue())
    if eventNumber % 1000 == 0:
        print eventNumber
    weight = event.GetBranch("Event").GetLeaf("weight").GetValue()
    energy = event.GetBranch("Event").GetLeaf("eventQ").GetValue()
    particles = {"id":None, "color":None, "flow":None, "px":None, "py":None, "pz":None }
    particlesList = []
    beamList = ["beam1","beam2"]
    othersList = ["neutrino","lepton","bottom","antibottom","specQuark"]
    for i in range(0,2):
        particles["id"] = int(event.GetBranch(beamList[i]).GetLeaf("id").GetValue())
        particles["color"] = int(event.GetBranch(beamList[i]).GetLeaf("color").GetValue())
        particles["flow"] = int(event.GetBranch(beamList[i]).GetLeaf("flow").GetValue())
        particles["pz"] = event.GetBranch(beamList[i]).GetLeaf("pz").GetValue()
        particlesList.append(particles.copy())

    for i in range(0,5):
        pt = event.GetBranch(othersList[i]).GetLeaf("pt").GetValue()
        eta = event.GetBranch(othersList[i]).GetLeaf("eta").GetValue()
        phi = event.GetBranch(othersList[i]).GetLeaf("phi").GetValue()
        mass = event.GetBranch(othersList[i]).GetLeaf("mass").GetValue()
        target = TLorentzVector(0,0,0,0)
        target.SetPtEtaPhiM(pt,eta,phi,mass)
        particles["id"] = int(event.GetBranch(othersList[i]).GetLeaf("id").GetValue())
        particles["color"] = int(event.GetBranch(othersList[i]).GetLeaf("color").GetValue())
        particles["flow"] = int(event.GetBranch(othersList[i]).GetLeaf("flow").GetValue())
        particles["px"] = target.Px()
        particles["py"] = target.Py()
        particles["pz"] = target.Pz()
        particlesList.append(particles.copy())

    txtOut.write ('{:>10}  {:>17}  {:>15}'.format(eventNumber,'%.8E' % Decimal(weight),'%.8E' % Decimal(energy)) + "\n")
    txtOut.write ('{:>3}  {:>2}  {:>2} {:>15} '.format(particlesList[0]["id"],particlesList[0]["color"], particlesList[0]["flow"], '%.8E' % Decimal(particlesList[0]["pz"])) + "\n")
    txtOut.write ('{:>3}  {:>2}  {:>2} {:>15} '.format(particlesList[1]["id"],particlesList[1]["color"], particlesList[1]["flow"], '%.8E' % Decimal(particlesList[1]["pz"])) + "\n")
    for i in range (2,len(particlesList)):
        txtOut.write ('{:>3}  {:>2}  {:>2} {:>15} {:>15} {:>15} '.format(particlesList[i]["id"],particlesList[i]["color"], particlesList[i]["flow"], '%.8E' % Decimal(particlesList[i]["px"]), '%.8E' % Decimal(particlesList[i]["py"]), '%.8E' % Decimal(particlesList[i]["pz"])) + "\n")
print ("Procedure done.")
