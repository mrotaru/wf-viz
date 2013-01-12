#include <iostream>
#include <string.h>
#include "platform/linux-gtk.h"

namespace xmx {

// Displays a browse folder dialog.
// Returns the absolute path to the file selected by the user,
// or an empty string if no files were selected.
// Adapted from: http://developer.gnome.org/gtkmm-tutorial/3.0/sec-dialogs-filechooserdialog.html.en
//------------------------------------------------------------------------------
std::string browseFile( std::string filetypes )
{
    Gtk::Main kit(false);

    Gtk::FileChooserDialog dialog( "Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN );
//    dialog.set_transient_for( (Gtk::Window*)0 );

    //Add response buttons the the dialog:
    dialog.add_button( Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL );
    dialog.add_button( Gtk::Stock::OPEN, Gtk::RESPONSE_OK );

    //Add filters, so that only certain file types can be selected:
    Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
    filter_any->set_name( "Any files" );
    filter_any->add_pattern( "*" );
    dialog.add_filter( filter_any );

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch( result )
    {
        case( Gtk::RESPONSE_OK ):
        {
            std::cout << "Open clicked." << std::endl;

            //Notice that this is a std::string, not a Glib::ustring.
            std::string filename = dialog.get_filename(  );
            std::cout << "File selected: " <<  filename << std::endl;
            kit.quit();
            dialog.hide();
            return filename;
        }
        case( Gtk::RESPONSE_CANCEL ): { std::cout << "Cancel clicked." << std::endl; break; }
        default: { std::cout << "Unexpected button clicked." << std::endl; break; }
    }
    return std::string( "" );
}

} // namespace xmx
