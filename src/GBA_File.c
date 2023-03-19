/**
 * GBA File hander
 * 
 * Makes the SRAM work as file storage
 * 
 * Written by: John314
 * 3/18/2023
 * 
*/

/*

Storage Format:

--BLOCKS:
  --FILE_HEADER:
    BLOCK_IDENTIFIER
    BLOCK_SIZE
  DATA
*/

#include "GBA_File.h"

// Stored LittleEndian
static struct F_Block_t {
    unsigned short identifier; // 2 bytes for file identifier
    unsigned short block_size; // size of block in bytes (does not include this header)
};

typedef unsigned char * FilePtr;

unsigned char *GBA_PakRam = (unsigned char*)0x0E000000;
unsigned char *GBA_EndOfSRAM = (unsigned char*)0x0E00FFFF;

FileHandle openHandle(unsigned short identifier, FileIOType iotype, BlockSize size){
    unsigned char* file;
    struct F_Block_t tempblock;

    if(iotype & FileIO_Read) {
        // Find the file
        file = GBA_PakRam;
        while(file < GBA_EndOfSRAM){
            tempblock.identifier = *(file++);
            tempblock.identifier |= (*(file++))<<8;
            tempblock.block_size = *(file++);
            tempblock.block_size |= (*(file++))<<8;
            // See if it's the file
            if(tempblock.identifier == identifier){
                if(tempblock.block_size == size){
                    return (FileHandle)file;
                }
            }
            // Skip the block size if it's a valid block
            if(tempblock.identifier == 0xFFFF || file >= GBA_EndOfSRAM){
                // Must be out of valid blocks???
                return File_OpenFail;
            }
            file += tempblock.block_size;
        }
        goto badfile;
    }
badfile:
    if(iotype & FileIO_Write) {
        // Write a new file
        file = GBA_PakRam;
        while(file < GBA_EndOfSRAM){
            tempblock.identifier = *(file++);
            tempblock.identifier |= (*(file++))<<8;
            tempblock.block_size = *(file++);
            tempblock.block_size |= (*(file++))<<8;
            // See if it's the same file
            if(tempblock.identifier == identifier){
                if(tempblock.block_size == size){
                    return (FileHandle)file;
                }
            }
            // Skip the block size if it's a valid block
            if(tempblock.identifier == 0xFFFF){
                // Must be out of valid blocks???
                if(file >= GBA_EndOfSRAM)
                    return File_OpenFail;
                // Write a new header
                tempblock.identifier = identifier;
                tempblock.block_size = size;
                file -= 4; // move back 4 bytes
                *(file++) = tempblock.identifier&0xFF;
                *(file++) = tempblock.identifier>>8;
                *(file++) = tempblock.block_size&0xFF;
                *(file++) = tempblock.block_size>>8;
                return (FileHandle)file;
            }
            file += tempblock.block_size;
        }
    }
    return File_OpenFail;
};
/*
void closeHandle(FileHandle handle){
    if(handle == 0) return; // Nothing to do
    if(handle >= 0xFFFF) return; // Again, bad handle
};
*/
FileErrors readHandle(FileHandle *handle, void *data, unsigned int size){
    if(*handle == 0) return File_HandleError; // Nothing to do
    if(*handle >= GBA_EndOfSRAM) return File_HandleError; // Again, bad handle
    unsigned char* readbuffer = (unsigned char*)data;
    unsigned char* filebuffer = (unsigned char*)*handle;
    unsigned int readsize = 0;
    while(size--){
        *(readbuffer++) = *(filebuffer++);
        if(filebuffer >= GBA_EndOfSRAM){
            return File_ReadFail;
        }
        readsize++;
    }
    *handle += readsize;
    return readsize;
};

FileErrors writeHandle(FileHandle *handle, void *data, unsigned int size){
    if(*handle == 0) return File_HandleError; // Nothing to do
    if(*handle >= GBA_EndOfSRAM) return File_HandleError; // Again, bad handle
    unsigned char* writebuffer = (unsigned char*)data;
    unsigned char* filebuffer = (unsigned char*)*handle;
    unsigned int writesize = 0;
    while(size--){
        *(filebuffer++) = *(writebuffer++);
        if(filebuffer >= GBA_EndOfSRAM){
            return File_WriteFail;
        }
        writesize++;
    }
    *handle += writesize;
    return writesize;
};


