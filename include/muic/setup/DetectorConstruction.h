#ifndef MUIC_SETUP_DETECTORCONSTRUCTION_H
#define MUIC_SETUP_DETECTORCONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    // Constructor
    DetectorConstruction();

    // Destructor
    virtual ~DetectorConstruction();

    // Defines the detector geometry and returns a pointer to the physical World Volume
    G4VPhysicalVolume *Construct() override;

    // Register some of the detector's volumes as "simulation"
    // Add the following, if you have fields / simulation detectors
    void ConstructSDandField() override;

private:
    std::map<std::string, int> pv_to_station_lut;

};

#endif

