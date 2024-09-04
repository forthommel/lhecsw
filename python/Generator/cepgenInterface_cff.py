from Gaudi.Configuration import *
from Configurables import CepGenEventGenerator
from Configurables import HepMCToEDMConverter, SimG4PrimariesFromEdmTool


class CepGenParameters(dict):
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

    def __init__(self, *args, **kwargs):
        self.update(*args, **kwargs)
        super(CepGenParameters, self).__init__(*args, **kwargs)

    def __str__(self):
        from json import dumps
        return dumps(self)

class CepGenModule(CepGenParameters):
    def __init__(self, name, *args, **kwargs):
        super(CepGenModule, self).__init__(*args, **kwargs)
        self.mod_name = name

cepgen = CepGenEventGenerator("CepGenEventGenerator",
    process = '',
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
