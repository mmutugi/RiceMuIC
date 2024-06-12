#ifndef MUIC_ACTIONS_RUNACTION_H
#define MUIC_ACTIONS_RUNACTION_H

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

namespace muic {
    class MuICContext;

    class RunAction : public G4UserRunAction {
    public:
        RunAction(MuICContext *);

        virtual ~RunAction();

        // virtual G4Run* GenerateRun();
        virtual void BeginOfRunAction(const G4Run *);

        virtual void EndOfRunAction(const G4Run *);

    private:
        MuICContext *context_ = nullptr;
    };

}

#endif