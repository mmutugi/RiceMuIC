#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"

#include "muic/setup/PhysicsList.h"

// Decays
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4DecayWithSpin.hh"
#include "G4EmStandardPhysics.hh"
#include "G4LossTableManager.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"
#include "globals.hh"

// Gamma Interactions
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

// Electron Interactions
#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4eplusAnnihilation.hh"

// Muon Interactions
#include "G4MuBremsstrahlung.hh"
#include "G4MuIonisation.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuPairProduction.hh"

// Pion Interactions
#include "G4hIonisation.hh"
#include "G4hMultipleScattering.hh"

#include "G4StepLimiter.hh"

PhysicsList::PhysicsList() : G4VUserPhysicsList() {
    defaultCutValue = 0.1 * mm;
    SetVerboseLevel(1);
}

PhysicsList::~PhysicsList() {
}

void PhysicsList::ConstructParticle() {
    // In this method, static member functions should be called
    // for all particles which you want to use.
    // This ensures that objects of these particle types will be
    // created in the program.

    ConstructBosons();
    ConstructLeptons();
    ConstructMesons();
    ConstructBaryons();

    // ********************************************************************************
    // To activate polarisation
    //   -  uncomment the lines below
    //   -  change the decay process in  PhysicsList::ConstructDecay() to
    //      G4DecayWithSpin
    // ********************************************************************************
    G4DecayTable *MuonPlusDecayTable = new G4DecayTable();
    MuonPlusDecayTable->Insert(
            new G4MuonDecayChannelWithSpin("mu+", 0.986)
    );
    MuonPlusDecayTable->Insert(
            new G4MuonRadiativeDecayChannelWithSpin("mu+", 0.014)
    );
    G4MuonPlus::MuonPlusDefinition()->SetDecayTable(MuonPlusDecayTable);

    G4DecayTable *MuonMinusDecayTable = new G4DecayTable();
    MuonMinusDecayTable->Insert(
            new G4MuonDecayChannelWithSpin("mu-", 0.986)
    );
    MuonMinusDecayTable->Insert(
            new G4MuonRadiativeDecayChannelWithSpin("mu-", 0.014)
    );
    G4MuonMinus::MuonMinusDefinition()->SetDecayTable(MuonMinusDecayTable);
}

void PhysicsList::ConstructBosons() {
    // pseudo-particles
    G4Geantino::GeantinoDefinition();
    G4ChargedGeantino::ChargedGeantinoDefinition();

    // gamma
    // G4Gamma::Gamma(); // [yy]
    G4Gamma::GammaDefinition();

    // optical photon
    G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsList::ConstructLeptons() {
    // leptons
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
    G4MuonPlus::MuonPlusDefinition();
    G4MuonMinus::MuonMinusDefinition();

    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();
    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void PhysicsList::ConstructMesons() {
    //  mesons
    G4PionPlus::PionPlusDefinition();
    G4PionMinus::PionMinusDefinition();
    G4PionZero::PionZeroDefinition();
    G4Eta::EtaDefinition();
    G4EtaPrime::EtaPrimeDefinition();
    G4KaonPlus::KaonPlusDefinition();
    G4KaonMinus::KaonMinusDefinition();
    G4KaonZero::KaonZeroDefinition();
    G4AntiKaonZero::AntiKaonZeroDefinition();
    G4KaonZeroLong::KaonZeroLongDefinition();
    G4KaonZeroShort::KaonZeroShortDefinition();
}

void PhysicsList::ConstructBaryons() {
    //  barions
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();
    G4Neutron::NeutronDefinition();
    G4AntiNeutron::AntiNeutronDefinition();
}

void PhysicsList::ConstructProcess() {
    AddTransportation();
    ConstructEM();
    ConstructGeneral();
    ConstructDecay(); // [yy]

#ifdef OPT_ON
    ConstructOp();
#endif
}

void PhysicsList::ConstructEM() {
    auto *particle_it = GetParticleIterator();
    particle_it->reset();

    while ((*particle_it)()) {
        auto *particle = particle_it->value();
        auto *process_manager = particle->GetProcessManager();
        auto particle_name = particle->GetParticleName();

        if (particle_name == "gamma") {
            // gamma
            process_manager->AddDiscreteProcess(new G4PhotoElectricEffect);
            process_manager->AddDiscreteProcess(new G4ComptonScattering);
            process_manager->AddDiscreteProcess(new G4GammaConversion);

        } else if (particle_name == "e-") {
            // electron
            process_manager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            process_manager->AddProcess(new G4eIonisation, -1, 2, 2);
            process_manager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);

        } else if (particle_name == "e+") {
            // positron
            process_manager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            process_manager->AddProcess(new G4eIonisation, -1, 2, 2);
            process_manager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);
            process_manager->AddProcess(new G4eplusAnnihilation, 0, -1, 4);

        } else if (particle_name == "mu+" || particle_name == "mu-") {
            // muon
            process_manager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
            process_manager->AddProcess(new G4MuIonisation, -1, 2, 2);
            process_manager->AddProcess(new G4MuBremsstrahlung, -1, 3, 3);
            process_manager->AddProcess(new G4MuPairProduction, -1, 4, 4);

        } else if (
                (!particle->IsShortLived()) &&
                (particle->GetPDGCharge() != 0.0) &&
                (particle->GetParticleName() != "chargedgeantino")
                ) {
            // all others charged particles except geantino
            process_manager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            process_manager->AddProcess(new G4hIonisation, -1, 2, 2);
        }
    }
}

