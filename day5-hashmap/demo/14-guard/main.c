/* Демо 14. Заголовок, включённый дважды.  Команды — в README.md
 *
 * main.c включает point.h напрямую, а shape.h включает его же ещё раз.
 *
 * До запуска: препроцессор подставляет текст заголовка в место #include.
 * Что окажется в тексте main.c после подстановки?
 */
#include <stdio.h>
#include "point.h"
#include "shape.h"

int main(void)
{
    struct Shape s = {{3, 4}, 10};
    printf("%d %d\n", point_sum(s.corner), s.size);
    return 0;
}
