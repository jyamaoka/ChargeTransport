#ifndef ChargeTransportDetectorConstruction_hh
#define ChargeTransportDetectorConstruction_hh 1

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

class G4CMPSurfaceProperty;
class G4LatticeManager;
class G4Material;
class G4VPhysicalVolume;
class G4ElectricField;
class ChargeTransportSensitiveDetector;
class ChargeTransportDetectorMessenger;


class ChargeTransportDetectorConstruction : public G4VUserDetectorConstruction {
public:
  ChargeTransportDetectorConstruction();
  virtual ~ChargeTransportDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

  void SetDetectorSizeX(G4double val);
  void SetDetectorSizeY(G4double val);
  void SetDetectorSizeZ(G4double val);

  G4double GetDetectorSizeX() const { return fDetSizeX; }
  G4double GetDetectorSizeY() const { return fDetSizeY; }
  G4double GetDetectorSizeZ() const { return fDetSizeZ; }

private:
  void DefineMaterials();
  void SetupGeometry();
  void AttachField(G4LogicalVolume* lv);
  void AttachLattice(G4VPhysicalVolume* pv);
  void AttachSensitivity(G4LogicalVolume* lv);

  ChargeTransportDetectorMessenger* fMessenger;
  ChargeTransportSensitiveDetector* fSensitivity;
  G4CMPSurfaceProperty*     fSurfProp;
  G4LatticeManager*         fLatManager;
  G4ElectricField*          fEMField;
  G4Material*               fSilicon;
  G4Material*               fVacuum;
  G4VPhysicalVolume*        fWorldPhys;
  G4bool                    fConstructed;

  G4double fDetSizeX;  // full detector X dimension (G4 internal units)
  G4double fDetSizeY;  // full detector Y dimension
  G4double fDetSizeZ;  // full detector Z dimension

  static constexpr double kFieldVolts = 0.;  // bias voltage [V]
};

#endif
