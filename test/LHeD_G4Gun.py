from Gaudi.Configuration import *
from Configurables import GenAlg
from Configurables import SimG4SingleParticleGeneratorTool
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr

from Geometry.geoservice_cfi import geoservice
from SimG4.sim_cff import geantservice, geantsim
from SimAlgos.digi_cff import digis


pgun = SimG4SingleParticleGeneratorTool("SimG4SingleParticleGeneratorTool",
    saveEdm = True,
    particleName = "e-",
    energyMin = 50,
    energyMax = 50,
    etaMin = 0,
    etaMax = 0,
    OutputLevel = INFO,
)

genalg = GenAlg()
genalg.hepmc.Path = "hepmc"

geantsim.eventProvider = pgun

out = PodioOutput("out",  # PODIO output algorithm
    outputCommands = [
        "keep *"
    ],
    filename = "output.root",
    OutputLevel = INFO,
)

podioevent = FCCDataSvc("EventDataSvc")

# wrap everything together
ApplicationMgr(
    TopAlg = [
        genalg,
        geantsim,
        *digis,
        out
    ],
    EvtSel = 'NONE',
    EvtMax   = 5000,
    ExtSvc = [  # order is important, as GeoSvc is needed by G4SimSvc
        podioevent,
        geoservice,
        geantservice,
        #audsvc,
    ],
    #OutputLevel = DEBUG,
    #OutputLevel = VERBOSE,
    OutputLevel = INFO,
)
