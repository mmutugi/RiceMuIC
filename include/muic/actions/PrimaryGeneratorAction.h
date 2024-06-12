#ifndef MUIC_ACTIONS_PRIMARYGENERATORACTION_H
#define MUIC_ACTIONS_PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"

// FOWARD DECLARATIONS
class G4GeneralParticleSource;

/// The primary generator action class
namespace muic {
    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
    public:
        // Constructor
        PrimaryGeneratorAction();

        // Destructor
        ~PrimaryGeneratorAction();

        // Main interface
        void GeneratePrimaries(G4Event *event) override;

    private:
        G4GeneralParticleSource *gps;
        G4double init_theta_min_=0;
		G4double init_theta_max_=0;
		G4double init_eta_min_=0;
		G4double init_eta_max_=0;
    };
}

#endif
