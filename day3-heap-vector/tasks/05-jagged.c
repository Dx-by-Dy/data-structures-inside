/* Задача 5. Список списков.  Условие — 05-jagged.md
 *
 *   make
 *   printf '3\n2 1 2\n3 10 20 30\n1 7\n' | ./05-jagged
 *   ./05-jagged --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но три функции
 * пока ничего не делают. Ваша работа — их тела; main и разбор ввода менять не
 * нужно.
 *
 * Считать выделения и освобождения нужно самим: для этого заведены обёртки
 * tracked_malloc и tracked_free, и вызывать в задаче надо только их.
 *
 * Пока free_rows пуста, санитайзер при завершении сообщит об утечке. Это
 * ожидаемо и само по себе полезно: посмотрите на отчёт, а потом добейтесь,
 * чтобы он исчез.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t allocated = 0;
static size_t released  = 0;

static void *tracked_malloc(size_t bytes)
{
    void *p = malloc(bytes);
    if (p != NULL)
        allocated++;
    return p;
}

static void tracked_free(void *p)
{
    if (p != NULL)
        released++;
    free(p);
}

static void print_row(const char *label, const int *row, size_t len)
{
    printf("%s", label);
    for (size_t i = 0; i < len; i++)
        printf(" %d", row[i]);
    printf("\n");
}

/* Освобождает nrows строк и сам массив указателей.
   Должна переживать rows == NULL и строки, равные NULL. */
static void free_rows(int **rows, size_t nrows)
{
    /* TODO
     * Порядок важен: сначала строки, потом внешний массив.
     */
    (void)rows; (void)nrows;
}

/* Новый внешний массив, строки — те же самые.
   ВЛАДЕЛЕЦ строк не меняется: освобождать через free_rows эту копию нельзя. */
static int **shallow_copy(int *const *rows, size_t nrows)
{
    /* TODO */
    (void)rows; (void)nrows;
    return NULL;
}

/* Новый внешний массив и новые строки. ВЛАДЕЛЕЦ всего — вызывающий.
   Если какая-то строка не выделилась, освободить надо и уже выделенные. */
static int **deep_copy(int *const *rows, const size_t *lens, size_t nrows)
{
    /* TODO
     * Обнулите указатели строк сразу после выделения внешнего массива —
     * тогда free_rows отработает на недособранной копии.
     */
    (void)rows; (void)lens; (void)nrows;
    return NULL;
}

