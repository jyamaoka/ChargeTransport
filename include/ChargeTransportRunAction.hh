#ifndef ChargeTransportRunAction_hh
#define ChargeTransportRunAction_hh 1

#include "G4UserRunAction.hh"
#include "G4String.hh"

class G4Run;

class ChargeTransportRunAction : public G4UserRunAction {
public:
  ChargeTransportRunAction();
  virtual ~ChargeTransportRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  void SetFilename(const G4String& name) { fFileName = name; }

private:
  G4String fFileName;
};

#endif
