//---------------------------------------------------------------------------
//--
//--	File	: FSO.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Definition of FSO object
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TOOLS_FILE_SYSTEM_OBJECT_h__
#define __GEE_TOOLS_FILE_SYSTEM_OBJECT_h__    1

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#ifdef DEST_CASIO_CALC
#include <gint/bfile.h>
#else
#include <cstdint>
#include <cstdio>
#endif // #ifdef DEST_CASIO_CALC


//  FSO - File System Object
//

class FSO{
public:

    // Construction
    FSO(){
        fd_ = 0;    // no file
        error_ = 0; // no error
    }

    // Destruction
    ~FSO(){
        if (fd_){
            close();
        }
    }

    bool isValid();
    int size();

    bool exists();
    bool open(int access);
    bool create();
    int write(const void*, int);
    int read(void*, int);
    void close();

    int getLastError(){
        return error_;
    }

private:
    int fd_;        // File descriptor (>0 if valid)
    int error_;     // Last error code
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TOOLS_FILE_SYSTEM_OBJECT_h__

// EOF
