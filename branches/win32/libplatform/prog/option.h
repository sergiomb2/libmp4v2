#ifndef MP4V2_PLATFORM_PROG_OPTION_H
#define MP4V2_PLATFORM_PROG_OPTION_H

namespace mp4v2 { namespace platform { namespace prog {

///////////////////////////////////////////////////////////////////////////////

/*
 * This function is identical in behavior to FreeBSD 7.0 getopt_long functions.
 * Virtually any OS which has getopt_long will adequately document this
 * functionality.
 *
 * To avoid an symbol clashes, code should avoid using namespace prog, and
 * instead use explicit namespace qualifiers to avoid symbol ambiguity.
 *
 * The following identifier have been renamed:
 *
 * getopt_long()      -> getOption()
 * getopt_long_only() -> getOptionWord()
 * option             -> Option
 * option.has_arg     -> Option.type
 */ 

extern char* optarg;
extern int   optind;
extern int   optopt;
extern int   opterr;
extern int   optreset;

struct Option
{
    enum Type {
        NO_ARG,
        REQUIRED_ARG,
        OPTIONAL_ARG,
    };

    char* name;  // name without leading double-dash
    Type  type;  // one of the enumerated types
    int*  flag;
    int   val;
};

int getOption     ( int, char* const*, const char*, const Option*, int* );
int getOptionWord ( int, char* const*, const char*, const Option*, int* );

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::prog

#endif // MP4V2_PLATFORM_PROG_OPTION_H
