#ifndef MuIC_NtupleMaker_h
#define MuIC_NtupleMaker_h

#include "globals.hh"
#include "muic/sensitive/MFSHit.h"
#include <vector>


class TFile;

class TTree;

namespace muic {
    class NtupleMaker {
    public:
        NtupleMaker();

        ~NtupleMaker();

        void onRunStart();

        void onRunEnd();

        void onEventStart();

        void addGenMuon(
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
        );

		void addHit(const muic::MFSHit &hit);

        void onEventEnd();

    private:
        // Metadata
        int n_events_ = 0;

        // Events
        int n_gen_ = 0;
        std::vector<double> energy_deposited_nozzle_;
        std::vector<double> energy_after_nozzle_;
        std::vector<double> energy_before_nozzle_;
		std::vector<double> momentum_phi_before_nozzle_;
		std::vector<double> momentum_phi_after_nozzle_;
		std::vector<double> momentum_phi_scatter_;
		std::vector<double> momentum_eta_before_nozzle_;
		std::vector< double> momentum_eta_after_nozzle_;
		std::vector <double> momentum_eta_scatter_;
		std::vector<double> position_phi_before_nozzle_;
		std::vector<double> position_phi_after_nozzle_;
		std::vector<double>position_eta_before_nozzle_;
		std::vector<double>position_eta_after_nozzle_;
		std::vector<double> position_eta_scatter_;
		std::vector<double>position_phi_scatter_;
		std::vector<double> momentum_theta_before_nozzle_;
		std::vector<double>momentum_theta_after_nozzle_;
		std::vector <double> position_theta_before_nozzle_;
		std::vector <double> position_theta_after_nozzle_;
		std::vector <double> momentum_theta_scatter_;
		std::vector <double> position_theta_scatter_;


		int n_hit_ = 0;

        std::vector<int> hit_station;
        std::vector<int> hit_chamber;
        std::vector<double> hit_glob_x;
        std::vector<double> hit_glob_y;
        std::vector<double> hit_glob_z;
        std::vector<double> hit_glob_t;
        std::vector<double> hit_glob_eta;
        std::vector<double> hit_glob_phi;


    };
}


#endif
