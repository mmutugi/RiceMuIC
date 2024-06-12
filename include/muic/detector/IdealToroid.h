
#ifndef MUIC_IDEALTOROID_H
#define MUIC_IDEALTOROID_H


#include <G4MagneticField.hh>
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"

namespace muic {

    class IdealToroid : public G4MagneticField {
    public:
        IdealToroid(const double &, const double &);

        ~IdealToroid();

        void GetFieldValue(
                const G4double[4],
                G4double *
        ) const override;

    private:
        double mag_field_;
        double coil_width_;
        double coil_height_;
        double rho0_;
        double z0_;
    };

}


#endif //MUIC_IDEALTOROID_H
