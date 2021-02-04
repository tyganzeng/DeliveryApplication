#include "provided.h"
#include <vector>
#include <random>

using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

double calculateDistance(const GeoCoord& depot, const vector<DeliveryRequest>& requests){
    if(requests.size() < 1){
        return -1;
    }
    //calculating distance from depot to the first location
    double distance = distanceEarthMiles(depot, requests[0].location);
    
    //calculating distance from each location to the next
    for(int i = 0; i < requests.size() - 1; i++){
        distance += distanceEarthMiles(requests[i].location, requests[i+1].location);
    }
    
    //calculating distance from last location to depot
    distance += distanceEarthMiles(requests[requests.size()-1].location, depot);
    return distance;
}

double acceptanceProbability(double currentDistance, double newDistance, double temp){
    if(newDistance < currentDistance){
        return 1;
    }
    
    return exp((currentDistance - newDistance) / temp);
    
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    //calculating the first distance
    oldCrowDistance = calculateDistance(depot, deliveries);

    vector<DeliveryRequest> best = deliveries;
    //simulated annealing
    double temp = 5;
    double coolingRate = 0.001;
    
    while (temp > .1){
        //creating a copy vector
        vector<DeliveryRequest> newCoords = deliveries;
        
        //random number generating
        random_device rd;
        mt19937 mt{ rd() };
        uniform_real_distribution<> ufd{ 0.0, 1.0 };
        int randPos1 = deliveries.size() * ufd(mt);
        int randPos2 = deliveries.size() * ufd(mt);
        
        //swapping two random requests in the copy vector
        DeliveryRequest tempReq = newCoords[randPos1];
        newCoords[randPos1] = newCoords[randPos2];
        newCoords[randPos2] = tempReq;
        
        //checking the total distance for both
        double currentDistance = calculateDistance(depot, deliveries);
        double newDistance = calculateDistance(depot, newCoords);
        
        //updating the current delivery request order
        if( acceptanceProbability(currentDistance, newDistance, temp) > ufd(mt)){
            deliveries = newCoords;
        }
        
        //updating the best delivery request order
        if(calculateDistance(depot, deliveries) < calculateDistance(depot, best)){
            best = deliveries;
        }
        
        //cooling down
        temp = temp * (1 - coolingRate);
    }
    //setting the current delivery request order to the best one found
    deliveries = best;
    newCrowDistance = calculateDistance(depot, best);
     
}



//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
