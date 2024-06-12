#include <G4SystemOfUnits.hh>
#include "G4ios.hh"
#include <g4root_defs.hh>
#include <G4GenericAnalysisManager.hh>

#include "muic/analysis/NtupleMaker.h"
#include "muic/utils/angles.h"
#include "muic/sensitive/MFSHit.h"

namespace muic {
    NtupleMaker::NtupleMaker() {
        auto *manager = G4GenericAnalysisManager::Instance();

        //
        manager->SetFirstNtupleId(0);
        manager->SetFirstNtupleColumnId(0);

        // Init metadata tree
        manager->CreateNtuple("metadata", "Metadata");
        manager->CreateNtupleIColumn("vsize_evt");
        manager->FinishNtuple();

        // Init event tree and create the columns
        manager->CreateNtuple("events", "Events");

		//count the events and generations.
        manager->CreateNtupleIColumn("vsize_gen");
		manager->CreateNtupleIColumn("vsize_hit");

		//get the information
        manager->CreateNtupleDColumn("energy_before_nozzle", energy_before_nozzle_);
        manager->CreateNtupleDColumn("energy_after_nozzle", energy_after_nozzle_);
        manager->CreateNtupleDColumn("energy_deposited_nozzle", energy_deposited_nozzle_);
        manager->CreateNtupleDColumn("momentum_phi_before_nozzle", momentum_phi_before_nozzle_);
		manager->CreateNtupleDColumn("momentum_phi_after_nozzle", momentum_phi_after_nozzle_);
		manager->CreateNtupleDColumn("momentum_gen_phi_scatter",momentum_phi_scatter_);
		manager->CreateNtupleDColumn("momentum_eta_before_nozzle", momentum_eta_before_nozzle_);
		manager->CreateNtupleDColumn("momentum_eta_after_nozzle", momentum_eta_after_nozzle_);
		manager->CreateNtupleDColumn ("momentum_eta_scatter", momentum_eta_scatter_);
		manager->CreateNtupleDColumn("position_phi_before_nozzle", position_phi_before_nozzle_);
		manager->CreateNtupleDColumn("position_phi_after_nozzle", position_phi_after_nozzle_);
		manager->CreateNtupleDColumn("position_eta_before_nozzle",position_eta_before_nozzle_);
		manager->CreateNtupleDColumn("position_eta_after_nozzle",position_eta_after_nozzle_);
		manager->CreateNtupleDColumn("position_phi_scattering", position_phi_scatter_);
		manager->CreateNtupleDColumn("position_eta_scattering",position_eta_scatter_);
		manager->CreateNtupleDColumn ("momentum_theta_before_nozzle",momentum_theta_before_nozzle_);
		manager->CreateNtupleDColumn ("momentum_heta_after_nozzle", momentum_theta_after_nozzle_);
		manager ->CreateNtupleDColumn("position_theta_before_nozzle", position_theta_before_nozzle_);
		manager->CreateNtupleDColumn( "position theta after nozzle ", position_theta_after_nozzle_);
		manager->CreateNtupleDColumn("momentum_theta_scatter",momentum_theta_scatter_);
		manager->CreateNtupleDColumn("position_theta_scatter",position_theta_scatter_);

	//add hit information with this
		manager->CreateNtupleIColumn("hit_station", hit_station);
        manager->CreateNtupleIColumn("hit_chamber", hit_chamber);
        manager->CreateNtupleDColumn("hit_glob_x", hit_glob_x);
        manager->CreateNtupleDColumn("hit_glob_y", hit_glob_y);
        manager->CreateNtupleDColumn("hit_glob_z", hit_glob_z);
        manager->CreateNtupleDColumn("hit_glob_t", hit_glob_t);
        manager->CreateNtupleDColumn("hit_glob_eta", hit_glob_eta);
        manager->CreateNtupleDColumn("hit_glob_phi", hit_glob_phi);


	

	//Close the event with this
        manager->FinishNtuple();

        // Open File
        auto file_open = manager->OpenFile("muic_ntuple.root");

        if (!file_open) {
            G4cout << "Failed to open file." << G4endl;
            return;
        }
    }

    NtupleMaker::~NtupleMaker() {
        // Do Nothing
    }

    void NtupleMaker::onRunStart() {
        // Do Nothing
    }

    void NtupleMaker::onRunEnd() {
        auto *analysis_manager = G4GenericAnalysisManager::Instance();

        G4cout << "Writing Ntuple." << G4endl;
        analysis_manager->Write();

        G4cout << "Closing Root File." << G4endl;
        analysis_manager->CloseFile();
    }

    void NtupleMaker::onEventStart() {
        // Increase number of events
        n_events_ += 1;

        // Nozzle Energy Deposit
        n_gen_ = 0;
        energy_before_nozzle_.clear();
        energy_after_nozzle_.clear();
        energy_deposited_nozzle_.clear();
		momentum_phi_before_nozzle_.clear();
		momentum_phi_after_nozzle_.clear();
		momentum_phi_scatter_.clear();
		momentum_eta_before_nozzle_.clear();
		momentum_eta_after_nozzle_.clear();
		momentum_eta_scatter_.clear();
	
	 	position_phi_before_nozzle_.clear();
	 	position_phi_after_nozzle_.clear();
	 	position_eta_before_nozzle_.clear();
	 	position_eta_after_nozzle_.clear();
	 	position_eta_scatter_.clear();
	 	position_phi_scatter_.clear();
	 	momentum_theta_before_nozzle_.clear();
	 	momentum_theta_after_nozzle_.clear();
	 	position_theta_before_nozzle_.clear();
	 	position_theta_after_nozzle_.clear();

	 	momentum_theta_scatter_.clear();
	 	position_theta_scatter_.clear();

	 //clear all for the Hits section of the detector

	 	n_hit_ = 0;
        hit_station.clear();
        hit_chamber.clear();
        hit_glob_x.clear();
        hit_glob_y.clear();
        hit_glob_z.clear();
        hit_glob_t.clear();
        hit_glob_eta.clear();
        hit_glob_phi.clear();

	 

    }

