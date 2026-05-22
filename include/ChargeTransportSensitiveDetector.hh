#ifndef ChargeTransportSensitiveDetector_hh
#define ChargeTransportSensitiveDetector_hh 1

#include "G4CMPElectrodeSensitivity.hh"
#include "G4String.hh"

class ChargeTransportSensitiveDetector : public G4CMPElectrodeSensitivity {
public:
  explicit ChargeTransportSensitiveDetector(const G4String& name);
  virtual ~ChargeTransportSensitiveDetector() = default;

  virtual void EndOfEvent(G4HCofThisEvent* HCE);

protected:
  virtual G4bool IsHit(const G4Step*, const G4TouchableHistory*) const;
};

#endif
