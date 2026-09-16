/* Демо 5. Где лежит строковый литерал.
 *
 *   gcc -std=c11 -Wall -Wextra 05-literal.c -o 05-literal && ./05-literal
 *
 * До запуска: два объявления рядом — char array[] = "hello" и
 * const char *pointer = "hello". Что из них массив, а что указатель, и чему
 * равен sizeof каждого?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char        array[]  = "hello";
    const char *pointer  = "hello";
    const char *pointer2 = "hello";

    int on_stack = 1;

    printf("размеры:\n");
    printf("  sizeof array   = %zu\n", sizeof array);
    printf("  sizeof pointer = %zu\n", sizeof pointer);
    printf("  strlen обоих   = %zu и %zu\n\n", strlen(array), strlen(pointer));

    printf("адреса:\n");
    printf("  array      = %p\n", (void *)array);
    printf("  pointer    = %p\n", (void *)pointer);
    printf("  pointer2   = %p\n", (void *)pointer2);
    printf("  &pointer   = %p\n", (void *)&pointer);
    printf("  &on_stack  = %p\n\n", (void *)&on_stack);

    /* Два литерала с одинаковым текстом — один или два блока? */
    printf("pointer == pointer2: %s\n", pointer == pointer2 ? "да" : "нет");
    printf("array   == pointer:  %s\n", (const char *)array == pointer ? "да" : "нет");

    return 0;
}
