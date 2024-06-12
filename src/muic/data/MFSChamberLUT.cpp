#include "boost/format.hpp"
#include "muic/data/MFSChamberLUT.h"

muic::MFSChamberLUT::MFSChamberLUT() {
    int chamber_global_id = 0;

    for (int station_id = 0; station_id < 4; ++station_id) {
        for (int chamber_id = 0; chamber_id < 18; ++chamber_id) {
            int station_no = station_id + 1;
            int chamber_no = chamber_id + 1;

            auto detid = boost::str(
                    boost::format("MFS_%d_%d.") % station_no % chamber_no
            );

            id_to_detid_[chamber_global_id] = detid;
            pv_to_station_[detid] = station_no;
            pv_to_chamber_[detid] = chamber_no;

            chamber_global_id += 1;
        }
    }
}

std::string muic::MFSChamberLUT::getDetId(const int &chamber_id) const {
    return id_to_detid_.at(chamber_id);
}

int muic::MFSChamberLUT::getStation(const std::string &detid) const {
    return pv_to_station_.at(detid);
}

int muic::MFSChamberLUT::getChamber(const std::string &detid) const {
    return pv_to_chamber_.at(detid);
}
