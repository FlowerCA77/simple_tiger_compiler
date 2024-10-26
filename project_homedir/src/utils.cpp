/**
 *  @defgroup utils
 *  @brief realize of utils.hpp
 *  @details USE ASSERT!!!
 */
#include <assert.h>
#include <cstdlib>
#include <cstring>

#include "utils.hpp"

/**
 * @details In C++, memory allocator in <cstdlib>
 * @warning malloc will return NULL (C) or nullptr (C++>=11)
 */
void *checked_malloc(std::size_t size)
{
    void *p = std::malloc(size);
    assert(p);
    return p;
}

string String(string origin)
{
    char* copy_in_heat = (char *)checked_malloc(std::strlen(origin) + 1);
    std::strcpy(copy_in_heat, origin);
    return copy_in_heat;
}