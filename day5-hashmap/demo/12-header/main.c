/* Демо 12. Зачем нужен заголовок.  Команды — в README.md
 *
 * Рядом лежит no-header.c: тот же вызов, но без #include.
 */
#include <stdio.h>
#include "counter.h"

int main(void)
{
    printf("%ld\n", counter_add(2));
    printf("%ld\n", counter_add(40));
    return 0;
}
