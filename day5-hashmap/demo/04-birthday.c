/* Демо 4. Когда случается первая коллизия.
 *
 *   gcc -std=c11 -Wall -Wextra 04-birthday.c -o 04-birthday -lm
 *   ./04-birthday
 *
 * В таблицу на cap корзин по одному кладутся случайные ключи из восьми букв.
 * Опыт останавливается на первом ключе, попавшем в занятую корзину, и
 * повторяется двести раз с разными ключами.
 *
 * До запуска: сколько случайных ключей, по-вашему, поместится в таблицу на
 * 1024 корзины, прежде чем два из них встретятся в одной?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

/* Свой генератор случайных чисел: результат не зависит от библиотеки, и
   таблица воспроизводится на любой машине. */
static uint64_t rng_state;

static uint32_t next_random(void)
{
    rng_state = rng_state * 6364136223846793005u + 1442695040888963407u;
    return (uint32_t)(rng_state >> 33);
}

static void random_key(char *out, size_t size)
{
    for (size_t i = 0; i + 1 < size; i++)
        out[i] = (char)('a' + next_random() % 26);
    out[size - 1] = '\0';
}

/* Кладёт случайные ключи, пока два не попадут в одну корзину. Возвращает
   номер ключа, на котором это случилось. */
static size_t first_collision(size_t cap)
{
    char *taken = calloc(cap, 1);
    if (taken == NULL)
        exit(1);

    char key[9];
    size_t i = 0;
    for (;; i++) {
        random_key(key, sizeof key);
        size_t bucket = fnv1a(key) % cap;
        if (taken[bucket])
            break;
        taken[bucket] = 1;
    }

    free(taken);
    return i;
}

int main(void)
{
    const size_t trials = 200;

    printf("среднее по %zu опытам\n\n", trials);
    printf("  корзин   первая коллизия   sqrt(cap)   доля таблицы\n");
    for (size_t cap = 64; cap <= 1048576; cap *= 4) {
        rng_state = cap;               /* один и тот же старт для каждой таблицы */
        size_t total = 0;
        for (size_t t = 0; t < trials; t++)
            total += first_collision(cap);
        double k = (double)total / (double)trials;
        printf("  %7zu   %15.1f   %9.0f   %11.1f%%\n",
               cap, k, sqrt((double)cap), 100.0 * k / (double)cap);
    }

    printf("\nа сколько ключей нужно, чтобы коллизия стала неизбежной:\n");
    for (size_t cap = 64; cap <= 4096; cap *= 4)
        printf("  корзин %5zu — любые %zu ключей\n", cap, cap + 1);

    return 0;
}
