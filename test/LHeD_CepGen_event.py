from Gaudi.Configuration import *
from Configurables import GenAlg
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr

from Generator.cepgenInterface_cff import *
from Geometry.geoservice_cfi import geoservice
from SimG4.sim_cff import geantservice, geantsim
from SimAlgos.digi_cff import digis


cepgen.process = [
    'name:lpair',
    'kinematics/beam1id:2212',
    'kinematics/beam2id:11',
    'kinematics/beam1pz:7000.0',
    'kinematics/beam2pz:50.0',
    'kinematics/ptmin:10.0',
]

genalg = GenAlg("CepGen",
    SignalProvider = cepgen,
)
genalg.hepmc.Path = "hepmc"

geantsim.eventProvider = cepgenParticles

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
        cepgenHepMCConverter,
        geantsim,
        *digis,
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
