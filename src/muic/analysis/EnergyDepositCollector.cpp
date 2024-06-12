//
// Created by mm207 on 4/15
//

#include <G4ios.hh>
#include <G4SystemOfUnits.hh>
#include "muic/analysis/EnergyDepositCollector.h"

EnergyDepositCollector::EnergyDepositCollector() {
    // Do Nothing
}

EnergyDepositCollector::~EnergyDepositCollector() {
    // Do Nothing
}

void EnergyDepositCollector::onEventStart() {
    // Reset collection
    energy_before_nozzle_collection_.clear();
    energy_after_nozzle_collection_.clear();
    energy_deposit_collection_.clear();
    momentum_phi_before_nozzle_collection_.clear();
    momentum_phi_after_nozzle_collection_.clear();
    momentum_phi_scatter_.clear();
    momentum_eta_before_nozzle_collection_.clear();
    momentum_eta_after_nozzle_collection_.clear();
    momentum_eta_scatter_.clear();
    position_phi_before_nozzle_collection_.clear();
    position_phi_after_nozzle_collection_.clear();
    position_eta_before_nozzle_collection_.clear();
    position_eta_before_nozzle_collection_.clear();
    position_eta_scatter_.clear();
    position_phi_scatter_.clear();
    momentum_theta_before_nozzle_.clear();
    momentum_theta_after_nozzle_.clear();
    position_theta_before_nozzle_.clear();
    position_theta_after_nozzle_.clear();
    momentum_theta_scatter_.clear();
    position_theta_scatter_.clear();
    
}

void EnergyDepositCollector::onEventEnd() {
	for(auto& [track_id, initial_phi] : momentum_phi_before_nozzle_collection_){
	G4cout << "Track Id: " << track_id
		<<" momentum angle before the nozzle collection: " << initial_phi / degree
		<< " momentum angle after the nozzle collection " << momentum_phi_after_nozzle_collection_[track_id] / degree
		<< "momentum phi scatter"<< (momentum_phi_after_nozzle_collection_[track_id] - initial_phi)
		<< G4endl;
	
	
	}

	for (auto& [track_id, initial_eta] : momentum_eta_before_nozzle_collection_){
	G4cout << "Track Id: " << track_id
	       << " momentum eta before the nozzle collection: " << initial_eta
               <<" momentum eta after the nozzle collection: "	<< momentum_eta_after_nozzle_collection_[track_id]
	       << "momentum eta scatter"<< (momentum_eta_after_nozzle_collection_[track_id] - initial_eta)
	       << G4endl;

	}	
     for (auto& [track_id, position_initial_phi]: position_phi_before_nozzle_collection_){
	     G4cout <<"Track Id:" <<track_id
		     <<"Position Phi before Nozzle Collection:"<< position_initial_phi
		     <<"Position Phi after nozzle collection"<< position_phi_after_nozzle_collection_[track_id]
		     <<"position Phi Scatter"<< (position_phi_after_nozzle_collection_[track_id] - position_initial_phi)
		     <<G4endl;
	
	}
     for (auto& [track_id, position_initial_eta]: position_eta_before_nozzle_collection_){
              G4cout<< "Track Id:" << track_id
		     <<"Position eta before Nozzle Collection:"<< position_initial_eta
		     << "Position Initial Eta after Nozzle collection"<< position_eta_after_nozzle_collection_[track_id]
		     <<"position Eta scattering"<< (position_eta_after_nozzle_collection_[track_id] - position_initial_eta)
	             <<G4endl;
	 
		     }

     for (auto& [track_id,initial_theta]:momentum_theta_before_nozzle_) {
          G4cout << "Track ID:" <<track_id
		  <<"momentum theta before nozzle collection" << initial_theta
		  <<"momentum theta after nozzle collection "<< momentum_theta_after_nozzle_[track_id]
		  <<"momentum theta scattering"<<(momentum_theta_after_nozzle_[track_id] - initial_theta) /degree //theta_before_nozzle_[track_id]) / degree
		  <<G4endl;

     
     
     }
	
   for(auto&[track_id, position_initial_theta] : position_theta_before_nozzle_){
     G4cout <<"Track ID" << track_id
	     <<"position theta before nozzle collection"<< position_initial_theta
	     <<"position theta after nozzle collection"<< position_theta_after_nozzle_[track_id]
	     <<"positiom theta scattering"<<(position_theta_after_nozzle_[track_id] - position_theta_before_nozzle_[track_id]) /degree
	     <<G4endl;

}	

    for (auto& [track_id, deposit] : energy_deposit_collection_) {
        G4cout << "Track Id: " << track_id
        << " Deposit: " << deposit / GeV
        << " Before: " << energy_before_nozzle_collection_[track_id] / GeV
        << " After: " << energy_after_nozzle_collection_[track_id] / GeV
        << " Loss: " << (energy_before_nozzle_collection_[track_id] - energy_after_nozzle_collection_[track_id]) / GeV
        << G4endl;
    }
}

