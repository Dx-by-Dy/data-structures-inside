/* Демо 4. Шаг +1.
 *
 *   gcc -std=c11 -Wall -Wextra 04-pointer-arith.c -o 04-pointer-arith
 *   ./04-pointer-arith
 *
 * Выпишите до запуска: на сколько БАЙТ изменится адрес при p + 1 для int *,
 * char * и double *.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

int main(void)
{
    int    ints[5]    = { 10, 20, 30, 40, 50 };
    char   chars[5]   = { 'a', 'b', 'c', 'd', 'e' };
    double doubles[5] = { 1.5, 2.5, 3.5, 4.5, 5.5 };

    printf("элементы массива лежат подряд:\n");
    for (int i = 0; i < 5; i++)
        printf("  &ints[%d] = %p\n", i, (void *)&ints[i]);

    printf("\nшаг +1 для указателей на разные типы:\n");
    printf("  int    *: %p -> %p\n", (void *)ints,    (void *)(ints + 1));
    printf("  char   *: %p -> %p\n", (void *)chars,   (void *)(chars + 1));
    printf("  double *: %p -> %p\n", (void *)doubles, (void *)(doubles + 1));

    printf("\nразмеры элементов: int %zu, char %zu, double %zu\n",
           sizeof ints[0], sizeof chars[0], sizeof doubles[0]);

    return 0;
}
