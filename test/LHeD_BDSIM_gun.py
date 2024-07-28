from Gaudi.Configuration import *
from Configurables import GenAlg
from Configurables import SimG4Svc, SimG4Alg, SimG4FullSimActions
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr

from Generator.bdsimInterface_cff import *
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

bdsim.filename = '/eos/project-l/lhec/public/examples/BDSIM_10evts_lstar23.root'
bdsim.scorerName = 'BEND_0'

genalg = GenAlg("bdsim",
    SignalProvider = bdsim,
)
genalg.hepmc.Path = "hepmc"

geantsim = SimG4Alg("SimG4Alg",
    outputs = g4outputs,
    eventProvider = bdsimParticles,
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
        bdsimHepMCConverter,
        geantsim,
        out
    ],
    EvtSel = 'NONE',
    EvtMax   = 10_000,
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
