##############################################
#Author: Runyu
#Created for generating/analyzing polarized
#top samples.
##############################################

import os, sys, getopt, string, commands
from math import ceil
from optparse import OptionParser

username = commands.getoutput("whoami")

# define colors
BLUE = '\033[94m'
GREEN = '\033[92m'
BOLD = "\033[1m"
WARNING = '\033[93m'
ERROR = '\033[91m'
ENDC = '\033[0m'

# =================================================================================
#  setupAthenaRelease
# =================================================================================
def setupAthenaRelease():
    # check AMI-tags used:
    # https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AtlasProductionGroupInternal#evgen_releases_recommendations
    #
    # to check which athena release was used for a given AMI-tag:
    # https://ami.in2p3.fr/new/?subapp=amiTags_shwo
    #
    # all releases:	
    # http://atlas-computing.web.cern.ch/atlas-computing/projects/releases/status/
    #
    # check tags:
    # http://atlas-project-mc-production.web.cern.ch/atlas-project-mc-production/Generators/MCProd
    #
    global AthenaRelease
    if effort == 'mc11_7TeV':
        AthenaRelease = '16.6.6.3,slc5,gcc43'
    elif effort == 'mc12_7TeV':
        AthenaRelease = 'MCProd,17.2.12.7.10,64,slc5' # Sherpa 2.1.1
    elif effort == 'mc12_8TeV' or effort == 'mc12_13TeV':
        # AthenaRelease = 'MCProd,17.2.12.7.4,slc5' # to setup Herwig++ 2.7.0
        AthenaRelease = '17.3.13.6' #
    elif effort == 'mc15_13TeV':
        # AthenaRelease = '19.2.5.25.1,MCProd' # need to use PowhegControl-00-03-00 for POWHEG-BOX V2
        # AthenaRelease = '19.2.5.26.1,MCProd' # need to use PowhegControl-00-03-02 for POWHEG-BOX V2
        # AthenaRelease = '19.2.5.26.2,MCProd' # need to use PowhegControl-00-03-02 for POWHEG-BOX V2
        # AthenaRelease = '19.2.5.26.3,MCProd' # need to use PowhegControl-00-03-02 for POWHEG-BOX V2
        # AthenaRelease = '19.2.5.26.4,MCProd' # need to use PowhegControl-00-03-04 for POWHEG-BOX V2 (Dominic)
        # AthenaRelease = '19.2.5.28.3,MCProd' # PowhegControl-00-03-07 (rel. suggested by Daniel Hayden)
        # AthenaRelease = '20.7.9.9.9,MCProd' # PowhegControl-00-03-09 is needed
        # AthenaRelease = '19.2.X.Y.Z-VAL,rel_0,MCProd' # PowhegControl-00-03-09 is needed
        AthenaRelease = '19.2.5.30.2,MCProd'
    elif effort == 'mc15_14TeV':
        AthenaRelease = '19.2.5.30.2,MCProd'
    else:
        print ERROR + 'ERROR :: athena release not defined' + ENDC
        sys.exit()

    # add "here"
    if not 'here' in AthenaRelease: AthenaRelease += ',here'

    # print
    # print 'effort',effort
    # print 'Athena release:',AthenaRelease
    # sys.exit()
    # print

