/* Демо 13. Что делает static перед функцией.  Команды — в README.md
 *
 * Обе функции определены в util.c, обе объявлены здесь. Одна из двух
 * строк в main до программы не доберётся.
 *
 * До запуска: скажите, какая, и на каком шаге сборки это выяснится.
 */
#include <stdio.h>
#include "util.h"

/* Объявление руками: в util.h этой функции нет. */
int doubled(int x);

int main(void)
{
    printf("%d\n", quadrupled(5));
    printf("%d\n", doubled(5));
    return 0;
}
