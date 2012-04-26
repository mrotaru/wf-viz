from dbfpy import dbf

db = dbf.Dbf( "../shapefiles/world_borders.dbf" )
for rec in db:
    print rec
print
