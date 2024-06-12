#include <array>
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "muic/detector/IronShielding.h"

std::tuple<G4VPhysicalVolume *, G4VPhysicalVolume *> muic::create_shielding(G4VPhysicalVolume *world) {
    auto *nist = G4NistManager::Instance();

    // Detector position
    const G4double pos_x = 0.0 * m;
    const G4double pos_y = 0.0 * m;
    const G4double pos_z = 8.0 * m;

    // Materials
    auto *material_Fe = nist->FindOrBuildMaterial("G4_Fe");

    // Build shielding shape
    constexpr unsigned int shielding_n_z_planes = 2;
    std::array<G4double, shielding_n_z_planes> shielding_z_planes = {0. * cm, 500. * cm};
    std::array<G4double, shielding_n_z_planes> shielding_r_inner = {50. * cm, 50. * cm};
    std::array<G4double, shielding_n_z_planes> shielding_r_outter = {150. * cm, 150. * cm};

    auto *shielding_shape = new G4Polycone(
            "shielding", 0, 2 * M_PI, shielding_n_z_planes,
            shielding_z_planes.data(), shielding_r_inner.data(), shielding_r_outter.data()
    );

    // Place Positive-Z volumes
    auto *shielding_pos_lv = new G4LogicalVolume(
            shielding_shape, material_Fe, "shielding_pos"
    );

    auto *shielding_pos = new G4PVPlacement(
            nullptr,
            G4ThreeVector(pos_x, pos_y, pos_z),
            shielding_pos_lv,
            "shielding_pos",
            world->GetLogicalVolume(),
            0,
            0
    );

    // Placed Negative-Z Logical Volumes
    G4RotationMatrix *mirror_rotation_matrix = new G4RotationMatrix();
    mirror_rotation_matrix->rotateY(180. * deg);

    auto *shielding_neg_lv = new G4LogicalVolume(
            shielding_shape, material_Fe, "shielding_neg"
    );

    auto *shielding_neg = new G4PVPlacement(
            mirror_rotation_matrix,
            G4ThreeVector(pos_x, pos_y, -pos_z),
            shielding_neg_lv,
            "shielding_neg",
            world->GetLogicalVolume(),
            0,
            0
    );


    return {shielding_pos, shielding_neg};
}
