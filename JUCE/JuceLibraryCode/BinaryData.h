/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   tarantino_png;
    const int            tarantino_pngSize = 70725;

    extern const char*   pulpFilter_png;
    const int            pulpFilter_pngSize = 38448;

    extern const char*   miaWallace_png;
    const int            miaWallace_pngSize = 267095;

    extern const char*   butch_png;
    const int            butch_pngSize = 165386;

    extern const char*   Vinsynth_png;
    const int            Vinsynth_pngSize = 22116;

    extern const char*   backgroundNew_png;
    const int            backgroundNew_pngSize = 146669;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
