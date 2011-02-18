#ifndef GETOPT_BASIC_H

#define GETOPT_BASIC_H

#ifdef _MSC_VER
  //#define snprintf(str, size, format, ...) _snprintf_s(str, size, size, format, __VA_ARGS__ )
  #define snprintf(str, size, format, ...) _snprintf_s(str, size, size, format, ## __VA_ARGS__ )

  // use token paste operator ##
  // which allows extra args to be ommitted entirely
  // see http://www.redhat.com/docs/manuals/enterprise/RHEL-4-Manual/gcc/variadic-macros.html
  //#define debug(format, ...) fprintf (stderr, format, ## __VA_ARGS__)
#endif

// using c++, so define a namespace
namespace getopt_basic {

// avoid conflicting with c++ calls by using extern c
//extern "C" {

// define extern here, it's being defined in another file (.c)
// otherwise get a redefinition error
extern char *optarg;

extern int optind;
extern int opterr;
extern int optopt;

struct option {
              const char *name;
              int has_arg;
              int *flag;
              int val;
};

# define no_argument		0
# define required_argument	1
# define optional_argument	2

// compiles without extern here, but this makes it clearer
extern int getopt_basic (int argc, char *const *argv, const char* shortopts, const struct option *longopts, int *longind);

//} // extern c

}
#endif

