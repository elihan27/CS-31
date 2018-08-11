//
//  main.cpp
//  Proj4.0
//
//  Created by Elizabeth Han on 3/10/17.
//  Copyright © 2017 Elizabeth Han. All rights reserved.
//


 #include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

int main()
{
    cout << "About to test MyMap" << endl;
    {
        MyMap<int, string> mm;
        mm.associate(20, "Ethel");
        mm.associate(10, "Fred");
        const string* p = mm.find(10);
        assert(p != nullptr  &&  *p == "Fred");
        assert(mm.find(30) == nullptr);
        assert(mm.size() == 2);
    }
    cout << "MyMap PASSED" << endl;
    
    cout << "About to test MapLoader" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        size_t numSegments = ml.getNumSegments();
        assert(numSegments == 7);
        bool foundAttraction = false;
        for (size_t i = 0; i < numSegments; i++)
        {
            StreetSegment seg;
            assert(ml.getSegment(i, seg));
            if (seg.streetName == "Picadilly")
            {
                assert(seg.attractions.size() == 1);
                assert(seg.attractions[0].name == "Eros Statue");
                foundAttraction = true;
                break;
            }
        }
        assert(foundAttraction);
    }
    cout << "MapLoader PASSED" << endl;
    
    cout << "About to test AttractionMapper" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        AttractionMapper am;
        am.init(ml);
        GeoCoord gc;
        assert(am.getGeoCoord("Hamleys Toy Store", gc));
        assert(gc.latitudeText == "51.512812");
        assert(gc.longitudeText == "-0.140114");
    }
    cout << "AttractionMapper PASSED" << endl;
    
    cout << "About to test SegmentMapper" << endl;
    {
        MapLoader ml;
        assert(ml.load("testmap.txt"));
        SegmentMapper sm;
        sm.init(ml);
        GeoCoord gc("51.510087", "-0.134563");
        vector<StreetSegment> vss = sm.getSegments(gc);
        assert(vss.size() == 4);
        string names[4];
        for (size_t i = 0; i < 4; i++)
            names[i] = vss[i].streetName;
        sort(names, names+4);
        const string expected[4] = {
            "Coventry Street", "Picadilly", "Regent Street", "Shaftesbury Avenue"
        };
        assert(equal(names, names+4, expected));
    }
    cout << "SegmentMapper PASSED" << endl;
    
    cout << "About to test Navigator" << endl;
    {
        Navigator nav;
        assert(nav.loadMapData("testmap.txt"));
        vector<NavSegment> directions;
        assert(nav.navigate("Eros Statue", "Hamleys Toy Store", directions) == NAV_SUCCESS);
        assert(directions.size() == 6);
        struct ExpectedItem
        {
            NavSegment::NavCommand command;
            string direction;
            double distance;
            string streetName;
        };
        const ExpectedItem expected[6] = {
            { NavSegment::PROCEED, "northwest", 0.0138, "Picadilly" },
            { NavSegment::TURN, "left", 0, "" },
            { NavSegment::PROCEED, "west", 0.0119, "Regent Street" },
            { NavSegment::PROCEED, "west", 0.0845, "Regent Street" },
            { NavSegment::PROCEED, "west", 0.0696, "Regent Street" },
            { NavSegment::PROCEED, "northwest", 0.1871, "Regent Street" },
        };
        for (size_t i = 0; i < 6; i++)
        {
            const NavSegment& ns = directions[i];
            const ExpectedItem& exp = expected[i];
            assert(ns.m_command == exp.command);
            assert(ns.m_direction == exp.direction);
            if (ns.m_command == NavSegment::PROCEED)
            {
                assert(abs(ns.m_distance - exp.distance) < 0.00051);
                assert(ns.m_streetName == exp.streetName);
            }
        }
    }
    cout << "Navigator PASSED" << endl;
}


/*

#include <iostream>
#include "provided.h"

using namespace std;

int main()
{
    MapLoader* ml = new MapLoader;
    if (ml->load("/Users/elizabethhan/Desktop/Proj4.0/Proj4.0/mapdata.txt"))
        cout << "You probably didn't fuck up!"<< endl;
    
    
    
    
    GeoCoord fillMe;
    AttractionMapper am;
    am.init(*ml); // let our object build its internal data structures
    // by iterating thru all segments from the MapLoader object
    string attraction = "The Coffee Bean & Tea Leaf";
    bool found = am.getGeoCoord(attraction, fillMe);
    if ( ! found)
    {
        cout << "No geolocation found for " << attraction << endl;
        return 0; }
    cout << "The location of " << attraction << " is " << fillMe.latitude << ", " << fillMe.longitude << endl;
    
    SegmentMapper sm;
    sm.init(*ml);
    GeoCoord lookMeUp("34.0572000", "-118.4417620");
    std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
    if (vecOfAssociatedSegs.empty())
    {
        cout << "Error - no segments found matching this coordinate\n";
        return 0;
    }
    cout << "Here are all the segments associated with your coordinate:" << endl;
    
    for (auto s: vecOfAssociatedSegs)
    {
        cout << "Segment’s street: " << s.streetName << endl;
        cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " <<
        s.segment.start.longitude << endl; cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " <<
        s.segment.end.longitude << endl;
        cout << "This segment has " << s.attractions.size() << " attractions on it." << endl;
        
    }

    
    return 0;
 
}
*/
