#ifndef MP4V2_PLATFORM_TIME_TIME_H
#define MP4V2_PLATFORM_TIME_TIME_H

namespace mp4v2 { namespace platform { namespace time {

///////////////////////////////////////////////////////////////////////////////

typedef int64_t milliseconds_t;
typedef int64_t seconds_t;

/*
 * Get local-time in milliseconds since midnight, January 1, 1970.
 */
milliseconds_t getLocalTimeMilliseconds();

/*
 * Get local-time in seconds since midnight, January 1, 1970.
 */
seconds_t getLocalTimeSeconds();

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::time

#endif // MP4V2_PLATFORM_TIME_TIME_H
