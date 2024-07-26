from Gaudi.Configuration import *
from Configurables import BDSIMParticleGun
from Configurables import HepMCToEDMConverter, SimG4PrimariesFromEdmTool


bdsim = BDSIMParticleGun("BDSIMParticleGun",
    OutputLevel = INFO,
)

# HepMC -> EDM format
bdsimHepMCConverter = HepMCToEDMConverter("Converter",
    #hepmcStatusList = [1],  # all statuses particles are converted
    # does not seem to be implemented in the current HepMCToEDMConverter plugin
    #OutputLevel = VERBOSE,
)
bdsimHepMCConverter.hepmc.Path = "hepmc"
bdsimHepMCConverter.GenParticles.Path = 'genParticles'

# EDM format -> Geant4 input
bdsimParticles = SimG4PrimariesFromEdmTool("EdmConverter")
bdsimParticles.GenParticles.Path = 'genParticles'
