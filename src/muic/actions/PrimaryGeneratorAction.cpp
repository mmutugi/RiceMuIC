#include <stdlib.h>
#include <G4Event.hh>
#include <G4Gamma.hh>
#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4SystemOfUnits.hh>

#include "muic/actions/PrimaryGeneratorAction.h"

namespace muic {
    PrimaryGeneratorAction::PrimaryGeneratorAction()
            : G4VUserPrimaryGeneratorAction() {
        gps = new G4GeneralParticleSource();

        // Get particle
        auto *particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");

        // Short-Circuit: Failed to find particle
        if (particle == nullptr) {
            return;
        }

        // G4ParticleDefinition* particle = G4Gamma::Definition();
        gps->SetParticleDefinition(particle);

        // Source will be a point
        gps->GetCurrentSource()->GetPosDist()->SetPosDisType("Point");

        // We will assume all particles are mono energetic
        gps->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
        gps->GetCurrentSource()->GetEneDist()->SetMonoEnergy(1000 * MeV);
	//initialize eta and theta
	{
		init_eta_max_=4.4;//3.5;//3.0;//6.4;5.7
		init_eta_min_=4.4;//3.5;//-3.0;//0.1;//6.0;//3.18;5.7
		double init_theta_one = 2*atan(exp(-init_eta_min_)) * rad;
		double init_theta_two = 2*atan(exp(-init_eta_max_)) * rad;
		init_theta_min_=fmin(init_theta_one , init_theta_two);
		init_theta_max_=fmax(init_theta_one , init_theta_two);

		



	
	}

        // Define opening angles
        gps->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");
        gps->GetCurrentSource()->GetAngDist()->SetMinTheta(init_theta_min_);
        gps->GetCurrentSource()->GetAngDist()->SetMaxTheta(init_theta_max_);
    }

    PrimaryGeneratorAction::~PrimaryGeneratorAction() {
        delete gps;
    }

    void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {
        gps->GeneratePrimaryVertex(event);
	//uncomment the following block to enable flat eta distribution
	double eta = (((double) rand())/RAND_MAX)*(init_eta_max_ - init_eta_min_)+init_eta_min_;
	double theta= 2 * atan(exp(-eta)) *rad;
	gps->GetCurrentSource()->GetAngDist()->SetMaxTheta(theta);
	gps->GetCurrentSource()->GetAngDist()->SetMinTheta(theta);

    }

}
