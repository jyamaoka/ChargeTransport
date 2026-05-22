#include "ChargeTransportRunAction.hh"
#include "ChargeTransportAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

ChargeTransportRunAction::ChargeTransportRunAction()
  : fFileName("gamma_si_ehpairs")
{
  // G4AnalysisManager is a singleton; constructor runs on every thread and on
  // the master.  Ntuple schema must be defined here so all threads share it.
  auto* am = G4AnalysisManager::Instance();
  am->SetVerboseLevel(1);
  //am->SetNtupleMerging(true);   // merge per-thread ntuples into one ROOT file

  // Ntuple columns — keep column ordering identical to LD400_Charge
  am->CreateNtuple("EHPairs", "Electron-hole pairs reaching detector boundary");
  am->CreateNtupleIColumn("RunID");             // 0
  am->CreateNtupleIColumn("EventID");           // 1
  am->CreateNtupleIColumn("TrackID");           // 2
  am->CreateNtupleSColumn("ParticleName");      // 3
  am->CreateNtupleDColumn("StartEnergy_eV");    // 4
  am->CreateNtupleDColumn("StartX_m");          // 5
  am->CreateNtupleDColumn("StartY_m");          // 6
  am->CreateNtupleDColumn("StartZ_m");          // 7
  am->CreateNtupleDColumn("StartTime_ns");      // 8
  am->CreateNtupleDColumn("EnergyDeposited_eV");// 9
  am->CreateNtupleDColumn("TrackWeight");       // 10
  am->CreateNtupleDColumn("EndX_m");            // 11
  am->CreateNtupleDColumn("EndY_m");            // 12
  am->CreateNtupleDColumn("EndZ_m");            // 13
  am->CreateNtupleDColumn("FinalTime_ns");      // 14
  am->FinishNtuple();
}

ChargeTransportRunAction::~ChargeTransportRunAction()
{
  delete G4AnalysisManager::Instance();
}

void ChargeTransportRunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager::Instance()->OpenFile(fFileName);
}

void ChargeTransportRunAction::EndOfRunAction(const G4Run*)
{
  auto* am = G4AnalysisManager::Instance();
  am->Write();
  am->CloseFile();
  G4cout << "\n[ChargeTransportRunAction] Results written to: "
         << fFileName << ".root" << G4endl;
}
