#include "ChargeTransportSensitiveDetector.hh"
#include "ChargeTransportAnalysis.hh"

#include "G4CMPDriftElectron.hh"
#include "G4CMPDriftHole.hh"
#include "G4CMPElectrodeHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

ChargeTransportSensitiveDetector::ChargeTransportSensitiveDetector(const G4String& name)
  : G4CMPElectrodeSensitivity(name)
{}

void ChargeTransportSensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  auto* hitCol =
    static_cast<G4CMPElectrodeHitsCollection*>(HCE->GetHC(HCID));
  if (!hitCol) return;

  auto* runMan = G4RunManager::GetRunManager();
  const G4int runID   = runMan->GetCurrentRun()->GetRunID();
  const G4int eventID = runMan->GetCurrentEvent()->GetEventID();

  auto* am = G4AnalysisManager::Instance();

  for (G4CMPElectrodeHit* hit : *hitCol->GetVector()) {
    am->FillNtupleIColumn(0, runID);
    am->FillNtupleIColumn(1, eventID);
    am->FillNtupleIColumn(2, hit->GetTrackID());
    am->FillNtupleSColumn(3, hit->GetParticleName());
    am->FillNtupleDColumn(4, hit->GetStartEnergy()             / eV);
    am->FillNtupleDColumn(5, hit->GetStartPosition().x()       / m);
    am->FillNtupleDColumn(6, hit->GetStartPosition().y()       / m);
    am->FillNtupleDColumn(7, hit->GetStartPosition().z()       / m);
    am->FillNtupleDColumn(8, hit->GetStartTime()               / ns);
    am->FillNtupleDColumn(9, hit->GetEnergyDeposit()           / eV);
    am->FillNtupleDColumn(10, hit->GetWeight());
    am->FillNtupleDColumn(11, hit->GetFinalPosition().x()      / m);
    am->FillNtupleDColumn(12, hit->GetFinalPosition().y()      / m);
    am->FillNtupleDColumn(13, hit->GetFinalPosition().z()      / m);
    am->FillNtupleDColumn(14, hit->GetFinalTime()              / ns);
    am->AddNtupleRow();
  }
}

G4bool ChargeTransportSensitiveDetector::IsHit(const G4Step* step,
                                        const G4TouchableHistory*) const
{
  const G4Track* track = step->GetTrack();
  const G4ParticleDefinition* particle = track->GetDefinition();

  G4bool isCarrier = (particle == G4CMPDriftElectron::Definition() ||
                      particle == G4CMPDriftHole::Definition());

  // Record any carrier that stops (at boundary or via trapping/recombination)
  G4bool isStopped = (track->GetTrackStatus() == fStopAndKill);

  return isCarrier && isStopped;
}
