#ifndef MUICSIMULATION_MFSCHAMBER_H
#define MUICSIMULATION_MFSCHAMBER_H

#include <G4VSensitiveDetector.hh>

#include "MFSHit.h"

namespace muic {

    class MFSChamber : public G4VSensitiveDetector {
    public:
        MFSChamber(const G4String &);

        void Initialize(G4HCofThisEvent *) override;

    protected:
        G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;

    private:
        MFSHitCollection *hit_collection_ = nullptr;
        int hit_collection_id_ = -1;
    };

}

#endif //MUICSIMULATION_MFSCHAMBER_H
