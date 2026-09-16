/* Задача 3. Цена неизменяемости.  Условие — 03-concat.md
 *
 *   make
 *   ./03-concat 20000
 *   ./03-concat --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но обе сборки
 * строки пока ничего не делают. Ваша работа — тела функций; main, замер
 * времени и печать менять не нужно.
 *
 * Пока str_concat возвращает NULL, запуск с числом печатает «памяти не
 * хватило», а --check не сходится почти нигде — так и должно быть до первой
 * правки. По той же причине `make release` пока предупреждает про
 * неинициализированные поля буфера: как только buf_init заработает,
 * предупреждение исчезнет. Обычный `make` молчит с самого начала.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* Счётчики. Увеличивать их должны ваши функции. */
static size_t stat_copied_naive = 0;   /* байт скопировано наивной склейкой */
static size_t stat_copied_buf   = 0;   /* байт скопировано буфером */
static size_t stat_growths      = 0;   /* сколько раз буфер переезжал */

/* ── способ первый: каждая склейка создаёт новую строку ────────────────── */

/* Возвращает новую строку в куче, содержащую a и следом b.
   Владелец результата — вызывающий. При нехватке памяти возвращает NULL. */
static char *str_concat(const char *a, const char *b)
{
    /* TODO
     * Длина результата — сумма длин. Байт под завершающий ноль просится
     * отдельно.
     *
     * Скопированные байты (обе части, без нуля) прибавьте к
     * stat_copied_naive.
     */
    (void)a; (void)b;
    return NULL;
}

/* ── способ второй: растущий буфер ─────────────────────────────────────── */

struct Buf {
    char  *data;   /* блок в куче или NULL */
    size_t len;    /* сколько байт занято, не считая завершающего нуля */
    size_t cap;    /* сколько байт помещается без переезда */
};

static void buf_init(struct Buf *b)
{
    /* TODO: пустой буфер — это NULL, 0, 0. */
    (void)b;
}

static void buf_free(struct Buf *b)
{
    /* TODO: после вызова структура описывает корректный пустой буфер. */
    (void)b;
}

/* Обеспечивает вместимость не меньше need байт. Единственное место, где
   вызывается realloc. */
static bool buf_reserve(struct Buf *b, size_t need)
{
    /* TODO
     * Удваивайте вместимость, пока её не хватает; нулевая при умножении на
     * два останется нулевой. Результат realloc кладите во временную
     * переменную.
     *
     * Когда переезд случился, увеличьте stat_growths.
     */
    (void)b; (void)need;
    return false;
}

/* Дописывает строку s в конец буфера. */
static bool buf_push_str(struct Buf *b, const char *s)
{
    /* TODO
     * Место просите у buf_reserve — с запасом на завершающий ноль.
     * Скопированные байты прибавьте к stat_copied_buf.
     * После каждого вызова buf_cstr должна давать законную строку.
     */
    (void)b; (void)s;
    (void)buf_reserve;   /* уберите, когда начнёте её вызывать */
    return false;
}

/* Содержимое буфера как обычная строка. Пустой буфер даёт пустую строку. */
static const char *buf_cstr(const struct Buf *b)
{
    return b->data == NULL ? "" : b->data;
}

/* ── измерение ─────────────────────────────────────────────────────────── */

/* Собирает строку из n кусочков наивной склейкой. Результат — в куче. */
static char *build_naive(const char *piece, size_t n)
{
    char *result = str_concat("", "");
    if (result == NULL)
        return NULL;

    for (size_t i = 0; i < n; i++) {
        char *bigger = str_concat(result, piece);
        if (bigger == NULL) {
            free(result);
            return NULL;
        }
        free(result);
        result = bigger;
    }
    return result;
}

/* Собирает ту же строку буфером. Результат — в куче, владелец вызывающий. */
static char *build_buffered(const char *piece, size_t n)
{
    struct Buf b;
    buf_init(&b);

    for (size_t i = 0; i < n; i++) {
        if (!buf_push_str(&b, piece)) {
            buf_free(&b);
            return NULL;
        }
    }

    size_t len = b.len;
    char *result = malloc(len + 1);
    if (result != NULL)
        memcpy(result, buf_cstr(&b), len + 1);

    buf_free(&b);
    return result;
}

static void measure(size_t n)
{
    const char *piece = "x";

    stat_copied_naive = stat_copied_buf = stat_growths = 0;

    clock_t t0 = clock();
    char *naive = build_naive(piece, n);
    clock_t t1 = clock();
    char *buffered = build_buffered(piece, n);
    clock_t t2 = clock();

    if (naive == NULL || buffered == NULL) {
        printf("памяти не хватило\n");
        free(naive);
        free(buffered);
        return;
    }

    printf("кусочков: %zu\n\n", n);
    printf("наивная склейка: длина %zu, скопировано %zu байт, %.3f с\n",
           strlen(naive), stat_copied_naive,
           (double)(t1 - t0) / CLOCKS_PER_SEC);
    printf("буфер:           длина %zu, скопировано %zu байт, "
           "переездов %zu, %.3f с\n",
           strlen(buffered), stat_copied_buf, stat_growths,
           (double)(t2 - t1) / CLOCKS_PER_SEC);
    printf("\nстроки совпадают: %s\n",
           strcmp(naive, buffered) == 0 ? "да" : "нет");

    free(naive);
    free(buffered);
}

