import os, sys
from optparse import OptionParser
#patterns:
#group.phys-gener.protos22b.410908.st_tchan_lept_tbj_SM_CTEQ6L1_13TeV.TXT.mc15_v1                                                                                         
#group.phys-gener.protos22b.410909.st_tchan_lept_tbj_Pxminus1_CTEQ6L1_13TeV.TXT.mc15_v1                                                                                   
#group.phys-gener.protos22b.410910.st_tchan_lept_tbj_Pxplus1_CTEQ6L1_13TeV.TXT.mc15_v1                                                                                   
#group.phys-gener.protos22b.410911.st_tchan_lept_tbj_Pyminus1_CTEQ6L1_13TeV.TXT.mc15_v1                                                                                   
#group.phys-gener.protos22b.410912.st_tchan_lept_tbj_Pyplus1_CTEQ6L1_13TeV.TXT.mc15_v1                                                                                    
#group.phys-gener.protos22b.410913.st_tchan_lept_tbj_Pzminus1_CTEQ6L1_13TeV.TXT.mc15_v1                                                                                   
#group.phys-gener.protos22b.410914.st_tchan_lept_tbj_Pzplus1_CTEQ6L1_13TeV.TXT.mc15_v1 
# define colors
BLUE = '\033[94m'
GREEN = '\033[92m'
BOLD = "\033[1m"
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

# =================================================================================
#  put
# =================================================================================
def put(text):
    # print '%s' % text
    myfile.write('%s\n' % text)

# =================================================================================
#  br
# =================================================================================
def br():
    # print
    myfile.write('\n')

# =================================================================================
#  prepareDATfile
# =================================================================================
def prepareDATfile(filename, rndseed, events):
    global myfile
    myfile = open(filename, "w")
    put(' 4                               ! Process code')
    put(' 13000.                          ! CM energy')
    put('172.50  1.45  4.95   125.0       ! mt, Gt, mb, MH')
    put('0    0.0                         ! Matching')
    put(' 1.000   0.000   0.000   0.000   ! Re VL, Re VR, Re gL, Re gR')
    put(' 0.000   0.000   0.000   0.000   ! Im VL, Im VR, Im gL, Im gR')
    put('    %d	 	          	 ! Initial random seed' % rndseed)
    put('    %d	 	 	 ! unweighted events' % events)
    put('4.4918   0.008445475	 	 ! t cross section and error')
    put('2.3507175   0.006097575	 	 ! tbar cross section and error')
    myfile.close()
