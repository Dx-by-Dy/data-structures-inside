/* Демо 2. Три блока, которых никто не заполнял.
 *
 *   gcc -std=c11 -Wall -Wextra 02-uninitialized.c -o 02-uninitialized
 *   ./02-uninitialized
 *
 * До запуска предскажите все три опыта: свежий блок от malloc, блок от calloc
 * и блок, взятый сразу после того, как такой же блок освободили.
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

static void print4(const char *label, const int *a)
{
    printf("%s %d %d %d %d\n", label, a[0], a[1], a[2], a[3]);
}

int main(void)
{
    /* Опыт 1. Свежий блок, ни одной записи. */
    int *fresh = malloc(4 * sizeof *fresh);
    if (fresh == NULL)
        return 1;
    print4("malloc, не заполняя:", fresh);

    /* Опыт 2. То же самое через calloc. */
    int *zeroed = calloc(4, sizeof *zeroed);
    if (zeroed == NULL) {
        free(fresh);
        return 1;
    }
    print4("calloc:            ", zeroed);

    free(fresh);
    free(zeroed);

    /* Опыт 3. Узнаваемые числа, free, и тут же блок того же размера. */
    printf("\n");
    int *first = malloc(4 * sizeof *first);
    if (first == NULL)
        return 1;
    for (int i = 0; i < 4; i++)
        first[i] = 1000 + i;
    printf("в блок %p записали 1000 1001 1002 1003, освобождаем\n",
           (void *)first);
    free(first);

    int *second = malloc(4 * sizeof *second);
    if (second == NULL)
        return 1;
    printf("новый блок %p, читаем не заполнив: %d %d %d %d\n",
           (void *)second, second[0], second[1], second[2], second[3]);
    free(second);

    return 0;
}
