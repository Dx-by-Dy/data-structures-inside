/* Демо 10. Сравнение строк.
 *
 *   gcc -std=c11 -Wall -Wextra 10-compare.c -o 10-compare && ./10-compare
 *
 * До запуска: в программе три строки с одинаковым текстом "hello". Какие из
 * них окажутся равны по ==, а какие по strcmp?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *a = "hello";
    const char *b = "hello";
    char        c[] = "hello";

    printf("адреса:\n");
    printf("  a = %p\n  b = %p\n  c = %p\n\n", (void *)a, (void *)b, (void *)c);

    printf("сравнение через ==:\n");
    printf("  a == b: %s\n", a == b ? "да" : "нет");
    printf("  a == c: %s\n\n", a == (const char *)c ? "да" : "нет");

    printf("сравнение через strcmp:\n");
    printf("  strcmp(a, b) = %d\n", strcmp(a, b));
    printf("  strcmp(a, c) = %d\n\n", strcmp(a, c));

    /* strcmp возвращает не только ноль. */
    printf("что возвращает strcmp:\n");
    printf("  strcmp(\"abc\", \"abd\") = %d\n", strcmp("abc", "abd"));
    printf("  strcmp(\"abd\", \"abc\") = %d\n", strcmp("abd", "abc"));
    printf("  strcmp(\"abc\", \"abcd\") = %d\n", strcmp("abc", "abcd"));
    printf("  strcmp(\"Zoo\", \"abc\") = %d\n", strcmp("Zoo", "abc"));

    return 0;
}
