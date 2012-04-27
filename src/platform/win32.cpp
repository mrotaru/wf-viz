#include <windows.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;

//This is needed for virtually 
//everything in BrowseFolder.
#include <shlobj.h>   

namespace xmx {

// Opens a browse folder dialog.
// Adapted from: http://old.nabble.com/i-have-a-problem-with-%22open-file-name%22-dialog-td3235202.html
//------------------------------------------------------------------------------
std::string browseFile( string filetypes )
{
    OPENFILENAME ofn;
    char szFileName[ 500 ];
    szFileName[0] = '\0';
    memset( &ofn, 0, sizeof( ofn ) );

    replace( filetypes.begin(), filetypes.end(), '\n', '\0' );

    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = NULL;
    ofn.lpstrFile       = szFileName;
    ofn.nMaxFile        = sizeof( szFileName );
    ofn.hInstance       = NULL;
    ofn.lpstrFilter     = filetypes.c_str();
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
