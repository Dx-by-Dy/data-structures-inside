/* Демо 1. Размеры типов.
 *
 *   gcc -std=c11 -Wall -Wextra 01-sizes.c -o 01-sizes && ./01-sizes
 *
 * До запуска: сколько байт в int? А откуда вы это знаете?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

int main(void) {
    printf("%-12s %s\n", "тип", "размер, байт");
    printf("%-12s %2zu\n", "char",      sizeof(char));
    printf("%-12s %2zu\n", "short",     sizeof(short));
    printf("%-12s %2zu\n", "int",       sizeof(int));
    printf("%-12s %2zu\n", "long",      sizeof(long));
    printf("%-12s %2zu\n", "long long", sizeof(long long));
    printf("%-12s %2zu\n", "size_t",    sizeof(size_t));
    printf("%-12s %2zu\n", "float",     sizeof(float));
    printf("%-12s %2zu\n", "double",    sizeof(double));
    printf("%-12s %2zu\n", "void *",    sizeof(void *));

    printf("\nINT_MAX  = %d\n", INT_MAX);
    printf("INT_MIN  = %d\n", INT_MIN);
    printf("UINT_MAX = %u\n", UINT_MAX);

    printf("\nбит в байте: %d\n", CHAR_BIT);
    return 0;
}
