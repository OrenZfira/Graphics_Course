#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>


class Bezier {
        
    public:
        Eigen::Matrix4d M;
        std::vector<Eigen::RowVector3d> cps; 
        int segNum;

        Bezier();
        double getPt( double n1 , double n2 , double perc );
        std::vector<Eigen::RowVector3d> GetPointsInSegment(int segmentId);
        void ChangeCpt(int index, Eigen::Vector3d diff);
        Eigen::Vector3d GetVelocity(int segid, float t, float dt);
        Eigen::Vector3d GetPointOnCurve(int segid, float t);
};