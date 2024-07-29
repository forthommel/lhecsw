from Gaudi.Configuration import *
from Geometry.components_cff import tracker_vertex_components, tracker_outer_barrel_components
#from Geometry.components_cff import calo_components, muon_components
from Configurables import TrackHitDigitiser, GaussianResolutionTrackHitDigitiser


digis = []

# tracker components
for (name, readout) in [*tracker_vertex_components, *tracker_outer_barrel_components]:
    _trackerGaussianResolution = GaussianResolutionTrackHitDigitiser(name + 'HitsDigitiser',
        readoutName = readout,
        barrel = True,
        OutputLevel = INFO,
    )
    digi = TrackHitDigitiser(name + 'Hits',
        simhits = name + 'SimHits',
        algorithm = _trackerGaussianResolution,
    )
    digis.append(digi)

# calorimeter components
#for (name, readout) in calo_components:

# muon chambers components
#for (name, readout) in muon_components:
