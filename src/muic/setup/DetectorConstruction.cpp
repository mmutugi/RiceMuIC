#include "muic/detector/World.h"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Colour.hh"
#include "G4FieldManager.hh"
#include "G4GenericMessenger.hh"
#include "G4Orb.hh"
#include "G4Polyline.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4TransportationManager.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "muic/setup/DetectorConstruction.h"
#include "muic/detector/MFS.h"
#include "muic/detector/Nozzle.h"
#include "muic/detector/IronShielding.h"
#include "muic/detector/Toroid.h"
#include "muic/detector/World.h"
#include "muic/sensitive/MFSChamber.h"
#include "muic/detector/IdealToroid.h"


using namespace muic;

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
    // Do Nothing
}

DetectorConstruction::~DetectorConstruction() {
    // Do Nothing
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    auto *world = muic::create_world();
    muic::create_nozzle(world);
    muic::create_shielding(world);
    muic::create_mfs(world);

    return world;
}

void DetectorConstruction::ConstructSDandField() {
    //create the field manager

    auto *atlas_toroid_1 = new IdealToroid(-60 * m, 4);
    //auto *atlas_toroid_1 = new ToroidField(-60 * m);
    auto *field_manager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    field_manager->SetDetectorField(atlas_toroid_1);
    field_manager->CreateChordFinder(atlas_toroid_1);



    // Get Sensitive Detector Manager
    auto *sd_manager = G4SDManager::GetSDMpointer();
    sd_manager->SetVerboseLevel(0);

    auto *mfs_detector = new MFSChamber("mfs");
    SetSensitiveDetector("mfs_chamber", mfs_detector);
    sd_manager->AddNewDetector(mfs_detector);

}

