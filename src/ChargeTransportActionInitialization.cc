#include "ChargeTransportActionInitialization.hh"
#include "ChargeTransportPrimaryGeneratorAction.hh"
#include "ChargeTransportRunAction.hh"
#include "G4CMPStackingAction.hh"

void ChargeTransportActionInitialization::BuildForMaster() const
{
  // Master thread only needs RunAction to handle ntuple merging
  SetUserAction(new ChargeTransportRunAction);
}

void ChargeTransportActionInitialization::Build() const
{
  SetUserAction(new ChargeTransportPrimaryGeneratorAction);
  SetUserAction(new ChargeTransportRunAction);
  SetUserAction(new G4CMPStackingAction);
}
