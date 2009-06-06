///////////////////////////////////////////////////////////////////////////////
//
//  The contents of this file are subject to the Mozilla Public License
//  Version 1.1 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.mozilla.org/MPL/
//
//  Software distributed under the License is distributed on an "AS IS"
//  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
//  License for the specific language governing rights and limitations
//  under the License.
// 
//  The Original Code is MP4v2.
// 
//  The Initial Developer of the Original Code is David Byron.
//  Portions created by David Byron are Copyright (C) 2009.
//  All Rights Reserved.
//
//  Contributors:
//      David Byron, dbyron@dbyron.com
//
///////////////////////////////////////////////////////////////////////////////

#include "src/impl.h"

namespace mp4v2 { namespace impl {

MP4LogCallback Log::_cb_func = NULL;

// There's no mechanism to set the log level at runtime at
// the moment so construct this so it only logs important
// stuff.
Log log(MP4_LOG_WARNING);

///////////////////////////////////////////////////////////////////////////////

/**
 * Log class constructor
 */
Log::Log( MP4LogLevel verbosity_ /* = MP4_LOG_NONE */ )
    : _verbosity ( verbosity_ )
    , verbosity  ( _verbosity )
{
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Constructor for the Log class that converts from the
 * bit-flag model of verbosity to the MP4LogLevel.
 */
Log::Log( uint32_t verbosity_ )
    : _verbosity ( Log::detailsToLevel(verbosity_) )
    , verbosity  ( _verbosity )
{
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log class destructor
 */
Log::~Log()
{
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Convert MP4_DETAILS_* bit-flag-based verbosity to MP4LogLevel
 *
 * @param details the bit-flag-based verbosity to convert
 *
 * @return the MP4LogLevel corresponding to @p details
 */
MP4LogLevel
Log::detailsToLevel ( uint32_t details )
{
    // Handle this special case first.  MP4_DETAILS_ALL has
    // all bits set.
    if (details == MP4_DETAILS_ALL)
    {
        return MP4_LOG_VERBOSE4;
    }

    MP4LogLevel retval = MP4_LOG_NONE;

    if (details & MP4_DETAILS_ERROR)
    {
        retval = MP4_LOG_ERROR;
    }

    if (details & MP4_DETAILS_WARNING)
    {
        retval = MP4_LOG_WARNING;
    }

    // None of the MP4_DETAILS_* flags maps to MP4_LOG_INFO

    if ((details & MP4_DETAILS_READ) ||
        (details & MP4_DETAILS_WRITE) ||
        (details & MP4_DETAILS_FIND) ||
        (details & MP4_DETAILS_HINT) ||
        (details & MP4_DETAILS_ISMA) ||
        (details & MP4_DETAILS_EDIT))
    {
        retval = MP4_LOG_VERBOSE1;
    }

    if (details & MP4_DETAILS_TABLE)
    {
        retval = MP4_LOG_VERBOSE2;
    }

    if (details & MP4_DETAILS_SAMPLE)
    {
        retval = MP4_LOG_VERBOSE3;
    }

    // If no flags were specified, we'll have verbosity set
    // to MP4_LOG_NONE which is valid.
    return retval;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Accessor for verbosity converted to MP4_DETAILS_* bit flags
 *
 * @return the MP4_DETAILS_* bit flag-representation of
 * this->verbosity
 */
uint32_t
Log::getVerbosity() const
{
    uint32_t retval = 0;

    // Set the maximum number of bits that correspond to the
    // log level

    // Process the levels from most verbose on down so each
    // case can fall through.
    switch (this->verbosity) {
    case MP4_LOG_VERBOSE4:
        // No flags correspond to this level specifically
        // but if someone sets this, they get everything so
        // we may as well break out instead of falling
        // through.
        retval = MP4_DETAILS_ALL;
        break;
    case MP4_LOG_VERBOSE3:
        retval |= MP4_DETAILS_SAMPLE;
    case MP4_LOG_VERBOSE2:
        retval |= MP4_DETAILS_TABLE;
    case MP4_LOG_VERBOSE1:
        retval |= (MP4_DETAILS_READ | MP4_DETAILS_WRITE |
                   MP4_DETAILS_FIND | MP4_DETAILS_HINT |
                   MP4_DETAILS_ISMA | MP4_DETAILS_EDIT);
    case MP4_LOG_INFO:
        // No flags correspond to MP4_LOG_INFO, but fall
        // through to get the more serious ones
    case MP4_LOG_WARNING:
        retval |= MP4_DETAILS_WARNING;
    case MP4_LOG_ERROR:
        retval |= MP4_DETAILS_ERROR;
    case MP4_LOG_NONE:
        break;
    default:
        ASSERT(0);

        // Tempting to log something here using errorf.  We
        // can't at the moment since this is a const
        // function but errorf isn't.  It's possible that
        // having errorf be const makes sense, but even if
        // it was there's a reasonable argument for not
        // using it here.  If we get here, something is
        // broken in the logging code.  In that case, it's
        // reasonable to assume we don't get to use logging
        // to help us find/fix it.  We've got the ASSERT
        // which should be enough.  If we do use logging
        // here we run the risk of endless recursion or some
        // other struggle. (16-feb-09, dbyron)
        retval = 0;
        break;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Mutator for the callback function
 *
 * @param value the function to call
 */
void
Log::setLogCallback( MP4LogCallback value )
{
    Log::_cb_func = value;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Mutator for the verbosity
 *
 * @param value the verbosity to use
 */
void
Log::setVerbosity( MP4LogLevel value )
{
    _verbosity = value;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Mutator for the verbosity
 *
 * @param value the verbosity to use
 */
void
Log::setVerbosity( uint32_t details )
{
    _verbosity = detailsToLevel(details);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log an error message
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::errorf( const char* format,
             ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(MP4_LOG_ERROR,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log a warning message
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::warningf( const char* format,
               ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(MP4_LOG_WARNING,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log an info message
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::infof( const char* format,
            ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(MP4_LOG_INFO,format,ap);
    va_end(ap);
}

/**
 * Log a verbose1 message
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::verbose1f( const char* format,
                ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(MP4_LOG_VERBOSE1,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log a verbose2 message
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::verbose2f( const char* format,
                ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(MP4_LOG_VERBOSE2,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log a verbose3 message
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::verbose3f( const char* format,
                ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(MP4_LOG_VERBOSE3,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log a verbose4 message
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::verbose4f( const char* format,
                ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(MP4_LOG_VERBOSE4,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log a message
 *
 * @param verbosity the level of detail the message contains
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::printf( MP4LogLevel        verbosity,
             const char*        format,
             ... )
{
    va_list     ap;

    va_start(ap,format);
    this->vprintf(verbosity,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log a message if it has appropriate verbosity, either to
 * standard out (with a newline appended to @p format) or to
 * the callback function (with no newline appended).
 *
 * @param verbosity the level of detail the message contains
 *
 * @param format the format string to use to process @p ap.
 * @p format should not contain a newline.
 *
 * @param ap varargs to build the message
 */
void
Log::vprintf( MP4LogLevel       verbosity_,
              const char*       format,
              va_list           ap )
{
    // Make sure nothing gets logged with MP4_LOG_NONE.
    // That way people who ask for nothing to get logged
    // won't get anything logged.
    ASSERT(verbosity_ != MP4_LOG_NONE);
    ASSERT(format);

    if (verbosity_ > this->_verbosity)
    {
        // We're not set verbose enough to log this
        return;
    }

    if (Log::_cb_func)
    {
        Log::_cb_func(verbosity_,format,ap);
        return;
    }

    // No callback set so log to standard out.  
    ::vfprintf(stdout,format,ap);
    ::fprintf(stdout,"\n");
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log a buffer as ascii-hex
 *
 * @param verbosity the level of detail the message contains
 *
 * @param pBytes the buffer to log
 *
 * @param numBytes the number of bytes to log
 *
 * @param format the format string to use to process the
 * remaining arguments, where the format + remaining args
 * describe @p pBytes.  The resulting string should not
 * contain a newline.  Only the first 255 characters of the
 * resulting string (not including the NUL terminator) make
 * it to the log callback.  If there is no log callback, the
 * entire string makes it to stdout.
 */
void
Log::hexDump( MP4LogLevel       verbosity_,
              uint8_t           *pBytes,
              uint32_t          numBytes,
              const char*       format,
              ... )
{
    va_list     ap;

    ASSERT(pBytes || (numBytes == 0));
    ASSERT(format);

    if (verbosity_ > this->_verbosity)
    {
        // We're not set verbose enough to log this
        return;
    }

    // For backwards compatibility, if there's no callback
    // function call MP4HexDump.  If there is a callback,
    // format things so they make sense in a log file.
    va_start(ap,format);
    if (!Log::_cb_func)
    {
        vfprintf(stdout,format,ap);
        va_end(ap);
        fprintf(stdout,"\n");
        MP4HexDump(pBytes,numBytes);
        return;
    }

    // Try to act like MP4HexDump, but to make sure each
    // line in the log file doesn't need other context to
    // make sense (since it may get interleaved with other
    // messages), put the description on each line, as well
    // as the offset into the buffer where the data comes
    // from.

    // Build the description.  Since we don't have asprintf,
    // pick an arbitrary length for the string and use
    // snprintf.
    char *desc = (char *)MP4Malloc(256);
    vsnprintf(desc,sizeof(desc),format,ap);
    va_end(ap);

    for (uint32_t i = 0;(i < numBytes);i += 16)
    {
        ostringstream oneLine(desc);

        // Append the byte offset this line starts with as
        // an 8 character, leading 0, hex number.  Leave the
        // fill character set to 0 for the remaining
        // operations
        oneLine << ":" << std::hex << std::setw(8) << setfill('0') <<
            std::right << i << ": ";

        // From here we the width to be 2 since we're
        // printing a byte at a time
        oneLine << std::setw(2);

        uint32_t curlen = min((uint32_t)16,numBytes - i);

        const uint8_t *b = pBytes + i;
        for (uint32_t j = 0;(j < curlen);j++)
        {
            oneLine << b[j] << " ";
        }

        // Finally, log this line.  We can either call the
        // callback directly or use the Log::printf
        // function.  To call the callback directly, we need
        // a va_list.  (I think) we need and extra function
        // call to build that, so we may as well call
        // Log::printf.  It's going to double-check the
        // verbosity and the callback function pointer, but
        // that seems OK (13-feb-09, dbyron)
        this->printf(verbosity_,"%s",oneLine.str());
    }

    MP4Free(desc);
    desc = NULL;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log an Exception as an error
 *
 * @param x the exception to log
 */
void
Log::errorf ( const Exception&      x )
{
    this->printf(MP4_LOG_ERROR,"%s",x.msg().c_str());
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log an error message
 *
 * @param olog the log object to use (or the global log
 * object if NULL)
 *
 * @param format the format string to use to process the
 * remaining arguments.  @p format should not contain a
 * newline.
 */
void
Log::errorf( Log*               olog,
             const char*        format,
             ... )
{
    va_list     ap;
    Log&        o = olog ? *olog : log;

    va_start(ap,format);
    o.vprintf(MP4_LOG_ERROR,format,ap);
    va_end(ap);
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Log an Exception as an error
 *
 * @param olog the log object to use (or the global log
 * object if NULL)
 *
 * @param x the exception to log
 */
void
Log::errorf( Log*               olog,
             const Exception&   x )
{
    Log& o = olog ? *olog : log;

    o.errorf(x);
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl

using namespace mp4v2::impl;

extern "C"
void MP4SetLogCallback( MP4LogCallback cb_func )
{
    Log::setLogCallback(cb_func);
}
