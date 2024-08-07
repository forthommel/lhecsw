from Gaudi.Configuration import *
from Configurables import GenAlg
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr

from Generator.pythia8Interface_cff import *
from Geometry.geoservice_cfi import geoservice
from SimG4.sim_cff import geantservice, geantsim


pythia8.preInitCommands = [
    'Beams:idA = 2212',     # beam 1 = proton
    'Beams:idB = 11',       # beam 2 = electron
    'Beams:frameType = 2',  # beams are back-to-back, but with different energies
    'Beams:eA = 7000.',     # proton energy (GeV)
    'Beams:eB = 50.',       # electron energy (GeV)
    'PDF:lepton2gamma = on',
    'PhotonCollision:gmgm2mumu = on',
]

genalg = GenAlg("Pythia8",
    SignalProvider = pythia8,
)
genalg.hepmc.Path = "hepmc"

geantsim.eventProvider = pythia8Particles

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
        pythia8HepMCConverter,
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
