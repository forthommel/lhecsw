from Gaudi.Configuration import *
from Configurables import SimG4Alg, SimG4SaveTrackerHits, SimG4SaveCalHits


g4outputs = []

# tracker components
for (name, readout) in [
        ("vertexBarrelHits", "SiVertexBarrelHits"),
        ("vertexOuterBarrelHits", "SiVertexBarrel2Hits"),
        ("trackerBarrelHits", "SiTrackerBarrelHits"),
        ("trackerOuterBarrelHits", "SiTrackerOBarrelHits"),
        ("trackerForwardHits", "SiTrackerForwardHits"),
        ("trackerBackwardHits", "SiTrackerBackwardHits"),
    ]:
    tmp = SimG4SaveTrackerHits(name, readoutName = readout)
    tmp.SimTrackHits.Path = name
    g4outputs.append(tmp)

# calorimeter components
for (name, readout) in [
        ("ecalBarrel1Hits", "EcalBarrelHits1"),
        ("ecalBarrel2Hits", "EcalBarrelHits2"),
        ("ecalBarrel3Hits", "EcalBarrelHits3"),
        ("ecalPlug0Hits", "EcalPlugHits0"),
        ("ecalPlug1Hits", "EcalPlugHits1"),
        ("hcalBarrelHits", "HcalBarrelHits"),
        ("hcalEndcap0Hits", "HcalEndcapHits0"),
        ("hcalEndcap1Hits", "HcalEndcapHits1"),
        ("hcalPlug01Hits", "HcalPlugHits01"),
        ("hcalPlug11Hits", "HcalPlugHits11"),
    ]:
    tmp = SimG4SaveCalHits(name, readoutName = readout)
    tmp.CaloHits.Path = name
    g4outputs.append(tmp)

# muon chambers components
for (name, readout) in [
        ("muonBarrelHits", "MuonBarrelHits"),
        ("muonEndcap01Hits", "MuonEndcapHits01"),
        ("muonEndcap02Hits", "MuonEndcapHits02"),
        ("muonEndcap11Hits", "MuonEndcapHits11"),
        ("muonEndcap12Hits", "MuonEndcapHits12"),
    ]:
    tmp = SimG4SaveCalHits(name, readoutName = readout)
    tmp.CaloHits.Path = name
    g4outputs.append(tmp)
