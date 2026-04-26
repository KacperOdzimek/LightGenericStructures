#ifndef GENERIC_H
#define GENERIC_H

// T/A macros selectors
#define LGS_FIRST_IMPL(a, ...) a
#define LGS_FIRST(...)   LGS_FIRST_IMPL(__VA_ARGS__)

#define LGS_SECOND_IMPL(a,b, ...) b
#define LGS_SECOND(...)  LGS_SECOND_IMPL(__VA_ARGS__)

#define LGS_THIRD_IMPL(a,b,c, ...) c
#define LGS_THIRD(...)   LGS_THIRD_IMPL(__VA_ARGS__)

#define LGS_FOURTH_IMPL(a,b,c,d, ...) d
#define LGS_FOURTH(...)  LGS_FOURTH_IMPL(__VA_ARGS__)

#define LGS_FIFTH_IMPL(a,b,c,d,e, ...) e
#define LGS_FIFTH(...)   LGS_FIFTH_IMPL(__VA_ARGS__)

#define LGS_SIXTH_IMPL(a,b,c,d,e,f, ...) f
#define LGS_SIXTH(...)   LGS_SIXTH_IMPL(__VA_ARGS__)

#define LGS_SEVENTH_IMPL(a,b,c,d,e,f,g, ...) g
#define LGS_SEVENTH(...) LGS_SEVENTH_IMPL(__VA_ARGS__)

#define LGS_EIGHTH_IMPL(a,b,c,d,e,f,g,h, ...) h
#define LGS_EIGHTH(...)  LGS_EIGHTH_IMPL(__VA_ARGS__)

#define LGS_NINTH_IMPL(a,b,c,d,e,f,g,h,i, ...) i
#define LGS_NINTH(...)   LGS_NINTH_IMPL(__VA_ARGS__)

#define LGS_TENTH_IMPL(a,b,c,d,e,f,g,h,i,j, ...) j
#define LGS_TENTH(...)   LGS_TENTH_IMPL(__VA_ARGS__)

// fir joining texts together
#define LGS_CAT_IMPL(a, b) a##b
#define LGS_CAT(a, b)  LGS_CAT_IMPL(a, b)

// for making instances-names
#define LGS_INST(thing, instance_name) LGS_CAT(LGS_CAT(LGS_implementation_of_, thing), LGS_CAT(_for_, instance_name))

// for defining functions
#define LGS_API(ret) static inline ret

// success flag
#define SCC 1

// error flag
#define ERR 0

// second layer, just to ensure correct expansion
#define LGS_PASS_VAARGS(...) __VA_ARGS__

// dispatch A macro
#define LGS_ALLOC   LGS_FIRST(LGS_PASS_VAARGS(A))
#define LGS_REALLOC LGS_SECOND(LGS_PASS_VAARGS(A))
#define LGS_FREE    LGS_THIRD(LGS_PASS_VAARGS(A))

#endif // GENERIC_H
