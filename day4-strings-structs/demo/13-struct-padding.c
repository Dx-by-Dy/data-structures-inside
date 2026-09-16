/* Демо 13. Дыры внутри структуры.
 *
 *   gcc -std=c11 -Wall -Wextra 13-struct-padding.c -o 13-struct-padding
 *   ./13-struct-padding
 *
 * До запуска: сложите размеры полей структуры Mixed и назовите sizeof Mixed.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stddef.h>
#include <string.h>

struct Mixed {
    char   c;
    int    i;
    char   d;
};

int main(void)
{
    printf("сумма размеров полей = %zu\n",
           sizeof(char) + sizeof(int) + sizeof(char));
    printf("sizeof(struct Mixed) = %zu\n\n", sizeof(struct Mixed));

    printf("смещения полей от начала структуры:\n");
    printf("  c: %zu\n", offsetof(struct Mixed, c));
    printf("  i: %zu\n", offsetof(struct Mixed, i));
    printf("  d: %zu\n\n", offsetof(struct Mixed, d));

    printf("требования выравнивания: char %zu, int %zu, структура %zu\n\n",
           _Alignof(char), _Alignof(int), _Alignof(struct Mixed));

    /* Заполняем все байты структуры, потом кладём в неё значения и смотрим,
       какие байты остались нетронутыми. */
    struct Mixed m;
    memset(&m, 0xAA, sizeof m);
    m.c = 1;
    m.i = 0x02020202;
    m.d = 3;

    const unsigned char *bytes = (const unsigned char *)&m;
    printf("байты структуры:\n  ");
    for (size_t k = 0; k < sizeof m; k++)
        printf("%02X ", bytes[k]);
    printf("\n");

    return 0;
}
