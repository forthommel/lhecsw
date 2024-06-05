# LHeC offline software

This library, built on top of Geant4/DD4hep, defines the geometry, digitisation, and reconstruction data formats for a generic LHeC detector definition.

## Building and running

First ensure that you have CMake, Geant4, and DD4hep installed in your local environment.
In case DD4hep is installed outside the "common" (local, or system-wide) area, define the `DD4hep_DIR` environment variable before running the usual CMake set of commands.
This latter may be, for instance:

```{bash}
cd /path/to/build/dir
cmake [-GNinja] [...] -B /path/to/sources
cmake --build /path/to/sources
```
This will generate, in your build directory, a set of shared libraries (all corresponding to one module of this toolset), and a global shared library defining all wrapper definitions to the DD4hep library.
You may then source the "typical" DD4hep environment preparation script:

```{bash}
source bin/thislhecsw.sh
```
A few tests are currently being prepared for the simulation of a set of initial conditions:
- simple particle gun ;
- Pythia 8 minimum bias beams definition ;
- BDSIM simulation of synchrotron radiation from the electron beam ;
- etc.
