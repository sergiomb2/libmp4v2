#ifndef MP4V2_PLATFORM_IO_FILESYSTEM_H
#define MP4V2_PLATFORM_IO_FILESYSTEM_H

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

class FileSystem
{
public:
    static bool exists       ( std::string );
    static bool isDirectory  ( std::string );
    static bool isFile       ( std::string );
    static bool rename       ( std::string, std::string );
    static bool tempFilename ( std::string&, string = ".", string = "tmp", string = "" );
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io

#endif // MP4V2_PLATFORM_IO_FILESYSTEM_H
