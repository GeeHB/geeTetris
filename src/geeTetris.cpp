//----------------------------------------------------------------------
//--
//--    geeTetris.cpp
//--
//--        Entry point
//--
//----------------------------------------------------------------------

#include "tetrisMenu.h"

// Program entry point
//
int main(){
    tetrisMenu appMenu;

    appMenu.about();    // Show about as splash screen
    appMenu.run();

#ifdef DEST_CASIO_CALC
    gint_setrestart(1);
    //gint_osmenu();
#endif // #ifdef DEST_CASIO_CALC

    // Finished
    return 1;
}

// EOF
