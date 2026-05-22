#include "ChargeTransportDetectorConstruction.hh"
#include "ChargeTransportDetectorMessenger.hh"
#include "ChargeTransportSensitiveDetector.hh"

#include "G4CMPSurfaceProperty.hh"
#include "G4CMPLogicalBorderSurface.hh"
#include "G4CMPFieldManager.hh"
#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4UniformElectricField.hh"
#include "G4VisAttributes.hh"


ChargeTransportDetectorConstruction::ChargeTransportDetectorConstruction()
  : fMessenger(nullptr),
    fSensitivity(nullptr), fSurfProp(nullptr),
    fLatManager(G4LatticeManager::GetLatticeManager()),
    fEMField(nullptr), fSilicon(nullptr), fVacuum(nullptr),
    fWorldPhys(nullptr), fConstructed(false),
    fDetSizeX(1.0*cm), fDetSizeY(1.0*cm), fDetSizeZ(1.0*cm)
{
  fMessenger = new ChargeTransportDetectorMessenger(this);
}

ChargeTransportDetectorConstruction::~ChargeTransportDetectorConstruction()
{
  delete fMessenger;
  delete fEMField;
  delete fSurfProp;
}

G4VPhysicalVolume* ChargeTransportDetectorConstruction::Construct()
{
  if (fConstructed) {
    if (!G4RunManager::IfGeometryHasBeenDestroyed()) {
      G4GeometryManager::GetInstance()->OpenGeometry();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4SolidStore::GetInstance()->Clean();
    }
    fLatManager->Reset();
    G4CMPLogicalBorderSurface::CleanSurfaceTable();
    delete fEMField; fEMField = nullptr;
  }

  DefineMaterials();
  SetupGeometry();
  fConstructed = true;
  return fWorldPhys;
}

void ChargeTransportDetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();
  fVacuum  = nist->FindOrBuildMaterial("G4_Galactic");
  fSilicon = nist->FindOrBuildMaterial("G4_Si");

  // Attach G4CMP silicon lattice properties (phonon + charge-carrier parameters)
  fLatManager->LoadLattice(fSilicon, "Si");
}

void ChargeTransportDetectorConstruction::SetupGeometry()
{
  const G4double halfX = fDetSizeX / 2.;
  const G4double halfY = fDetSizeY / 2.;
  const G4double halfZ = fDetSizeZ / 2.;
  const G4double worldHalf = 1.5 * std::max({fDetSizeX, fDetSizeY, fDetSizeZ});

  // --- World ---
  G4VSolid*        worldSolid   = new G4Box("World", worldHalf, worldHalf, worldHalf);
  G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, fVacuum, "World");
  fWorldPhys = new G4PVPlacement(nullptr, G4ThreeVector(), worldLogical,
                                 "World", nullptr, false, 0);

  // --- Silicon detector ---
  G4VSolid*        siSolid   = new G4Box("SiCube", halfX, halfY, halfZ);
  G4LogicalVolume* siLogical = new G4LogicalVolume(siSolid, fSilicon, "SiCube");
  G4VPhysicalVolume* siPhysical =
    new G4PVPlacement(nullptr, G4ThreeVector(), siLogical,
                      "SiCube", worldLogical, false, 0);

  AttachField(siLogical);
  AttachLattice(siPhysical);

  // --- Surface properties ---
  // All six faces of the cube are absorbing electrodes for charge carriers
  // (probability 1.0 absorption, 0.0 reflection).
  if (!fConstructed) {
    fSurfProp = new G4CMPSurfaceProperty("SiSurface",
      1.0,  // qAbsProb  — absorb carrier
      0.0,  // qReflProb — no reflection
      0.0,  // eMinK
      0.0,  // hMinK
      1.0,  // pAbsProb  — absorb phonon
      0.0,  // pReflProb
      0.0,  // pSpecProb
      0.0); // pMinK
  }

  // Register boundary between Si and the vacuum world
  new G4CMPLogicalBorderSurface("SiWorldSurface", siPhysical, fWorldPhys,
                                fSurfProp);

  AttachSensitivity(siLogical);

  // --- Visualization ---
  worldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());
  auto* siVis = new G4VisAttributes(G4Colour(0.5, 0.5, 1.0, 0.3));
  siVis->SetVisibility(true);
  siLogical->SetVisAttributes(siVis);
}

void ChargeTransportDetectorConstruction::AttachField(G4LogicalVolume* lv)
{
  if (!fEMField) {
    const G4double fieldMag = kFieldVolts * volt / fDetSizeZ;
    fEMField = new G4UniformElectricField(fieldMag * G4ThreeVector(0., 0., 1.));
  }

  if (!lv->GetFieldManager()) {
    G4FieldManager* fm = new G4CMPFieldManager(fEMField);
    lv->SetFieldManager(fm, true);
  }
  lv->GetFieldManager()->SetDetectorField(fEMField);
}

void ChargeTransportDetectorConstruction::AttachLattice(G4VPhysicalVolume* pv)
{
  auto* phys = new G4LatticePhysical(fLatManager->GetLattice(fSilicon));
  // Standard silicon (100) orientation with 45-degree rotation (flats at [110])
  phys->SetMillerOrientation(1, 0, 0, 45.*deg);
  fLatManager->RegisterLattice(pv, phys);
}

void ChargeTransportDetectorConstruction::AttachSensitivity(G4LogicalVolume* lv)
{
  if (!fSensitivity) {
    fSensitivity = new ChargeTransportSensitiveDetector("SiDetector");
  }
  G4SDManager::GetSDMpointer()->AddNewDetector(fSensitivity);
  lv->SetSensitiveDetector(fSensitivity);
}

void ChargeTransportDetectorConstruction::SetDetectorSizeX(G4double val)
{
  fDetSizeX = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void ChargeTransportDetectorConstruction::SetDetectorSizeY(G4double val)
{
  fDetSizeY = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void ChargeTransportDetectorConstruction::SetDetectorSizeZ(G4double val)
{
  fDetSizeZ = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
