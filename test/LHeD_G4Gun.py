from Gaudi.Configuration import *
from Configurables import GenAlg
from Configurables import SimG4Svc, SimG4Alg, SimG4FullSimActions
from Configurables import SimG4SingleParticleGeneratorTool
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr

from Geometry.geoservice_cfi import geoservice
from SimG4.sim_cff import g4outputs


actions = SimG4FullSimActions(
    enableHistory = True
)

geantservice = SimG4Svc("SimG4Svc",  # Geant4 simulation service
    detector = 'SimG4DD4hepDetector',
    physicslist = "SimG4FtfpBert",
    actions = actions,
)

pgun = SimG4SingleParticleGeneratorTool("SimG4SingleParticleGeneratorTool",
    saveEdm = True,
    particleName = "e-",
    energyMin = 50,
    energyMax = 50,
    etaMin = 0,
    etaMax = 0,
    OutputLevel = DEBUG,
)

genalg = GenAlg()
genalg.hepmc.Path = "hepmc"

geantsim = SimG4Alg("SimG4Alg",
    outputs = g4outputs,
    eventProvider = pgun,
    OutputLevel = DEBUG,
)

out = PodioOutput("out",  # PODIO output algorithm
    outputCommands = [
        "keep *"
    ],
    filename = "output.root",
    OutputLevel = DEBUG,
)

podioevent = FCCDataSvc("EventDataSvc")

# wrap everything together
ApplicationMgr(
    TopAlg = [
        genalg,
        geantsim,
        out
    ],
    EvtSel = 'NONE',
    EvtMax   = 100,
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
