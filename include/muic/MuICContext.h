#ifndef MUICSIMULATION_MUICCONTEXT_H
#define MUICSIMULATION_MUICCONTEXT_H


#include "muic/analysis/NtupleMaker.h"
#include "muic/analysis/EnergyDepositCollector.h"

namespace muic {
    class MuICContext {
    public:
        MuICContext();

        ~MuICContext();

        NtupleMaker ntuple_maker_;
        EnergyDepositCollector energy_deposit_collector_;
    };
}


#endif //MUICSIMULATION_MUICCONTEXT_H
