#ifndef ChargeTransportDetectorMessenger_hh
#define ChargeTransportDetectorMessenger_hh 1

#include "G4UImessenger.hh"

class ChargeTransportDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

class ChargeTransportDetectorMessenger : public G4UImessenger {
public:
  ChargeTransportDetectorMessenger(ChargeTransportDetectorConstruction* det);
  virtual ~ChargeTransportDetectorMessenger();

  virtual void SetNewValue(G4UIcommand* cmd, G4String val) override;

private:
  ChargeTransportDetectorConstruction* fDetector;
  G4UIdirectory*               fDetDir;
  G4UIcmdWithADoubleAndUnit*   fSizeXCmd;
  G4UIcmdWithADoubleAndUnit*   fSizeYCmd;
  G4UIcmdWithADoubleAndUnit*   fSizeZCmd;
};

#endif
