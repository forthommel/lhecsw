from Gaudi.Configuration import *
from Configurables import GeoSvc
from Configurables import GenAlg, HepMCToEDMConverter, SimG4PrimariesFromEdmTool
from Configurables import SimG4Svc, SimG4Alg, SimG4FullSimActions, SimG4SaveTrackerHits
from Configurables import SimG4SingleParticleGeneratorTool
from Configurables import CepGenEventGenerator
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr
from Generator.pythia8Interface_cff import *


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
    eventProvider = pythia8Particles,
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
