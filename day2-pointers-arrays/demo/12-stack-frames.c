/* Демо 12. Адреса локальных переменных вложенных вызовов.
 *
 *   gcc -std=c11 -Wall -Wextra 12-stack-frames.c -o 12-stack-frames
 *   ./12-stack-frames
 *
 * До запуска: адреса будут расти или убывать по мере углубления в вызовы? И
 * совпадут ли они со вторым разом, когда те же три вызова делаются заново?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

static void level3(void)
{
    int c = 3;
    printf("    level3: &c = %p\n", (void *)&c);
}

static void level2(void)
{
    int b = 2;
    printf("  level2: &b = %p\n", (void *)&b);
    level3();
}

static void level1(void)
{
    int a = 1;
    printf("level1: &a = %p\n", (void *)&a);
    level2();
}

int main(void)
{
    int m = 0;
    printf("main:   &m = %p\n", (void *)&m);
    level1();

    printf("\nвторой раз, те же вызовы:\n");
    printf("main:   &m = %p\n", (void *)&m);
    level1();

    return 0;
}
