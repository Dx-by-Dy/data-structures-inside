/* Демо 12. Что копируется, когда внутри структуры указатель.
 *
 *   gcc -std=c11 -Wall -Wextra 12-shared-string.c -o 12-shared-string
 *   ./12-shared-string
 *
 * До запуска: структура копируется целиком — это было в предыдущем демо.
 * Поле name здесь имеет тип char *. Что получит копия: свою строку или ту же?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    char *name;    /* строка в куче, выделенная отдельно */
    int   age;
};

int main(void)
{
    struct Person a;
    a.name = malloc(16);
    if (a.name == NULL)
        return 1;
    strcpy(a.name, "Anna");
    a.age = 17;

    struct Person b = a;     /* смотреть сюда */

    printf("a: name = %s (адрес %p), age = %d\n", a.name, (void *)a.name, a.age);
    printf("b: name = %s (адрес %p), age = %d\n\n", b.name, (void *)b.name, b.age);

    b.age = 30;
    b.name[0] = 'J';

    printf("после b.age = 30 и b.name[0] = 'J':\n");
    printf("a: name = %s, age = %d\n", a.name, a.age);
    printf("b: name = %s, age = %d\n\n", b.name, b.age);

    /* Строка одна, поэтому free вызывается один раз. */
    free(a.name);

    return 0;
}
