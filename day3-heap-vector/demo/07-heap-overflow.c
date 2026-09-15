/* Демо 7. Запись за границу блока.
 *
 *   gcc -std=c11 -Wall -Wextra 07-heap-overflow.c -o 07-heap-overflow
 *   ./07-heap-overflow
 *   echo $?
 *
 * Блок рассчитан на четыре элемента, а программа пишет в a[4]. До запуска:
 * пройдёт ли эта запись, что прочитается обратно и чем всё закончится?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *a = malloc(4 * sizeof *a);
    if (a == NULL)
        return 1;
    for (int i = 0; i < 4; i++)
        a[i] = 100 + i;
    printf("блок на четыре элемента: %d %d %d %d\n", a[0], a[1], a[2], a[3]);

    printf("пишем в a[4]\n");
    fflush(stdout);
    a[4] = -1;
    printf("прочитали обратно: %d\n", a[4]);

    free(a);
    printf("free прошёл\n");
    return 0;
}
