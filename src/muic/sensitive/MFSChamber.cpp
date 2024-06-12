#include <G4SDManager.hh>

#include "muic/sensitive/MFSChamber.h"
#include "muic/MuICConstants.h"

namespace muic {

    MFSChamber::MFSChamber(
            const G4String &name
    ) : G4VSensitiveDetector(name) {
        collectionName.insert("MFSHit");
    }

    void MFSChamber::Initialize(G4HCofThisEvent *hcof) {
        hit_collection_ = new MFSHitCollection(SensitiveDetectorName, collectionName[0]);

        if (hit_collection_id_ == -1) {
            hit_collection_id_ = G4SDManager::GetSDMpointer()->GetCollectionID(
                    SensitiveDetectorName + "/" + collectionName[0]
            );
        }

        hcof->AddHitsCollection(hit_collection_id_, hit_collection_);
    }

    G4bool MFSChamber::ProcessHits(G4Step *step, G4TouchableHistory *history) {
        // Only Detect Charged Particles
        if (step->GetPreStepPoint()->GetCharge() == 0) {
            return false;
        }

        // Only Detect Charged Particles
        if (abs(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()) != 13) {
            return false;
        }

        // Build Hits
        auto *hit = new MFSHit();

        std::string pv_name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();

        hit->SetStation(MFS_CHAMBER_LUT.getStation(pv_name));
        hit->SetChamber(MFS_CHAMBER_LUT.getChamber(pv_name));
        hit->SetTime(step->GetPreStepPoint()->GetGlobalTime());
        hit->SetPosition(step->GetPreStepPoint()->GetPosition());

        hit_collection_->insert(hit);

        // Return
        return true;
    }
}