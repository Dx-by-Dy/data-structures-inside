/* Демо 14. Порядок полей меняет размер.
 *
 *   gcc -std=c11 -Wall -Wextra 14-field-order.c -o 14-field-order
 *   ./14-field-order
 *
 * Три структуры из одних и тех же трёх полей, записанных в разном порядке.
 *
 * До запуска: сколько разных значений sizeof получится у трёх структур?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stddef.h>

struct Bad {
    char   flag;
    double value;
    int    count;
};

struct Good {
    double value;
    int    count;
    char   flag;
};

struct Mixed {
    int    count;
    double value;
    char   flag;
};

int main(void)
{
    printf("сумма размеров полей во всех трёх: %zu\n\n",
           sizeof(char) + sizeof(double) + sizeof(int));

    printf("struct Bad:   sizeof = %2zu, смещения: flag %zu, value %zu, count %zu\n",
           sizeof(struct Bad), offsetof(struct Bad, flag),
           offsetof(struct Bad, value), offsetof(struct Bad, count));

    printf("struct Good:  sizeof = %2zu, смещения: value %zu, count %zu, flag %zu\n",
           sizeof(struct Good), offsetof(struct Good, value),
           offsetof(struct Good, count), offsetof(struct Good, flag));

    printf("struct Mixed: sizeof = %2zu, смещения: count %zu, value %zu, flag %zu\n\n",
           sizeof(struct Mixed), offsetof(struct Mixed, count),
           offsetof(struct Mixed, value), offsetof(struct Mixed, flag));

    /* Массив из миллиона записей — разница в размере одной записи,
       умноженная на миллион. */
    printf("на миллион записей:\n");
    printf("  struct Bad:  %.1f МБ\n", sizeof(struct Bad) * 1e6 / 1048576.0);
    printf("  struct Good: %.1f МБ\n", sizeof(struct Good) * 1e6 / 1048576.0);

    return 0;
}
