from Gaudi.Configuration import *
from Configurables import CepGenEventGenerator
from Configurables import HepMCToEDMConverter, SimG4PrimariesFromEdmTool


cepgen = CepGenEventGenerator("CepGenEventGenerator",
    process = [],
    OutputLevel = INFO,
)

# HepMC -> EDM format
cepgenHepMCConverter = HepMCToEDMConverter("Converter",
    #hepmcStatusList = [1],  # all statuses particles are converted
    # does not seem to be implemented in the current HepMCToEDMConverter plugin
    #OutputLevel = VERBOSE,
)
cepgenHepMCConverter.hepmc.Path = "hepmc"
cepgenHepMCConverter.GenParticles.Path = 'genParticles'

# EDM format -> Geant4 input
cepgenParticles = SimG4PrimariesFromEdmTool("EdmConverter")
cepgenParticles.GenParticles.Path = 'genParticles'
