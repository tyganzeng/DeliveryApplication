#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O

using namespace std;


unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
    
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> geoMap;

};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream inf(mapFile);
      // Test for failure to open
    if ( ! inf)
    {
        cout << "Cannot open expenses file!" << endl;
        return false;
    }
    string line;
    
    //each iteration of this loop goes through an entire block of segments for each name
    while (getline(inf, line))
    {

        //first getting the name of the street
        istringstream iss(line);

        string streetName;
        string startX, startY, endX, endY;
        
        //now getting the number of segments
        int numSegs;
        streetName = line;
        getline(inf, line);
        istringstream iss2(line);
        
        
        if ( ! (iss2 >> numSegs) )
        {
            cout << "Not a number:  " << line << endl;
            return false;
        }

        //iterating through all the segments in the street
        for(int i = 0; i < numSegs; i++){
            
            //getting the coordinates of the start and end coordinates
            getline(inf, line);
            istringstream iss3(line);
            if ( ! (iss3 >> startX >> startY >> endX >> endY) )
            {
                cout << "Not a valid segment:  " << line << endl;
                return false;
            }
            GeoCoord startCoord(startX, startY);
            GeoCoord endCoord(endX, endY);
            StreetSegment street1(startCoord, endCoord, streetName);
            StreetSegment street2(endCoord, startCoord, streetName);
            
            //check to see if the start and end already exist in the map, and if so append the new segment
            if(geoMap.find(startCoord) == nullptr){
                vector<StreetSegment> newSegs;
                newSegs.push_back(street1);
                geoMap.associate(startCoord, newSegs);
            } else {
                geoMap.find(startCoord)->push_back(street1);
            }
            if(geoMap.find(endCoord) == nullptr){
                vector<StreetSegment> newSegs;
                newSegs.push_back(street2);
                geoMap.associate(endCoord, newSegs);
            } else {
                geoMap.find(endCoord)->push_back(street2);
            }
        }
    }
    
    
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    
    if(geoMap.find(gc) != nullptr){
        segs = *geoMap.find(gc);
        return true;
    }
    return false;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}