# =================================================================================
#  main
# =================================================================================
def main(argv):
    parser = OptionParser()
    parser.add_option("-j", "--jobOptions", dest="jobOptionsPath", default="",
                      help="set the jobOptions", metavar="JO")
    parser.add_option("--extras", dest="extras", default="",
                      help="set the extra jobOptions to be included in the MC1XJobOptions package", metavar="EXTRAS")
    parser.add_option("-i", "--inputfile", dest="inputGeneratorFilePath", default="",
                      help="set the inputfile", metavar="INPUTFILE")
    parser.add_option("-p", "--premadeGrid", action="store_true", dest="isPremadeGrid",
                      help="set if the inputfile is a premade grid")
    parser.add_option("-d", "--inputDS", dest="inputDS", default="",
                      help="set the input DS", metavar="DS")
    parser.add_option("-e", "--energy", dest="energy", default=0,
                      help="set energy manually [GeV]", metavar="ENERGY", type="int")
    parser.add_option("-c", "--campaign", dest="campaign", default="",
                      help="set campaign manually [for example: mc15]", metavar="CAMPAIGN")
    parser.add_option("--maxEvents", dest="maxEvents", default=-1,
                      help="set maximum number of events", metavar="EVENTS", type="int")
    parser.add_option("-f", "--maxFiles", dest="numOfFiles", default=1,
                      help="set number of files to be produced", metavar="FILES", type="int")
    parser.add_option("--iniFile", dest="iniFile", default=0,
                      help="set initial file number to be produced", metavar="FILENUM", type="int")
    parser.add_option("--subdir", dest="subdir", default="",
                      help="set subdirectory name to store the output files in EOS", metavar="SUBDIR") 
    parser.add_option("-b", "--lxbatch", action="store_true", dest="lxbatch",
                      help="run in batch mode (lxbatch)")
    parser.add_option("-q", "--queue", dest="queue", default="8nm",
                      help="set the lxbatch queue [8nm, 1nh, 8nh, 1nd, 2nd, 1nw, 2nw] [default: %default]", metavar="ROUTINE_LIST")
    parser.add_option("--reprocess", action="store_true", dest="reprocess",
                      help="re-process a file if already exists")
    parser.add_option("-s", "--suffix", dest="suffix", default="",
                      help="set suffix to the output", metavar="SUFFIX")
    parser.add_option("--packages", dest="packages", default="",
                      help="set extra needed packages", metavar="PKGS")
    parser.add_option("--compilePackages", action="store_true", dest="compilePackages",
                      help="flag to just execute the package preparation")
    (options, args) = parser.parse_args()

    # force just few events
    # options.maxEvents = 100

    # normal execution
    global jobOptionsPath
    jobOptionsPath = options.jobOptionsPath
    if jobOptionsPath == '':
        print 'FATAL: jobOptions has not set!'
        sys.exit()

    global jobOptions
    jobOptions = jobOptionsPath.split("/")[-1]
    # print 'jobOptions:',jobOptions

    global extra_jobOptions
    extra_jobOptions = options.extras

    # check physicsShort lenth (max. 60 characters)
    global physicsShort
    physicsShort = jobOptions.split('.')[2]
    # print
    # print 'jobOptions check: physicsShort (length = '+str(len(physicsShort))+'): '+physicsShort
    # print
    if len(physicsShort) > 60:
        print '%sERROR :: %s contains a physicsShort field of more than 60 characters (%d): please rename! %s' % (ERROR, jobOptions, len(physicsShort), ENDC)
        sys.exit() 

    # ===================================================================

    writeHepMC = False

    # ===================================================================

    global WorkingDir
    WorkingDir = os.environ['PWD']

    global DSID
    DSID = int(jobOptions.split('.')[1])
    # print DSID

    # ===================================================================

    global numOfFiles
    numOfFiles = options.numOfFiles
    global iniFile
    iniFile = options.iniFile

    # substract one unit to fit loops
    if iniFile > 0: iniFile -= 1

    # ===================================================================

    # queues
    if options.lxbatch and not any(x in options.queue for x in ['8nm', '1nh', '8nh', '1nd', '2nd', '1nw', '2nw']):
        print '\033[91m' + 'ERROR :: Please select a proper queue for lxbatch!' + '\033[0m'
        parser.print_help()
        sys.exit()

    # ===================================================================

    global inputGeneratorFile
    inputGeneratorFile = []

    global inputGeneratorFilePath
    inputGeneratorFilePath = options.inputGeneratorFilePath

    # print
    # print 'inputGeneratorFilePath:',inputGeneratorFilePath
    # print 'inputGeneratorFile:',inputGeneratorFile
    # print
    # sys.exit()

    global isPremadeGrid
    isPremadeGrid = options.isPremadeGrid

    # inputDS
    global inputDS
    inputDS = options.inputDS
    # print 'inputDS:',inputDS

    global maxEvents
    maxEvents = options.maxEvents
    # if maxEvents == -1: maxEvents = numOfFiles*5000
    if maxEvents == -1: maxEvents = 5000

    global inputFilesPerJob
    inputFilesPerJob = -1
    if maxEvents < 5000: inputFilesPerJob = 1
    if inputDS != '': inputFilesPerJob = int(ceil(float(maxEvents)/5000))

    # print 'inputFilesPerJob:',inputFilesPerJob
    # sys.exit()

    # run in the lxbatch
    lxbatch = options.lxbatch
    if lxbatch == None: lxbatch = False
    # print 'lxbatch:',lxbatch

    # re-process file if already exists
    global reprocess
    reprocess = options.reprocess
    if reprocess == None: reprocess = False
    # print 'reprocess:',reprocess

    # suffix
    global suffix
    suffix = options.suffix

    # retrieve campaign and energy
    global campaign
    energy = options.energy
    campaign = options.campaign

    if inputDS =='': campaign, energy = getRunConf(inputGeneratorFilePath.split("/")[-1][:-14], campaign, energy)
    else: campaign, energy = getRunConf(inputDS, campaign, energy)
    
    global effort
    effort = "%s_%dTeV" % (campaign, int(energy/1000))
    print 'effort:',effort

    # ===================================================================

    # configure
    setupAthenaRelease()

    # ===================================================================
    
    # execute the package preparation if needed
    global packageList
    packageList = options.packages
    global compilePackages
    compilePackages = options.compilePackages
    if len(packageList) > 0: _preparePackages(packageList)
    # ===================================================================

    global outputDir
    if options.subdir == '':
        # print 'jobOptions:',jobOptions
        evgenDS = jobOptions.split('.')
        evgenDS = evgenDS[1]+'.'+evgenDS[2]
        if suffix!='': evgenDS = evgenDS + '_' + suffix
        version = inputDS.split('.')[-1].split('_')[-1]
        if version=='': version = 'v1'
        # print 'version',version
        evgenDS = evgenDS +'.evgen.EVNT.'+version
        options.subdir = effort+'.'+evgenDS
    outputDir = '/eos/atlas/user/%s/%s/top/validation/evgen/%s/' % (username[:1], username, options.subdir)
    # print '* outputDir (EOS):',outputDir
    # sys.exit()

    # ===================================================================

    print
    print BOLD + "==========================================================================================================" + ENDC
    print BOLD + "Summary" + ENDC
    print BOLD + "==========================================================================================================" + ENDC
    print '* effort',effort
    print '* Athena release:',AthenaRelease
    print '* maxEvents:',maxEvents
    print '* initial file number:', str(iniFile+1)
    print '* number of input files per job:',inputFilesPerJob
    print '* total number of output files (i.e. number of jobs):',numOfFiles
    if inputDS != "": print '* inputDS:',inputDS
    print '* output: ' + outputDir
    # print '* inputGeneratorFilePath:',inputGeneratorFilePath
    # print '* inputGeneratorFile:',inputGeneratorFile
    print BOLD + "==========================================================================================================" + ENDC
    print
    # sys.exit()

    # ===================================================================


    global myOutputFileList
    myOutputFileList = []

    global myFilesList
    myFilesList = {}
    
    numOfJobs = iniFile

    if inputDS != '':
        global myInputFileList
        myInputFileList = []
        for i in commands.getoutput("ls %s | grep tar.gz" % (inputDS)).splitlines(): myInputFileList += [ i ]
        # for i in myInputFileList: print i
        # print len(myInputFileList)
        # sys.exit()

        # print 'inputFilesPerJob',inputFilesPerJob
        # print 'iniFile',iniFile+1
        # print

        DSversion = int((inputDS.split('.')[-1].split('_')[-1]).replace('/','').replace('v',''))

        if inputFilesPerJob > 1:
            if iniFile+1 > 1: numOfJobs = iniFile # not working properly!!!!
            else: numOfJobs = iniFile
        else: numOfJobs = iniFile

        _prepareWorkdir(True, numOfJobs, (inputFilesPerJob-1), DSversion)

    if inputDS == '':
        DSversion = 1
        _prepareWorkdir(False, numOfJobs, 1, DSversion)

    #print numOfJobs
    #print myFilesList
    #sys.exit()

