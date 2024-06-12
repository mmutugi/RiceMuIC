#ifndef MUIC_DETECTOR_NOZZLE_H
#define MUIC_DETECTOR_NOZZLE_H

#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"

namespace muic {

    std::tuple<G4VPhysicalVolume *, G4VPhysicalVolume *, G4VPhysicalVolume *, G4VPhysicalVolume *>
    create_nozzle(G4VPhysicalVolume *world);

}

#endif //MUIC_DETECTOR_NOZZLE_H
