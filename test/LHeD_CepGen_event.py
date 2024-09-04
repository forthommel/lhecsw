from Gaudi.Configuration import *
from Configurables import GenAlg
from Configurables import PodioOutput, FCCDataSvc
from Configurables import ApplicationMgr

from Generator.cepgenInterface_cff import *
from Geometry.geoservice_cfi import geoservice
from SimG4.sim_cff import geantservice, geantsim
from SimAlgos.digi_cff import digis


cepgen.process = str(CepGenModule('lpair',
    kinematics = CepGenParameters(
        beam1id = 2212,
        beam2id = 11,
        beam1pz = 7000.,
        beam2pz = 50.,
        ptmin = 20.,
    )
))
#cepgen.modifiers = str(CepGenParameters(
#    pythia6 = CepGenParameters(),
#))
#cepgen.outputs = str(CepGenParameters(
#    dump = CepGenParameters(
#        printEvery = 10,
#    ),
#))

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
