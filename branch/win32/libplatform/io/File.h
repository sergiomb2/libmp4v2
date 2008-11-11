#ifndef MP4V2_PLATFORM_IO_FILE_H
#define MP4V2_PLATFORM_IO_FILE_H

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

/*
 * All files are 64-bit files unless built on a 32-bit platform which does not
 * support largefiles. The actual interface uses a signed 64-bit value which
 * can help with filesize math at the expense of limiting things to 63-bits
 * in size, roughly 9.22 million TB.
 */
class File
{
public:
    typedef int64_t Size;

public:
    virtual ~File();

    virtual bool close() = 0;

    virtual bool getPosition ( Size& ) = 0;
    virtual bool getSize     ( Size& ) = 0;
    virtual bool setPosition ( Size )  = 0;

    virtual bool read  ( void*, Size, Size&, Size = 0 )       = 0;
    virtual bool write ( const void*, Size, Size&, Size = 0 ) = 0;

protected:
    File( std::string, bool );

protected:
    std::string _name;
    const bool  _owner;

private:
    File(); // disabled

public:
    const std::string& name;
};

///////////////////////////////////////////////////////////////////////////////

class StdioFile : public File
{
public:
    typedef std::FILE* Handle;

public:
    explicit StdioFile ( std::string = "", bool = true );
    explicit StdioFile ( Handle, bool = false );
    ~StdioFile();

    bool open( std::string );
    bool open( std::string, std::string );

public: // virtual implementation
    bool close();

    bool getPosition ( Size& );
    bool getSize     ( Size& );
    bool setPosition ( Size );

    bool read  ( void*, Size, Size&, Size = 0 );
    bool write ( const void*, Size, Size&, Size = 0 );

private:
    Handle _handle;

public:
    const Handle& handle;

public:
    static bool getPosition ( Handle, Size& );
    static bool getSize     ( Handle, Size& );
    static bool setPosition ( Handle, Size );
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io

#endif // MP4V2_PLATFORM_IO_FILE_H
