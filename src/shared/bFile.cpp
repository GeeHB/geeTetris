//---------------------------------------------------------------------------
//--
//--	File	: bFile.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Implementation of bFile object
//--
//--		This very first release doesn't handle folder and is only
//--		functionnal for FXCG50 calculator
//--
//---------------------------------------------------------------------------

#include "bFile.h"

// Construction
//
bFile::bFile(){
#ifdef DEST_CASIO_CALC
    fd_ = 0;    // no file
#else
    fileName_ = "";
#endif // #ifdef DEST_CASIO_CALC

    error_ = 0; // no error
}

// Destruction
//
bFile::~bFile(){
    close();
}

// isOpen() : Is the file already open ?
//
//  Check wether file (or folder) exists
//  and last access error
//
//  @return : true if the object is valid
//
bool bFile::isOpen(){
#ifdef DEST_CASIO_CALC
    return (fd_ && !error_);
#else
    return file_.is_open();
#endif // #ifdef DEST_CASIO_CALC
}

// size() : Get file size
//
// @return : size of current opened file or an error code
//
int bFile::size(){
    if (isOpen()){
#ifdef DEST_CASIO_CALC
        return gint_world_switch(GINT_CALL(BFile_Size, fd_));
#else
        file_.seekg (0, file_.end);
        return file_.tellg();
#endif // #ifdef DEST_CASIO_CALC
    }

    // No open file
    return 0;
}

// open() : Open an existing file
//
// @filename : name of the file to open
// @access : Access mode to the file
//
// @return : file opened ?
//
bool bFile::open(FONTCHARACTER filename, int access){
    if (access && !isOpen()){
#ifdef DEST_CASIO_CALC
        fd_ = gint_world_switch(GINT_CALL(BFile_Open, filename, access));
        if (fd_ < 0){
            error_ = fd_;
            fd_ = 0;
        }
        else{
            error_ = 0;     // The file is open
        }
#else
        std::ios_base::openmode mode;
        if (access & BFile_ReadOnly){
            mode = std::fstream::in | std::ifstream::binary;
        }
        else {
        //if (access & BFile_WriteOnly){
            mode = std::fstream::out | std::fstream::trunc;
        }

        file_.open(filename, mode);
        fileName_ = filename;
#endif // #ifdef DEST_CASIO_CALC

        // Done ?
        return isOpen();
    }

    return false;
}

// create() : Create a file or a folder
//
// @filename : name of the file or folder to create (must not exist)
// @type : Entry type (BFile_File or BFile_Folder)
// @size : Pointer to file size if type is BFile_File, use NULL otherwise
//
// @return : file successfully created ?
//
bool bFile::create(FONTCHARACTER filename, int type, int *size){
    // File can't be open
    if (!isOpen()){
#ifdef DEST_CASIO_CALC
        error_ = gint_world_switch(GINT_CALL(BFile_Create, filename, type, size));
        return (error_ == 0);	// Created ?
#else
	// size if ignored
	if (open(filename, BFile_ReadWrite)){
		if (isOpen()){
			close();
			return true;
		}
	}
#endif // #ifdef DEST_CASIO_CALC
    }

    return false;
}

// write() : Write data in the current file
//
// @data : Pointer to the data buffer
// @size : Size in byte to write
//
// @return : data written ?
//
bool bFile::write(void const *data, int even_size){
    if (data && even_size && isOpen()){
#ifdef DEST_CASIO_CALC
        error_ = gint_world_switch(GINT_CALL(BFile_Write, fd_, data, even_size));
        return (error_ == 0);	// data written ?
#else
    file_.write((const char*)data, even_size);

	// done ?
	return file_.good();
#endif // #ifdef DEST_CASIO_CALC
    }

    return false;
}

// read() : Read data from the current file
//
// @data : Pointer to the destination buffer
// @lg : Size in byte to read
// @whence :
//
// @return : # bytes read
//
int bFile::read(void *data, int lg, int whence){
    if (data && lg && isOpen()){
#ifdef DEST_CASIO_CALC
        int read = gint_world_switch(GINT_CALL(BFile_Read, fd_, data, lg, whence));
        if (read < 0){
        	error_ = read;
        	return 0;
        }

        error_ = 0;
        return read;	// #bytes read
#else
    file_.read((char*)data, lg);
    int red = file_.gcount();

    // # bytes read
    return red;
#endif // #ifdef DEST_CASIO_CALC
    }

    return 0;   // read nothing
}

// remove() : Remove a file
//
// @filename : name of the file to remove
//
// @return : file successfully removed ?
//
bool bFile::remove(FONTCHARACTER filename){
    // File can't be open
    if (!isOpen()){
#ifdef DEST_CASIO_CALC
        error_ = gint_world_switch(GINT_CALL(BFile_Remove, filename));
        return (error_ == 0);	// Removed ?
#else
	return (0 == std::remove(filename));
#endif // #ifdef DEST_CASIO_CALC
    }

    return false;
}

// close() : Close the file
//
void bFile::close(){
#ifdef DEST_CASIO_CALC
    if (fd_){
        error_ = gint_world_switch(GINT_CALL(BFile_Close, fd_));
        fd_ = 0;
    }
#else
    if (file_.is_open()){
        file_.close();
    }

    fileName_ = "";
#endif // #ifdef DEST_CASIO_CALC
}

// EOF
