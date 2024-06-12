//
// Created by om15 on 6/14/23.
//

#include "muic/utils/angles.h"

namespace muic {

    float wrap_phi(const float &angle) {
        float twopi = 2. * M_PI;
        float recip = 1.0 / twopi;

        return angle - (std::round(angle * recip) * twopi);
    }

}