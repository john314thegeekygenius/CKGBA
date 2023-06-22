/**
 * GBA File hander
 * 
 * Makes the SRAM work as file storage
 * 
 * Written by: John314
 * 3/18/2023
 * 
*/
#ifndef __GBA_FILE_H_
#define __GBA_FILE_H_

typedef unsigned int FileHandle;

typedef unsigned short BlockSize;

typedef enum {
//    File_NoError, // Should just return a valid handle
    File_OpenFail = -1,
    File_ReadFail = -2,
    File_WriteFail = -3,
    File_HandleError = -4,
    File_Unknown = -5,
}FileErrors;

typedef enum {
    FileIO_Read = 0x1,
    FileIO_Write = 0x2,
}FileIOType;

#define FILE_MAX_BLOCKS 16

#define SIZE_OF_SRAM 0x7FFF

FileHandle openHandle(unsigned short identifier, FileIOType iotype, BlockSize size);
// Will return an error code, or size written
FileErrors readHandle(FileHandle *handle, void *data, unsigned int size);
FileErrors writeHandle(FileHandle *handle, void *data, unsigned int size);

// Erases the entire SRAM with 0xFE
void WipeSRam();
void SetupSRam();

#endif