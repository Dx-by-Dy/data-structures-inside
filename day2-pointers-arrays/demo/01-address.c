/* Демо 1. Адреса четырёх переменных.
 *
 *   gcc -std=c11 -Wall -Wextra 01-address.c -o 01-address && ./01-address
 *
 * До запуска: переменные объявлены подряд — насколько далеко их адреса друг
 * от друга и в каком порядке они расположены?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

int main(void)
{
    int    x = 37;
    int    y = 100;
    double d = 0.5;
    char   c = 'A';

    printf("значения:\n");
    printf("  x = %d,  y = %d,  d = %g,  c = %c\n\n", x, y, d, c);

    printf("адреса:\n");
    printf("  &x = %p   sizeof x = %zu\n", (void *)&x, sizeof x);
    printf("  &y = %p   sizeof y = %zu\n", (void *)&y, sizeof y);
    printf("  &d = %p   sizeof d = %zu\n", (void *)&d, sizeof d);
    printf("  &c = %p   sizeof c = %zu\n", (void *)&c, sizeof c);

    return 0;
}
