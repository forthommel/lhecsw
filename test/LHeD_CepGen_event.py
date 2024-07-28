from Gaudi.Configuration import *
from Configurables import GeoSvc, GenAlg
from Configurables import SimG4Svc, SimG4Alg, SimG4FullSimActions
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr
from Generator.cepgenInterface_cff import *
from SimG4.sim_cff import g4outputs
from SimAlgos.digi_cff import digis


geoservice = GeoSvc("GeoSvc",  # DD4hep geometry service
    detectors = [
        'file:${lhecsw}/Geometry/data/compact/LHeD.xml',
    ],
    OutputLevel = INFO,
)

actions = SimG4FullSimActions(
    enableHistory = True
)

geantservice = SimG4Svc("SimG4Svc",  # Geant4 simulation service
    detector = 'SimG4DD4hepDetector',
    physicslist = "SimG4FtfpBert",
    actions = actions,
)

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

geantsim = SimG4Alg("SimG4Alg",
    outputs = g4outputs,
    eventProvider = cepgenParticles,
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
