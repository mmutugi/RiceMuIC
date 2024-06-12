#ifndef MUIC_SIMULATION_CSCSIMHIT_H
#define MUIC_SIMULATION_CSCSIMHIT_H

#include <G4VHit.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>

namespace muic {
    class CSCSimHit : public G4VHit {
    public:
        // Memory allocation and de-allocation
        inline void *operator new(size_t);

        inline void operator delete(void *);

        int GetStation() const { return station_; }

        void SetStation(int station) { station_ = station; }

        G4double GetTime() const { return time_; }

        void SetTime(G4double time) { time_ = time; }

        G4ThreeVector GetPosition() const { return position_; }

        void SetPosition(G4ThreeVector position) { position_ = position; }

    private:
        int station_;
        G4double time_;
        G4ThreeVector position_;
    };

    using CSCSimHitCollection = G4THitsCollection<CSCSimHit>;

    extern G4ThreadLocal G4Allocator<CSCSimHit> *csc_simhit_allocator;

    inline void *CSCSimHit::operator new(size_t) {
        if (!csc_simhit_allocator) {
            csc_simhit_allocator = new G4Allocator<CSCSimHit>;
        }

        return csc_simhit_allocator->MallocSingle();
    }

    inline void CSCSimHit::operator delete(void *simhit) {
        if (!csc_simhit_allocator) {
            csc_simhit_allocator = new G4Allocator<CSCSimHit>;
        }

        csc_simhit_allocator->FreeSingle((CSCSimHit *) simhit);
    }
}

#endif
