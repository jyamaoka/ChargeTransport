#include "ChargeTransportPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4CMPDriftElectron.hh"
#include "G4CMPDriftHole.hh"

ChargeTransportPrimaryGeneratorAction::ChargeTransportPrimaryGeneratorAction()
  : fParticleGun(new G4ParticleGun(1))
{
  // default particle kinematics -- user may specify individual particle
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));
  fParticleGun->SetParticleEnergy(1*MeV);
  //fParticleGun->SetParticleEnergy(1e-6*eV);

}

ChargeTransportPrimaryGeneratorAction::~ChargeTransportPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void ChargeTransportPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // If user did not set particle explicitly, do e/h pairs
  if (fParticleGun->GetParticleDefinition() == G4Geantino::Definition()) {
    // Shoot a geantino to create e/h pairs in the detector
    fParticleGun->SetParticleDefinition(G4CMPDriftHole::Definition());
    fParticleGun->GeneratePrimaryVertex(event);
    fParticleGun->SetParticleDefinition(G4CMPDriftElectron::Definition());
    fParticleGun->GeneratePrimaryVertex(event);
    
    // Restore "not set" condition for next event
    fParticleGun->SetParticleDefinition(G4Geantino::Definition());
  } else {
    // Shoot the user-defined particle 
    fParticleGun->GeneratePrimaryVertex(event);
  } 
}
