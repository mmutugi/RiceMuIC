#include <array>
#include <G4Polycone.hh>
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "muic/detector/World.h"

namespace muic {
    G4VPhysicalVolume *create_world() {
        auto *nist = G4NistManager::Instance();

        // Detector position
        G4double pos_x = 0.0 * m;
        G4double pos_y = 0.0 * m;
        G4double pos_z = 0.0 * m;

        // Materials
        auto *material_air = nist->FindOrBuildMaterial("G4_AIR");

        // Build world
        constexpr unsigned int world_n_z_planes = 2;
        std::array<G4double, world_n_z_planes> world_z_planes = {-100. * m, 100. * m};
        std::array<G4double, world_n_z_planes> world_r_inner = {0. * cm, 0. * cm};
        std::array<G4double, world_n_z_planes> world_r_outter = {700. * cm, 700. * cm};

        auto *world_shape = new G4Polycone(
                "world", 0, 2 * M_PI, world_n_z_planes,
                world_z_planes.data(), world_r_inner.data(), world_r_outter.data()
        );

        auto *lv = new G4LogicalVolume(
                world_shape, material_air, "world"
        );

        auto *world = new G4PVPlacement(
                0,
                G4ThreeVector(pos_x, pos_y, pos_z),
                lv,
                "world",
                nullptr,
                0,
                0
        );

        return world;
    }
}