#        if myoutput == myoutputfile:
#            if reprocess == False:
#                executeCommand = False
#                print "WARNING: file ("+myoutputfile+") already exists in EOS! skipping execution!"
#                print
#                sys.exit()
#            else: print "INFO: file ("+myoutputfile+") already exists in EOS but it will re-processed!"
#        mydir = "tmp_%d_%05d_v%d" % (DSID, iniFile+1, DSversion)
#        # print 'mydir:',mydir
#        if suffix!='': mydir = mydir + '_' + suffix
#        myInputDS = os.environ['PWD'] + '/' + inputDS
#        # print 'myInputDS:',myInputDS
#        os.chdir(mydir)
#        # os.system('echo \"PWD: $PWD\"')
#        if '/eos/atlas' in inputDS:
#            # cmdExe = 'xrdcp -f root://eosatlas.cern.ch/' + inputGeneratorFilePath + ' ' + inputGeneratorFile[0]
#            # cmdExe = 'xrdcp -f root://eosatlas.cern.ch/' + inputDS
#            cmdExe = 'ln -s ' + inputDS
#            myoutput = commands.getoutput("ls %s | grep tar.gz" % (inputDS)).splitlines()
#            # print myoutput
#            # print len(myoutput)
#            # print 'numOfFiles:',numOfFiles
#            for idx, i in enumerate(myoutput):
#                # print str(idx+1)
#                if idx+1 == iniFile+1+numOfFiles: break
#                if idx+1 < iniFile+1: continue
#                # print str(idx+1) + ': ' + i
#                myCmdExe = cmdExe + '/' + i + ' ' + os.environ['PWD'] + '/' + mydir + '/' + i
#                # print myCmdExe
#                os.system(myCmdExe)
#                inputGeneratorFile += [ i ]
#        else:
#            for idx, line in enumerate(commands.getoutput('ls ' + myInputDS + ' | grep tar.gz | sort').splitlines()):
#                # print str(idx+1)
#                if idx+1 == iniFile+1+numOfFiles: break
#                if idx+1 < iniFile+1: continue
#                # print line
#                inputGeneratorFile = [ line ]
#                cmdExe = 'ln -fs ' + myInputDS + '/' + line + ' .'
#                # print cmdExe
#                os.system(cmdExe)
        # print cmdExe

    # ===================================================================

    # print 'inputGeneratorFilePath:', inputGeneratorFilePath
#    if inputGeneratorFilePath != '' and inputDS == '':
#        # print 'inputGeneratorFilePath',inputGeneratorFilePath
#        inputDS = inputGeneratorFilePath.split("/")[-1][:-14]
#        # print 'inputDS:',inputDS
#        DSversion = int((inputDS.split('.')[-1].split('_')[-1]).replace('/','').replace('v',''))
#        # print 'DSversion: v',DSversion
#        myoutputfile = "%s.%d.EVNT._%05d.pool.root" % (effort, DSID, iniFile+1)
#        # print '* outputfile:',myoutputfile
#        print '* outputfile (EOS): ' + outputDir + '/' + myoutputfile
#        myoutput = commands.getoutput("ls %s/%s" % (outputDir, myoutputfile))
#        if myoutput == myoutputfile:
#            if reprocess == False:
#                executeCommand = False
#                print WARNING + "WARNING: file ("+myoutputfile+") already exists in EOS! skipping execution!" + ENDC
#                print
#                sys.exit()
#            else: print "INFO: file ("+myoutputfile+") already exists in EOS but it will re-processed!"

