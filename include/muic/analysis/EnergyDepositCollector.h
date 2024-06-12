//
//Modified by mm207 on 19/04/24

#ifndef MUIC_ANALYSIS_MUONENERGYDEPOSITCOLLECTOR_H
#define MUIC_ANALYSIS_MUONENERGYDEPOSITCOLLECTOR_H


#include <map>
#include <G4Types.hh>

class EnergyDepositCollector {
    
public:
    EnergyDepositCollector();
    ~EnergyDepositCollector();

    void onEventStart();
    
    void onEventEnd();
    void setMomentumEtaBeforeNozzle(const unsigned int &track_id, const G4double &eta);
    void setMomentumEtaAfterNozzle(const unsigned int &track_id, const G4double &eta);
    void setMomentumPhiBeforeNozzle(const unsigned int &track_id, const G4double &phi);
    void setMomentumPhiAfterNozzle(const unsigned int &track_id, const G4double &phi);

    void addMomentumPhiScatter(const unsigned int& track_id, const G4double &);
    void addMomentumEtaScatter(const unsigned int& track_id, const G4double &);
    

    void setPositionPhiBeforeNozzle(const unsigned int &track_id, const G4double &phi);
    void setPositionPhiAfterNozzle (const unsigned int &track_id, const G4double &phi);
    void setPositionEtaBeforeNozzle (const unsigned int &track_id, const G4double &eta);
    void setPositionEtaAfterNozzle(const unsigned int &track_id, const G4double &eta);
   

    void setEnergyBeforeNozzle(const unsigned int &track_id, const G4double &energy);
    void setEnergyAfterNozzle(const unsigned int &track_id, const G4double &energy);
    void addEnergyDeposit(const unsigned int&, const G4double&);

    void addPositionEtaScatter(const unsigned int& track_id, const G4double &);
    void addPositionPhiScatter(const unsigned int& track_id, const G4double &);
    void setMomentumThetaBeforeNozzle (const unsigned int& track_id , const G4double &theta);
    void setMomentumThetaAfterNozzle(const unsigned int& track_id, const G4double &theta);
    void setPositionThetaBeforeNozzle(const unsigned int &track_id, const G4double &theta);
    void setPositionThetaAfterNozzle(const unsigned int &track_id, const G4double &theta);

    void addMomentumThetaScatter(const unsigned int &track_id, const G4double &);
    void addPositionThetaScatter(const unsigned int &track_id, const G4double &);

public:
    std::map<unsigned int, double> momentum_eta_before_nozzle_collection_;
    std::map<unsigned int, double> momentum_eta_after_nozzle_collection_;
    std::map<unsigned int, double> momentum_phi_before_nozzle_collection_;
    std::map<unsigned int, double> momentum_phi_after_nozzle_collection_;
    std::map<unsigned int, double> momentum_phi_scatter_;
    std::map<unsigned int, double> energy_before_nozzle_collection_;
    std::map<unsigned int, double> energy_after_nozzle_collection_;
    std::map<unsigned int, double> energy_deposit_collection_;
    std::map<unsigned int, double> momentum_eta_scatter_;
    

    std::map<unsigned int,double>position_phi_before_nozzle_collection_;
    std::map< unsigned int , double>position_phi_after_nozzle_collection_;
    std::map< unsigned int, double> position_eta_before_nozzle_collection_;
    std::map<unsigned int, double>position_eta_after_nozzle_collection_;
    std::map<unsigned int, double> position_eta_scatter_;
    std::map<unsigned int, double>position_phi_scatter_;
    std::map<unsigned int, double> momentum_theta_before_nozzle_;
    std::map<unsigned int, double> momentum_theta_after_nozzle_;
    std::map<unsigned int, double> position_theta_before_nozzle_;
    std::map<unsigned int, double >position_theta_after_nozzle_;
    std::map<unsigned int, double>momentum_theta_scatter_;
    std::map<unsigned int, double> position_theta_scatter_;
    

};


#endif //MUIC_ANALYSIS_MUONENERGYDEPOSITCOLLECTOR_H
