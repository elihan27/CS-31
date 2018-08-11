//
//  support.cpp
//  Proj4.0
//
//  Created by Elizabeth Han on 3/14/17.
//  Copyright © 2017 Elizabeth Han. All rights reserved.
//

#include <stdio.h>
#include "provided.h"

bool operator==(const GeoCoord& a, const GeoCoord& b)
{
    if (a.latitude==b.latitude && a.longitude==b.longitude)
        return true;
    return false;
}

bool operator<(const GeoCoord& a, const GeoCoord& b)
{
    if (a.latitude < b.latitude)
        return true;
    if (a.latitude > b.latitude)
      return false;
    if (a.latitude==b.latitude)
    {
       if (a.longitude < b.longitude)
           return true;
    }
    
    return false;

    /*
     if (! (a.latitude < b.latitude))
     return false; //okay, so you know for sure if this passes this point, latitude a is in fact less than latitude b.
     if (a.longitude < b.longitude) //okay, what if
     return true;
     return false;
     
     */
    
}


bool operator>(const GeoCoord& a, const GeoCoord& b)
{
    if (a.latitude > b.latitude)
        return true;
    if (a.latitude < b.latitude)
        return false;
    if (a.latitude==b.latitude)
    {
        if (a.longitude > b.longitude)
            return true;
    }
    
    return false;
    

    
    /*if (! (a.latitude > b.latitude))
        return false; //okay, latitude a is greater than latitude b and
    if (a.longitude > b.longitude)
        return true;
    return false;
     */
    
}

/*bool operator<(const NavSegment& a, const NavSegment& b)
{
    if (a.m_distance<b.m_distance)
        return true;
    return false;
}
 */


std::string dirTurn(double angle)
{
    if (angle<180)
        return "left";
    return "right";
}


std::string dirProc(double angle)
{
    if (angle>=0 && angle<= 22.5)
        return "east";
    if (angle>22.5&& angle<=67.5)
        return "northeast";
    if (angle>67.5&& angle<=112.5)
        return "north";
    if (angle>112.5&& angle<=157.5)
        return "northwest";
    if (angle>157.5 &&angle<=202.5)
        return "west";
    if (angle>202.5 && angle<=247.5)
        return "southwest";
    if (angle>247.5&& angle<=292.5)
        return "south";
    if (angle<=337.5)
        return "southeast";
    else
        return "east"; //I'm not really paying attention to the 360 here, should I?
                        //might want to go back and fix.

}
