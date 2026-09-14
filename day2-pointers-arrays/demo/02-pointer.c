/* Демо 2. Указатель.
 *
 *   gcc -std=c11 -Wall -Wextra 02-pointer.c -o 02-pointer && ./02-pointer
 *
 * До запуска: чему равен sizeof(char *) и sizeof(double *)? И почему после
 * строки *p = 42 изменился x, которому никто ничего не присваивал?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

int main(void)
{
    int x = 37;

    int *p = &x;
    printf("p = &x:\n");
    printf("  p  = %p   (то же число, что и &x)\n", (void *)p);
    printf("  *p = %d   (значение по этому адресу)\n", *p);
    printf("  &p = %p   (адрес самого указателя)\n\n", (void *)&p);

    *p = 42;
    printf("после *p = 42:  x = %d\n\n", x);

    printf("размер самих указателей:\n");
    printf("  sizeof(int *)    = %zu\n", sizeof(int *));
    printf("  sizeof(char *)   = %zu\n", sizeof(char *));
    printf("  sizeof(double *) = %zu\n", sizeof(double *));
    printf("  sizeof(int **)   = %zu\n\n", sizeof(int **));

    int **pp = &p;
    printf("pp = &p:  *pp = %p,  **pp = %d\n", (void *)*pp, **pp);

    return 0;
}
