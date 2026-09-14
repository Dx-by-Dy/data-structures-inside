/* Демо 3. Нулевой указатель.
 *
 *   gcc -std=c11 -Wall -Wextra 03-null.c -o 03-null && ./03-null
 *
 * До запуска: чем закончится последняя строка программы?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>

int main(void)
{
    int *none = NULL;

    printf("none            = %p\n", (void *)none);
    printf("none == NULL    = %d\n", none == NULL);

    if (none != NULL)
        printf("сюда программа не зайдёт: %d\n", *none);
    printf("проверка none != NULL прошла, разыменования не было\n\n");

    printf("а теперь разыменуем\n");
    fflush(stdout);   /* иначе строки выше потеряются, если программа упадёт */
    printf("*none = %d\n", *none);

    return 0;
}
