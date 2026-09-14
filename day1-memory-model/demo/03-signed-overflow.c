/* Демо 3. Та же операция над знаковым типом.
 *
 *   gcc -std=c11 -Wall -Wextra 03-signed-overflow.c -o 03-signed-overflow
 *   ./03-signed-overflow
 *
 * До запуска предскажите обе напечатанные строки.
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <limits.h>

int main(void)
{
    int x = INT_MAX;
    printf("INT_MAX     = %d\n", x);
    x = x + 1;                        /* санитайзер укажет на эту строку */
    printf("INT_MAX + 1 = %d\n", x);

    int y = INT_MIN;
    printf("\nINT_MIN     = %d\n", y);
    y = y - 1;
    printf("INT_MIN - 1 = %d\n", y);

    return 0;
}