#include "G4Decay.hh"

void PhysicsList::ConstructGeneral() {
    // Add Decay Process
    auto *decay_process = new G4Decay();
    auto *step_limiter = new G4StepLimiter;
    auto *particle_it = GetParticleIterator();
    particle_it->reset();

    while ((*particle_it)()) {
        auto *particle = particle_it->value();
        auto *process_manager = particle->GetProcessManager();

        if (
                decay_process->IsApplicable(*particle)
                ) {
            process_manager->AddProcess(decay_process);
            // set ordering for PostStepDoIt and AtRestDoIt
            process_manager->SetProcessOrdering(decay_process, idxPostStep);
            process_manager->SetProcessOrdering(decay_process, idxAtRest);
        }

        if (
                (!particle->IsShortLived()) && (particle->GetPDGCharge() != 0.0) &&
                (particle->GetParticleName() != "chargedgeantino")
                ) {
            // step limit
            process_manager->AddDiscreteProcess(step_limiter);
        }
    }
}

#include "G4Cerenkov.hh"

void PhysicsList::ConstructOp() {
    G4Cerenkov *cerenkov_process = new G4Cerenkov("Cerenkov");
    cerenkov_process->SetMaxNumPhotonsPerStep(300);
    cerenkov_process->SetTrackSecondariesFirst(true);

    auto *particle_it = GetParticleIterator();
    particle_it->reset();

    while ((*particle_it)()) {
        auto *particle = particle_it->value();
        auto *process_manager = particle->GetProcessManager();
        auto particle_name = particle->GetParticleName();

        if (cerenkov_process->IsApplicable(*particle)) {
            process_manager->AddProcess(cerenkov_process);
            process_manager->SetProcessOrdering(cerenkov_process, idxPostStep);
        }
    }
}

// http://geant4.lngs.infn.it/corso_infn/doxygen/task3/task3b/index.html
void PhysicsList::ConstructDecay() {
    auto *decay_process = new G4DecayWithSpin();

    auto *mu_plus = G4MuonPlus::MuonPlusDefinition();
    auto *mu_minus = G4MuonMinus::MuonMinusDefinition();

    auto *mu_plus_manager = mu_plus->GetProcessManager();
    mu_plus_manager->AddProcess(decay_process);
    mu_plus_manager->SetProcessOrdering(decay_process, idxPostStep);
    mu_plus_manager->SetProcessOrdering(decay_process, idxAtRest);

    auto *mu_minus_manager = mu_minus->GetProcessManager();
    mu_minus_manager->AddProcess(decay_process);
    mu_minus_manager->SetProcessOrdering(decay_process, idxPostStep);
    mu_minus_manager->SetProcessOrdering(decay_process, idxAtRest);
}

void PhysicsList::SetCuts() {
    if (verboseLevel > 0) {
        G4cout << "muPhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue, "Length")
               << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    //
    SetCutValue(defaultCutValue, "gamma");
    SetCutValue(defaultCutValue, "e-");
    SetCutValue(defaultCutValue, "e+");

    if (verboseLevel > 0) {
        DumpCutValuesTable();
    }
}
