/// charge_transport.cc
/// G4CMP simulation: 660 keV gammas in a 1x1x1 cm silicon cube.
/// Electron-hole pairs produced by EM ionization are drifted under a uniform
/// electric field and recorded in a ROOT TTree (gamma_si_ehpairs.root).
///
/// Usage:
///   ./charge_transport            — interactive mode
///   ./charge_transport run.mac    — batch mode

//#ifdef G4MULTITHREADED
//#include "G4MTRunManager.hh"
//#else
#include "G4RunManager.hh"
//#endif

#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "ChargeTransportActionInitialization.hh"
#include "ChargeTransportDetectorConstruction.hh"
#include "ChargeTransportPhysicsList.hh"

#include "G4CMPConfigManager.hh"

#include "Randomize.hh"


int main(int argc, char** argv)
{
  // Use Mersenne-Twister RNG (MT-safe)
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);

//#ifdef G4MULTITHREADED
//  G4MTRunManager* runManager = new G4MTRunManager;
//#else
  G4RunManager* runManager = new G4RunManager;
//#endif

  runManager->SetUserInitialization(new ChargeTransportDetectorConstruction);

  G4VUserPhysicsList* physics = new ChargeTransportPhysicsList;
  physics->SetCuts();
  runManager->SetUserInitialization(physics);

  runManager->SetUserInitialization(new ChargeTransportActionInitialization);

  // Instantiate G4CMP config manager so /g4cmp/* macro commands are available
  G4CMPConfigManager::Instance();

  G4VisManager* visManager = new G4VisExecutive("quiet");
  visManager->Initialize();

  G4UImanager* UI = G4UImanager::GetUIpointer();

  if (argc == 1) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    //UI->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  } else {
    UI->ApplyCommand(G4String("/control/execute ") + argv[1]);
  }

  delete visManager;
  delete runManager;
  return 0;
}
