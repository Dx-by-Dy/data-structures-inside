/* Демо 3. Функция отдаёт адрес наружу.
 *
 *   gcc -std=c11 -Wall -Wextra 03-return-heap.c -o 03-return-heap
 *   ./03-return-heap
 *
 * Запустите рядом вчерашнее демо 14-return-local и сравните две программы
 * построчно: отличие будет ровно в одном объявлении.
 *
 * До запуска: кто обязан вызвать free для возвращённого блока?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

/* Выделяет массив из n элементов, заполняет квадратами и отдаёт наружу.
   ВЛАДЕЛЕЦ возвращённого блока — тот, кто вызвал: он обязан вызвать free.
   Записать это в языке негде, поэтому написано здесь. */
static int *make_squares(size_t n)
{
    int *a = malloc(n * sizeof *a);
    if (a == NULL)
        return NULL;
    for (size_t i = 0; i < n; i++)
        a[i] = (int)(i * i);
    printf("make_squares: выделила %zu элементов по адресу %p\n",
           n, (void *)a);
    return a;
}

int main(void)
{
    int *sq = make_squares(5);
    if (sq == NULL) {
        fprintf(stderr, "память не выделена\n");
        return 1;
    }

    printf("main: читаем по возвращённому адресу %p:", (void *)sq);
    for (size_t i = 0; i < 5; i++)
        printf(" %d", sq[i]);
    printf("\n");

    free(sq);
    printf("main: память возвращена\n");
    return 0;
}
