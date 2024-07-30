from Gaudi.Configuration import *
from Geometry.components_cff import tracker_vertex_components, tracker_outer_barrel_components
from Geometry.components_cff import calo_ecal_barrel_components, calo_hcal_barrel_components
#from Geometry.components_cff import calo_components, muon_components
from Configurables import TrackHitDigitiser, GaussianResolutionTrackHitDigitiser
from Configurables import CaloHitDigitiser, GaussianResolutionCaloHitDigitiser


digis = []

# vertex barrel
for (name, readout) in tracker_vertex_components:
    _pixelDigitiser = GaussianResolutionTrackHitDigitiser(name + 'HitsDigitiser',
        readoutName = readout,
        isBarrel = True,
        # resolutions taken from LHeC CDR#1 (arXiv:1206.2913 [hep-ex], p. 514)
        uRes = 0.008,  # in mm
        vRes = 0.008,  # in mm
        OutputLevel = INFO,
    )
    digi = TrackHitDigitiser(name + 'Hits',
        simhits = name + 'SimHits',
        algorithm = _pixelDigitiser,
    )
    digis.append(digi)

# tracker components
for (name, readout) in tracker_outer_barrel_components:
    _trackerDigitiser = GaussianResolutionTrackHitDigitiser(name + 'HitsDigitiser',
        readoutName = readout,
        isBarrel = True,
        # resolutions taken from LHeC CDR#1 (arXiv:1206.2913 [hep-ex], p. 514)
        uRes = 0.015,  # in mm
        vRes = 0.015,  # in mm
        OutputLevel = INFO,
    )
    digi = TrackHitDigitiser(name + 'Hits',
        simhits = name + 'SimHits',
        algorithm = _trackerDigitiser,
    )
    digis.append(digi)

# calorimeter components
for (name, readout) in calo_ecal_barrel_components:
    _ecalDigitiser = GaussianResolutionCaloHitDigitiser(name + 'HitsDigitiser',
        readoutName = readout,
        isBarrel = True,
        # resolutions taken from LHeC CDR#1 (arXiv:1206.2913 [hep-ex], p. 539) for LAr
        aTerm = 0.14,
        bTerm = 0.053,
    )
    digi = CaloHitDigitiser(name + 'HitsDigitiser',
        simhits = name + 'SimHits',
        algorithm = _ecalDigitiser,
    )
    digis.append(digi)

for (name, readout) in calo_hcal_barrel_components:
    _hcalDigitiser = GaussianResolutionCaloHitDigitiser(name + 'HitsDigitiser',
        readoutName = readout,
        isBarrel = True,
        # resolutions taken from LHeC CDR#1 (arXiv:1206.2913 [hep-ex], p. 539) for LAr
        aTerm = 0.453,
        bTerm = 0.048,
    )
    digi = CaloHitDigitiser(name + 'HitsDigitiser',
        simhits = name + 'SimHits',
        algorithm = _hcalDigitiser,
    )
    digis.append(digi)
#for (name, readout) in calo_components:

# muon chambers components
#for (name, readout) in muon_components:
