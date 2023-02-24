//
// Created by Jacob Lim on 2023/02/23.
//

#ifndef ENGINE_COMMONHEADER_H
#define ENGINE_COMMONHEADER_H

#include <cstdlib>
#define Assert(expression, message) assert(expression && message)
#define FORCEINLINE __attribute__((always_inline))

typedef const char* c_pointer;
static const size_t NPOS = -1;

#endif // ENGINE_COMMONHEADER_H
