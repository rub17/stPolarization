
#!/usr/bin/python

import os, sys
WARNING = '\033[91m'
BLUE = '\033[94m'
BOLD = '\033[1m'
ENDC = '\033[0m'

fileName = raw_input("Name of the file to change: \n")
#print fileName
newfileName = raw_input("Rename it to: \n")
#print newfileName
iBegin = raw_input("Type the starting index: \n")
#print iBegin

numberFile = len([file for file in os.listdir('.') if file.startswith(fileName)])
numberNewFile = len([file for file in os.listdir('.') if file.startswith(newfileName)])

#print numberFile
#print numberNewFile
currentFile = fileName + "._%05d.tar.gz" %(int(iBegin))
iNew = numberNewFile + 1
newFile = newfileName + "._%05d.tar.gz" %(iNew)
cmdString =  str("mv " + currentFile + " " + newFile)
print (BOLD + "This is the command about to be executed. Proceed?" + ENDC)
print (WARNING + cmdString + ENDC)
contFlag = raw_input(BOLD + "Yes/No:" + ENDC)

if (contFlag == "Yes"):
    if (os.path.isfile(currentFile)):
        for i in range(0, numberFile):
            workingDir = os.environ['PWD']
            currentFile = fileName + "._%05d.tar.gz" %(int(iBegin) + i)
            iNew = i + numberNewFile + 1
            newFile = newfileName + "._%05d.tar.gz" %(iNew)
            cmdString =  str("mv " + currentFile + " " + newFile)
            #print os.path.isfile(newFile)
            #print cmdString
            os.system(cmdString)
            print (BLUE + currentFile + " -> " + newFile + ENDC)
    else:
        print (BOLD + "No such file is found." +ENDC)
else:
    print ("Bye.")
