#ifndef MUIC_ACTIONS_STEPPINGACTION_H
#define MUIC_ACTIONS_STEPPINGACTION_H

#include <G4UserSteppingAction.hh>

#include "muic/MuICContext.h"

namespace muic {
    class SteppingAction : public G4UserSteppingAction {
    public:
        SteppingAction(muic::MuICContext *);

        ~SteppingAction();

        void SetSteppingManagerPointer(G4SteppingManager *);

        void UserSteppingAction(const G4Step *);
    private:
        MuICContext* context_;
    };
}


#endif //MUIC_ACTIONS_STEPPINGACTION_H
