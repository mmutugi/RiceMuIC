#ifndef MUIC_MFS_H
#define MUIC_MFS_H


#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"

namespace muic {

    std::vector<G4PVPlacement *> create_mfs(G4VPhysicalVolume *world);

}


#endif //MUIC_MFS_H
