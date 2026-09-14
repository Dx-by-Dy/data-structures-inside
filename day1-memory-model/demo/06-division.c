/* Демо 6. Деление и остаток.
 *
 *   gcc -std=c11 -Wall -Wextra 06-division.c -o 06-division && ./06-division
 *
 * Откройте рядом питон и посчитайте сами все четыре пары:
 *   >>> 7 // 2, 7 % 2
 *   >>> -7 // 2, -7 % 2
 *   >>> 7 // -2, 7 % -2
 *   >>> -7 // -2, -7 % -2
 *
 * Выпишите ответы до запуска и сравните с таблицей программы.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stddef.h>

int main(void)
{
    int pairs[][2] = { {7, 2}, {-7, 2}, {7, -2}, {-7, -2} };

    printf(" %4s %4s | %8s %8s\n", "a", "b", "a / b", "a % b");
    for (size_t i = 0; i < sizeof pairs / sizeof pairs[0]; i++) {
        int a = pairs[i][0], b = pairs[i][1];
        printf(" %4d %4d | %8d %8d\n", a, b, a / b, a % b);
    }

    printf("\nверно ли (a / b) * b + a %% b == a? проверьте по таблице\n");
    printf("а на питоновских ответах?\n");

    return 0;
}
