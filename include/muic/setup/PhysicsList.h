#ifndef MUIC_SETUP_PHYSICSLIST_H
#define MUIC_SETUP_PHYSICSLIST_H

#include "G4VUserPhysicsList.hh"
#include "globals.hh"


class PhysicsList : public G4VUserPhysicsList {
public:
    // Constructor
    PhysicsList();

    // Destructor
    ~PhysicsList();

protected:
    // Construye la particula y la fisica
    void ConstructParticle();

    void ConstructProcess();

    //! Define los  user cuts
    void SetCuts();

    // Construye las particulas
    void ConstructBosons();

    void ConstructLeptons();

    void ConstructMesons();

    void ConstructBaryons();

private:
    // Se Construye la fisica y la registra
    void ConstructGeneral();

    //! Define electro magnetic interactions
    void ConstructEM();

    // Define los decaimientos
    void ConstructDecay();

    // Deine la optica
    void ConstructOp();
};

#endif