#        mydir = "tmp_%d_%05d_v%d" % (DSID, iniFile+1, DSversion)
#        if suffix!='': mydir = mydir + '_' + suffix
#        # print 'mydir:',mydir
#        os.system('mkdir -p %s' % mydir)
#        os.chdir(mydir)
#        # os.system('pwd')
#        inputGeneratorFile = [ inputGeneratorFilePath.split('/')[-1] ]
#        if '/eos/atlas' in inputGeneratorFilePath:
#            # cmdExe = 'xrdcp -f root://eosatlas.cern.ch/' + inputGeneratorFilePath + ' ' + os.environ['PWD'] + '/' + mydir + '/' + inputGeneratorFile[0]
#            cmdExe = 'ln -s ' + inputGeneratorFilePath + ' ' + os.environ['PWD'] + '/' + mydir + '/' + inputGeneratorFile[0]
#            # print cmdExe
#            os.system(cmdExe)
#        else:
#            # cmdExe = 'ln -fs '+inputGeneratorFilePath + ' ' + inputGeneratorFile[0]+' .'
#            if '/afs/cern.ch/user/' in inputGeneratorFilePath: cmdExe = 'ln -fs ' + inputGeneratorFilePath + ' .'
#            else: cmdExe = 'ln -fs ' + WorkingDir + '/' + inputGeneratorFilePath + ' .'
#            #print
#            #print cmdExe
#            #print
#            os.system(cmdExe)
#        _copyJobOptions()

    # ===================================================================

    # print inputGeneratorFilePath
    # print
    # print 'inputGeneratorFile:', str(inputGeneratorFile)
    # print

    # os.system("pwd")
    os.chdir(WorkingDir)

    # print myTmpDirList
    # sys.exit()


    # os.system("ls -lth --color=auto")
    # sys.exit()
        
    # sys.exit()

    # ===================================================================

    global isAfterBurnerRequested
    isAfterBurnerRequested = checkIfAfterBurnerRequested()

    # ===================================================================
    # ===================================================================
    # ===================================================================
    # ===================================================================

    # running locally
    evgenJobOpts = retrieveEvgenJobOpts(campaign)
    # print 'evgenJobOpts',evgenJobOpts

    print BOLD + "==========================================================================================================" + ENDC
    if len(myTmpDirList) == 0:
        print
        print ERROR + "Nothing to be done!" + ENDC
        print
        print BOLD + "==========================================================================================================" + ENDC
        print
        sys.exit()

    print
    if pathena: print 'running in the GRID ['+effort+']...'
    elif lxbatch: print 'running in batch mode ['+effort+']...'
    else: print 'running locally ['+effort+']...'

    # DS version
#    if inputDS != '': DSversion = int((inputDS.split('.')[-1].split('_')[-1]).replace('/','').replace('v',''))
#    else: DSversion = 1
    # print 'DSversion: v' + str(DSversion)
    # sys.exit()

    if pathena:
        exeCmd = _preparePathenaCmdExe(energy, evgenJobOpts, DSversion, extra_jobOptions)

        scriptname = "pathena_%d_%dGeV_v%d.sh" % (DSID, energy/1000, DSversion)
        try:
            myScript = open(scriptname, "w")
            myScript.write("#!/bin/bash\n")

            myScript.write('echo\n')
            myScript.write('echo \"setting up athena %s...\"\n' % AthenaRelease)
            myScript.write('export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase\n')
            myScript.write('alias setupATLAS=\"source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh\"\n')
            myScript.write('export ALRB_localConfigDir=$HOME/localConfig\n')
            myScript.write('setupATLAS --quiet\n')
            myScript.write('asetup %s\n' % AthenaRelease)
            myScript.write('localSetupPandaClient\n')

            # compile requested packages
            # myTempDir = os.environ['PWD']
            myTempDir = commands.getoutput('pwd')
            for i in packages:
                myScript.write('echo\n')
                myScript.write('echo -> %s\n' % i)
                if not os.path.exists(i): myScript.write('pkgco.py %s\n' % i)
                if compilePackages:
                    myScript.write('cd %s/cmt \n' % i)
                    myScript.write('cmt br cmt config\n')
                    myScript.write('source setup.sh\n')
                    myScript.write('cmt br gmake\n')
                    myScript.write('cd %s\n' % myTempDir)

            if packageList != '': myScript.write('python produce_evgen.py --packages %s --compilePackages\n\n' % packageList)

            # myScript.write('# use the existing local proxy\n')
            # myScript.write('echo\n')
            # myScript.write('echo \"creating proxy...\"\n')
            # myScript.write('export PROXY=$HOME/private/proxy/x509up_u`id -u`\n')

            # myScript.write('if ! [ -s $PROXY ] ; then\n')
            # myScript.write('    voms-proxy-init -voms atlas\n')
            # myScript.write('else\n')
            # myScript.write('    voms-proxy-init -cert $PROXY -key $PROXY -voms atlas\n')
            # myScript.write('fi\n')

            myScript.write('\n')
            myScript.write('%s\n' % exeCmd)
            myScript.write('\n')
            myScript.close()

            print
            os.system('chmod a+x %s' % scriptname)
            os.system('more %s' % scriptname)
            _execute(executeCommand, 'source ./%s' % scriptname)
            os.system('rm %s' % scriptname)
        except getopt.GetoptError, err:
            print "%sERROR: %s can not be opened :(%s" % (ERROR, scriptname, ENDC)
            sys.exit(1)
    else:
        idx = 0
        for key,val in sorted(myFilesList.items()):
            # print key, "=>", val
            # continue

            fileIdx = int((key.split('.')[-3])[2:])
            # print 'fileIdx:',fileIdx
            # print 'idx:',idx
            # print "myTmpDirList[" + str(idx) + "]: " + myTmpDirList[idx]
            inputGeneratorFile = val
            # os.system('cp produce_evgen.py %s/' % myTmpDirList[idx])

            mydir = myTmpDirList[idx].split('/')[-1]
            exeCmd = _prepareCmdExe(energy, evgenJobOpts, fileIdx, mydir)
        
            print
            print BLUE + exeCmd + ENDC
            print

            _prepareScript(exeCmd, fileIdx, mydir)
            exeScriptCmd = "source %s/%s/%s.sh" % (WorkingDir, mydir, mydir)
            # os.system("ls -lrh --color=auto")
            os.system("chmod a+x %s/%s.sh" % (myTmpDirList[idx], mydir))
            # os.system("pwd")
            # os.system("ls -lrh --color=auto")
            # sys.exit()
            idx += 1

            if lxbatch: exeScriptCmd = 'bsub -q ' + options.queue + ' ' + exeScriptCmd + '; sleep 2'
            # print exeScriptCmd

            # set either execute command or not
            if options.noSubmit == True: executeCommand = False
            else: executeCommand = True
                    
            _execute(executeCommand, exeScriptCmd)
            
            # print "executeCommand",executeCommand
            # print ""
            # os.system("ls -lht")
            # print ""
            
            os.chdir(WorkingDir)
            print ""

