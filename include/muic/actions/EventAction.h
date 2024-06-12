#ifndef MUIC_ACTIONS_EVENTACTION_H
#define MUIC_ACTIONS_EVENTACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "muic/MuICContext.h"

namespace muic {
    class EventAction : public G4UserEventAction {
    public:
        EventAction(
                MuICContext *
        );

        virtual ~EventAction();

    public:
        void BeginOfEventAction(const G4Event *);

        void EndOfEventAction(const G4Event *);

    private:
        MuICContext *context_ = nullptr;

        int mfs_hit_collection_id_ = -1;
        G4int print_every_n_events_ = 1000;

    };
}

#endif
