/* Демо 4. Программа считает правильно.
 *
 *   gcc -std=c11 -Wall -Wextra 04-leak.c -o 04-leak && ./04-leak
 *   echo $?
 *
 * Найдите в ней ошибку глазами до запуска. Потом соберите с санитайзером и
 * посмотрите, когда именно он о ней сообщит:
 *
 *   gcc -std=c11 -Wall -Wextra -g -fsanitize=address 04-leak.c -o 04-leak
 *   ./04-leak
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

/* Возвращает сумму первых n квадратов, считая их в блоке на куче. */
static long sum_of_squares(size_t n)
{
    int *a = malloc(n * sizeof *a);
    if (a == NULL)
        return -1;
    for (size_t i = 0; i < n; i++)
        a[i] = (int)(i * i);

    long total = 0;
    for (size_t i = 0; i < n; i++)
        total += a[i];
    return total;
}

int main(void)
{
    printf("сумма квадратов до 5  = %ld\n", sum_of_squares(5));
    printf("сумма квадратов до 10 = %ld\n", sum_of_squares(10));
    printf("оба ответа верные, программа завершается нормально\n");
    return 0;
}
