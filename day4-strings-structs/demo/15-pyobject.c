/* Демо 15. Из чего сложены 28 байт.
 *
 *   gcc -std=c11 -Wall -Wextra 15-pyobject.c -o 15-pyobject && ./15-pyobject
 *
 * Структура повторяет то, как устроено целое число внутри CPython:
 * счётчик ссылок, указатель на тип, тег с длиной и знаком, а за ними —
 * цифры числа по 30 бит в каждой.
 *
 * До запуска: откройте питон и посмотрите sys.getsizeof(1).
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

struct PyLong {
    size_t    refcnt;      /* сколько имён ссылается на объект */
    void     *type;        /* указатель на описание типа */
    uintptr_t tag;         /* число цифр и знак */
    uint32_t  digit[1];    /* сами цифры, по 30 бит в каждой */
};

int main(void)
{
    printf("размеры полей:\n");
    printf("  refcnt %zu, type %zu, tag %zu, одна цифра %zu\n\n",
           sizeof(size_t), sizeof(void *), sizeof(uintptr_t), sizeof(uint32_t));

    printf("смещения:\n");
    printf("  refcnt %zu, type %zu, tag %zu, digit %zu\n\n",
           offsetof(struct PyLong, refcnt), offsetof(struct PyLong, type),
           offsetof(struct PyLong, tag),    offsetof(struct PyLong, digit));

    printf("sizeof(struct PyLong) = %zu\n", sizeof(struct PyLong));
    printf("заголовок + одна цифра = %zu\n\n",
           offsetof(struct PyLong, digit) + sizeof(uint32_t));

    printf("сколько байт займёт число из k цифр:\n");
    for (size_t k = 1; k <= 5; k++)
        printf("  k = %zu: %2zu байт, помещаются числа до 2^%zu\n",
               k, offsetof(struct PyLong, digit) + k * sizeof(uint32_t), 30 * k);

    return 0;
}