# =================================================================================
#  _prepareCmdExe
# =================================================================================
def _prepareCmdExe(energy, evgenJobOpts, fileNum, mydir):
    
    extraChr = ''
    if effort=='mc15_13TeV' or effort=='mc15_14TeV': extraChr = '--'

    # Generate_trf command
    exeCmd = "Generate_trf.py"
    if effort=='mc15_13TeV' or effort=='mc15_14TeV': exeCmd = "Generate_tf.py"
    exeCmd += " %secmEnergy=%d" % (extraChr, energy)
    exeCmd += " %srunNumber=%d" % (extraChr, DSID)
    exeCmd += " %sfirstEvent=1 %smaxEvents=%d" % (extraChr, extraChr, maxEvents)
    # exeCmd += " %srandomSeed=12345" % (extraChr)
    RNDM = 100 + fileNum
    # print RNDM
    exeCmd += " %srandomSeed=%d" % (extraChr, RNDM)
    exeCmd += " %sjobConfig=%s" % (extraChr, jobOptions)
    exeCmd += " %sevgenJobOpts=%s" % (extraChr, evgenJobOpts)
    if (effort=='mc15_13TeV' or effort=='mc15_14TeV') and isAfterBurnerRequested: exeCmd += " --steering=afterburn" # just needed for mc15

    if isPremadeGrid:
        if len(inputGeneratorFile) > 0:
            exeCmd += " %sinputGenConfFile=%s" % (extraChr, inputGeneratorFile[fileNum-iniFile-1])
    else:
        if len(inputGeneratorFile) > 0:
            exeCmd += " %sinputGeneratorFile=" % extraChr
            for i in inputGeneratorFile:
                exeCmd += "%s," % i
            exeCmd = exeCmd[:-1]

    if suffix!='': mydir = mydir + '_' + suffix
    exeCmd += " %soutputEVNTFile=/tmp/%s/%s/%s.%d.EVNT._%05d.pool.root" % (extraChr, username, mydir, effort, DSID, fileNum)

    # full command
    exeCmd += " |& tee -a /tmp/%s/%s/%s.%d.EVNT._%05d.log" % (username, mydir, effort, DSID, fileNum)

    # using stdbuf -o0 to force STDOUT and STDERR into an unbuffered state
    exeCmd = 'stdbuf -o0 ' + exeCmd

    # print exeCmd
    return exeCmd

# =================================================================================
#  _preparePathenaCmdExe
# =================================================================================
def _preparePathenaCmdExe(energy, evgenJobOpts, DSversion, extras):
    # Generate_trf command
    exeCmd = "Generate_trf.py"
    if effort=='mc15_13TeV' or effort=='mc15_14TeV': exeCmd = "Generate_tf.py"
    exeCmd += " ecmEnergy=%d" % energy
    exeCmd += " runNumber=%d" % DSID
    exeCmd += " firstEvent=1 maxEvents=%d" % maxEvents
    exeCmd += " randomSeed=%RNDM:12345"
    exeCmd += " jobConfig=%s" % jobOptions
    exeCmd += " evgenJobOpts=%s" % evgenJobOpts
    if inputGeneratorFile != '': exeCmd += " inputGeneratorFile=%IN"
    
    exeCmd += " outputEVNTFile=%OUT.EVNT.pool.root"

    # full command
    exeCmd = "pathena --trf \"%s\" --inDS %s" % (exeCmd, inputDS)
    if numOfFiles > 0: exeCmd += " --nFiles %d" % numOfFiles
    exeCmd += " --nFilesPerJob 1"

    outputDS = "user.%s.%s.%d.%s" % (username, effort, DSID, physicsShort)
    if suffix!='': outputDS = outputDS + "_" + suffix
    outputDS = outputDS + ".evgen.EVNT.v%d" % (DSversion)
    exeCmd += " --outDS %s" % (outputDS)

    if extras != '': exeCmd += ' --extFile=%s' % extras
    # exeCmd += ' --mergeOutput'

    # print exeCmd
    return exeCmd

# =================================================================================
#  checkIfAfterBurnerRequested
# =================================================================================
def checkIfAfterBurnerRequested():
    isEvtGenRequested = False
    file = open(jobOptions, 'r')
    jOtext = file.read()
    # print jOtext

    # if 'EvtGen' in jOtext: isEvtGenRequested = True
    with open(jobOptions) as f:
        for line in f:
            # print line
            if not line.startswith('#') and 'EvtGen' in line: isEvtGenRequested = True
    f.close()

    # print 'isEvtGenRequested',isEvtGenRequested

    #  --steering=afterburn is only needed for fortran generators
    if any(x in jOtext for x in ['Pythia8', 'Herwigpp']): isEvtGenRequested = False

    file.close()

    # print isEvtGenRequested
    return isEvtGenRequested

# =================================================================================
#  retrieveEvgenJobOpts
# =================================================================================
def retrieveEvgenJobOpts(campaign):
    EvgenJobOptsDir = '/afs/cern.ch/atlas/www/GROUPS/COMPUTING/links/kitsDirectory/EvgenJobOpts/'
    exeCmd = 'ls -1 '+EvgenJobOptsDir+campaign.upper()+'JobOpts-0* --sort=time'
    # print exeCmd
    # os.system(exeCmd)

    evgenJobOpts = ''
    for i in commands.getoutput(exeCmd).splitlines():
        tarsize = (commands.getoutput('du -sh %s' % i).split('/')[0]).strip()
        if tarsize.endswith('M'):
            evgenJobOpts = i
            break

    # print 'evgenJobOpts:',evgenJobOpts
    evgenJobOpts = evgenJobOpts.split('/')[-1]

    # print 'evgenJobOpts:',evgenJobOpts
    return evgenJobOpts

