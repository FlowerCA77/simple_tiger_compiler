/** @file utils.hpp */
#include <assert.h>
#include <cstdlib>
#include <string>

/** @warning we will store it in heap */
using string = const char *;
/** @brief string constructor */
string String(string);

/**
 * @brief Don't use `new`/`delete`(C++), `malloc`/`calloc`/`free`(C)
 * @details noticed that, we will allocate memory for struct with union member
 * because of the uncertain size of union, the default constructor is deleted,
 * so new cannot used to create an struct object which with union member.
 *
 * for some reason, we do not use `new`/`delete` in all project code, and we
 * will use checked_malloc forever instead of `malloc`/`calloc`, and we never
 * use free because of garbage collector. (Reason can be found in textbook.)
 *
 * @param std::size_t the size will allocated
 * @return a `void *` pointer point to memory which is allocated by this call.
 */
void *checked_malloc(std::size_t);