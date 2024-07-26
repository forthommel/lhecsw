from Gaudi.Configuration import *
from Configurables import GeoSvc
from Configurables import GenAlg, HepMCToEDMConverter, SimG4PrimariesFromEdmTool
from Configurables import SimG4Svc, SimG4Alg, SimG4FullSimActions, SimG4SaveTrackerHits
from Configurables import SimG4SingleParticleGeneratorTool
from Configurables import CepGenEventGenerator
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr
from Generator.cepgenInterface_cff import *


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


outputs = []
for (name, readout) in [
        ("vertexBarrelHits", "SiVertexBarrelHits"),
        ("vertexOuterBarrelHits", "SiVertexBarrel2Hits"),
        ("trackerBarrelHits", "SiTrackerBarrelHits"),
        ("trackerOuterBarrelHits", "SiTrackerOBarrelHits"),
        ("trackerForwardHits", "SiTrackerForwardHits"),
        ("trackerBackwardHits","SiTrackerBackwardHits"),
    ]:
    tmp = SimG4SaveTrackerHits(name, readoutName = readout)
    tmp.SimTrackHits.Path = name
    outputs.append(tmp)

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

pgun = SimG4SingleParticleGeneratorTool("SimG4SingleParticleGeneratorTool",
    saveEdm = True,
    particleName = "e-",
    energyMin = 50,
    energyMax = 50,
    etaMin = 0,
    etaMax = 0,
    OutputLevel = DEBUG,
)

geantsim = SimG4Alg("SimG4Alg",
    outputs = outputs,
    #eventProvider = pgun,
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
