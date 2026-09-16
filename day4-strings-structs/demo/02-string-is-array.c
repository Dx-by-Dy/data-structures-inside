/* Демо 2. Строка — это массив char, за которым стоит ноль.
 *
 *   gcc -std=c11 -Wall -Wextra 02-string-is-array.c -o 02-string-is-array
 *   ./02-string-is-array
 *
 * До запуска: чему равны sizeof s и strlen(s) для char s[] = "hello"?
 * Если числа разные — откуда берётся разница?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char s[] = "hello";

    printf("s = \"%s\"\n\n", s);

    printf("sizeof s   = %zu\n", sizeof s);
    printf("strlen(s)  = %zu\n\n", strlen(s));

    /* Тот же массив побайтно, включая то, что стоит после последней буквы. */
    printf("побайтно:\n");
    for (size_t i = 0; i < sizeof s; i++)
        printf("  s[%zu] = %3d  '%c'\n", i, s[i],
               s[i] == '\0' ? '.' : s[i]);
    printf("\n");

    /* Массив свой, поэтому его можно менять. */
    s[0] = 'H';
    printf("после s[0] = 'H': %s\n\n", s);

    /* Ноль, поставленный в середину. */
    s[3] = '\0';
    printf("после s[3] = '\\0':\n");
    printf("  напечатано как строка: %s\n", s);
    printf("  strlen(s)  = %zu\n", strlen(s));
    printf("  sizeof s   = %zu\n", sizeof s);
    printf("  s[4] = '%c', s[5] = %d\n", s[4], s[5]);

    return 0;
}
