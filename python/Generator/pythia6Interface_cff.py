from Gaudi.Configuration import *
from Configurables import Pythia6EventGenerator
from Configurables import HepMCToEDMConverter, SimG4PrimariesFromEdmTool


pythia6 = Pythia6EventGenerator("Pythia6EventGenerator",
    preInitCommands = [],
    OutputLevel = INFO,
)

# HepMC -> EDM format
pythia6HepMCConverter = HepMCToEDMConverter("Converter",
    #hepmcStatusList = [1],  # all statuses particles are converted
    # does not seem to be implemented in the current HepMCToEDMConverter plugin
    #OutputLevel = VERBOSE,
)
pythia6HepMCConverter.hepmc.Path = "hepmc"
pythia6HepMCConverter.GenParticles.Path = 'genParticles'

# EDM format -> Geant4 input
pythia6Particles = SimG4PrimariesFromEdmTool("EdmConverter")
pythia6Particles.GenParticles.Path = 'genParticles'
