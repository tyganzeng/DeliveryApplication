#include "provided.h"
#include <cmath>
#include "ExpandableHashMap.h"
#include <queue>
#include <list>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* streetMap;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    streetMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

struct Coordinate {
    double score;
    GeoCoord coord;
    
    Coordinate(GeoCoord nCoord, double nScore){
        coord = nCoord;
        score = nScore;
    }
};

bool operator<(const Coordinate& c1, const Coordinate& c2){
    return c1.score > c2.score;
}



double findDistance(const GeoCoord& first, const GeoCoord& second){
    return sqrt( pow(first.latitude - second.latitude, 2) + pow(first.longitude - second.longitude,2));
}

double findDistance(StreetSegment s){
    return findDistance(s.start, s.end);
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    bool pathFound = false;
    priority_queue<Coordinate> coordinates;
    Coordinate first(start, 0);
    coordinates.push(first);
    ExpandableHashMap<GeoCoord, StreetSegment> cameFrom;
    ExpandableHashMap<GeoCoord, double> cost;
    cost.associate(start, 0);
    
    cout.precision(10);
    while(!coordinates.empty()){
        //gets the first coord
        const GeoCoord current = coordinates.top().coord;
        
        coordinates.pop();
        
        //return if ending coordinate found
        if(current.latitude == end.latitude && current.longitude == end.longitude){
            pathFound = true;
            break;
        }
        
        
        //get all connecting segments for current coordinate
        vector<StreetSegment> connectingSegments;
        streetMap->getSegmentsThatStartWith(current, connectingSegments);

        //iterating through segments
        for(StreetSegment s : connectingSegments){
            //get a pointer the coordinate at the end of the segment
            GeoCoord neighbor = s.end;
            
            //calculate the cost of going there by adding the cost of the current coordinate and the distance between the two coordinates
            double newCost = *(cost.find(current)) + findDistance(s);
            
            //if the neighbor has not been found yet, OR the new cost is better, reassociate
            //if(cost.find(neighbor) == nullptr){
            if(cost.find(neighbor) == nullptr || newCost < *(cost.find(neighbor))){
               // cout << "Adding" << neighbor.latitude << " " << neighbor.longitude << endl;
                cost.associate(neighbor, newCost);
                cameFrom.associate(neighbor, s);
                double priority = newCost + findDistance(neighbor, end);
                Coordinate newCoord(neighbor, priority);
                coordinates.push(newCoord);
                
            }
        }
    }
    
    //return no route found if queue empties before finding the end
    if(!pathFound){
        return NO_ROUTE;
    }
    
    //reconstructing path
    GeoCoord g = end;
    list<StreetSegment> r;
    while(g.latitude != start.latitude && g.longitude != start.longitude){
        StreetSegment previousPath = *cameFrom.find(g);
        r.push_front(previousPath);
        totalDistanceTravelled += distanceEarthMiles(previousPath.start, previousPath.end);
        g = cameFrom.find(g)->start;
    }
    route = r;

    
    return DELIVERY_SUCCESS;
    
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}


