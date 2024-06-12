#include <G4Step.hh>
#include <G4SystemOfUnits.hh>

#include "muic/actions/SteppingAction.h"
#include "muic/MuICContext.h"

namespace muic {
    SteppingAction::SteppingAction(muic::MuICContext *context) {
        context_ = context;
    }

    SteppingAction::~SteppingAction() {
        // Do Nothing
    }

    void SteppingAction::SetSteppingManagerPointer(G4SteppingManager *stepping_manager) {
        fpSteppingManager = stepping_manager;
    }

    void SteppingAction::UserSteppingAction(const G4Step *step) {
        auto *particle = step->GetTrack()->GetDefinition();

        // Short-Circuit: Only track muons
        if (std::abs(particle->GetPDGEncoding()) != 13) {
            return;
        }

        // Only Track Primary Muon
        if (step->GetTrack()->GetParentID() != 0) {
            return;
        }

        // Get Points:
        auto *prestep_point = step->GetPreStepPoint();
        auto *poststep_point = step->GetPostStepPoint();

        // Check Volumes
        auto prestep_volume = prestep_point->GetPhysicalVolume()->GetName();

        // Save energy deposition inside nozzle
        bool keep_point = (prestep_volume == "nozzle_w_neg" || prestep_volume == "nozzle_bch2_neg");

        if (poststep_point->GetPhysicalVolume() != nullptr) {
            auto poststep_volume = poststep_point->GetPhysicalVolume()->GetName();
            keep_point |= poststep_volume == "nozzle_w_neg" || poststep_volume == "nozzle_bch2_neg";
        }

        if (!keep_point) {
            return;
        }

        auto energy_deposited = (prestep_point->GetTotalEnergy() - poststep_point->GetTotalEnergy());

        context_->energy_deposit_collector_.addEnergyDeposit(
                step->GetTrack()->GetTrackID(),
                (energy_deposited * MeV)
        );

        if (step->IsFirstStepInVolume()) {
            context_->energy_deposit_collector_.setEnergyBeforeNozzle(
                    step->GetTrack()->GetTrackID(),
                    prestep_point->GetTotalEnergy() * MeV
            );
            context_->energy_deposit_collector_.setMomentumEtaBeforeNozzle(
                    step->GetTrack()->GetTrackID(),
	
                    prestep_point->GetMomentum().getEta()
		    );
            context_->energy_deposit_collector_.setMomentumPhiBeforeNozzle(
                    step->GetTrack()->GetTrackID(),
		    
                    prestep_point->GetMomentum().getPhi() * radian
		    );
        context_->energy_deposit_collector_.setPositionEtaBeforeNozzle(
                    step->GetTrack()->GetTrackID(),
		    prestep_point->GetPosition().eta()
                  
		    );
            context_->energy_deposit_collector_.setPositionPhiBeforeNozzle(
                    step->GetTrack()->GetTrackID(),
		    prestep_point->GetPosition().phi()* radian
		    );
	    context_->energy_deposit_collector_.setMomentumThetaBeforeNozzle(
			    step->GetTrack() ->GetTrackID(),
			    prestep_point -> GetMomentum().theta() * degree
			    );
	    context_-> energy_deposit_collector_.setPositionThetaBeforeNozzle(
			    step->GetTrack() -> GetTrackID(),
			    prestep_point -> GetPosition().theta() * degree
			    );

        } else if (step->IsLastStepInVolume()) {
            context_->energy_deposit_collector_.setEnergyAfterNozzle(
                    step->GetTrack()->GetTrackID(),
                    poststep_point->GetTotalEnergy() * MeV
            );
            context_->energy_deposit_collector_.setMomentumEtaAfterNozzle(
                    step->GetTrack()->GetTrackID(),
		   
                    poststep_point->GetMomentum().getEta()
            );
            context_->energy_deposit_collector_.setMomentumPhiAfterNozzle(
                    step->GetTrack()->GetTrackID(),
		    
                    poststep_point->GetMomentum().getPhi() * radian
            );
    context_->energy_deposit_collector_.setPositionEtaAfterNozzle(
                    step->GetTrack()->GetTrackID(),
		   poststep_point->GetPosition().eta()
                   
            );
            context_->energy_deposit_collector_.setPositionPhiAfterNozzle(
                    step->GetTrack()->GetTrackID(),
		    poststep_point->GetPosition().phi()*radian
		    );
	    context_ ->energy_deposit_collector_.setMomentumThetaAfterNozzle(
			    step->GetTrack() -> GetTrackID(),
			    poststep_point -> GetMomentum(). theta() * degree
			    );
	    context_ ->energy_deposit_collector_.setPositionThetaAfterNozzle(
			    step ->GetTrack() ->GetTrackID(),
			    poststep_point ->GetPosition(). theta() * degree
			    );
                    

        }
    }
}

