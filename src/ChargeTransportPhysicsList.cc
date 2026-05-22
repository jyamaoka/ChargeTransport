#include "ChargeTransportPhysicsList.hh"
#include "G4CMPPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

ChargeTransportPhysicsList::ChargeTransportPhysicsList(G4int ver)
  : G4VModularPhysicsList()
{
  SetVerboseLevel(ver);

  // Minimum production threshold — let G4CMP generate very-low-energy carriers
  defaultCutValue = DBL_MIN;

  // EM physics handles gamma interactions (Compton, photoelectric, pair prod)
  RegisterPhysics(new G4EmStandardPhysics(ver));
  RegisterPhysics(new G4EmExtraPhysics(ver));
  RegisterPhysics(new G4DecayPhysics(ver));

  // G4CMP: phonons + charge-carrier (e/h) drift and secondary production
  RegisterPhysics(new G4CMPPhysics);
}

ChargeTransportPhysicsList::~ChargeTransportPhysicsList() {}

void ChargeTransportPhysicsList::SetCuts()
{
  SetCutsWithDefault();
}
