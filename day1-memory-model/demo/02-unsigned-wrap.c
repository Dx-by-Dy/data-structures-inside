/* Демо 2. Беззнаковая арифметика на границе типа.
 *
 *   gcc -std=c11 -Wall -Wextra 02-unsigned-wrap.c -o 02-unsigned-wrap
 *   ./02-unsigned-wrap
 *
 * До запуска: что окажется в переменной типа unsigned int, если к её
 * максимальному значению прибавить единицу? А если из нуля вычесть единицу?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <limits.h>

int main(void)
{
    unsigned int u = UINT_MAX;
    printf("UINT_MAX     = %u\n", u);
    u = u + 1;
    printf("UINT_MAX + 1 = %u\n", u);

    unsigned int z = 0;
    printf("\n0            = %u\n", z);
    z = z - 1;
    printf("0 - 1        = %u\n", z);

    printf("\n2 в степени %d = %llu\n",
           (int)(sizeof(unsigned int) * CHAR_BIT),
           (unsigned long long)UINT_MAX + 1);

    return 0;
}
