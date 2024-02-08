//----------------------------------------------------------------------
//--
//--    bFileLocals.h
//--
//--        BFile consts and types for other platforms
//--
//----------------------------------------------------------------------

#ifndef __LOCALS_B_FILE_h__
#define __LOCALS_B_FILE_h__      1

#ifndef DEST_CASIO_CALC
#include <cstdio>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <filesystem>       // to create a folder
namespace fs = std::filesystem;

typedef char* FONTCHARACTER;
typedef DIR* SEARCHHANDLE;

#define BFILE_CHAR_ZERO 0x00

// Error codes
#define BFile_IllegalParam  -1

// Entry types
#define BFile_File    1
#define BFile_Folder  5

// Access modes
#define BFile_ReadOnly      0x01
#define BFile_WriteOnly     0x02
#define BFile_ReadWrite     (BFile_ReadOnly | BFile_WriteOnly)
#define BFile_Share         0x80

struct BFile_FileInfo
{
    uint16_t index;
    uint16_t type;
    uint32_t file_size;
    /* Data size (file size minus the header) */
    uint32_t data_size;
    /* Is 0 if the file is complete */
    uint32_t property;
    /* Address of first fragment (do not use directly) */
    void *address;
};

// Enum. types
#define BFile_Type_Directory  0x0000
#define BFile_Type_File       0x0001
#define BFile_Type_Addin      BFile_Type_File
#define BFile_Type_Eact       0x0003
#define BFile_Type_Language   0x0004
#define BFile_Type_Bitmap     0x0005
#define BFile_Type_MainMem    0x0006
#define BFile_Type_Temp       0x0007
#define BFile_Type_Dot        0x0008
#define BFile_Type_DotDot     0x0009
#define BFile_Type_Volume     0x000a
#define BFile_Type_Archived   BFile_Type_File

#endif // #ifndef DEST_CASIO_CALC

#endif // __LOCALS_B_FILE_h__

// EOF
