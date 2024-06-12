#include <array>
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Polycone.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "muic/detector/Toroid.h"

muic::ToroidField::ToroidField(const double &z_position) {
    coil_current_ = 20e3 * 116;
    coil_width_ = 4.5;
    coil_height_ = 4.05;
    rho0_ = 1.05;
    z0_ = z_position / m - coil_width_ / 2;
}

muic::ToroidField::~ToroidField() {
    // Do Nothing
}

void muic::ToroidField::GetFieldValue(
        const G4double point[4],
        G4double b_field[3]
) const {
    double x = point[0] / m;
    double y = point[1] / m;
    double z = point[2] / m;
    // double t = point[3];

    double Bx = 0;
    double By = 0;
    double Bz = 0;

    // Calculate Loop Mag Field
    for (int coil_id = 0; coil_id < 8; ++coil_id) {
        G4double coil_field[3];
        G4double wire_field[4][3];

        double rot = coil_id * (2 * M_PI) / 8;

        // Init Fields
        coil_field[0] = 0.;
        coil_field[1] = 0.;
        coil_field[2] = 0.;

        for (int wire_id = 0; wire_id < 4; ++wire_id) {
            for (int comp_id = 0; comp_id < 3; ++comp_id) {
                wire_field[wire_id][comp_id] = 0.;
            }
        }

        // Calculate Wire Fields
        calculateBHorizontalWire(
                1, x, y, z,
                rho0_ + coil_height_, z0_, rot,
                wire_field[0]
        );

        calculateBHorizontalWire(
                -1, x, y, z,
                rho0_, z0_, rot,
                wire_field[1]
        );

        calculateBVerticalWire(
                1, x, y, z,
                rho0_, z0_, rot,
                wire_field[2]
        );

        calculateBVerticalWire(
                -1, x, y, z,
                rho0_, z0_ + coil_width_, rot,
                wire_field[3]
        );

        // Sum Wire Fields
        for (int wire_id = 0; wire_id < 4; ++wire_id) {
            for (int comp_id = 0; comp_id < 3; ++comp_id) {
                coil_field[comp_id] += wire_field[wire_id][comp_id];
            }
        }

        // Sum Coil Fields
        Bx += coil_field[0];
        By += coil_field[1];
        Bz += coil_field[2];
    }


    // Set Field
    b_field[0] = Bx * tesla;
    b_field[1] = By * tesla;
    b_field[2] = Bz * tesla;

    // Debug
    //    double bfield = std::hypot(Bx, By, Bz);
    //
    //    if (bfield > 4) {
    //        G4cout << "x=" << x
    //               << " y=" << y
    //               << " z=" << z
    //               << " B=" << bfield
    //               << G4endl;
    //    }
}

double muic::ToroidField::calculateIntegralHorizontalPart(
        const double &x,
        const double &y,
        const double &z,
        const double &rho0,
        const double &z0,
        const double &rot,
        const double &z_prime
) const {
    double numerator = z0 + z_prime - z;

    double denominator = std::hypot(
            z0 + z_prime - z,
            x - rho0 * std::cos(rot),
            y - rho0 * std::sin(rot)
    );

    denominator *= (std::pow(rho0, 2) + std::pow(x, 2) + std::pow(y, 2) - \
    2 * rho0 * (x * std::cos(rot) + y * sin(rot)));

    if (denominator == 0) {
        return 0;
    }

    return numerator / denominator;
}

double muic::ToroidField::calculateIntegralVerticalPart(
        const double &x,
        const double &y,
        const double &z,
        const double &rho0,
        const double &z0,
        const double &rot,
        const double &rho_prime
) const {
    double numerator = 2 * (rho0 + rho_prime - x * std::cos(rot) - y * std::sin(rot));

    double denominator = std::sqrt(
            std::pow(rho0 + rho_prime, 2)
            + std::pow(x, 2)
            + std::pow(y, 2)
            + std::pow(z - z0, 2)
            - 2 * (rho0 + rho_prime) * (x * std::cos(rot) + y * std::sin(rot))
    );

    denominator *= (
            std::pow(x, 2) + std::pow(y, 2) + \
            2 * std::pow(z - z0, 2) + \
            (std::pow(y, 2) - std::pow(x, 2)) * std::cos(2 * rot) - \
            2 * x * y * std::sin(2 * rot)
    );

    if (denominator == 0) {
        return 0;
    }

    return numerator / denominator;
}

void muic::ToroidField::calculateBHorizontalWire(
        const double &dir,
        const double &x,
        const double &y,
        const double &z,
        const double &rho0,
        const double &z0,
        const double &rot,
        G4double mag_field[3]
) const {
    // Constants
    double sf = 1e-7;

    // Calculate Integrals
    double int_part = calculateIntegralHorizontalPart(
            x, y, z,
            rho0, z0, rot,
            z0 + coil_width_
    );
    int_part -= calculateIntegralHorizontalPart(
            x, y, z,
            rho0, z0, rot,
            z0
    );

    // Calculate Fields
    double Bx = sf * dir * coil_current_ * (rho0 * std::sin(rot) - y) * int_part;
    double By = sf * dir * coil_current_ * (x - rho0 * std::cos(rot)) * int_part;

    mag_field[0] = Bx;
    mag_field[1] = By;
    mag_field[2] = 0;
}

void muic::ToroidField::calculateBVerticalWire(
        const double &dir,
        const double &x,
        const double &y,
        const double &z,
        const double &rho0,
        const double &z0,
        const double &rot,
        G4double mag_field[3]
) const {
    // Constants
    double sf = 1e-7;

    // Calculate Integrals
    double int_part = calculateIntegralVerticalPart(
            x, y, z,
            rho0, z0, rot,
            rho0 + coil_height_
    );
    int_part -= calculateIntegralVerticalPart(
            x, y, z,
            rho0, z0, rot,
            rho0
    );

    // Calculate Fields
    double Bx = sf * dir * coil_current_ * (z - z0) * std::sin(rot) * int_part;
    double By = sf * dir * coil_current_ * (z0 - z) * std::cos(rot) * int_part;
    double Bz = sf * dir * coil_current_ * (y * std::cos(rot) - x * std::sin(rot)) * int_part;

    mag_field[0] = Bx;
    mag_field[1] = By;
    mag_field[2] = Bz;
}


std::tuple<G4VPhysicalVolume *, G4VPhysicalVolume *> muic::create_toroid(
        G4VPhysicalVolume *world,
        G4ThreeVector &position
) {
    return {nullptr, nullptr};
}