#include "counter.h"

static long total = 0;

long counter_add(long value)
{
    total += value;
    return total;
}
