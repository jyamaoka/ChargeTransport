#ifndef ChargeTransportPhysicsList_hh
#define ChargeTransportPhysicsList_hh 1

#include "G4VModularPhysicsList.hh"

class ChargeTransportPhysicsList : public G4VModularPhysicsList {
public:
  explicit ChargeTransportPhysicsList(G4int verbosity = 0);
  virtual ~ChargeTransportPhysicsList();
  virtual void SetCuts();
};

#endif
