/* Демо 11. Программа из двух единиц трансляции.  Команды — в README.md
 *
 * До запуска: компилятор обрабатывает main.c, ничего не зная про area.c.
 * Откуда он берёт адрес rect_area?
 */
#include <stdio.h>
#include "area.h"

int main(void)
{
    printf("%d\n", rect_area(3, 4));
    return 0;
}
