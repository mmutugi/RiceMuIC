#include "G4Event.hh"
#include "G4ios.hh"
#include <G4SDManager.hh>
#include "muic/simulation/CSCSimHit.h"
#include "muic/MuICContext.h"

#include "muic/actions/EventAction.h"

namespace muic {
    EventAction::EventAction(MuICContext *context)
            : G4UserEventAction(), context_(context) {
        print_every_n_events_ = 100;
    }


    EventAction::~EventAction() {
        // Do Nothing
    }


    void EventAction::BeginOfEventAction(const G4Event *evt) {
        G4int event_id = evt->GetEventID();

        if (event_id % print_every_n_events_ == 0) {
            G4cout << "Begin of event: " << event_id << G4endl;
        }

        // Propagate Event Start Signal
        context_->ntuple_maker_.onEventStart();
        context_->energy_deposit_collector_.onEventStart();
    }


    void EventAction::EndOfEventAction(const G4Event *evt) {
        // Save Nozzle Muons
        for (const auto& [track_id, energy_deposited] : context_->energy_deposit_collector_.energy_deposit_collection_) {
            const auto& energy_before = context_->energy_deposit_collector_.energy_before_nozzle_collection_[track_id];
            const auto& energy_after = context_->energy_deposit_collector_.energy_after_nozzle_collection_[track_id];
	        const auto& momentum_phi_before = context_->energy_deposit_collector_.momentum_phi_before_nozzle_collection_[track_id];
	        const auto& momentum_phi_after = context_->energy_deposit_collector_.momentum_phi_after_nozzle_collection_[track_id];
	        const auto& momentum_eta_before = context_->energy_deposit_collector_.momentum_eta_before_nozzle_collection_[track_id];
	        const auto& momentum_eta_after = context_->energy_deposit_collector_.momentum_eta_after_nozzle_collection_[track_id];
	   
	        const auto& position_phi_before = context_->energy_deposit_collector_.position_phi_before_nozzle_collection_[track_id];
	        const auto& position_phi_after = context_->energy_deposit_collector_.position_phi_after_nozzle_collection_[track_id];
	        const auto& position_eta_before = context_->energy_deposit_collector_.position_eta_before_nozzle_collection_[track_id];
	        const auto& position_eta_after = context_->energy_deposit_collector_. position_eta_after_nozzle_collection_[track_id];
	        const auto& momentum_theta_before_nozzle = context_-> energy_deposit_collector_.momentum_theta_before_nozzle_[track_id];
	        const auto& momentum_theta_after_nozzle = context_-> energy_deposit_collector_. momentum_theta_after_nozzle_[track_id];
	        const auto& position_theta_before_nozzle = context_-> energy_deposit_collector_.position_theta_before_nozzle_[track_id];
	        const auto& position_theta_after_nozzle = context_->energy_deposit_collector_.position_theta_after_nozzle_[track_id];
	   




            context_->ntuple_maker_.addGenMuon(
                    energy_before,
                    energy_after,
                    energy_deposited,
		    momentum_phi_before,
		    momentum_phi_after,
		    momentum_eta_before,
		    momentum_eta_after,
		   
		    position_phi_before,
		    position_phi_after,
		    position_eta_before,
		    position_eta_after,
		    momentum_theta_before_nozzle,
		    momentum_theta_after_nozzle,
		    position_theta_before_nozzle,
		    position_theta_after_nozzle
		   

		   

            );

        
        }
 //This is what I was missing when I was creating negative/blank ntuples for the hits. I was not collecting nor storing the hits.
        // Get Sensitive Detector
        auto *sd_manager = G4SDManager::GetSDMpointer();

        // Get MFS Hit Collection; Gets the hit collection for the process.
        if (mfs_hit_collection_id_ < 0) {
            mfs_hit_collection_id_ = sd_manager->GetCollectionID("mfs/MFSHit");
        }

        // Get the hit collections
        auto *hit_collection = evt->GetHCofThisEvent();

        if (!hit_collection) {
            return;
        }

        // Get Hits
        auto *mfs_hit_collection = dynamic_cast<MFSHitCollection *>(
                hit_collection->GetHC(mfs_hit_collection_id_)
        );
         // add the hits to the ntuple after getting them.
        if (mfs_hit_collection) {
            for (auto hit: *mfs_hit_collection->GetVector()) {
                context_->ntuple_maker_.addHit(*hit);
            }
        }


        // Propagate Event End Signal
        context_->energy_deposit_collector_.onEventEnd();
        context_->ntuple_maker_.onEventEnd();
    }
}