#=====================================================================
# main()
#=====================================================================
def main():
    usage = "usage: %prog [options]"
    parser = OptionParser(usage=usage, version="%prog 1.0")
    parser.add_option("-p", "--pattern", dest="pattern", default="mc15_13TeV.410908.ProtosPythiaP2011C_ProtosLHEF_singletop_tchan_lept_SM.TXT.v1",
                      help="set dataset pattern name [default: %default]", metavar="DS name", )
    parser.add_option("-i", "--inputfilename", dest="inputfilepath", default="../test/test10K24R01.dat",
                      help="set path of the binary input file [default: %default]", metavar="PATH")
    parser.add_option( "--index", dest="index", default=1,
                      help="keep the index of the current file",type = "int")
    parser.add_option("--step", dest="step", default=5000,
                      help="set the number of events per file [default: %default]", metavar="NUM", type="int")
    parser.add_option("-r", "--reprocess", action="store_true", dest="reprocess", default=False,
                      help="set if the files should be reprocessed [default: %default]")
    parser.add_option("-d", "--debug", action="store_true", dest="debug", default=False,
                      help="set debug mode [default: %default]")
    parser.add_option("--polManip", action="store_true", dest="polManip", default=False,
                      help="polManip algorithm on [default: %default]")
    parser.add_option("--mode", action="store", dest="polMode", default="sm",
                      help="polarization mode wanted for polManip. [default: %default]", type="string")
    (options, args) = parser.parse_args()

    workingDir = os.environ['PWD']
    myindex = options.index
    filenameEVENTS = '%s._%05d.events' % (options.pattern, myindex)
    filenameDAT = '%s._%05d.dat' % (options.pattern, myindex)
    filenameROOT = 'nTuple_%s.%05d.root' % (options.polMode,myindex)
    
    os.system('cp /afs/cern.ch/work/r/rubi/work/polManip.tar.gz .' )
    os.system('tar -xf polManip.tar.gz' )
    # os.system('rm -fr %s' % options.pattern)

    cmdPolZ = '%s/stPolarization/stPolManip/build/stPolManip -f %s -px 0 -py 0 -pz 1.0 -g 1 -n 10000' % (workingDir,options.inputfilepath)
    cmdPolZ0 = '%s/stPolarization/stPolManip/build/stPolManip -f %s -px 0 -py 0 -pz -1.0 -g 1 -n 10000' %  (workingDir,options.inputfilepath)
    cmdPolX = '%s/stPolarization/stPolManip/build/stPolManip -f %s -px 1.0 -py 0 -pz 0 -g 1 -n 10000' %  (workingDir,options.inputfilepath)
    cmdPolX0 = '%s/stPolarization/stPolManip/build/stPolManip -f %s -px -1.0 -py 0 -pz 0 -g 1 -n 10000' %  (workingDir,options.inputfilepath)
    cmdPolY = '%s/stPolarization/stPolManip/build/stPolManip -f %s -px 0 -py 1 -pz 0 -g 1 -n 10000' %  (workingDir,options.inputfilepath)
    cmdPolY0 = '%s/stPolarization/stPolManip/build/stPolManip -f %s -px 0 -py -1.0 -pz 0 -g 1 -n 10000' %  (workingDir,options.inputfilepath)
    cmdPolSM = '%s/stPolarization/stPolManip/build/stPolManip -f %s -g -1 -n 10000' % (workingDir,options.inputfilepath)    
    cmdPolNG = '/afs/cern.ch/work/r/rubi/work/stPolarization/stPolManip/build/stPolManip -f %s -g 0' % (options.inputfilepath)    
    if options.polManip:
        if options.polMode == "+z":
            print Magenta + cmdPolZ + ENDC
            os.system(cmdPolZ)
            nameDir = "group.phys-gener.protos22b.410914.st_tchan_lept_tbj_Pzplus1_CTEQ6L1_13TeV.TXT.mc15_v1"
        if options.polMode == "-z":
            print Magenta + cmdPolZ0 + ENDC
            os.system(cmdPolZ0)
            nameDir = "group.phys-gener.protos22b.410913.st_tchan_lept_tbj_Pzminus1_CTEQ6L1_13TeV.TXT.mc15_v1"
        if options.polMode == "+x":
            print Magenta + cmdPolX + ENDC
            os.system(cmdPolX)
            nameDir = "group.phys-gener.protos22b.410910.st_tchan_lept_tbj_Pxplus1_CTEQ6L1_13TeV.TXT.mc15_v1"
        if options.polMode == "-x":
            print Magenta + cmdPolX0 + ENDC
            os.system(cmdPolX0)
            nameDir = "group.phys-gener.protos22b.410909.st_tchan_lept_tbj_Pxminus1_CTEQ6L1_13TeV.TXT.mc15_v1"
        if options.polMode == "+y":
            print Magenta + cmdPolY + ENDC
            os.system(cmdPolY)
            nameDir = "group.phys-gener.protos22b.410912.st_tchan_lept_tbj_Pyplus1_CTEQ6L1_13TeV.TXT.mc15_v1"
        if options.polMode == "-y":
            print Magenta + cmdPolY0 + ENDC
            os.system(cmdPolY0)
            nameDir = "group.phys-gener.protos22b.410911.st_tchan_lept_tbj_Pyminus1_CTEQ6L1_13TeV.TXT.mc15_v1"
        if options.polMode == "sm":
            print Magenta + cmdPolSM + ENDC
            os.system(cmdPolSM)
            nameDir = "group.phys-gener.protos22b.410915.st_tchan_lept_tbj_GenSM_CTEQ6L1_13TeV.TXT.mc15_v1"
        if options.polMode == "ng":
            print Magenta + cmdPolNG + ENDC
            os.system(cmdPolNG)
            nameDir = "group.phys-gener.protos22b.410908.st_tchan_lept_tbj_SM_CTEQ6L1_13TeV.TXT.mc15_v1"
        #os.system('python /afs/cern.ch/work/r/rubi/work/stPolarization/stPolManip/pyScript/rootTotxt.py -f stPolarization.root')
        #os.system('mv fromROOT.events %s' % filenameEVENTS)
        os.system('mv stPolarization.root %s' % filenameROOT)
        os.system('mv protos.events %s' % filenameEVENTS)
        os.system('ln -fs %s protos.events' % (filenameEVENTS))
        prepareDATfile(filenameDAT, myindex, options.step)
        os.system('ln -fs %s._%05d.dat protos.dat' % (options.pattern, options.index))
        #os.system('rm -f %s._%05d.tar.gz' % (options.pattern, myindex))
        os.system('tar -cf %s._%05d.tar protos.dat %s %s protos.events' % (options.pattern, myindex, filenameEVENTS, filenameDAT)) #Without root file saved.
        #os.system('tar -cf %s._%05d.tar protos.dat %s %s protos.events %s' % (options.pattern, myindex, filenameEVENTS, filenameDAT, filenameROOT)) #With Root file
        os.system('gzip -f %s._%05d.tar' % (options.pattern, myindex))
        #os.system('mv %s._%05d.tar.gz /eos/user/r/rubi/shares/Nello/12M/%s' % (options.pattern, myindex, nameDir))
        os.system('mv %s._%05d.tar.gz /eos/user/r/rubi/shares/Nello/12M/%s' % (options.pattern, myindex, options.pattern))
        os.system('mv %s /eos/user/r/rubi/shares/Nello/12M/%s' % (filenameROOT,options.pattern))
        
    print '%s is finished working!' %filenameEVENTS

# ====================================================================
#  __main__
# ====================================================================
if __name__ == "__main__":
    main()
