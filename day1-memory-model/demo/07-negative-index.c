/* Демо 7. Остаток и индекс корзины.
 *
 *   gcc -std=c11 -Wall -Wextra 07-negative-index.c -o 07-negative-index
 *   ./07-negative-index
 *
 * Самый частый способ разложить значение по корзинам — взять остаток от хеша
 * по числу корзин.
 *
 * До запуска: что даёт эта запись в C при отрицательном хеше и чем закончится
 * обращение по такому индексу? Питон рядом:
 *   >>> -12345 % 16
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stddef.h>

int main(void)
{
    int size = 16;
    int hashes[] = { 12345, -12345, 0, -1 };

    printf("size = %d\n\n", size);
    printf(" %8s | %11s | %s\n", "hash", "hash % size", "годится?");
    for (size_t i = 0; i < sizeof hashes / sizeof hashes[0]; i++) {
        int h = hashes[i];
        int idx = h % size;
        printf(" %8d | %11d | %s\n", h, idx,
               (idx >= 0 && idx < size) ? "да" : "нет, выход за границы");
    }

    /* К этому выражению вы вернётесь на пятом дне. */
    printf("\nодин из способов починить: ((h %% size) + size) %% size\n");
    for (size_t i = 0; i < sizeof hashes / sizeof hashes[0]; i++) {
        int h = hashes[i];
        printf(" %8d -> %d\n", h, ((h % size) + size) % size);
    }

    return 0;
}
