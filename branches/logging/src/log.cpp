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

///////////////////////////////////////////////////////////////////////////////

/**
 * Log class constructor
 */
Log::Log( MP4LogLevel verbosity_ /* = MP4LogNone */ )
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
    MP4LogLevel retval = MP4LogNone;

    if (details & MP4_DETAILS_ERROR)
    {
        retval = MP4LogError;
    }

    if (details & MP4_DETAILS_WARNING)
    {
        retval = MP4LogWarning;
    }

    // None of the MP4_DETAILS_* flags maps to MP4LogInfo

    if ((details & MP4_DETAILS_READ) ||
        (details & MP4_DETAILS_WRITE) ||
        (details & MP4_DETAILS_FIND) ||
        (details & MP4_DETAILS_HINT) ||
        (details & MP4_DETAILS_ISMA) ||
        (details & MP4_DETAILS_EDIT))
    {
        retval = MP4LogVerbose1;
    }

    if (details & MP4_DETAILS_TABLE)
    {
        retval = MP4LogVerbose2;
    }

    if (details & MP4_DETAILS_SAMPLE)
    {
        retval = MP4LogVerbose3;
    }

    // All of the specific flags are covered above, but if
    // the caller wants _everything_, give it to 'em
    if (details & MP4_DETAILS_ALL)
    {
        retval = MP4LogVerbose4;
    }

    // If no flags were specified, we'll have verbosity set
    // to MP4LogNone which is valid.
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
    this->vprintf(MP4LogError,format,ap);
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
    this->vprintf(MP4LogWarning,format,ap);
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
    this->vprintf(MP4LogInfo,format,ap);
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
    this->vprintf(MP4LogVerbose1,format,ap);
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
    this->vprintf(MP4LogVerbose1,format,ap);
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
    // Make sure nothing gets logged with MP4LogNone.  That
    // way people who ask for nothing to get logged won't
    // get anything logged.
    ASSERT(verbosity_ != MP4LogNone);

    if (verbosity_ > this->_verbosity)
    {
        // We're not set verbose enough to log this
        return;
    }

    if (Log::_cb_func)
    {
        Log::_cb_func(verbosity,format,ap);
        return;
    }

    // No callback set so log to standard out.  
    ::vfprintf(stdout,format,ap);
    ::fprintf(stdout,"\n");
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl

using namespace mp4v2::impl;

extern "C"
void MP4SetLogCallback( MP4LogCallback cb_func )
{
    Log::setLogCallback(cb_func);
}
