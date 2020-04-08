# Usage
**Dependency: root-cern CMake**

## Analysis of PROTOS Outputs
The main program is in the stPolManip package, and it includes all the legacy code from the other folders.
To be able to run it with CMake:
```
cd ./stPolManip/
mkdir build
cd build
cmake ../
make -j 8
./stPolarization -f [inputFile] -n [Number of generation for each input event] -px [px polarization] -py [py polarization] -pz [pz polarization] -g [generation on/off]
```
The event file from PROTOS is located in cern box. Can be found with the following links:

[Weighted Events](https://cernbox.cern.ch/index.php/s/IY8jN6D7ghkXBIL)

[Unweighted Events](https://cernbox.cern.ch/index.php/s/moMNaTjrP4bPDyA)

The program generates a root file in the end, with all the information stored in a TTree.

Moreover, if a protos-like txt file is desired, one can move forward to /pyScript/ folder and run
```
python rootTotxt.py -f [file]
```
and It will output a text file in the end called "fromRoot.events"


