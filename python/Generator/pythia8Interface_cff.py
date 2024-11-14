from Gaudi.Configuration import *
try:
    from Configurables import Pythia8EventGenerator
except ImportError:
    raise ImportError('Pythia 8 interface was not found in search path. Was Pythia linked to this installation?')
from Configurables import HepMCToEDMConverter, SimG4PrimariesFromEdmTool


pythia8 = Pythia8EventGenerator("Pythia8EventGenerator",
    preInitCommands = [],
    OutputLevel = INFO,
)

# HepMC -> EDM format
pythia8HepMCConverter = HepMCToEDMConverter("Converter",
    #hepmcStatusList = [1],  # all statuses particles are converted
    # does not seem to be implemented in the current HepMCToEDMConverter plugin
    #OutputLevel = VERBOSE,
)
pythia8HepMCConverter.hepmc.Path = "hepmc"
pythia8HepMCConverter.GenParticles.Path = 'genParticles'

# EDM format -> Geant4 input
pythia8Particles = SimG4PrimariesFromEdmTool("EdmConverter")
pythia8Particles.GenParticles.Path = 'genParticles'