/* ── самопроверка ────────────────────────────────────────────────────────
   expect_* сравнивают то, что вернули ваши функции, с тем, что должно быть.
   Ожидаемые значения здесь — часть условия. */
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

static void expect_str(const char *what, const char *got, const char *want)
{
    checks_run++;
    if (got != NULL && strcmp(got, want) == 0) {
        printf("  ok      %s = \"%s\"\n", what, got);
        return;
    }
    checks_failed++;
    printf("  ОШИБКА  %s = %s%s%s, ожидалось \"%s\"\n", what,
           got == NULL ? "" : "\"", got == NULL ? "NULL" : got,
           got == NULL ? "" : "\"", want);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

static void self_check_body(void)
{
    /* str_concat: обычный случай и две пустые строки. */
    char first[] = "ab";
    char second[] = "cd";

    char *joined = str_concat(first, second);
    expect_str("str_concat(\"ab\", \"cd\")", joined, "abcd");
    expect_ll("результат — новый блок, не first",
              joined != NULL && joined != first, 1);
    if (joined != NULL) {
        joined[0] = 'Z';
        expect_ll("запись в результат не задела first", first[0] == 'a', 1);
    } else {
        expect_ll("запись в результат не задела first", 0, 1);
    }
    free(joined);

    char *empty = str_concat("", "");
    expect_str("str_concat(\"\", \"\")", empty, "");
    expect_ll("и это не NULL", empty != NULL, 1);
    free(empty);

    char *left = str_concat("", "abc");
    expect_str("str_concat(\"\", \"abc\")", left, "abc");
    free(left);

    char *right = str_concat("abc", "");
    expect_str("str_concat(\"abc\", \"\")", right, "abc");
    free(right);

    /* Буфер. */
    struct Buf b;
    buf_init(&b);
    expect_ll("после buf_init: len", (long long)b.len, 0);
    expect_ll("после buf_init: cap", (long long)b.cap, 0);
    expect_ll("после buf_init: data == NULL", b.data == NULL, 1);
    expect_str("пустой буфер как строка", buf_cstr(&b), "");

    expect_ll("buf_push_str(\"abc\")", buf_push_str(&b, "abc"), 1);
    expect_ll("длина после него", (long long)b.len, 3);
    expect_str("содержимое", buf_cstr(&b), "abc");
    expect_ll("завершающий ноль на месте",
              b.data != NULL && b.data[b.len] == '\0', 1);

    expect_ll("buf_push_str(\"\") вернула успех", buf_push_str(&b, ""), 1);
    expect_ll("и не изменила длину", (long long)b.len, 3);

    buf_push_str(&b, "de");
    expect_str("после второго дописывания", buf_cstr(&b), "abcde");
    expect_ll("длина", (long long)b.len, 5);
    expect_ll("вместимости хватает на строку с нулём", b.cap >= b.len + 1, 1);

    buf_free(&b);
    buf_free(&b);
    expect_ll("после двух buf_free: len", (long long)b.len, 0);
    expect_ll("после двух buf_free: cap", (long long)b.cap, 0);
    expect_ll("после двух buf_free: data == NULL", b.data == NULL, 1);

    /* Оба способа дают одну строку. */
    char *a = build_naive("ab", 3);
    char *c = build_buffered("ab", 3);
    expect_str("build_naive(\"ab\", 3)", a, "ababab");
    expect_str("build_buffered(\"ab\", 3)", c, "ababab");
    free(a);
    free(c);

    /* Сколько байт скопировал каждый способ на тысяче кусочков по одному
       байту. У наивной склейки это сумма 0 + 1 + ... + 1000 плюс сам кусочек
       на каждом шаге; у буфера — по байту на кусочек плюс переносы при
       переездах. */
    stat_copied_naive = stat_copied_buf = stat_growths = 0;
    char *big_naive = build_naive("x", 1000);
    char *big_buf   = build_buffered("x", 1000);
    expect_ll("длина наивной", (long long)(big_naive ? strlen(big_naive) : 0), 1000);
    expect_ll("длина буферной", (long long)(big_buf ? strlen(big_buf) : 0), 1000);
    expect_ll("наивная скопировала", (long long)stat_copied_naive, 500500);
    expect_ll("буфер скопировал ровно по байту на кусочек",
              (long long)stat_copied_buf, 1000);
    expect_ll("переездов у буфера меньше двадцати", stat_growths < 20, 1);
    free(big_naive);
    free(big_buf);
}

static int self_check(void)
{
    self_check_body();
    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    size_t n = 20000;
    if (argc > 1) {
        long long given = atoll(argv[1]);
        if (given < 0) {
            fprintf(stderr, "нужно неотрицательное число\n");
            return 1;
        }
        n = (size_t)given;
    }

    measure(n);
    return 0;
}