void EnergyDepositCollector::setMomentumEtaBeforeNozzle(const unsigned int & track_id, const G4double & eta) {
    momentum_eta_before_nozzle_collection_[track_id] = eta;
}

void EnergyDepositCollector::setMomentumEtaAfterNozzle(const unsigned int & track_id, const G4double & eta) {
    momentum_eta_after_nozzle_collection_[track_id] = eta;
}
void EnergyDepositCollector::setMomentumPhiBeforeNozzle(const unsigned int & track_id, const G4double & phi) {
    momentum_phi_before_nozzle_collection_[track_id] = phi;
}

void EnergyDepositCollector::setMomentumPhiAfterNozzle(const unsigned int & track_id, const G4double & phi) {
    momentum_phi_after_nozzle_collection_[track_id] = phi;
}
void EnergyDepositCollector::setEnergyBeforeNozzle(const unsigned int & track_id, const G4double & energy) {
    energy_before_nozzle_collection_[track_id] = energy;
}

void EnergyDepositCollector::setEnergyAfterNozzle(const unsigned int & track_id, const G4double & energy) {
    energy_after_nozzle_collection_[track_id] = energy;
}

void EnergyDepositCollector::addEnergyDeposit(const unsigned int & track_id, const G4double & energy) {
    auto energy_deposit_it = energy_deposit_collection_.find(track_id); 

 // Fetch previous energy deposit
    G4double deposit = 0.;

    if (energy_deposit_it != energy_deposit_collection_.end()) {
        deposit += energy_deposit_it->second;
    } 

    // Accumulate energy
    deposit += energy;

    // Record new energy deposit
    energy_deposit_collection_[track_id] = deposit;

}

 /*void EnergyDepositCollector::addPhiBending (const unsigned int & track_id, const G4double &energy) {
	 auto phi_bending = energy_deposit_collection_.find(track_id);
 }
void EnergyDepositCollector::addEtaBending (const unsigned int & track_id, const G4double &energy) {
	auto eta_bending = energy_deposit_collection_.find(track_id);


}*/
void EnergyDepositCollector::setPositionPhiBeforeNozzle(const unsigned int & track_id, const G4double &phi){
	position_phi_before_nozzle_collection_[track_id] = phi;}

void EnergyDepositCollector::setPositionPhiAfterNozzle(const unsigned int & track_id, const G4double &phi){
	position_phi_after_nozzle_collection_[track_id]=phi ;}

void EnergyDepositCollector::setPositionEtaBeforeNozzle(const unsigned int &track_id, const G4double & eta){
	position_eta_before_nozzle_collection_[track_id] = eta;}

void EnergyDepositCollector::setPositionEtaAfterNozzle(const unsigned int & track_id, const G4double &eta){
	position_eta_after_nozzle_collection_[track_id] =eta ;}
void EnergyDepositCollector::setMomentumThetaBeforeNozzle(const unsigned int &track_id, const G4double &theta){
	momentum_theta_before_nozzle_[track_id] = theta ;
}
void EnergyDepositCollector::setMomentumThetaAfterNozzle (const unsigned int &track_id, const G4double &theta){
	momentum_theta_after_nozzle_[track_id] = theta;
}
void EnergyDepositCollector::setPositionThetaBeforeNozzle(const unsigned int &track_id ,const G4double &theta){
	position_theta_before_nozzle_[track_id] = theta;
}
void EnergyDepositCollector::setPositionThetaAfterNozzle( const unsigned int &track_id,const  G4double &theta){
	position_theta_after_nozzle_[track_id] = theta;
}
	

  

