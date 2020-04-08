#!/usr/bin/python

import os, sys, subprocess
import time
import glob

WARNING = '\033[91m'
BLUE = '\033[94m'
BOLD = '\033[1m'
ENDC = '\033[0m'

def main():
    #isFit = os.path.exists("./polFit_recolevel_v29star_2ASR/Fits/")
    #if isFit:
    #    print("There exists a Fits/ folder already! Please remove.")
    #    sys.exit()
    #runFit = subprocess.call("trex-fitter wf ./config/polFit_recolevel_v29star_2ASR.config",shell=True)
    #subCondor = subprocess.call('condor_submit job_Condor.sub',shell=True)
    logfiles = glob.glob('./condoroutputs/log/*.log')
    latestLog = max(logfiles, key=os.path.getctime)
    print(BOLD + "Found the last created log file: "+latestLog + ENDC)
    checkCndr = subprocess.Popen("condor_wait "+latestLog, shell=True, stdout=subprocess.PIPE)
    print(BLUE + "Now waiting for the condor jobs to finish...")
    poll = checkCndr.poll()
    while poll == None:
        print("...")
        time.sleep(10)
    #checkCndr.wait()
    print("DONE." +ENDC)
    pltRnk = subprocess.call('trex-fitter r ./config/polFit_recolevel_v29star_2ASR.config Ranking=plot', shell=True)
    NPFile = open('./polFit_recolevel_v29star_2ASR/Fits/NPRanking.txt','r')
    NPs = NPFile.readlines()
    with open('./polFit_recolevel_v29star_2ASR/Fits/NPRanking.txt','w') as f:
        #seenSet = set()
        seenList = list()
        for line in NPs:
            syst = line.split(" ")[0]
            if syst not in seenList:
                if ("Px" not in line) and ("Py" not in line) and ("Pz" not in line) and ('ttbar_NORM' not in line) and ('Wjets_NORM' not in line) and ('tchan_NORM' not in line):
                    seenList.append(syst)
                    f.write(line)
    pltNewRnk = subprocess.call('trex-fitter r ./config/polFit_recolevel_v29star_2ASR.config Ranking=plot', shell=True)
    

if __name__ == "__main__":
    main()



