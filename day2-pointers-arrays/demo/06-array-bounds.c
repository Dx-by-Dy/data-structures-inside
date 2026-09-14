/* Демо 6. Разность указателей и граница массива.
 *
 *   gcc -std=c11 -Wall -Wextra 06-array-bounds.c -o 06-array-bounds
 *   ./06-array-bounds
 *
 * До запуска: чему будет равна разность last - first и на сколько при этом
 * различаются сами адреса?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stddef.h>

int main(void)
{
    int ints[5] = { 10, 20, 30, 40, 50 };

    int *first = &ints[0];
    int *last  = &ints[4];
    printf("first = %p\n", (void *)first);
    printf("last  = %p\n", (void *)last);
    printf("last - first = %td (элементов), адреса отличаются на %td байт\n\n",
           last - first, (char *)last - (char *)first);

    int *end = ints + 5;
    printf("end = ints + 5 = %p\n", (void *)end);

    printf("\nобход массива без единого индекса:\n  ");
    for (int *it = ints; it != end; it++)
        printf("%d ", *it);
    printf("\n");

    return 0;
}
