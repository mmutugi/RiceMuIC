#include <G4SDManager.hh>

#include "muic/simulation/CSCSimHitSD.h"

namespace muic {
    CSCSimHitSD::CSCSimHitSD(
            const G4String &name,
            const std::map<std::string, int> &pv_to_station_lut
    ) :
            G4VSensitiveDetector(name) {
        collectionName.insert("cscsimhit");
        pv_to_station_lut_ = pv_to_station_lut;
    }

    void CSCSimHitSD::Initialize(G4HCofThisEvent *hit_collections) {
        // Get a Collection Id
        if (hit_collection_id_ < 0) {
            hit_collection_id_ = G4SDManager::GetSDMpointer()->GetCollectionID(
                    GetName() + "/" + collectionName[0]
            );
        }

        // Short-Circuit: Failed to get hit collection id
        if (hit_collection_id_ < 0) {
            return;
        }

        // Create a hit collection
        hit_collection_ = new CSCSimHitCollection(SensitiveDetectorName, collectionName[0]);

        // Add hit collection
        hit_collections->AddHitsCollection(hit_collection_id_, hit_collection_);
    }

    G4bool CSCSimHitSD::ProcessHits(G4Step *step, G4TouchableHistory *history) {
        // Short-Circuit: Hit collection not available
        if (hit_collection_ == nullptr) {
            return false;
        }

        // Get point before G4 simulates the interaction with the detector
        auto *step_point = step->GetPreStepPoint();

        // Short-Circuit: Ignore neutral particles
        if (step_point->GetCharge() == 0) {
            return false;
        }

        // Create a CSC Hit
        CSCSimHit *simhit = new CSCSimHit();
        simhit->SetStation(pv_to_station_lut_.at(step_point->GetPhysicalVolume()->GetName()));
        simhit->SetTime(step_point->GetGlobalTime());
        simhit->SetPosition(step_point->GetPosition());

        // Append simhit
        hit_collection_->insert(simhit);

        // Return
        return true;
    }
}