# =================================================================================
#  getRunConf
# =================================================================================
def getRunConf(inputDS, campaign, energy):

    if campaign == '':
        if 'mc15' in inputDS: campaign = 'mc15'
        elif 'mc12' in inputDS: campaign = 'mc12'
        elif 'mc11' in inputDS: campaign = 'mc11'

    # if there is no inputDS, use jobOptions
    if campaign == '':
        # print 'jobOptions',jobOptions
        if jobOptions.startswith('MC15'): campaign = 'mc15'
        elif jobOptions.startswith('MC12'): campaign = 'mc12'
        elif jobOptions.startswith('MC11'): campaign = 'mc11'
        else:
            print 'ERROR: campaign not supported. Introduce manually the campaign?'
            try:
                campaign = input("Enter campaign [for instance mc12]: ")
                if not campaign.startswith('mc'): 
                    print 'ERROR:campaign  not campaign!'
                    sys.exit()
            except (SyntaxError, NameError):
                print 'ERROR: campaign not supported!'
                sys.exit()

    if energy > 0: return campaign, energy
    else:
        # print 'inputDS:',inputDS
        energy = int(0)
        if '7TeV' in inputDS: energy = int(7000)
        elif '8TeV' in inputDS: energy = int(8000)
        elif '14TeV' in inputDS: energy = int(14000)
        elif '15TeV' in inputDS: energy = int(15000)
        else:
            print 'ERROR: energy not supported. Introduce manually the energy?'
            try: 
                energy = int(input("Enter energy [GeV]: "))
                energy = energy*1000
            except (SyntaxError, NameError):
                print 'ERROR: energy not supported!'
                sys.exit()

        return campaign, energy

# =================================================================================
#  _copyJobOptions
# =================================================================================
def _copyJobOptions():
    if not os.path.isfile(jobOptions): os.system("cp -f ../%s ." % jobOptionsPath)

    # copy extra jobOptions (if required/needed)
    if extra_jobOptions != '':
        extrasList = []
        if len(extra_jobOptions.split(',')) > 1:
            for i in split(','): extrasList += [ i ]
        else: extrasList += [ extra_jobOptions ]
        # copy extra_jobOptions
        os.system('mkdir -p %sJobOptions' % campaign.upper())
        # for i in extra_jobOptions: print i
        for i in extrasList: os.system('cp -f ../%s %sJobOptions' % (i, campaign.upper()))
        # print extra_jobOptions

# =================================================================================
#  _prepareScript
# =================================================================================
def _prepareScript(exeCmd, numFile, mydir):
    try:
        # print "%s/%s/%s.sh" % (WorkingDir, mydir, mydir)
        myScript = open("%s/%s/%s.sh" % (WorkingDir, mydir, mydir), "w")
        myScript.write("#!/bin/bash\n")
        
        myScript.write("echo\n")
        # myScript.write("echo changing to the working directory\n")
        # myScript.write("pwd\n")
        myScript.write("cd %s/%s\n" % (WorkingDir, mydir))
        # myScript.write("pwd\n")
        # myScript.write("ls -lth --color=auto\n")

        myScript.write("echo \" - setting up Athena... %s\"\n" % AthenaRelease)
        myScript.write("export AtlasSetup=/afs/cern.ch/atlas/software/dist/AtlasSetup\n")
        myScript.write("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase\n")
        myScript.write("source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh  --quiet\n")
        myScript.write("lsetup --quiet \"asetup %s\"\n" % AthenaRelease)
        myScript.write("export RUCIO_ACCOUNT=\"$USER\"\n\n")
        # myScript.write("export ATHENA_PROC_NUMBER=4\n")
        #myScript.write("env")
