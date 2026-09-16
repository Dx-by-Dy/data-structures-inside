/* Демо 8. strcpy не знает, куда копирует.
 *
 *   gcc -std=c11 -Wall -Wextra 08-strcpy-overflow.c -o 08-strcpy-overflow
 *   ./08-strcpy-overflow
 *   echo $?
 *
 * До запуска: в буфере восемь ячеек, копируемая строка длиннее. Что остановит
 * копирование?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <string.h>

/* Имя приходит снаружи: в настоящей программе — от пользователя. */
static const char *incoming = "Konstantinopolsky";

int main(void)
{
    char after[8]  = "-------";
    char buffer[8] = "";
    char before[8] = "-------";

    printf("до копирования:\n");
    printf("  before = %s\n", before);
    printf("  buffer = %s\n", buffer);
    printf("  after  = %s\n\n", after);

    printf("копируем %zu байт в буфер на %zu\n",
           strlen(incoming) + 1, sizeof buffer);
    fflush(stdout);   /* иначе строки потеряются при падении */

    strcpy(buffer, incoming);

    printf("\nпосле копирования:\n");
    printf("  before = %s\n", before);
    printf("  buffer = %s\n", buffer);
    printf("  after  = %s\n", after);

    return 0;
}
