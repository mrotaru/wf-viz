from dbfpy import dbf

db = dbf.Dbf( "../shapefiles/world_borders.dbf" )
xml_file = open( "out.xml", "w" )

xml_file.write( '<?xml version="1.0" encoding="utf-8"?>\n' )
xml_file.write( '<records>\n' )

to_stdout = False

i=0
for rec in db:
    i = i+1

    if to_stdout:
        print rec[ "NAME" ] + " ( " + rec[ "ISO3" ] + " ): " + str(i)

    xml_file.write( '    <record>\n' )
    xml_file.write( '        <name>' + rec[ "NAME" ] + '</name>\n' )
    xml_file.write( '        <iso3>' + rec[ "ISO3" ] + '</iso3>\n' )
    xml_file.write( '        <index>' + str( i ) + '</index>\n' )
    xml_file.write( '    </record>\n' )

xml_file.write( '</records>\n' )
xml_file.close();

print "Done; " + str( i ) + " records written to the xml file."
