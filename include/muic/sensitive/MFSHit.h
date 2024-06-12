#ifndef MUICSIMULATION_MFSHIT_H
#define MUICSIMULATION_MFSHIT_H

#include <G4VHit.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>

namespace muic {

    class MFSHit : public G4VHit {
    public:
        inline void *operator new(size_t);

        inline void operator delete(void *);

        int GetStation() const { return station_; }

        void SetStation(int station) { station_ = station; }

        int GetChamber() const { return chamber_; }

        void SetChamber(int chamber) { chamber_ = chamber; }

        G4double GetTime() const { return time_; }

        void SetTime(G4double glob_time) { time_ = glob_time; }

        G4ThreeVector GetPosition() const { return position_; }

        void SetPosition(G4ThreeVector glob_pos) { position_ = glob_pos; }

    private:
        int station_;
        int chamber_;
        G4double time_;
        G4ThreeVector position_;
    };

    typedef G4THitsCollection<MFSHit> MFSHitCollection;

    extern G4ThreadLocal G4Allocator<MFSHit> *mfs_hit_allocator;

    inline void *MFSHit::operator new(size_t) {
        if (!mfs_hit_allocator) {
            mfs_hit_allocator = new G4Allocator<MFSHit>;
        }

        return mfs_hit_allocator->MallocSingle();
    }

    inline void MFSHit::operator delete(void *aHit) {
        if (!mfs_hit_allocator) {
            mfs_hit_allocator = new G4Allocator<MFSHit>;
        }

        mfs_hit_allocator->FreeSingle((MFSHit *) aHit);
    }
}

#endif //MUICSIMULATION_MFSHIT_H
