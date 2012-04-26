#include <windows.h>
#include <iostream>
#include <string.h>

//This is needed for virtually 
//everything in BrowseFolder.
#include <shlobj.h>   

namespace xmx {

// Opens a browse folder dialog.
// Adapted from: http://old.nabble.com/i-have-a-problem-with-%22open-file-name%22-dialog-td3235202.html
//------------------------------------------------------------------------------
std::string browseFile( void )
{
    OPENFILENAME ofn;
    char szFileName[ 500 ];
    szFileName[0] = '\0';
    memset( &ofn, 0, sizeof( ofn ) );

    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = NULL;
    ofn.lpstrFile       = szFileName;
    ofn.nMaxFile        = sizeof( szFileName );
    ofn.hInstance       = NULL;
    ofn.lpstrFilter     = TEXT( "Shapefiles *.shp,*.shx\0*.shp;*.shx\0All Files *.*\0*.*\0\0" );
    ofn.nFilterIndex    = 1;
    ofn.Flags           = OFN_FILEMUSTEXIST;
    
    int bRes;
    bRes = GetOpenFileName(&ofn); 

    return std::string( szFileName );
}

// returns the path to the currently running executable
//------------------------------------------------------------------------------
std::string getExecutableFolder( void )
{
    return "";
}

} // namespace xmx
