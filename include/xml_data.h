#ifndef XML_DATA_H
#define XML_DATA_H

#include <boost/assign.hpp>
using namespace boost::assign;

#include <iostream>
#include <string>
using namespace std;

namespace xmx {

class XMLData
{
public:
    XMLData(): filename( "" ), loaded( false ) {}
    virtual ~XMLData() {}

    void load( string );
    double getMaxDataValue( int year );
    double getMinDataValue( int year );
    int getMaxDataYear();
    int getMinDataYear();
    double getValueForYear( string iso3_code, int year );
    
    bool isLoaded() { return loaded; }

private:
    map< string, shared_ptr< map< int, double > > > data;
    string filename;
    bool loaded;
};

extern map< string, string > ISO3_codes;

} // namespace xmx
#endif /* XML_DATA_H */
