from Gaudi.Configuration import *
from SimAlgos.common_cff import tracker_components, calo_components, muon_components
from Configurables import SimG4SaveTrackerHits, SimG4SaveCalHits


g4outputs = []

# tracker components
for (name, readout) in tracker_components:
    simhits = SimG4SaveTrackerHits(name + 'SimHits', readoutName = readout)
    simhits.SimTrackHits.Path = name + 'SimHits'
    g4outputs.append(simhits)

# calorimeter components
for (name, readout) in calo_components:
    simhits = SimG4SaveCalHits(name + 'SimHits', readoutName = readout)
    simhits.CaloHits.Path = name + 'SimHits'
    g4outputs.append(simhits)

# muon chambers components
for (name, readout) in muon_components:
    simhits = SimG4SaveCalHits(name + 'SimHits', readoutName = readout)
    simhits.CaloHits.Path = name + 'SimHits'
    g4outputs.append(simhits)