#        myScript.write("echo ${LHAPATH}\n")
#        myScript.write("export LHAPDF_DATA_PATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/:${LHAPATH}\n")
#        myScript.write("echo ${LHAPDF_DATA_PATH}\n")

        # compile requested packages
        myTempDir = commands.getoutput('pwd')
        # print myTempDir
        if compilePackages:
            for i in packages:
                myScript.write("\n")
                myScript.write("echo\n")
                myScript.write('mkdir -p %s\n' % i)
                myScript.write("echo \" - syncronizing package %s...\"\n" % i)
                myScript.write('cd %s/mypackages/%s \n' % (WorkingDir, i))
                myScript.write('rm -rf %s/mypackages/%s/$CMTCONFIG \n' % (WorkingDir, i))
                # myScript.write("find . -name '.svn' -exec rm -fr {} \;\n")
                myScript.write("find . -name '*~' -exec rm -fr {} \;\n")
                myScript.write("find . -name '#*' -exec rm -fr {} \;\n")
                myScript.write("find . -name '.#*' -exec rm -fr {} \;\n")
                myScript.write("find . -name 'install.*' -exec rm -fr {} \;\n")
                myScript.write("cd %s/%s\n" % (WorkingDir, mydir))
                myScript.write('rsync -vazh %s/mypackages/%s/* %s\n' % (WorkingDir, i, i))
                myScript.write("cd %s/%s \n" % (WorkingDir, mydir))
            for i in packages:
                myScript.write("echo \" - compiling %s...\"\n" % i)
                myScript.write('cd %s/%s/%s \n' % (WorkingDir, mydir, i))
                myScript.write("if [ \"$(ls -A $CMTCONFIG)\" ]; then\n")
                myScript.write("echo \" - package %s is already compiled!\"\n" % i) 
                myScript.write("else\n")
                myScript.write('cd %s/%s/%s/cmt \n' % (WorkingDir, mydir, i))
                # myScript.write('source setup.sh\n')
                myScript.write('cmt br cmt config\n')
                myScript.write('cmt br gmake\n')
                myScript.write("fi\n")
                myScript.write("cd %s/%s \n" % (WorkingDir, mydir))

        myScript.write("\n")
        myScript.write("echo\n")
        myScript.write("# remove possible old files\n")
        myScript.write("rm -rf /tmp/%s/%s\n" % (username, mydir))
        myScript.write("mkdir -p /tmp/%s/%s\n" % (username, mydir))
        
        # myScript.write("echo\n")
        # myScript.write("echo\n")
        # myScript.write("pwd\n")
        # myScript.write("ls -lth --color=auto\n")
        # myScript.write("echo\n")
        # myScript.write("echo\n")

        #print jobOptions
        #sys.exit()

        # myScript.write("echo \"*******************************************************************************\" > /tmp/%s/%s/%s.%d.EVNT._%05d.log\n" % ((username, mydir, effort, DSID, numFile)))
        myScript.write("\n")
        myScript.write("echo \"more %s\" > /tmp/%s/%s/%s.%d.EVNT._%05d.log\n" % ((jobOptions, username, mydir, effort, DSID, numFile)))
        myScript.write("echo \"\" >> /tmp/%s/%s/%s.%d.EVNT._%05d.log\n" % ((username, mydir, effort, DSID, numFile)))
        myScript.write("more %s >> /tmp/%s/%s/%s.%d.EVNT._%05d.log\n" % ((jobOptions, username, mydir, effort, DSID, numFile)))
        myScript.write("echo \"\" >> /tmp/%s/%s/%s.%d.EVNT._%05d.log\n" % ((username, mydir, effort, DSID, numFile)))

        myScript.write("\n# execute...\n")
        myScript.write("echo\n")
        myScript.write("echo \"*******************************************************************************\"\n")
        myScript.write("echo\n")
        myScript.write("echo %s\n" % exeCmd)
        myScript.write("echo\n")
        myScript.write("echo \"*******************************************************************************\"\n")
        myScript.write("echo\n")
        myScript.write("%s\n\n" % exeCmd)
        # myScript.write("return\n")
        
        myScript.write("echo\n")
        myScript.write("cd /tmp/%s/%s/\n" % (username, mydir))
        myScript.write("pwd\n")
        myScript.write("ls -hl\n")
        # myScript.write("echo 'compressing the log file'\n")      
        myScript.write("gzip %s.%d.EVNT._%05d.log\n" % (effort, DSID, numFile))
        myScript.write("echo\n")
        myScript.write("ls -hl\n")
        myScript.write("echo\n")

        # just copy to the public directory the first log file 
        if numFile == 1:
            myScript.write("echo\n")        
            myScript.write("mkdir -p ~/public/top_validation/%s\n" % effort)
            myCmdText = 'cp -f %s.%d.EVNT._%05d.log.gz ~/public/top_validation/%s/%s.%d.EVNT.log.gz' % (effort, DSID, numFile, effort, effort, DSID)
            myScript.write("echo %s\n" % myCmdText)
            myScript.write(myCmdText+'\n')

        myScript.write("echo\n")
        myScript.write("pwd\n")
        myScript.write("echo\n")
        myScript.write("echo\n")
        myScript.write("echo copyting files to EOS...\n")
        myScript.write("# copy files to EOS\n")
        for extension in ['pool.root', 'log.gz']:
            myScript.write('echo\n')
            myScript.write('echo "which xrdcp (after asetup):"\n')
            myScript.write('which xrdcp\n')
            # myScript.write('echo \"xrdcp --version\"\n')
            # myScript.write('xrdcp --version\n')

            myScript.write('echo\n')
            myScript.write("if [ -e %s.%d.EVNT._%05d.%s ]; then\n" % (effort, DSID, numFile, extension))
            #myCmdText = 'xrdcp -d 2 -f %s.%d.EVNT._%05d.%s root://eosatlas/%s/%s.%d.EVNT._%05d.%s\n' % (effort, DSID, numFile, extension, \
            #                                                                                                outputDir, effort, DSID, \
            #                                                                                                numFile, extension)

            # the usage of xrdcp is not needed anymore
            #myCmdText = 'xrdcp -f %s.%d.EVNT._%05d.%s root://eosatlas/%s/%s.%d.EVNT._%05d.%s\n' % (effort, DSID, numFile, extension, \
            #                                                                                           outputDir, effort, DSID, \
            #                                                                                           numFile, extension)

            myCmdText = 'cp -fv %s.%d.EVNT._%05d.%s %s/%s.%d.EVNT._%05d.%s\n' % (effort, DSID, numFile, extension, \
                                                                                    outputDir, effort, DSID, \
                                                                                    numFile, extension)
            myScript.write('  echo %s' % myCmdText)
            myScript.write('  ' + myCmdText)
            myScript.write("fi\n")

        myScript.write("echo\n")
        myScript.write("echo %s\n" % outputDir)
        for extension in ['pool.root', 'log.gz']:
            myScript.write("ls -l %s/%s.%d.EVNT._%05d.%s | grep root\n" % (outputDir, effort, DSID, \
                                                                                  numFile, extension))
        myScript.write("echo\n")        
        myScript.write("cd ~/public/top_validation/%s/\n" % effort)
        myScript.write("pwd\n")

        # for extension in ['pool.root', 'log.gz']:
        #     myScript.write("ls -lh %s.%d.EVNT.%s\n" % (effort, DSID, extension))

        # myScript.write("ls -lh %s.%d.EVNT.root\n" % (effort, DSID))
        myScript.write("cd %s\n" % WorkingDir)

        # myScript.write("rm -rf /tmp/%s/%s/\n" % (username, mydir))
        myScript.write("rm -rf %s/%s\n" % (WorkingDir, mydir))
        
        myScript.close()
        # os.system("more %s.sh" % mydir)

    except getopt.GetoptError, err:
        print "ERROR: %s.sh can not be opened :(" % mydir
        sys.exit(1)

