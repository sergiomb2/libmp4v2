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

#ifndef MP4V2_IMPL_LOG_H
#define MP4V2_IMPL_LOG_H

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

/**
 * Handle logging either to standard out or to a callback
 * function
 */
class MP4V2_EXPORT Log {
private:
    MP4LogLevel                 _verbosity;
    static MP4LogCallback       _cb_func;

public:
    const MP4LogLevel&          verbosity;

public:
    Log( MP4LogLevel = MP4LogNone );
    Log( uint32_t );
    virtual ~Log();

    static void setLogCallback ( MP4LogCallback );

    void setVerbosity   ( MP4LogLevel );
    void setVerbosity   ( uint32_t );

    void errorf ( const char* format, ... );
    void warningf ( const char* format, ... );
    void infof ( const char* format, ... );
    void verbose1f ( const char* format, ... );
    void verbose2f ( const char* format, ... );
    void verbose3f ( const char* format, ... );
    void verbose4f ( const char* format, ... );

    void printf ( MP4LogLevel   verbosity_,
                  const char*   format, ... );
    void vprintf ( MP4LogLevel  verbosity_,
                   const char*  format, va_list ap );

private:
    static MP4LogLevel detailsToLevel ( uint32_t );
};

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl

#endif // MP4V2_IMPL_LOG_H
