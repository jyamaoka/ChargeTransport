#ifndef ChargeTransportActionInitialization_hh
#define ChargeTransportActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class ChargeTransportActionInitialization : public G4VUserActionInitialization {
public:
  ChargeTransportActionInitialization() = default;
  virtual ~ChargeTransportActionInitialization() = default;

  // Master thread: RunAction only (for ntuple merging)
  virtual void BuildForMaster() const;

  // Worker threads: all user actions
  virtual void Build() const;
};

#endif
