/* Демо 5. Четыре записи одного элемента.
 *
 *   gcc -std=c11 -Wall -Wextra 05-indexing.c -o 05-indexing && ./05-indexing
 *
 * В программе есть строка 2[ints]. До запуска скажите, соберётся ли она и что
 * напечатает.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

int main(void)
{
    int ints[5] = { 10, 20, 30, 40, 50 };

    printf("ints     = %p\n", (void *)ints);
    printf("&ints[0] = %p\n\n", (void *)&ints[0]);

    int *p = ints;
    printf("четыре записи одного и того же элемента:\n");
    printf("  ints[2]     = %d\n", ints[2]);
    printf("  *(ints + 2) = %d\n", *(ints + 2));
    printf("  *(p + 2)    = %d\n", *(p + 2));
    printf("  2[ints]     = %d\n", 2[ints]);

    return 0;
}
