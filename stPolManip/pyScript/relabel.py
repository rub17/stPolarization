
#!/usr/bin/python

import os, sys
WARNING = '\033[91m'
BLUE = '\033[94m'
BOLD = '\033[1m'
ENDC = '\033[0m'

fileName = raw_input("Name of the file to change: \n")
iBegin = raw_input("Type the starting index: \n")
numberFile = len([file for file in os.listdir('.') if file.startswith(fileName)])
currentFile = fileName + "._%05d.tar.gz" %(int(iBegin))
iNew = int(iBegin) + 1000
newFile = fileName + "._%05d.tar.gz" %(iNew)
cmdString =  str("mv " + currentFile + " " + newFile)
print (BOLD + BLUE + "There are " + str(numberFile) + " files in this directory. " + ENDC)
print (BOLD + "This is the command about to be executed. Proceed?" + ENDC)
print (WARNING + cmdString + ENDC)
contFlag = raw_input(BOLD + "Yes/No:" + ENDC)

if (contFlag == "Yes"):
    if (os.path.isfile(currentFile)):
        for i in range(0, numberFile):
            workingDir = os.environ['PWD']
            currentFile = fileName + "._%05d.tar.gz" %(int(iBegin) + i)
            currentEvents = fileName + "._%05d.events" %(int(iBegin) + i)
            currentDat = fileName + "._%05d.dat" %(int(iBegin) + i)
            if (os.path.isfile(currentFile)==False):
                print (WARNING + currentFile + " does not exist! Exit..." + ENDC)
                break
            newFile = fileName + "._%05d.tar.gz" %(iNew)
            newEvents = fileName + "._%05d.events" %(iNew)
            newDat = fileName + "._%05d.dat" %(iNew)

            os.system('mv %s ./tmp/' %currentFile)
            os.chdir('./tmp/')
            #print ('tar -xf %s' %currentFile)
            os.system('tar -xf %s' %currentFile)
            os.system("rm %s" %currentFile)
            os.system("mv " + currentEvents + " " + newEvents)
            os.system("mv " + currentDat + " " + newDat)
            os.system("ln -fs " + newEvents + " protos.events")
            os.system("ln -fs " + newDat + " protos.dat")
            os.system("tar czvf %s *" %newFile)
            os.system("mv %s .." %newFile)
            os.system("rm *.events *.dat")
            os.chdir(workingDir)
            #print os.path.isfile(newFile)
            #print cmdString
            #os.system(cmdString)
            print (BLUE + currentFile + " -> " + newFile + ENDC)
            iNew += 1
    else:
        print (BOLD + "No such file is found." +ENDC)
else:
    print ("Bye.")
