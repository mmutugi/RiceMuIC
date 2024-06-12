#include <array>
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "muic/detector/IdealToroid.h"

muic::IdealToroid::IdealToroid(const double &z_position, const double &mag) {
    mag_field_ = mag;
    coil_width_ = 4.5;
    coil_height_ = 4.05;
    rho0_ = 1.05;
    z0_ = z_position / m - coil_width_ / 2;
}
muic::IdealToroid::~IdealToroid() {

   
}

void muic::IdealToroid::GetFieldValue(
        const G4double point[4],
        G4double b_field[3]

) const{
    double x = point[0] / m;
    double y = point[1] / m;
    double z = point[2] / m;

    // Init BField
    double Bx = 0;
    double By = 0;
    double Bz = 0;


    // Set Field
    b_field[0] = Bx * tesla;
    b_field[1] = By * tesla;
    b_field[2] = Bz * tesla;

    // Calculate boundaries
    auto z_min = z0_;
    auto z_max = z_min + coil_width_;

    auto rho_min = rho0_;
    auto rho_max = rho_min + coil_height_;



    // Convert Point to Cylindrical coordinates
    auto cyl_z = z;
    auto cyl_rho = std::hypot(x, y);

    // Short-Circuit: Outside of toroid
    if ((cyl_z < z_min) || (z_max < cyl_z)) {
        return;
    }

    if ((cyl_rho < rho_min) || (rho_max < cyl_rho)) {
        return;
    }

    // Magnetic field goes along the phi direction
    auto cyl_phi = std::atan(y / x);

    Bx = mag_field_ * std::sin(cyl_phi);
    By = -mag_field_ * std::cos(cyl_phi);
    Bz = 0;

    if (x < 0) {
        Bx = -Bx;
        By = -By;
    }

    // Set Field
    b_field[0] = Bx * tesla;
    b_field[1] = By * tesla;
    b_field[2] = Bz * tesla;








}