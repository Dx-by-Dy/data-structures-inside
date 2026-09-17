#include "util.h"

/* static — имя видно только внутри этого файла. */
static int doubled(int x)
{
    return 2 * x;
}

int quadrupled(int x)
{
    return doubled(doubled(x));
}
