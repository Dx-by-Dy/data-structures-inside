/* Демо 6. Один блок освобождается дважды.
 *
 *   gcc -std=c11 -Wall -Wextra 06-double-free.c -o 06-double-free
 *   ./06-double-free
 *   echo $?
 *
 * До запуска: дойдёт ли программа до последней строки вывода?
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
    printf("блок %p выделен\n", (void *)a);

    free(a);
    printf("освободили первый раз\n");

    printf("освобождаем второй раз\n");
    fflush(stdout);
    free(a);

    printf("последняя строка вывода\n");
    return 0;
}
