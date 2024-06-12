#ifndef MUIC_ANALYSIS_GENPARTICLECOLLECTOR_H
#define MUIC_ANALYSIS_GENPARTICLECOLLECTOR_H


#include <G4Types.hh>
#include <map>
#include <vector>

class GenParticleCollector {
public:
    GenParticleCollector();
    ~GenParticleCollector();

    void onEventStart();

    void onEventEnd();

    void addEta(const unsigned int&, const G4double&);
    void addPhi(const unsigned int&, const G4double&);
    void addPx(const unsigned int&, const G4double&);
    void addPy(const unsigned int&, const G4double&);
    void addPz(const unsigned int&, const G4double&);
    void addVx(const unsigned int&, const G4double&);
    void addVy(const unsigned int&, const G4double&);
    void addVz(const unsigned int&, const G4double&);

private:
    std::map<unsigned int, unsigned int> track_id_to_gen_id_lut;
    std::vector<G4double> eta;
    std::vector<G4double> phi;
    std::vector<G4double> px;
    std::vector<G4double> py;
    std::vector<G4double> pz;
    std::vector<G4double> vx;
    std::vector<G4double> vy;
    std::vector<G4double> vz;
};


#endif //MUIC_ANALYSIS_GENPARTICLECOLLECTOR_H
