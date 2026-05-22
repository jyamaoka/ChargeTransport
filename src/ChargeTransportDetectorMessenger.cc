#include "ChargeTransportDetectorMessenger.hh"
#include "ChargeTransportDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

ChargeTransportDetectorMessenger::ChargeTransportDetectorMessenger(ChargeTransportDetectorConstruction* det)
  : fDetector(det)
{
  fDetDir = new G4UIdirectory("/chargeTransport/detector/");
  fDetDir->SetGuidance("Si detector geometry commands");

  fSizeXCmd = new G4UIcmdWithADoubleAndUnit("/chargeTransport/detector/setSizeX", this);
  fSizeXCmd->SetGuidance("Set the full X dimension of the Si detector");
  fSizeXCmd->SetParameterName("SizeX", false);
  fSizeXCmd->SetRange("SizeX>0");
  fSizeXCmd->SetUnitCategory("Length");
  fSizeXCmd->SetDefaultUnit("cm");
  fSizeXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSizeYCmd = new G4UIcmdWithADoubleAndUnit("/chargeTransport/detector/setSizeY", this);
  fSizeYCmd->SetGuidance("Set the full Y dimension of the Si detector");
  fSizeYCmd->SetParameterName("SizeY", false);
  fSizeYCmd->SetRange("SizeY>0");
  fSizeYCmd->SetUnitCategory("Length");
  fSizeYCmd->SetDefaultUnit("cm");
  fSizeYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSizeZCmd = new G4UIcmdWithADoubleAndUnit("/chargeTransport/detector/setSizeZ", this);
  fSizeZCmd->SetGuidance("Set the full Z dimension of the Si detector");
  fSizeZCmd->SetParameterName("SizeZ", false);
  fSizeZCmd->SetRange("SizeZ>0");
  fSizeZCmd->SetUnitCategory("Length");
  fSizeZCmd->SetDefaultUnit("cm");
  fSizeZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

ChargeTransportDetectorMessenger::~ChargeTransportDetectorMessenger()
{
  delete fSizeXCmd;
  delete fSizeYCmd;
  delete fSizeZCmd;
  delete fDetDir;
}

void ChargeTransportDetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if (cmd == fSizeXCmd)
    fDetector->SetDetectorSizeX(fSizeXCmd->GetNewDoubleValue(val));
  else if (cmd == fSizeYCmd)
    fDetector->SetDetectorSizeY(fSizeYCmd->GetNewDoubleValue(val));
  else if (cmd == fSizeZCmd)
    fDetector->SetDetectorSizeZ(fSizeZCmd->GetNewDoubleValue(val));
}
