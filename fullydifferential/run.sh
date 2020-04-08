# top
root -l -b -q 'ndimpdf.C++( 0, 0, 1, true)'
root -l -b -q 'ndimpdf.C++( 0, 0,-1, true)'
root -l -b -q 'ndimpdf.C++( 1, 0, 0, true)'
root -l -b -q 'ndimpdf.C++(-1, 0, 0, true)'
root -l -b -q 'ndimpdf.C++( 0, 1, 0, true)'
root -l -b -q 'ndimpdf.C++( 0,-1, 0, true)'
root -l -b -q 'ndimpdf.C++( 0, 0, 0.88, true)' # SM

# antitop
root -l -b -q 'ndimpdf.C++( 0, 0, 1, false)'
root -l -b -q 'ndimpdf.C++( 0, 0,-1, false)'
root -l -b -q 'ndimpdf.C++( 1, 0, 0, false)'
root -l -b -q 'ndimpdf.C++(-1, 0, 0, false)'
root -l -b -q 'ndimpdf.C++( 0, 1, 0, false)'
root -l -b -q 'ndimpdf.C++( 0,-1, 0, false)'
root -l -b -q 'ndimpdf.C++(-0.11, 0, -0.85, false)' # SM

