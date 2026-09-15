/* Демо 1. Блок, который вы попросили сами.
 *
 *   gcc -std=c11 -Wall -Wextra 01-malloc.c -o 01-malloc && ./01-malloc
 *
 * До запуска: адрес блока от malloc окажется рядом с адресами локальных
 * переменных или далеко от них? И чему равен sizeof этого указателя?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int on_stack = 1;

    int *heap = malloc(4 * sizeof *heap);
    if (heap == NULL) {
        fprintf(stderr, "память не выделена\n");
        return 1;
    }

    printf("адреса:\n");
    printf("  &on_stack (стек) = %p\n", (void *)&on_stack);
    printf("  heap      (куча) = %p\n", (void *)heap);
    printf("  &heap            = %p   (сам указатель лежит на стеке)\n\n",
           (void *)&heap);

    printf("размеры:\n");
    printf("  sizeof heap  = %zu   (это указатель, и только)\n", sizeof heap);
    printf("  sizeof *heap = %zu   (размер одного элемента)\n", sizeof *heap);
    printf("  просили байт = %zu\n\n", 4 * sizeof *heap);

    for (int i = 0; i < 4; i++)
        heap[i] = 10 * (i + 1);
    printf("блок как массив: %d %d %d %d\n\n",
           heap[0], heap[1], heap[2], heap[3]);

    size_t n = 4;
    long total = 0;
    for (size_t i = 0; i < n; i++)
        total += heap[i];
    printf("сумма %zu элементов = %ld\n\n", n, total);

    free(heap);

    heap = NULL;
    printf("после free и heap = NULL:  heap = %p\n", (void *)heap);

    return 0;
}
