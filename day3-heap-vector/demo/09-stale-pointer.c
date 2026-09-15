/* Демо 9. Указатель, снятый до realloc.
 *
 *   gcc -std=c11 -Wall -Wextra 09-stale-pointer.c -o 09-stale-pointer
 *   ./09-stale-pointer
 *
 * Сначала прочитайте предупреждение компилятора: оно называет две строки
 * сразу. Потом запускайте.
 *
 * До запуска: что напечатает first[0] после realloc и с каким кодом
 * завершится программа?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t cap = 4;
    int *a = malloc(cap * sizeof *a);
    if (a == NULL)
        return 1;
    for (size_t i = 0; i < cap; i++)
        a[i] = (int)(10 * (i + 1));

    int *neighbour = malloc(64 * sizeof *neighbour);
    if (neighbour == NULL) {
        free(a);
        return 1;
    }

    int *first = a;
    printf("запомнили адрес %p, first[0] = %d\n", (void *)first, first[0]);

    int *bigger = realloc(a, cap * 64 * sizeof *bigger);
    if (bigger == NULL) {
        free(a);
        free(neighbour);
        return 1;
    }
    a = bigger;
    cap *= 64;
    printf("после realloc блок по адресу %p, a[0] = %d\n", (void *)a, a[0]);

    printf("читаем first[0]: ");
    fflush(stdout);
    printf("%d\n", first[0]);

    free(a);
    free(neighbour);
    return 0;
}
