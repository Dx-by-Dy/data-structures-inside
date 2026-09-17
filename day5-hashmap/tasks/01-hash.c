/* Задача 1. Хеш-функции своими руками.  Условие — 01-hash.md
 *
 *   make
 *   printf 'abc\nbca\ncab\nhello\n' | ./01-hash
 *   ./01-hash --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все четыре
 * функции пока возвращают заведомо неверное. Ваша работа — их тела; main и
 * печать менять не нужно.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define WORD_MAX  32
#define WORDS_MAX 200
#define BUCKETS   16

/* Сумма байтов строки. Байты берутся как unsigned char. */
static uint32_t hash_sum(const char *s)
{
    /* TODO */
    (void)s;
    return 0;
}

/* FNV-1a, 32 бита: начальное значение 2166136261, множитель 16777619.
   Для каждого байта: сначала исключающее ИЛИ, потом умножение. */
static uint32_t hash_fnv1a(const char *s)
{
    /* TODO */
    (void)s;
    return 0;
}

/* Номер корзины для хеша. Число корзин — степень двойки; деления в этой
   функции быть не должно. */
static size_t bucket_of(uint32_t hash, size_t cap)
{
    /* TODO */
    (void)hash; (void)cap;
    return 0;
}

/* Сколько ключей окажется в самой полной корзине, если разложить count слов
   по cap корзинам функцией hash. Память под счётчики берётся в куче и
   освобождается здесь же. */
static size_t longest_bucket(char words[][WORD_MAX], size_t count, size_t cap,
                             uint32_t (*hash)(const char *))
{
    /* TODO */
    (void)words; (void)count; (void)cap; (void)hash;
    return 0;
}

/* ── печать ─────────────────────────────────────────────────────────────── */

static void print_histogram(const char *title, char words[][WORD_MAX],
                            size_t count, uint32_t (*hash)(const char *))
{
    size_t counts[BUCKETS] = {0};
    for (size_t i = 0; i < count; i++)
        /* Остаток здесь — страховка печати от bucket_of, вернувшей лишнее. */
        counts[bucket_of(hash(words[i]), BUCKETS) % BUCKETS]++;

    printf("%s\n", title);
    for (size_t b = 0; b < BUCKETS; b++) {
        printf("  %2zu | ", b);
        for (size_t k = 0; k < counts[b]; k++)
            putchar('#');
        putchar('\n');
    }
    printf("  в самой полной корзине: %zu\n\n",
           longest_bucket(words, count, BUCKETS, hash));
}

/* ── самопроверка ────────────────────────────────────────────────────────
   Ожидаемые значения FNV-1a — часть условия: эта функция описана стандартом
   алгоритма, и её результат одинаков на любой машине. */
static int checks_run = 0, checks_failed = 0;

static void expect_ll(const char *what, long long got, long long want)
{
    checks_run++;
    if (got == want) {
        printf("  ok      %s = %lld\n", what, got);
        return;
    }
    checks_failed++;
    printf("  ОШИБКА  %s = %lld, ожидалось %lld\n", what, got, want);
}

static int self_check(void)
{
    /* Сумма байтов. */
    expect_ll("hash_sum(\"\")", hash_sum(""), 0);
    expect_ll("hash_sum(\"a\")", hash_sum("a"), 97);
    expect_ll("hash_sum(\"abc\")", hash_sum("abc"), 294);
    expect_ll("hash_sum(\"bca\") — те же байты", hash_sum("bca"), 294);
    expect_ll("hash_sum(\"listen\") == hash_sum(\"silent\")",
              hash_sum("listen") == hash_sum("silent"), 1);
    /* Байт со старшим установленным битом. */
    expect_ll("hash_sum(\"я\")", hash_sum("я"), 352);

    /* FNV-1a. */
    expect_ll("hash_fnv1a(\"\")", hash_fnv1a(""), 2166136261LL);
    expect_ll("hash_fnv1a(\"a\")", hash_fnv1a("a"), 3826002220LL);
    expect_ll("hash_fnv1a(\"b\")", hash_fnv1a("b"), 3876335077LL);
    expect_ll("hash_fnv1a(\"hell\")", hash_fnv1a("hell"), 477198310LL);
    expect_ll("hash_fnv1a(\"hello\")", hash_fnv1a("hello"), 1335831723LL);
    expect_ll("hash_fnv1a(\"привет\")", hash_fnv1a("привет"), 2168984447LL);
    expect_ll("hash_fnv1a(\"abc\") != hash_fnv1a(\"bca\")",
              hash_fnv1a("abc") != hash_fnv1a("bca"), 1);
    expect_ll("одна строка — один хеш",
              hash_fnv1a("hello") == hash_fnv1a("hello"), 1);

    /* Номер корзины. */
    expect_ll("bucket_of(hash_fnv1a(\"\"), 16)",
              (long long)bucket_of(2166136261u, 16), 5);
    expect_ll("bucket_of(0xFFFFFFFF, 256)",
              (long long)bucket_of(0xFFFFFFFFu, 256), 255);
    expect_ll("bucket_of(hash_fnv1a(\"hello\"), 1024)",
              (long long)bucket_of(1335831723u, 1024), 171);
    expect_ll("bucket_of(любой хеш, 1)", (long long)bucket_of(12345u, 1), 0);

    /* Самая полная корзина. */
    char words[3][WORD_MAX] = {"abc", "bca", "cab"};
    expect_ll("три анаграммы, сумма байтов, 16 корзин",
              (long long)longest_bucket(words, 3, 16, hash_sum), 3);
    expect_ll("те же три слова, FNV-1a, 16 корзин",
              (long long)longest_bucket(words, 3, 16, hash_fnv1a), 1);
    expect_ll("ни одного слова",
              (long long)longest_bucket(words, 0, 16, hash_fnv1a), 0);

    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    static char words[WORDS_MAX][WORD_MAX];
    size_t count = 0;

    printf("  слово              сумма  корзина      FNV-1a  корзина\n");
    while (count < WORDS_MAX && scanf("%31s", words[count]) == 1) {
        const char *w = words[count];
        printf("  %-16s %8u %8zu  %10u %8zu\n",
               w, hash_sum(w), bucket_of(hash_sum(w), BUCKETS),
               hash_fnv1a(w), bucket_of(hash_fnv1a(w), BUCKETS));
        count++;
    }
    printf("\nслов %zu, корзин %d\n\n", count, BUCKETS);

    print_histogram("сумма байтов:", words, count, hash_sum);
    print_histogram("FNV-1a:", words, count, hash_fnv1a);

    return 0;
}
