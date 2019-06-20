#include "OTDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"

OTDetectorConstruction::OTDetectorConstruction()
: G4VUserDetectorConstruction()
{
}

OTDetectorConstruction::~OTDetectorConstruction()
{
}

G4VPhysicalVolume* OTDetectorConstruction::Construct()
{  
  G4NistManager* nist = G4NistManager::Instance();


  // -----------------------------------------------------
  // World

  G4Material* world_mat = nist -> FindOrBuildMaterial("G4_AIR");
  G4double world_size = 3000*mm;

  G4Box* solidWorld =    
    new G4Box("World",                       // its name
              0.5*world_size,                // half x
              0.5*world_size,                // half y
              0.5*world_size);               // half z
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //overlaps checking


  // -----------------------------------------------------
  // Detector

  //G4Material* detector_mat = nist -> FindOrBuildMaterial("G4_WATER");

  auto element_H  = nist -> FindOrBuildElement(1);
  auto element_C  = nist -> FindOrBuildElement(6);
  auto element_O  = nist -> FindOrBuildElement(8);
  auto element_Si = nist -> FindOrBuildElement(14);
  auto element_Ar = nist -> FindOrBuildElement(18);
  auto element_Br = nist -> FindOrBuildElement(35);

  auto tempSTP = CLHEP::STP_Temperature;
  auto tempLAB = tempSTP + 20.*kelvin;
  auto tempToLAB = tempSTP/tempLAB;

  auto denArGas = 1.782e-3*g/cm3 * tempToLAB;
  auto denMethane = 0.717e-3*g/cm3 * tempToLAB;
  auto denP10 = .9*denArGas + .1*denMethane;
  G4double detector_size = 2400*mm;

  auto matArGas = new G4Material("ArgonGas", denArGas, 1, kStateGas, tempLAB);
  matArGas -> AddElement(element_Ar, 4);

  auto matMethaneGas = new G4Material("MethaneGas ", denMethane, 2, kStateGas, tempLAB);
  matMethaneGas -> AddElement(element_H, 4);
  matMethaneGas -> AddElement(element_C, 1);

  auto matP10 = new G4Material("P10", denP10, 2, kStateGas, tempLAB);
  matP10 -> AddMaterial(matArGas,      .9*denArGas/denP10);
  matP10 -> AddMaterial(matMethaneGas, .1*denMethane/denP10);



  G4Box* solidDetector = new G4Box("Detector", 0.5*detector_size, 0.5*detector_size, 0.5*detector_size);
  G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector, matP10, "Detector");
  logicDetector -> SetUserLimits(new G4UserLimits(1.*mm));
  new G4PVPlacement(0, G4ThreeVector(0,0,0), logicDetector, "Detector", logicWorld, false, 1, true);

  return physWorld;
}
