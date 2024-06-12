#ifndef MUIC_TOROID_H
#define MUIC_TOROID_H


#include <G4MagneticField.hh>
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"

namespace muic {

    class ToroidField : public G4MagneticField {
    public:
        ToroidField(const double &);

        ~ToroidField();

        void GetFieldValue(
                const G4double[4],
                G4double *
        ) const override;

    private:
        double coil_current_;
        double coil_width_;
        double coil_height_;
        double rho0_;
        double z0_;

        double calculateIntegralHorizontalPart(
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &) const;

        double calculateIntegralVerticalPart(
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &) const;

        void calculateBHorizontalWire(
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                G4double[3]) const;

        void calculateBVerticalWire(
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                const double &,
                G4double[3]) const;
    };

    std::tuple<G4VPhysicalVolume *, G4VPhysicalVolume *> create_toroid(
            G4VPhysicalVolume *world,
            G4ThreeVector &position
    );

}


#endif //MUIC_TOROID_H
