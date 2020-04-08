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
Default = '\033[0m'

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
    parser.add_option("-p", "--pattern", dest="pattern", default="group.phys-gener.protos22b.410908.st_tchan_lept_tbj_SM_CTEQ6L1_13TeV.TXT.mc15_v1",                     help="set dataset pattern name [default: %default]", metavar="DS name")
    parser.add_option("-i", "--inputfilename", dest="inputfilepath", default="../test/test10K24R01.dat",
                      help="set path of the binary input file [default: %default]", metavar="PATH")
    parser.add_option("-s", "--savePath", dest="savePath", default="/eos/user/r/rubi/shares/Nello/12M/",
                      help="set path to save the output files.[default: %default]", metavar="PATH")
    parser.add_option("--start", dest="start", default=1,
                      help="set the starting event number to be processed [default: %default]", metavar="NUM", type="int")
    parser.add_option("--end", dest="end", default=240000,
                      help="set the last event number to be processed [default: %default]", metavar="NUM", type="int")
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
    parser.add_option("--time", action="store", dest="qtime", default="1nh",
                      help="Queue time desired for thw job. [default: %default]", type="string")
    (options, args) = parser.parse_args()

    if options.end < options.step:
        print +RED + "ERROR :: check your requested end value" + ENDC
        sys.exit()

    workingDir =os.environ['PWD']
    # print workingDir

    # os.system('rm -fr %s' % options.pattern)
    os.system('mkdir -p %s' % options.pattern)
    os.chdir(options.pattern)
    
    doDummy = False
    
    extFiles = len([file for file in os.listdir(options.savePath + options.pattern) if file.startswith(options.pattern)])
    print (Magenta + "There existing %i files in %s right now, for your information." %(extFiles,options.savePath) + ENDC)
    fileIndex = int(raw_input(BOLD + "What index would you like to start with? \n"))
    #print(BOLD + "Which production mode is this going to be?")
    #modeFlag = raw_input("new/re: " + ENDC)
    #if (modeFlag == "new"):
    #    fileIndex = startIndex + 1
    #elif (modeFlag =="re"):
    #    fileIndex = (options.start-1)/options.step + 1
    #else:
    #    print ("Bye")
    #    sys.exit()
    print(Red + "This file will be the first one to create. Continue?" + ENDC)
    print(BOLD + '%s._%05d.events'%(options.pattern,fileIndex) + ENDC)
    contFlag = raw_input("Yes/No: ")
    if (contFlag != "Yes"):
        print ("Bye")
        sys.exit()

    for index,i in enumerate(range(options.start-1, options.end, options.step)):
        # print index
        myindex = fileIndex + index
        firstEvent = (i+1)-(options.start-1)
        #lastEvent = (myindex*options.step)-(options.start-1)
        lastEvent = firstEvent - 1 + options.step
        print str(firstEvent) + '; ' + str(lastEvent)
        # print myindex
        if os.path.isfile('%s/%s/%s._%05d.tar.gz' % (options.savePath,options.pattern,options.pattern, myindex)):
            if options.reprocess:
                print 'INFO :: File %s._%05d.tar.gz already exists but it will be overwritten!' % (options.pattern, myindex)
            else:
                print WARNING + 'WARNING :: File %s._%05d.tar.gz already exists! It will not be overwritten!' % (options.pattern, myindex) + ENDC
                continue
        if not doDummy:
            filenameEVENTS = '%s._%05d.events' % (options.pattern, myindex)
            cmd = '/afs/cern.ch/work/r/rubi/work/Protos22-Prod/toAtlas %s %d %d > %s' % (options.inputfilepath, firstEvent, lastEvent, filenameEVENTS)
            if options.debug: print BLUE + cmd + ENDC
            os.system(cmd)
            #filenameDAT = '%s._%05d.dat' % (options.pattern, myindex)
            # print filenameDAT
            # prepareDATfile(filenameDAT, myindex, options.step)
            # os.system('more %s' % filenameDAT)
            if options.debug:
                os.system('head -8 %s' % filenameEVENTS)
                os.system('tail -n 8 %s' % filenameEVENTS)
            #cmdSub = 'python ../subJob.py -i %s/%s/%s --polManip --mode %s --step %s --index %d -p %s'%(workingDir,options.pattern,filenameEVENTS, options.polMode, options.step, myindex,options.pattern)
                cmdSub = 'bsub -q %s "export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase && source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh && lsetup root python&& python /afs/cern.ch/work/r/rubi/work/Protos22-Prod/subJob.py -i %s/%s/%s --polManip --mode %s --step %s --index %d -p %s"'%(options.qtime,workingDir,options.pattern,filenameEVENTS, options.polMode, options.step, myindex,options.pattern)
                #cmdSub = 'python /afs/cern.ch/work/r/rubi/work/Protos22-Prod/subJob.py -i %s/%s/%s --polManip --mode %s --step %s --index %d -p %s'%(workingDir,options.pattern,filenameEVENTS, options.polMode, options.step, myindex,options.pattern)

               # cmdSub = 'prun --exec "python subJob.py -i %s/%s/%s --polManip --mode %s --step %s --index %d -p %s" --outDS user.rubi.testRun --maxFileSize 12000000 --outputs out1.tar.gz'%(workingDir,options.pattern,filenameEVENTS,options.polMode, options.step, myindex,options.pattern) 
#cmdSub = 'source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh && lsetup root &&lsetup "lcgenv -p LCG_93 x86_64-slc6-gcc62-opt Python" && python /afs/cern.ch/work/r/rubi/work/Protos22-Prod/subJob.py -i %s/%s/%s --polManip --mode %s --step %s --index %d -p %s'%(workingDir,options.pattern,filenameEVENTS, options.polMode, options.step, myindex,options.pattern)
            print Red + BOLD + cmdSub
            print Grey + "============== Start of subJob.py script ================="
            os.system(cmdSub)
            print Grey + "============== End of subJob.py script ================="
#            if options.debug:
#                os.system('ls -lh protos.events protos.dat')
#                os.system('ls -lh %s._%05d*' % (options.pattern, myindex))
            #os.system('rm %s' % (filenameEVENTS))
            #os.system('rm %s %s' % (filenameEVENTS, filenameDAT))
            #os.system('rm protos.events protos.dat')
            #os.system('rm *.root')
#    os.chdir(workingDir)
    print Default + '%s job is sent!' % options.pattern

# ====================================================================
#  __main__
# ====================================================================
if __name__ == "__main__":
    main()
