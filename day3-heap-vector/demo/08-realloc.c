/* Демо 8. Блок растёт.
 *
 *   gcc -std=c11 -Wall -Wextra 08-realloc.c -o 08-realloc && ./08-realloc
 *
 * Программа кладёт в массив шестнадцать чисел, удваивая вместимость по мере
 * надобности, и печатает адрес блока до и после каждого realloc.
 *
 * До запуска: если блок вырос вдвое, останется ли он на прежнем месте?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t cap = 2;
    size_t len = 0;

    int *a = malloc(cap * sizeof *a);
    if (a == NULL)
        return 1;

    /* Сосед, выделенный сразу после нашего блока. */
    int *neighbour = malloc(64 * sizeof *neighbour);
    if (neighbour == NULL) {
        free(a);
        return 1;
    }
    printf("сосед занял %p\n\n", (void *)neighbour);

    for (int value = 1; value <= 16; value++) {
        if (len == cap) {
            size_t new_cap = cap * 2;

            printf("cap %zu -> %zu, было по адресу %p\n",
                   cap, new_cap, (void *)a);

            int *bigger = realloc(a, new_cap * sizeof *bigger);
            if (bigger == NULL) {
                free(a);
                free(neighbour);
                return 1;
            }
            a = bigger;
            cap = new_cap;
            printf("             стало по адресу %p\n", (void *)a);

            printf("  данные на месте:");
            for (size_t i = 0; i < len; i++)
                printf(" %d", a[i]);
            printf("\n");
        }
        a[len++] = value;
    }

    printf("\nитого %zu элементов при вместимости %zu\n", len, cap);

    free(a);
    free(neighbour);
    return 0;
}
