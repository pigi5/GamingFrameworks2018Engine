#include "../header/Utils.h"

// Implements the signum function.
// params:
//   val - the value to check sign of
// returns: -1 if val is negative, 0 if val is 0, 1 if val is positive
template <typename T> int engine_util::sign(T val)
{
    return (T(0) < val) - (val < T(0));
}