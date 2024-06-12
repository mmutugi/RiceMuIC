#ifndef MUIC_SETUP_ACTIONINITIALIZATION_H
#define MUIC_SETUP_ACTIONINITIALIZATION_H

#include "G4VUserActionInitialization.hh"


/// Action initialization class.
class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization();

    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;

    virtual void Build() const;
};

#endif
