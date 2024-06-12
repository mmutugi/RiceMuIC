#ifndef MUICSIMULATION_MUICCONSTANTS_H
#define MUICSIMULATION_MUICCONSTANTS_H

#include <G4SystemOfUnits.hh>

#include "muic/data/MFSChamberLUT.h"

namespace muic {

    // Particle Gun
    const double PGUN_ETA_MAX = 6.4;
    // const double PGUN_ETA_MAX = 4.4;
    const double PGUN_ETA_MIN = 4.4;
    const double PGUN_THETA_MAX = 2 * std::atan(std::exp(-PGUN_ETA_MIN)) * rad;
    const double PGUN_THETA_MIN = 2 * std::atan(std::exp(-PGUN_ETA_MAX)) * rad;

    // Forward Spectrometer
    const int MFS_N_STATIONS = 4;
    const int MFS_N_CHAMBERS = 18;
    const MFSChamberLUT MFS_CHAMBER_LUT;

}

#endif //MUICSIMULATION_MUICCONSTANTS_H
