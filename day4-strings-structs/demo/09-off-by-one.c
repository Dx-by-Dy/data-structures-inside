/* Демо 9. Байт, про который забыли.
 *
 *   gcc -std=c11 -Wall -Wextra 09-off-by-one.c -o 09-off-by-one
 *   ./09-off-by-one
 *   echo $?
 *
 * Функция копирует строку в кучу. Размер блока она берёт равным strlen.
 *
 * До запуска: сколько байт не хватит и на каком именно символе?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *my_strdup(const char *s)
{
    size_t len = strlen(s);

    char *copy = malloc(len);   /* смотреть сюда */
    if (copy == NULL)
        return NULL;

    strcpy(copy, s);
    return copy;
}

int main(void)
{
    const char *original = "hello";

    printf("strlen(original) = %zu\n", strlen(original));
    fflush(stdout);   /* иначе строки потеряются при падении */

    char *copy = my_strdup(original);
    if (copy == NULL)
        return 1;

    printf("копия: %s\n", copy);
    printf("strlen(copy) = %zu\n", strlen(copy));

    free(copy);
    return 0;
}
