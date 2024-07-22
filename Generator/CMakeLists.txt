find_package(HepMC3)

add_subdirectory(BDSIMInterface)
add_subdirectory(CepGenInterface)
add_subdirectory(Common)
add_subdirectory(Pythia8Interface)
set(lhecswlibs ${lhecswlibs} PARENT_SCOPE)
