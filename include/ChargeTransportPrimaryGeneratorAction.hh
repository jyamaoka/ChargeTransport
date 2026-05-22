#ifndef ChargeTransportPrimaryGeneratorAction_hh
#define ChargeTransportPrimaryGeneratorAction_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class ChargeTransportPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  ChargeTransportPrimaryGeneratorAction();
  virtual ~ChargeTransportPrimaryGeneratorAction();
  virtual void GeneratePrimaries(G4Event* event);
// method to access particle gun
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
private:
  G4ParticleGun* fParticleGun;
};

#endif