/* Один прогон. Все блоки освобождаются здесь же. */
static void run(int **rows, const size_t *lens, size_t nrows)
{
    printf("исходный:\n");
    for (size_t r = 0; r < nrows; r++) {
        char prefix[48];
        snprintf(prefix, sizeof prefix, "  [%zu] len %zu:", r, lens[r]);
        print_row(prefix, rows[r], lens[r]);
    }

    int **shallow = shallow_copy(rows, nrows);
    int **deep = deep_copy(rows, lens, nrows);
    if (shallow == NULL || deep == NULL) {
        printf("\nкопию сделать не удалось\n");
        tracked_free(shallow);
        free_rows(deep, nrows);
        return;
    }

    size_t r0 = 0;
    size_t r1 = (nrows > 1) ? 1 : 0;

    if (nrows > 0 && lens[r0] > 0) {
        shallow[r0][0] = 999;
        printf("\nshallow_copy, затем shallow[%zu][0] = 999:\n", r0);
        print_row("  исходный:", rows[r0], lens[r0]);
        print_row("  shallow: ", shallow[r0], lens[r0]);
    }

    if (nrows > 0 && lens[r1] > 0) {
        deep[r1][0] = 777;
        printf("\ndeep_copy, затем deep[%zu][0] = 777:\n", r1);
        print_row("  исходный:", rows[r1], lens[r1]);
        print_row("  deep:    ", deep[r1], lens[r1]);
    }

    printf("\nsizeof rows = %zu, sizeof rows[0] = %zu\n",
           sizeof rows, sizeof rows[0]);

    /* Поверхностной копии принадлежит только внешний массив. */
    tracked_free(shallow);
    free_rows(deep, nrows);
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

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

/* Строит три строки длиной 2, 3 и 1 своими руками — не вашими функциями. */
static int **build3(const size_t *lens)
{
    static const int r0[] = { 1, 2 }, r1[] = { 10, 20, 30 }, r2[] = { 7 };
    const int *src[] = { r0, r1, r2 };

    int **rows = tracked_malloc(3 * sizeof *rows);
    if (rows == NULL)
        return NULL;
    for (size_t r = 0; r < 3; r++)
        rows[r] = NULL;
    for (size_t r = 0; r < 3; r++) {
        rows[r] = tracked_malloc(lens[r] * sizeof *rows[r]);
        if (rows[r] == NULL)
            return rows;
        for (size_t i = 0; i < lens[r]; i++)
            rows[r][i] = src[r][i];
    }
    return rows;
}

/* Краевые случаи. */
static void self_check_body(void)
{
    static const size_t lens[] = { 2, 3, 1 };

    int **rows = build3(lens);
    if (rows == NULL)
        return;

    int **shallow = shallow_copy(rows, 3);
    expect_ll("shallow_copy вернул блок", shallow != NULL, 1);
    if (shallow != NULL) {
        expect_ll("shallow — другой внешний массив", shallow != rows, 1);
        expect_ll("shallow[0] — та же строка", shallow[0] == rows[0], 1);
        expect_ll("shallow[2] — та же строка", shallow[2] == rows[2], 1);
        shallow[0][0] = 999;
        expect_ll("после shallow[0][0] = 999: исходный", rows[0][0], 999);
    }

    int **deep = deep_copy(rows, lens, 3);
    expect_ll("deep_copy вернул блок", deep != NULL, 1);
    if (deep != NULL) {
        expect_ll("deep[1] — другая строка", deep[1] != rows[1], 1);
        expect_ll("deep[1][0] до записи", deep[1][0], 10);
        expect_ll("deep[0][0] перенёс изменение", deep[0][0], 999);
        deep[1][0] = 777;
        expect_ll("после deep[1][0] = 777: копия", deep[1][0], 777);
        expect_ll("после deep[1][0] = 777: исходный не тронут", rows[1][0], 10);
        expect_ll("deep[2][0]", deep[2][0], 7);
    }

    /* Поверхностной копии принадлежит только внешний массив, поэтому
       free_rows к ней не применяется. */
    tracked_free(shallow);
    free_rows(deep, 3);
    free_rows(rows, 3);
    expect_ll("выделено блоков минус освобождено",
              (long long)allocated - (long long)released, 0);

    /* Строка нулевой длины: писать в row[0] там некуда. */
    allocated = released = 0;
    static const size_t zero_len[] = { 0 };
    int **one = tracked_malloc(1 * sizeof *one);
    if (one != NULL) {
        one[0] = tracked_malloc(1);
        int **dz = deep_copy(one, zero_len, 1);
        expect_ll("deep_copy строки нулевой длины вернул блок", dz != NULL, 1);
        free_rows(dz, 1);
        free_rows(one, 1);
        expect_ll("на строке нулевой длины выделено == освобождено",
                  (long long)allocated - (long long)released, 0);
    }

    /* free_rows(NULL, 0) обязан ничего не делать и не падать. */
    allocated = released = 0;
    free_rows(NULL, 0);
    expect_ll("free_rows(NULL, 0) ничего не освободил", (long long)released, 0);
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

    long long got;
    if (scanf("%lld", &got) != 1 || got <= 0) {
        fprintf(stderr, "нужно положительное число строк\n");
        return 1;
    }
    size_t nrows = (size_t)got;

    size_t *lens = tracked_malloc(nrows * sizeof *lens);
    int **rows = tracked_malloc(nrows * sizeof *rows);
    if (lens == NULL || rows == NULL) {
        tracked_free(lens);
        tracked_free(rows);
        return 1;
    }
    for (size_t r = 0; r < nrows; r++)
        rows[r] = NULL;

    for (size_t r = 0; r < nrows; r++) {
        long long len;
        if (scanf("%lld", &len) != 1 || len < 0) {
            fprintf(stderr, "нужна неотрицательная длина строки\n");
            free_rows(rows, nrows);
            tracked_free(lens);
            return 1;
        }
        lens[r] = (size_t)len;
        rows[r] = tracked_malloc((lens[r] == 0 ? 1 : lens[r]) * sizeof *rows[r]);
        if (rows[r] == NULL) {
            free_rows(rows, nrows);
            tracked_free(lens);
            return 1;
        }
        for (size_t i = 0; i < lens[r]; i++) {
            if (scanf("%d", &rows[r][i]) != 1) {
                fprintf(stderr, "не хватило чисел\n");
                free_rows(rows, nrows);
                tracked_free(lens);
                return 1;
            }
        }
    }

    run(rows, lens, nrows);
    free_rows(rows, nrows);
    tracked_free(lens);

    printf("выделено блоков: %zu, освобождено: %zu\n", allocated, released);
    return 0;
}
