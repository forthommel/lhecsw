from Gaudi.Configuration import *
from Configurables import GenAlg
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr

from Generator.pythia6Interface_cff import *
from Geometry.geoservice_cfi import geoservice
from SimG4.sim_cff import geantservice, geantsim


pythia6.preInitCommands = [
    'P(1,3) = 7000d0',  # proton beam
    'P(2,3) = -50d0',   # electron target
    'MSEL = 0',
    'MSTI(19) = 1',
    'MSTP(11) = 1',     # photon-inside-electron
    'MSTP(12) = 1',
    'MSTP(14) = 30',
    'MSTP(51) = 10100', # CTEQ6.1 structure functions
    'MSTP(52) = 2',
    'MSUB(58) = 1',     # gamma gamma -> l+ l-
    #'CKIN(3) = 50d0',
]

genalg = GenAlg("Pythia6",
    SignalProvider = pythia6,
)
genalg.hepmc.Path = "hepmc"

geantsim.eventProvider = pythia6Particles

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
        pythia6HepMCConverter,
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
