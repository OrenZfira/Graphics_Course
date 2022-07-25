#include "Bezier.h"

Bezier :: Bezier(){
    Eigen::Matrix4d points;
    points << -0.5,0,0,0,
            -0.25, 0.5, 0,0,
            0.25, 0.5, 0,0,
            0.5,0,0,0;
    segments = {points};
}