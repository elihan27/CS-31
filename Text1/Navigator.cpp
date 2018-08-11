#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>
#include "MyMap.h"
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader ml;
    AttractionMapper am;
    SegmentMapper sm;
    
    

};

struct node
{
    node();
    node* parent = nullptr;
    GeoCoord coord;
    StreetSegment seg;
    float g, h;
};


node::node()
{
    g =0;
    h =0;
}



bool operator<(const node& a, const node& b)
{
    float af = a.g+ a.h;
    float bf = b.g+ b.h;
    
    if (af > bf)
        return true;
    return false;

}

NavigatorImpl::NavigatorImpl()
{
    


}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if (ml.load(mapFile)== false)
        return false;
    am.init(ml);
    sm.init(ml);
    return true;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    GeoCoord sgc;
    if (! am.getGeoCoord(start, sgc))
        return NAV_BAD_SOURCE;
    GeoCoord egc;
    if (! am.getGeoCoord(end, egc))
        return NAV_BAD_DESTINATION;
    
    priority_queue<node*> open;
    //vector<node*> close;
    MyMap<GeoCoord, string> close;
    
    vector<StreetSegment> begin = sm.getSegments(sgc);
    for (int i =0; i!= begin.size(); i++) //our first nodes (so no parent)
    {
        
        node* first = new node;
        first->coord = sgc;
        first->seg = begin[i];
        first->h = distanceEarthMiles(sgc, egc);
        open.push(first);
    }   ///should only have a couple nodes;
    int foundit=0;
    
    node* FINAL = nullptr;
    while ((! open.empty()) && foundit==0)
    {
        
        node* cur = open.top();
        vector<StreetSegment> check = sm.getSegments(egc);
        int foundit = 0;
        for(int i =0; i!= check.size(); i++)
        {
            if (cur->seg.streetName == check.at(i).streetName)
            {   FINAL = cur;
                foundit++;
                break;
            }
        }
        
        open.pop();
        
        vector<StreetSegment> generate = sm.getSegments(cur->coord);
        vector<node*> justincase;
        for (int i =0; i!= generate.size(); i++) //our first nodes (so no parent)
        {
            
            node* child1 = new node; //start of a segment
            child1->parent = cur;
            child1->coord = generate.at(i).segment.start;
            child1->seg = generate[i];
            child1->g += distanceEarthMiles(cur->coord, child1->coord);
            child1->h = distanceEarthMiles(child1->coord, egc);
            
            if (! close.find(child1->coord))
            {
                close.associate(child1->coord, "Visited");
                open.push(child1);
            }
            
            //justincase.push_back(child1);
            
            
            node* child2 = new node; //end of a segment
            child2->parent = cur;
            child2->coord = generate.at(i).segment.end;
            child2->seg = generate[i];
            child2->g += distanceEarthMiles(cur->coord, child1->coord);
            child2->h = distanceEarthMiles(child1->coord, egc);
           // justincase.push_back(child1);
            
            if (! close.find(child2->coord))
            {
                close.associate(child2->coord, "Visited");
                open.push(child2);
            }
   
        }
    }
    
    if (foundit!=0)
    {
        if(! directions.empty()) //this is just for clearing the array.  For later, of course
            directions.clear();// because we have absolutely no idea if it's actually clear or not
       

        //Final SHOULDN'T be nullptr, but if it does, you've fucked up
        
        GeoSegment firstLast(FINAL->coord, egc);
        
        NavSegment thefirst (dirProc(angleOfLine(firstLast)), FINAL->seg.streetName, distanceEarthMiles(FINAL->coord, egc), firstLast); //this will proceed, always, because FIRST SEGMENT;
        
        directions.push_back(thefirst);
        node* cur = FINAL;

        while(cur->parent!=nullptr) //FINAL should not equal nullptr, but if it does
        {
          
            GeoSegment next(cur->parent->coord, cur->coord);
            if (cur->parent->seg.streetName==cur->seg.streetName)
            {
                 NavSegment straightline (dirProc(angleOfLine(next)), cur->seg.streetName, distanceEarthMiles(cur->parent->coord, cur->coord), next);
                directions.push_back(straightline);
            }
            else //you need to turnnnn
            {
                NavSegment turn(dirTurn(angleBetween2Lines(cur->parent->seg.segment, cur->seg.segment)), cur->seg.streetName);
                NavSegment straightline (dirProc(angleOfLine(next)), cur->seg.streetName, distanceEarthMiles(cur->parent->coord, cur->coord), next);
                directions.push_back(turn);
                directions.push_back(straightline);
            
            }
        
        }
        
        return NAV_SUCCESS;
    
    }
        
    return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

    /*vector<StreetSegment> begin = sm.getSegments(sgc);//the very first;
    if (! begin.empty())
    {
        NavSegment start (dirProc(angleOfLine(begin[0].segment)), begin[0].streetName, distanceEarthKM(sgc, begin[0].segment.start), begin[0].segment);
        ///uh, check with someone
        NavSegment end (dirProc(angleOfLine(begin[0].segment)), begin[0].streetName, distanceEarthKM(sgc, begin[0].segment.end), begin[0].segment); ///uh, check with someone
        
        open.push(start);
        open.push(end);
    }
 
    while (!open.empty())
    {
        NavSegment current = open.top();
    
       // current.m_geoSegment.end;
        vector<StreetSegment> temp = sm.getSegments(current.m_geoSegment.start);
        int foundit=0;
        for (int i=0;i!=temp.size();i++)
        {
            for (int j = 0; j!= temp.at(i).attractions.size(); j++)
            {
               if (temp.at(i).attractions.at(j).name== end && temp.at(i).attractions.at(j).geocoordinates == egc)
               {foundit++;
                   break;}
            }
        }
        
    
        open.pop();
        close.push_back(current);
        
        for (int i = 0; i!= temp.size(); i++)
        {
            //for(int j=0; j!= close.size(); j++)
               // if (temp[i] == close[j])
                    continue;
        
        }
        
        
        
        //if foundit>0 return your vector;
        
        
    }
    

    
    
    
    if(! directions.empty()) //this is just for clearing the array.  For later, of course
        directions.clear();// because we have absolutely no idea if it's actually clear or not
     
     */
    
    
    
 

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
