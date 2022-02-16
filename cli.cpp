#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <windows.h>

extern "C"
{

#pragma pack(push, 2)
    typedef struct {
        WORD Reserved1;       // reserved, must be 0
        WORD ResourceType;    // type is 1 for icons
        WORD ImageCount;      // number of icons in structure (1)
        BYTE Width;           // icon width (32)
        BYTE Height;          // icon height (32)
        BYTE Colors;          // colors (0 means more than 8 bits per pixel)
        BYTE Reserved2;       // reserved, must be 0
        WORD Planes;          // color planes
        WORD BitsPerPixel;    // bit depth
        DWORD ImageSize;      // size of structure
        WORD ResourceID;      // resource ID
    } GROUPICON;
#pragma pack(pop)

    __declspec(dllexport)void InjectMainIcon(char* ExecutableFile, char* IconFile)
    {
        //HANDLE hWhere = BeginUpdateResource((LPCWSTR)ExecutableFile, FALSE);

        //char* buffer;    // buffer to store raw icon data
        //long buffersize; // length of buffer
        //int hFile;       // file handle

        //hFile = open(IconFile, O_RDONLY | O_BINARY);
        //if (hFile == -1)
        //    return; // if file doesn't exist, can't be opened etc. 

        // // calculate buffer length and load file into buffer
        //buffersize = filelength(hFile);
        //buffer = (char*)malloc(buffersize);
        //read(hFile, buffer, buffersize);
        //close(hFile);

        //UpdateResource(
        //    hWhere,  // Handle to executable
        //    RT_ICON, // Resource type - icon
        //    MAKEINTRESOURCE(1), // Make the id 1
        //    MAKELANGID(LANG_ENGLISH,
        //        SUBLANG_DEFAULT), // Default language
        //    (buffer + 22),
        //    // skip the first 22 bytes because this is the 
        //    // icon header&directory entry (if the file 
        //    // contains multiple images the directory entries
        //    // will be larger than 22 bytes
        //    buffersize - 22  // length of buffer
        //);


        //// Again, we use this structure for educational purposes.
        //// The icon header and directory entries can be read from 
        //// the file.
        //GROUPICON grData;

        //// This is the header
        //grData.Reserved1 = 0;     // reserved, must be 0
        //grData.ResourceType = 1;  // type is 1 for icons
        //grData.ImageCount = 1;    // number of icons in structure (1)

        //// This is the directory entry
        //grData.Width = 32;        // icon width (32)
        //grData.Height = 32;       // icon height (32)
        //grData.Colors = 0;        // colors (256)
        //grData.Reserved2 = 0;     // reserved, must be 0
        //grData.Planes = 2;        // color planes
        //grData.BitsPerPixel = 32; // bit depth
        //grData.ImageSize = buffersize - 22; // size of image
        //grData.ResourceID = 1;       // resource ID is 1

        //UpdateResource(
        //    hWhere,
        //    RT_GROUP_ICON,
        //    // RT_GROUP_ICON resources contain information
        //    // about stored icons
        //    L"MAINICON",
        //    // MAINICON contains information about the
        //    // application's displayed icon
        //    MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        //    &grData,
        //    // Pointer to this structure
        //    sizeof(GROUPICON)
        //);

        //delete buffer; // free memory

        //// Perform the update, don't discard changes
        //EndUpdateResource(hWhere, FALSE);
    }


}