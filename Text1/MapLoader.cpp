#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> segment;//stl container of somesort
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
    
    
}

bool MapLoaderImpl::load(string mapFile)
{
    
    ifstream infile(mapFile);    // infile is a name of our choosing
    if ( ! infile )		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open map file!" << endl;
        return false;
    }
    
    string streetname, start_lat, start_long, end_lat, end_long;
    int attraction;
    while(true)
    {
        if(! getline(infile, streetname, '\n'))
            break;
        getline(infile, start_lat, ' '); ///figure out
        getline(infile, start_long, ' ');
        getline(infile, end_lat, ',');
        getline(infile, end_long, '\n');
        string attractions;
        getline(infile, attractions, '\n');
        
        
        
        GeoCoord* start = new GeoCoord(start_lat, start_long);
        GeoCoord* end = new GeoCoord(end_lat, end_long);
        GeoSegment* seg = new GeoSegment(*start, *end);
        
        
        StreetSegment welp;
        welp.streetName = streetname;
        welp.segment = *seg;
        
      
        attraction = stoi(attractions, 0, 10);
        if (attraction!=0)
        {
            for (int i =0; i!= attraction; i++)
            {
                Attraction* someSort = new Attraction;
                string name,lat, log;
                getline(infile, name, '|'); //this is an attraction;
                /*for (int a=0; a!=name.size(); a++)
                {
                    if(isupper(name[a])) //try to make lower?
                        name[a]=tolower(name[a]);
                }*/
                
                
                getline(infile, lat, ',');
                char c;
                infile.get(c);
                getline(infile, log, '\n');
                GeoCoord* attract = new GeoCoord(lat, log);
                someSort->geocoordinates = *attract;
                someSort->name = name;
                welp.attractions.push_back(*someSort);
            
            }
        }
        
        segment.push_back(welp);

    
    }
    return true;  // This compiles, but may not be correct
}

size_t MapLoaderImpl::getNumSegments() const
{
    return segment.size(); // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum < 0 ||segNum > getNumSegments()) //keeping the first there anyway
        return false;
    seg = segment[segNum];
    return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
    m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
    delete m_impl;
}

bool MapLoader::load(string mapFile)
{
    return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
    return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
    return m_impl->getSegment(segNum, seg);
}
 


