/* Демо 4. Массив символов без нуля на конце.
 *
 *   gcc -std=c11 -Wall -Wextra 04-no-terminator.c -o 04-no-terminator
 *   ./04-no-terminator
 *
 * До запуска: в массиве пять букв и ровно пять ячеек. Что напечатает "%s"?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    /* Пять ячеек, пять букв, места под ноль не осталось. */
    char no_zero[5] = { 'h', 'e', 'l', 'l', 'o' };

    /* Соседний массив — чтобы было видно, докуда дойдёт печать. */
    char neighbour[16] = "XYZXYZXYZXYZXYZ";

    printf("sizeof no_zero  = %zu\n", sizeof no_zero);
    printf("адрес no_zero   = %p\n", (void *)no_zero);
    printf("адрес neighbour = %p\n\n", (void *)neighbour);

    fflush(stdout);   /* иначе строки потеряются при падении */

    printf("печать через %%s: [");
    printf("%s", no_zero);
    printf("]\n\n");

    printf("strlen(no_zero) = %zu\n", strlen(no_zero));

    return 0;
}
