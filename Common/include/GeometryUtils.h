#ifndef Common_GeometryUtils_h
#define Common_GeometryUtils_h

namespace dd4hep {
  class DetElement;
  class Volume;
}  // namespace dd4hep

namespace utils {
  /// calculate path in a given theta
  double computeDpt(double ra, double rb, double theta);
  void placeStaves(dd4hep::DetElement& parent,
                   const dd4hep::DetElement& stave,
                   double rmin,
                   int numsides,
                   double total_thickness,
                   dd4hep::Volume& envelopeVolume,
                   double innerAngle,
                   const dd4hep::Volume& sectVolume);
}  // namespace utils

#endif