    void NtupleMaker::onEventEnd() {
        auto *manager = G4GenericAnalysisManager::Instance();

        // Fill Metadata Ntuple
        manager->FillNtupleIColumn(0, 0, n_events_);
        manager->AddNtupleRow(0);

        // Fill Event ntuple
        manager->FillNtupleIColumn(1, 0, n_gen_);
		manager->FillNtupleIColumn(1, 1, n_hit_);
        manager->AddNtupleRow(1);
    }
 
 //write the details into the Ntuples themseves
    void NtupleMaker::addGenMuon(
       	 	const G4double &energy_before_nozzle,
        	const G4double &energy_after_nozzle,
        	const G4double &energy_deposited_nozzle,
	    	const G4double &momentum_phi_before_nozzle,
	    	const G4double &momentum_phi_after_nozzle,
	    	const G4double &momentum_eta_before_nozzle,
	    	const G4double &momentum_eta_after_nozzle,
	    
	    
	     	const G4double &position_phi_before_nozzle,
	     	const G4double &position_phi_after_nozzle,
	     	const G4double &position_eta_before_nozzle,
	     	const G4double &position_eta_after_nozzle,
	     	const G4double &momentum_theta_before_nozzle,
	     	const G4double &momentum_theta_after_nozzle,
	     	const G4double &position_theta_before_nozzle,
	     	const G4double &position_theta_after_nozzle
	     
	      

    ) {
	G4double  momentum_phi_scatter = (momentum_phi_before_nozzle - momentum_phi_after_nozzle);
	if(momentum_phi_scatter > 180) 
		momentum_phi_scatter += -360.0;
	else if (momentum_phi_scatter < -180)
		momentum_phi_scatter +=360.0;

         G4double momentum_eta_scatter = (momentum_eta_before_nozzle - momentum_eta_after_nozzle);
	 G4double position_eta_scatter = (position_eta_before_nozzle - position_eta_after_nozzle);
	 G4double position_phi_scatter = (position_phi_before_nozzle - position_phi_after_nozzle);
	 
	if(position_phi_scatter > 180) 
		position_phi_scatter += -360.0;
	else if (position_phi_scatter < -180)
		position_phi_scatter +=360.0;

	G4double momentum_theta_scatter = (momentum_theta_before_nozzle - momentum_theta_after_nozzle);

	if(momentum_theta_scatter > 180) 
		momentum_theta_scatter += -360.0;
	else if (momentum_theta_scatter < -180)
		momentum_theta_scatter +=360.0;


	G4double position_theta_scatter = (position_theta_before_nozzle - position_theta_after_nozzle);

	if(position_theta_scatter > 180) 
		position_theta_scatter += -360.0;
	else if (position_theta_scatter < -180)
		position_theta_scatter +=360.0;

    	n_gen_ += 1;
    	energy_before_nozzle_.push_back(energy_before_nozzle / GeV);
    	energy_after_nozzle_.push_back(energy_after_nozzle / GeV);
    	energy_deposited_nozzle_.push_back(energy_deposited_nozzle / GeV);
		momentum_phi_before_nozzle_.push_back(momentum_phi_before_nozzle / degree);
		momentum_phi_after_nozzle_.push_back(momentum_phi_after_nozzle / degree);
		momentum_phi_scatter_.push_back(momentum_phi_scatter /degree);
		momentum_eta_before_nozzle_.push_back(momentum_eta_before_nozzle);
		momentum_eta_after_nozzle_.push_back(momentum_eta_after_nozzle);
		momentum_eta_scatter_.push_back(momentum_eta_scatter);
		position_eta_scatter_.push_back(position_eta_scatter);
		position_phi_scatter_.push_back(position_phi_scatter / degree);


	
		position_phi_before_nozzle_.push_back(position_phi_before_nozzle /degree);
		position_phi_after_nozzle_.push_back(position_phi_after_nozzle / degree);
		position_eta_before_nozzle_.push_back(position_eta_before_nozzle);
    	position_eta_after_nozzle_.push_back(position_eta_after_nozzle);
		momentum_theta_before_nozzle_.push_back(momentum_theta_before_nozzle / degree );
		momentum_theta_after_nozzle_.push_back(momentum_theta_after_nozzle /degree);
		position_theta_before_nozzle_.push_back(position_theta_before_nozzle / degree);
		position_theta_after_nozzle_.push_back(position_theta_after_nozzle / degree);

		momentum_theta_scatter_.push_back(momentum_theta_scatter /degree);
		position_theta_scatter_.push_back(position_theta_scatter / degree);
	}

	//add the hits information here
	void NtupleMaker::addHit(
			const MFSHit &hit
	) {
		n_hit_ += 1;
    	hit_station.push_back(hit.GetStation());
    	hit_chamber.push_back(hit.GetChamber());
    	hit_glob_x.push_back(hit.GetPosition().getX() * mm / cm);  //  cm
    	hit_glob_y.push_back(hit.GetPosition().getY() * mm / cm);  //  cm
    	hit_glob_z.push_back(hit.GetPosition().getZ() * mm / cm);  //  cm
    	hit_glob_t.push_back(hit.GetTime());  // ns
    	hit_glob_eta.push_back(hit.GetPosition().getEta());
    	hit_glob_phi.push_back(hit.GetPosition().getPhi() * rad / deg);  //  deg






		}






	


	 
    }


