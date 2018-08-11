//
//  support.h
//  Proj4.0
//
//  Created by Elizabeth Han on 3/14/17.
//  Copyright © 2017 Elizabeth Han. All rights reserved.
//

#ifndef support_h
#define support_h
#include "provided.h"
#include <string>




bool operator==(const GeoCoord& a, const GeoCoord& b);
bool operator<(const GeoCoord& a, const GeoCoord& b);
bool operator>(const GeoCoord& a, const GeoCoord& b);

//bool operator<(const NavSegment& a, const NavSegment& b);

std::string dirTurn(double angle);
std::string dirProc(double angle);


#endif /* support_h */
