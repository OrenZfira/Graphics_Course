#include "Bezier.h"
#include <iostream>

Bezier :: Bezier(){
    cps.push_back({-0.5,0,0});
    cps.push_back({-0.25,0.5,0});
    cps.push_back({0.25,0.5,0});
    cps.push_back({0.5,0,0});
    M << -1.0, 3.0, -3.0, 1.0,
        3.0, -6.0, 3.0, 0.0,
        -3.0, 3.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0;
    segNum = 1;
}

double Bezier :: getPt( double n1 , double n2 , double perc )
    {
        double diff = n2 - n1;

        return n1 + ( diff * perc );
    }  

std::vector<Eigen::RowVector3d> Bezier:: GetPointsInSegment(int segid){
    int index = segid*4;
    double x1 = cps[index][0];
    double y1 = cps[index][1];
    double x2 = cps[index+1][0];
    double y2 = cps[index+1][1];
    double x3 = cps[index+2][0];
    double y3 = cps[index+2][1];
    double x4 = cps[index+3][0];
    double y4 = cps[index+3][1];
    std::vector<Eigen::RowVector3d> points;

    for( double i = 0 ; i <= 1 ; i += 0.01 )
    {
        double xa = getPt( x1 , x2 , i );
        double ya = getPt( y1 , y2 , i );
        double xb = getPt( x2 , x3 , i );
        double yb = getPt( y2 , y3 , i );
        double xc = getPt( x3 , x4 , i );
        double yc = getPt( y3 , y4 , i );

        // The Blue Line
        double xm = getPt( xa , xb , i );
        double ym = getPt( ya , yb , i );
        double xn = getPt( xb , xc , i );
        double yn = getPt( yb , yc , i );

        // The Black Dot
        double x = getPt( xm , xn , i );
        double y = getPt( ym , yn , i );
        points.push_back({x,y,0});
    }
    return points;
}

void Bezier :: ChangeCpt(int index, Eigen::Vector3d diff){
    cps[index] += diff;
}

Eigen::Vector3d Bezier::GetPointOnCurve(int segid, float t){
    Eigen::RowVector4d T = Eigen::RowVector4d(t*t*t, t*t, t, 1.0);
    Eigen::Matrix4d G;
    G << cps[segid*4][0], cps[segid*4][1], 0, 1,
    cps[segid*4+1][0], cps[segid*4+1][1], 0, 1,
    cps[segid*4+2][0], cps[segid*4+2][1], 0, 1,
    cps[segid*4+3][0], cps[segid*4+3][1], 0, 1;
    
    Eigen::Vector4d A =  T*M*G.transpose();//.head(3);
    return A.head(3);
}


Eigen::Vector3d Bezier :: GetVelocity(int segid, float t, float dt){
    // float dt = 0.05;
    // if(t < 1){
    //     Eigen::Vector3d first = GetPointOnCurve(segid, t+dt);
    //     std::cout << "first is: " << first << std::endl;

    //     Eigen::Vector3d sec = GetPointOnCurve(segid, t);
    //     std::cout << "second is: " << sec << std::endl;

    //     return GetPointOnCurve(segid, t+dt) - GetPointOnCurve(segid, t);
    // }
    // else
    //     if(segid < segNum - 1)
    //         return GetVelocity(segid + 1, 0);
    //     else
    //         return Eigen::Vector3d(0,0,0);

    Eigen::Vector3d derivative = 3*(1-t)*(1-t)*(cps[segid*4+1]-cps[segid*4])
        + 6*(1-t)*t*(cps[segid*4+2]-cps[segid*4+1])
        + 3*t*t*(cps[segid*4+3]-cps[segid*4+2]);
    derivative.normalize();
    return (derivative)*dt*3;
}


