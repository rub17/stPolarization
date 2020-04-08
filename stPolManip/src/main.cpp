//
//  main.cpp
//  
//
//  Created by Runyu on 1/24/18.
//
//  Working Version 1.0 : Tagged 2018/04/10

#include <stdio.h>
#include "stPolManip.h"

int main(int argc, char **argv)
{
    std::string usage = std::string("Usage: ")
    + argv[0]
    + " [-?] [-f inputFile] [-px polarization] [-py polarization] [-pz polarization] [-g generation] [-n generation#]\n"
    + "polarization examples: -px 0 -py 0 -pz 1\n"
    + "generation options: -1,0,1,2,3 stand for \"SM decay only\", \"no generation\", \"decay only\", \"production and decay\"\n"
    + "generation#: 5000, 10000\n\n";

    if (argc>1 && argv[1]==std::string("-?")) {
        std::cout << usage << std::endl;
        exit (0);
    }
    std::vector<double> pol = {0.,0.,0.};
    int gen = 0;
    int genNum = 5000;
    std::string filePath("/Users/runyu/Documents/Git/CERN/stPolarization/Events/protos00.events");
    try {
        // overwritten by command line:
        for (int i=1; i<argc;i+=2) {
            std::istringstream stream(argv[i+1]);
            if (std::string(argv[i])=="-f") stream >> filePath;
            if (std::string(argv[i])=="-px") stream >> pol[0];
            if (std::string(argv[i])=="-py") stream >> pol[1];
            if (std::string(argv[i])=="-pz") stream >> pol[2];
            if (std::string(argv[i])=="-g") stream >> gen;
            if (std::string(argv[i])=="-n") stream >> genNum;
            //            if (pol!="+Z"||"-Z"||"+X"||"-X"||"+Y"||"-Y") {
            //                std::cout << "Polarization not recongized" << std::endl;
            //                exit(0);
            //            }
        }
    }
    catch (std::exception &) {
        std::cout << usage << std::endl;
        exit (0);
    }
    if (filePath == "") {
        std::cout << "Need an input file!" << std::endl;
        exit (0);
    }
    
    std::clock_t begin = clock();
    stPolManip a(filePath,gen,genNum,pol[0],pol[1],pol[2]);
    a.ReadProtos();
    for (unsigned int i=0; i < a.sampleSize() - 1; i++) { //The vector size is 1 bigger; suspect if the blank line in the EOF caused the problem. Quick fix with -1 for now.
        a.RecordProtos(i);
        a.compute();
        a.WriteIn();
        a.OutputProtos();
        if(i%1000==0){
            std::cout << std::fixed << "\r Generated:   [ "
            << i << " ] Events" << std::flush;
        }
    }
    std::cout << std::endl;
    a.tree->Print();
    std::cout << "Generated and collected " << a.sampleSize() - 1 << " events!" << std::endl;
    a.hfile.Write();
    a.hfile.Close();
    std::clock_t end = clock();
    double elapsed_secs = double(end-begin)/CLOCKS_PER_SEC;
    std::cout << "Generation in total took " << elapsed_secs << " seconds." << std::endl;
    return 0;
}
