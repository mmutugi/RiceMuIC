#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "boost/format.hpp"

#include "muic/detector/MFS.h"
#include "muic/MuICConstants.h"

std::vector<G4PVPlacement *> muic::create_mfs(G4VPhysicalVolume *world) {
    auto *nist = G4NistManager::Instance();

    // Materials
    auto *chamber_material = nist->FindOrBuildMaterial("G4_Al");

    // Build shielding shape
    double chamber_opening_ang = 22 * deg;
    double chamber_thickness = 20 * cm;
    double chamber_outer_radius = 7 * m;
    double chamber_inner_radius = 1 * m;

    auto *chamber_shape = new G4Trd(
            "mfs_chamber",
            chamber_inner_radius * std::tan(chamber_opening_ang / 2),
            (chamber_inner_radius + chamber_outer_radius) * std::tan(chamber_opening_ang / 2),
            chamber_thickness / 2, chamber_thickness / 2,
            chamber_outer_radius / 2
    );

    // Build Chambers
    int chamber_global_id = 0;
    std::vector<G4PVPlacement *> chambers;

    auto *chamber_lv = new G4LogicalVolume(
            chamber_shape, chamber_material, "mfs_chamber"
    );

    double magnet_z = 60 * m;
    double magnet_width = 10 * m;

    for (int station_id = 0; station_id < MFS_N_STATIONS; ++station_id) {
        G4double station_pos_z = 0;

        if (station_id == 3) {
            station_pos_z = -magnet_z - magnet_width / 2 - 4 * m;
        } else if (station_id == 2) {
            station_pos_z = -magnet_z - magnet_width / 2;
        } else if (station_id == 1) {
            station_pos_z = -magnet_z + magnet_width / 2;
        } else if (station_id == 0) {
            station_pos_z = -magnet_z + magnet_width / 2 + 4 * m;
        }

        for (int chamber_id = 0; chamber_id < MFS_N_CHAMBERS; ++chamber_id) {
            // Calculate Angle
            double chamber_rot = (360 * deg) / MFS_N_CHAMBERS * chamber_id + chamber_opening_ang / 2 - M_PI / 2;

            // Rotation Matrix
            auto *rotation_matrix = new G4RotationMatrix();
            rotation_matrix->rotateX(90. * deg);
            rotation_matrix->rotateY(chamber_rot);

            // Detector position
            const G4double rho = chamber_outer_radius / 2 + chamber_inner_radius;
            G4double pos_x = rho * std::cos(chamber_rot + M_PI / 2);
            G4double pos_y = rho * std::sin(chamber_rot + M_PI / 2);
            G4double pos_z = station_pos_z;

            // Calculate z-position
            if (station_id < 2) {
                if ((chamber_id % 2) == 0) {
                    pos_z += chamber_thickness;
                }
            } else if ((chamber_id % 2) == 0) {
                pos_z -= chamber_thickness;
            }

            // Build detector id
            auto detid = MFS_CHAMBER_LUT.getDetId(chamber_global_id);

            auto *chamber = new G4PVPlacement(
                    rotation_matrix,
                    G4ThreeVector(pos_x, pos_y, pos_z),
                    chamber_lv,
                    detid,
                    world->GetLogicalVolume(),
                    0,
                    chamber_global_id
            );

            chambers.push_back(chamber);

            // Increase Counter
            chamber_global_id += 1;
        }
    }

    return chambers;
}
