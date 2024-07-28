from Gaudi.Configuration import *
from SimAlgos.common_cff import tracker_components, calo_components, muon_components
from Configurables import TrackHitDigitiser


digis = []

# tracker components
for (name, readout) in tracker_components:
    digi = TrackHitDigitiser(name + 'Hits',
        simhits = name + 'SimHits',
    )
    digis.append(digi)

# calorimeter components
#for (name, readout) in calo_components:

# muon chambers components
#for (name, readout) in muon_components:
