#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "muic/actions/EventAction.h"
#include "muic/actions/PrimaryGeneratorAction.h"
#include "muic/actions/RunAction.h"

#include "muic/setup/ActionInitialization.h"
#include "muic/actions/SteppingAction.h"

// Constructor
ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {
    // Do Nothing
}

// Destructor
ActionInitialization::~ActionInitialization() {
    // Do Nothing
}

void ActionInitialization::BuildForMaster() const {
    // Register Run Action
    auto *context = new muic::MuICContext();
    auto *run_action = new muic::RunAction(context);

    SetUserAction(run_action);
}

void ActionInitialization::Build() const {
    // Register Run Action
    auto *context = new muic::MuICContext();
    auto *run_action = new muic::RunAction(context);

    SetUserAction(run_action);

    // Register primary generator
    auto *primary_generator = new muic::PrimaryGeneratorAction();

    SetUserAction(primary_generator);

    // Initialize event handler
    auto *event_action = new muic::EventAction(context);

    SetUserAction(event_action);

    // Initialize muon tracking handler
    auto *muon_tracking_action = new muic::SteppingAction(context);

    SetUserAction(muon_tracking_action);
}


