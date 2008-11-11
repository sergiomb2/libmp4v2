#include "libplatform/impl.h"
#include <sys/timeb.h>

namespace mp4v2 { namespace platform { namespace time {

///////////////////////////////////////////////////////////////////////////////

milliseconds_t
getLocalTimeMilliseconds()
{
    _timeb buf;
    _ftime( &buf );
    return milliseconds_t( buf.time ) * 1000 + buf.millitm;
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::time
