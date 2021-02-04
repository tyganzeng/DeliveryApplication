#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


/*
bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
        return 1;
    }

    StreetMap sm;
        
    if (!sm.load(argv[1]))
    {
        cout << "Unable to load map data file " << argv[1] << endl;
        return 1;
    }

    GeoCoord depot;
    vector<DeliveryRequest> deliveries;
    if (!loadDeliveryRequests(argv[2], depot, deliveries))
    {
        cout << "Unable to load delivery request file " << argv[2] << endl;
        return 1;
    }

    cout << "Generating route...\n\n";

    DeliveryPlanner dp(&sm);
    vector<DeliveryCommand> dcs;
    double totalMiles;
    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
    if (result == BAD_COORD)
    {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
        return 1;
    }
    if (result == NO_ROUTE)
    {
        cout << "No route can be found to deliver all items." << endl;
        return 1;
    }
    cout << "Starting at the depot...\n";
    for (const auto& dc : dcs)
        cout << dc.description() << endl;
    cout << "You are back at the depot and your deliveries are done!\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
    ifstream inf(deliveriesFile);
    if (!inf)
        return false;
    string lat;
    string lon;
    inf >> lat >> lon;
    inf.ignore(10000, '\n');
    depot = GeoCoord(lat, lon);
    string line;
    while (getline(inf, line))
    {
        string item;
        if (parseDelivery(line, lat, lon, item))
            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
    }
    return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
    const size_t colon = line.find(':');
    if (colon == string::npos)
    {
        cout << "Missing colon in deliveries file line: " << line << endl;
        return false;
    }
    istringstream iss(line.substr(0, colon));
    if (!(iss >> lat >> lon))
    {
        cout << "Bad format in deliveries file line: " << line << endl;
        return false;
    }
    item = line.substr(colon + 1);
    if (item.empty())
    {
        cout << "Missing item in deliveries file line: " << line << endl;
        return false;
    }
    return true;
}*/

int main(){
   
    
    cout.precision(10);
    StreetMap s;
    
    s.load("/Users/tyganzeng/Documents/CS32/Project5Real/Project5Real/mapdata.txt");

    //checking if Helena 10 has the right segs mapped
    /*GeoCoord g("34.0544590","-118.4801137");
    
    vector<StreetSegment> seg;
    s.getSegmentsThatStartWith(g, seg);
    
    for(StreetSegment s : seg){
        cout << s.start.latitude << " " << s.start.longitude << " " << s.end.latitude << " " << s.end.longitude <<" " <<s.name<< endl;
    }*/
    
    //testing with small data file
    /*s.load("/Users/tyganzeng/Documents/CS32/Project5Real/Project5Real/shortData.txt");
    //GeoCoord g("334.0724746", "-118.4923463");
    
    GeoCoord g("34.0731003", "-118.4931016");
    GeoCoord g("34.0665862", "-118.4900441");
    vector<StreetSegment> seg;
    s.getSegmentsThatStartWith(g, seg);
    
    
    for(StreetSegment s : seg){
        cout << s.start.latitude << " " << s.start.longitude << " " << s.end.latitude << " " << s.end.longitude <<" " <<s.name<< endl;
    }*/
    //cout << "done." << endl;
    //cout << "found name :) : " << seg[0].name;
    
    
    /*
    PointToPointRouter p(&s);
    
    GeoCoord start("34.0547000", "-118.4794734");
    //GeoCoord start("34.0588062", "-118.4770229");
    
    
    //GeoCoord end("34.0568080", "-118.4798130");
    //GeoCoord end("34.0549825", "-118.4795629");
    //GeoCoord end("34.0649918", "-118.4023659");
    
    //really far one
    GeoCoord end("34.0815297", "-118.4163807");
    //can find this one
    //GeoCoord end("34.0541514", "-118.4799152");
    
    std::list<StreetSegment> r;
    double dist;
    p.generatePointToPointRoute(start, end, r, dist);
    cout << dist;*/
    
  /*
    list<StreetSegment> route;
    double dist;
    PointToPointRouter ptpr{ &s };
    ptpr.generatePointToPointRoute(GeoCoord("34.0547000", "-118.4794734"), GeoCoord("34.0927917", "-118.4233265"), route, dist);
    cout << dist;
    
    for(list<StreetSegment>::iterator i = route.begin(); i != route.end(); i++){
        cout << (*i).name << endl;
    }*/
    
    
    
    //34.054459, -118.4801137
    
    DeliveryPlanner d(&s);
    
    GeoCoord depot("34.0625329", "-118.4470263");
    
    vector<DeliveryRequest> deliveries;
    GeoCoord firstSpot("34.0712323", "-118.4505969");
    DeliveryRequest req("chicken", firstSpot);
    deliveries.push_back(req);
    
    vector<DeliveryCommand> commands;
    
    double distanceTravelled;
    
    d.generateDeliveryPlan(depot, deliveries, commands, distanceTravelled);
    
    cout << distanceTravelled << endl;
    
}


