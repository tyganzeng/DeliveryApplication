#include "provided.h"
#include <vector>
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
    
private:
    PointToPointRouter router;
    DeliveryOptimizer d;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm) :
router(sm), d(sm)
{
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    vector<DeliveryRequest> copy = deliveries;
    double oldDist;
    double newDist;
    
    d.optimizeDeliveryOrder(depot, copy, oldDist, newDist);
    
    //loading in deliveries into coordinate vector
    vector<GeoCoord> coords;
    vector<string> foods;
    for(int i = 0; i < copy.size(); i++){
        coords.push_back(copy[i].location);
        foods.push_back(copy[i].item);
    }
    
    coords.push_back(depot);

    
    for(int j = 0; j < coords.size(); j++){
        
        //generating a route from current location to next location
        list<StreetSegment> segs;
        double totalDistance;
        if(j == 0){
            router.generatePointToPointRoute(depot, coords[0], segs, totalDistance);
        } else {
            router.generatePointToPointRoute(coords[j-1], coords[j], segs, totalDistance);
        }
        
        //iterating through each segment
        list<StreetSegment>::iterator i = segs.begin();
        list<StreetSegment>::iterator prev;
        while(i != segs.end()){

            //get the name and direction of the first seg
            StreetSegment current = *i;

            string streetName = current.name;
            double angle = angleOfLine(current);
            string direction;
            if(0 <= angle && angle < 22.5){
                direction = "east";
            } else if(22.5 <= angle && angle < 67.5){
                direction = "northeast";
            } else if(67.5 <= angle && angle < 112.5){
                direction = "north";
            } else if(112.5 <= angle && angle < 157.5){
                direction = "northwest";
            } else if(157.5 <= angle && angle < 202.5){
                direction = "west";
            } else if(202.5 <= angle && angle < 247.5){
                direction = "southwest";
            } else if(247.5 <= angle && angle < 292.5){
                direction = "south";
            } else if(292.5 <= angle && angle < 337.5){
                direction = "southeast";
            } else if(angle >= 337.5){
                direction = "east";
            }
            
            double distanceTravelled = 0;
            //while the next seg has the same name, increment the distance travelled
            while(current.name == streetName){
                distanceTravelled += distanceEarthMiles(current.start, current.end);
                prev = i;
                i++;
                if(i == segs.end()){
                    break;
                }
                current = *i;
            }
            
                
            
            //add the proceed command to the vector
            DeliveryCommand d;
            d.initAsProceedCommand(direction, streetName, distanceTravelled);
            commands.push_back(d);
            totalDistanceTravelled += distanceTravelled;
            
            if(i == segs.end()){
                break;
            }
            
            //generate a turn command if needed
            double angleBetweenStreets = angleBetween2Lines(*prev, *i);
            if(angleBetweenStreets >= 1 && angleBetweenStreets < 180){
                DeliveryCommand t;
                t.initAsTurnCommand("left", i->name);
                commands.push_back(t);
            } else if (angleBetweenStreets >= 180 && angleBetweenStreets <= 359){
                DeliveryCommand t;
                t.initAsTurnCommand("right", i->name);
                commands.push_back(t);
            }
            
            
        }
        
        if(j != foods.size()){
            DeliveryCommand drop;
            string item = foods[j];
            drop.initAsDeliverCommand(item);
            commands.push_back(drop);
        }
        
    }
    
    
    
    
    
    
    return DELIVERY_SUCCESS;  // Delete this line and implement this function correctly
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
