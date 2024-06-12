#include "muic/MuICContext.h"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "muic/actions/RunAction.h"

namespace muic {
    RunAction::RunAction(MuICContext *context) :
            G4UserRunAction(),
            context_(context) {
        // Do Nothing
    }

    RunAction::~RunAction() {
        // Do Nothing
    }

    void RunAction::BeginOfRunAction(const G4Run *run) {
        G4cout << "Begin Run " << run->GetRunID() << " start." << G4endl;

        // Start run in AM
        context_->ntuple_maker_.onRunStart();
    }

    void RunAction::EndOfRunAction(const G4Run *run) {
        G4int n_events = run->GetNumberOfEvent();

        // Short-Circuit: No events have been processed
        if (n_events == 0) {
            return;
        }

        // End run in AM
        context_->ntuple_maker_.onRunEnd();
    }
}