# =================================================================================
#  _execute
# =================================================================================
def _execute(executeCommand, exeCmd):
  # print exeCmd
  if (executeCommand): os.system(exeCmd)

# =================================================================================
#  _prepareWorkdir
# =================================================================================
def _prepareWorkdir(inputNeeded, numOfJobs, inumOfJobs, DSversion):

    global myTmpDirList
    myTmpDirList = []

    # print reprocess

    for idx, i in enumerate(range(iniFile+1, iniFile+1+numOfFiles)):
        print BOLD + str(iniFile+idx+1) + '. effort: ' + effort + ', jobOptions: ' + jobOptions + ENDC
        filenum = iniFile+idx+1
        myoutputfile = "%s.%d.EVNT._%05d.pool.root" % (effort, DSID, filenum)
        print '* outputfile: ' + outputDir + myoutputfile
        myoutput = commands.getoutput("ls %s%s" % (outputDir, myoutputfile))
        # print '* myoutput: ' + myoutput
        # print outputDir+myoutputfile

        if myoutput == outputDir+myoutputfile:
            if reprocess == False:

                filesize = int(commands.getoutput('wc -c <%s' % myoutput))
                print filesize
                # if existing output file is less than 1M, then the file is corrupted
                print maxEvents
                if maxEvents > 4000 and filesize < 1000000:
                    print WARNING+"NOTIFICATION :: file ("+myoutputfile+") seems to be corrupted in EOS! reprocessing!"+ENDC
                else:
                    executeCommand = False
                    print WARNING+"WARNING :: file ("+myoutputfile+") already exists in EOS! skipping execution!"+ENDC
                    print
                    numOfJobs += inumOfJobs
                    continue
            else: print GREEN+"INFO :: file ("+myoutputfile+") already exists in EOS but it will re-processed!"+ENDC
        mydir = "%s/tmp_%d_%s_%05d_v%d" % (WorkingDir, DSID, jobOptions.split('.')[2], filenum, DSversion)
        myTmpDirList += [ mydir ]
        os.system('mkdir -p %s' % mydir)
        os.system('cd %s' % mydir)
        os.chdir(mydir)
        # if inputDS.endswith('/'): os.system('rm -f %s/%s._*' % (mydir, inputDS.split('/')[-2]))
        # else: os.system('rm -f %s/%s._*' % (mydir, inputDS.split('/')[-1]))

        myInputSelectedFileList = []

        if inputNeeded:
            if inputFilesPerJob == 1:
                # print "* inputsfile: "+str(myInputFileList[filenum-inputFilesPerJob])
                # os.system('ln -sf %s%s' % (inputDS, str(myInputFileList[filenum-inputFilesPerJob])))
                print "* inputsfile: "+inputDS+str(myInputFileList[filenum-inputFilesPerJob])
                # myInputSelectedFileList += [inputDS+'/'+j]
                myInputSelectedFileList += [inputDS+str(myInputFileList[filenum-inputFilesPerJob])]
            else:
                # print " * inputsfiles: "+str(myInputFileList[(filenum-1)+numOfJobs:(filenum-1)+inputFilesPerJob+numOfJobs])
                for j in myInputFileList[(filenum-1)+numOfJobs:(filenum-1)+inputFilesPerJob+numOfJobs]:
                    os.system('ln -sf %s' % (inputDS+j))
                    print "* inputsfile: "+inputDS+j
                    myInputSelectedFileList += [inputDS+'/'+j]
                    # myInputSelectedFileList += [j]

        myFilesList[myoutputfile] = myInputSelectedFileList
        numOfJobs += inumOfJobs
        _copyJobOptions()
        os.chdir(WorkingDir)
        print

# =================================================================================
#  _preparePackages
# =================================================================================
def _preparePackages(packageList):
    print
    print 'The following (extra) packages will be used...'
    global packages
    packages = []
    if len(packageList.split(',')) > 1:
        for i in packageList.split(','): packages += [ i ]
    else: packages += [ packageList ]
    # print packages

    scriptname = "preparePackages.sh"
    os.system("mkdir -p mypackages")
    try:
        myScript = open(scriptname, "w")
        myScript.write("#!/bin/bash\n")
        myScript.write('cd mypackages\n')
        myScript.write('echo\n')
        myScript.write("echo \"setting up Athena... %s\"\n" % AthenaRelease)
        myScript.write("export AtlasSetup=/afs/cern.ch/atlas/software/dist/AtlasSetup\n")
        myScript.write("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase\n")
        myScript.write("source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh  --quiet\n")
        myScript.write("lsetup --quiet \"asetup %s\"\n" % AthenaRelease)
        myScript.write('\n')

        # compile requested packages
        runScript = False
        myTempDir = commands.getoutput('pwd')
        for i in packages:
            print '-> ' + i
            if not os.path.exists("mypackages/%s" % i):
                myScript.write('pkgco.py %s\n' % i)
                runScript = True
 
        myScript.close()
    except getopt.GetoptError, err:
        print "%sERROR: %s can not be opened :(%s" % (ERROR, scriptname, ENDC)
        sys.exit(1)

    if runScript: os.system("source %s/%s" % ( WorkingDir, scriptname))
    os.system("rm %s" % scriptname)

    print

# =================================================================================
#  __main__
# =================================================================================
if __name__ == '__main__':
  main(sys.argv[1:])
