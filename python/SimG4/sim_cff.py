from Gaudi.Configuration import *
from Geometry.components_cff import tracker_components, calo_components, muon_components
from Configurables import SimG4Svc, SimG4Alg
from Configurables import SimG4FullSimActions, SimG4SaveTrackerHits, SimG4SaveCalHits


_actions = SimG4FullSimActions(
    enableHistory = True
)

geantservice = SimG4Svc("SimG4Svc",  # Geant4 simulation service
    detector = 'SimG4DD4hepDetector',
    physicslist = "SimG4FtfpBert",
    actions = _actions,
)

_outputs = []

# tracker components
for (name, readout) in tracker_components:
    simhits = SimG4SaveTrackerHits(name + 'SimHits', readoutName = readout)
    simhits.SimTrackHits.Path = name + 'SimHits'
    _outputs.append(simhits)

# calorimeter components
for (name, readout) in calo_components:
    simhits = SimG4SaveCalHits(name + 'SimHits', readoutName = readout)
    simhits.CaloHits.Path = name + 'SimHits'
    _outputs.append(simhits)

# muon chambers components
for (name, readout) in muon_components:
    simhits = SimG4SaveCalHits(name + 'SimHits', readoutName = readout)
    simhits.CaloHits.Path = name + 'SimHits'
    _outputs.append(simhits)

geantsim = SimG4Alg("SimG4Alg",
    outputs = _outputs,
    OutputLevel = INFO,
)
