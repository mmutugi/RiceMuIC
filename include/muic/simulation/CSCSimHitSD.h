#ifndef MUIC_SIMULATION_CSCSIMHITSD_H
#define MUIC_SIMULATION_CSCSIMHITSD_H

#include <G4VSensitiveDetector.hh>

#include "CSCSimHit.h"

namespace muic {
    class CSCSimHitSD : public G4VSensitiveDetector {
    public:
        CSCSimHitSD(const G4String &, const std::map<std::string, int> &);

        void Initialize(G4HCofThisEvent *) override;

    protected:
        G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;

    private:
        // HIT COLLECTIONS
        CSCSimHitCollection *hit_collection_ = nullptr;
        G4int hit_collection_id_ = -1;

        // STATIONS
        std::map<std::string, int> pv_to_station_lut_;
    };
}

#endif
