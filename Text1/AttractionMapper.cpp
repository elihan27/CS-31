#include "provided.h"
#include <string>
#include "MyMap.h"

using namespace std;

class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord>m_map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{


}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for(int i=0; i!= ml.getNumSegments(); i++)
    {
        StreetSegment seg;
        ml.getSegment(i, seg);
        for (int j = 0; j!= seg.attractions.size(); j++)
        {
            for (int k=0; k!=seg.attractions.at(j).name.size(); k++)
            {
                if(isupper(seg.attractions[j].name[k])) //try to make lower?
                    seg.attractions[j].name[k] = tolower(seg.attractions[j].name[k]);
            }
            
            
            m_map.associate(seg.attractions[j].name, seg.attractions[j].geocoordinates);
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    for (int i=0; i!=attraction.size(); i++)
    {
        if(isupper(attraction[i])) //try to make lower?
            attraction[i] = tolower(attraction[i]);
    }
    if (m_map.find(attraction)==nullptr)
       return false;
    else
        gc = *m_map.find(attraction);
    return true;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
    m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
    delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
    return m_impl->getGeoCoord(attraction, gc);
}
