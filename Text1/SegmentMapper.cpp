#include "provided.h"
#include <vector>
#include "MyMap.h"
using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, vector<StreetSegment> > m_map;

};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for(int i=0; i!= ml.getNumSegments(); i++)
    {
        StreetSegment seg;
        ml.getSegment(i, seg);
        GeoCoord end = seg.segment.end;
        GeoCoord start = seg.segment.start;
        
        
        int problemA =0;
        vector<StreetSegment> tempA = getSegments(start); //herrreeee
        for (int a = 0; a!=tempA.size(); a++)
        {
            if (tempA[a].streetName == seg.streetName)
                problemA++;
        }
        if (problemA==0)
            tempA.push_back(seg);
        m_map.associate(start, tempA); //does this not do anything?
        
        
        int problemB =0;
        vector<StreetSegment> tempB = getSegments(end);
        for (int a = 0; a!=tempB.size(); a++)
        {
            if (tempB[a].streetName == seg.streetName)
                problemB++;
        }
        if (problemB==0)
            tempB.push_back(seg);
        m_map.associate(end, tempB);
        
        
        for (int j = 0; j!= seg.attractions.size(); j++)
        {
            int problemC=0;
            vector<StreetSegment> tempC = getSegments(seg.attractions[j].geocoordinates);
            for (int a = 0; a!=tempC.size(); a++)
            {
                if (tempC[a].streetName == seg.streetName)
                    problemC++;
            }
            if (problemC==0)
                tempC.push_back(seg);
            m_map.associate(seg.attractions[j].geocoordinates, tempC);
            
            
        }
        
        
        /*
        
    
   
       vector<StreetSegment>* tempA = m_map.find(start); ///this is just for the start
        if (tempA == nullptr)
        {
            vector<StreetSegment> tempA1;
            tempA1.push_back(seg);
            m_map.associate(start, tempA1);
        }
        else
        {
                tempA->push_back(seg);
            
        }
        
        vector<StreetSegment>* tempB = m_map.find(end); ///this is for the end
        if (tempB == nullptr)
        {
            vector<StreetSegment> tempB1;
            tempB1.push_back(seg);
            m_map.associate(start, tempB1);
        }
        else
            tempB->push_back(seg);
        
        
        for (int j = 0; j!= seg.attractions.size(); j++)
        {
            vector<StreetSegment>* tempC = m_map.find(seg.attractions[j].geocoordinates);
            if (tempC == nullptr)
            {
                vector<StreetSegment> tempC1;
                tempC1.push_back(seg);
                m_map.associate(seg.attractions[j].geocoordinates, tempC1);
            }
            else
                tempC->push_back(seg);
            
        }
        */
        
     
    }
    
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    
    if (m_map.find(gc)==nullptr)
    {
        vector<StreetSegment> temp;
        return temp;
    }
    
    vector<StreetSegment> vec = *m_map.find(gc);
    return vec;
    
    
    //vector<StreetSegment> segments;
    //return segments;  // This compiles, but may not be correct
  
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
    m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
    delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
    return m_impl->getSegments(gc);
}
