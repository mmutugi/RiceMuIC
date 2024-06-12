#include <array>
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "muic/detector/Nozzle.h"

namespace muic {

    std::tuple<G4VPhysicalVolume *, G4VPhysicalVolume *, G4VPhysicalVolume *, G4VPhysicalVolume *>
    create_nozzle(G4VPhysicalVolume *world) {
        auto *nist = G4NistManager::Instance();

        // Detector position
        const G4double pos_x = 0.0 * m;
        const G4double pos_y = 0.0 * m;
        const G4double pos_z = 0.0 * m;

        // Materials
        auto *material_W = nist->FindOrBuildMaterial("G4_CONCRETE");

        const G4double bch2_density = (1.01 * 997.6) * (kg / m3);
        auto *element_B = nist->FindOrBuildElement(5, false);
        auto *element_C = nist->FindOrBuildElement(6, false);
        auto *element_H = nist->FindOrBuildElement(1, false);
        auto *material_BCH2 = new G4Material("BCH2", bch2_density, 3);
        material_BCH2->AddElement(element_B, 1);
        material_BCH2->AddElement(element_C, 1);
        material_BCH2->AddElement(element_H, 2);

        // Build raw nozzle_w shape
        constexpr unsigned int w_n_z_planes = 6;
        std::array<G4double, w_n_z_planes> w_z_planes = {
                6. * cm,
                15. * cm,
                100. * cm,
                100. * cm,
                200. * cm,
                600. * cm
        };
        std::array<G4double, w_n_z_planes> w_r_inner = {
                1 * cm,
                0.5 * cm,
                0.03 * cm,
                0.03 * cm,
                ((1.78 - 0.3) / (500) * 200 + 0.03) * cm,
                1.78 * cm
        };
        std::array<G4double, w_n_z_planes> w_r_outter = {
                1 * cm,
                (1 + (17.63 - 1) / (100 - 6) * 15) * cm,
                17.63 * cm,
                15. * cm,
                17 * cm,
                50. * cm
        };

        auto *nozzle_w_shape = new G4Polycone(
                "nozzle_w",
                0, 2 * M_PI, w_n_z_planes,
                w_z_planes.data(), w_r_inner.data(), w_r_outter.data()
        );

        // Define the BCH2 shape
        // Assume the BCH2 part of the nozzle_w_pos is a cone with 3 radius and a hole in the middle of varying radii
        constexpr unsigned int bch2_n_z_planes = 3;
        std::array<G4double, bch2_n_z_planes> bch2_z_planes = {100 * cm, 200 * cm, 600 * cm};
        std::array<G4double, bch2_n_z_planes> bch2_r_inner = {15 * cm, 17. * cm, 50. * cm};
        std::array<G4double, bch2_n_z_planes> bch2_r_outter = {
                17.63 * cm,
                ((60 - 17.63) / (500) * 100 + 17.63) * cm,
                60. * cm
        };

        auto *nozzle_bch2_shape = new G4Polycone(
                "nozzle_bch2",
                0, 2 * M_PI, bch2_n_z_planes,
                bch2_z_planes.data(), bch2_r_inner.data(), bch2_r_outter.data()
        );

        // Place Positive-Z volumes
        auto *nozzle_w_pos_lv = new G4LogicalVolume(
                nozzle_w_shape, material_W, "nozzle_w_pos"
        );

        auto *nozzle_bch2_pos_lv = new G4LogicalVolume(
                nozzle_bch2_shape, material_BCH2, "nozzle_bch2_pos"
        );

        auto *nozzle_w_pos = new G4PVPlacement(
                nullptr,
                G4ThreeVector(pos_x, pos_y, pos_z),
                nozzle_w_pos_lv,
                "nozzle_w_pos",
                world->GetLogicalVolume(),
                0,
                0
        );

        auto *nozzle_bch2_pos = new G4PVPlacement(
                nullptr,
                G4ThreeVector(pos_x, pos_y, pos_z),
                nozzle_bch2_pos_lv,
                "nozzle_bch2_pos",
                world->GetLogicalVolume(),
                0,
                0
        );

        // Placed Negative-Z Logical Volumes
        G4RotationMatrix *mirror_rotation_matrix = new G4RotationMatrix();
        mirror_rotation_matrix->rotateY(180. * deg);

        auto *nozzle_w_neg_lv = new G4LogicalVolume(
                nozzle_w_shape, material_W, "nozzle_w_neg"
        );

        auto *nozzle_bch2_neg_lv = new G4LogicalVolume(
                nozzle_bch2_shape, material_BCH2, "nozzle_bch2_neg"
        );

        auto *nozzle_w_neg = new G4PVPlacement(
                mirror_rotation_matrix,
                G4ThreeVector(pos_x, pos_y, -pos_z),
                nozzle_w_neg_lv,
                "nozzle_w_neg",
                world->GetLogicalVolume(),
                0,
                0
        );

        auto *nozzle_bch2_neg = new G4PVPlacement(
                mirror_rotation_matrix,
                G4ThreeVector(pos_x, pos_y, -pos_z),
                nozzle_bch2_neg_lv,
                "nozzle_bch2_neg",
                world->GetLogicalVolume(),
                0,
                0
        );


        return {nozzle_w_pos, nozzle_bch2_pos, nozzle_w_neg, nozzle_bch2_neg};
    }
}
