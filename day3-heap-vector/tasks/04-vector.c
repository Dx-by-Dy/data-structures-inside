/* Задача 4. Свой динамический массив.  Условие — 04-vector.md
 *
 *   make
 *   printf '+ 1\n+ 2\n?\n=\n' | ./04-vector
 *   ./04-vector --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но операции
 * вектора пока ничего не делают. Ваша работа — их тела; main и разбор команд
 * менять не нужно.
 *
 * move_range возьмите из вчерашней задачи 4 — она там уже написана.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Vec {
    int    *data;   /* блок в куче или NULL */
    size_t  len;    /* сколько элементов занято */
    size_t  cap;    /* сколько помещается без переезда */
};

/* Счётчики. Увеличивать их должны ваши функции, а не main. */
static size_t stat_growths = 0;   /* сколько раз выделялся новый блок */
static size_t stat_copied  = 0;   /* сколько элементов при этом перенесено */
static size_t stat_moved   = 0;   /* сколько сдвинуто при вставках и удалениях */

/* Журнал роста. Внутри команды * молчит, иначе * 100000 зальёт экран. */
static bool verbose = true;

static void vec_init(struct Vec *v)
{
    /* TODO: пустой вектор — это NULL, 0, 0. */
    (void)v;
}

static void vec_free(struct Vec *v)
{
    /* TODO
     * После вызова структура должна описывать корректный пустой вектор:
     * повторный vec_free ничего не ломает.
     */
    (void)v;
}

/* Обеспечивает вместимость не меньше need. Единственное место во всей
   программе, где вызывается realloc. Если вместимости уже хватает — ничего не
   делает и realloc не зовёт. */
static bool vec_reserve(struct Vec *v, size_t need)
{
    /* TODO
     * Удваивайте текущую вместимость, пока её не хватает. Нулевая вместимость —
     * отдельный случай: посчитайте, что даст удвоение, прежде чем писать
     * формулу.
     *
     * Результат realloc кладите во временную переменную: при неудаче вектор
     * должен остаться прежним и пригодным к работе.
     *
     * Когда переезд действительно случился, увеличьте stat_growths и
     * stat_copied и напечатайте журнальную строку — но только если verbose:
     *     printf("  (рост %zu -> %zu, перенесено %zu)\n", старая, новая, v->len);
     */
    (void)v; (void)need;
    return false;
}

/* Сдвиг участка. Возьмите тело из вчерашней задачи и не забудьте про
   stat_moved. */
static void move_range(int *dst, const int *src, size_t n)
{
    /* TODO */
    (void)dst; (void)src; (void)n;
}

static bool vec_push(struct Vec *v, int value)
{
    /* TODO: место для нового элемента просит vec_reserve, а растить блок
       самому здесь нечего. */
    if (!vec_reserve(v, v->len + 1))
        return false;
    (void)value;
    return false;
}

static bool vec_pop(struct Vec *v, int *out)
{
    /* TODO: false, если вектор пуст. */
    (void)v; (void)out;
    return false;
}

static bool vec_insert(struct Vec *v, size_t pos, int value)
{
    /* TODO
     * pos == v->len — законная позиция, это добавление в конец.
     * Хвост двигает move_range; аргументы посчитайте на бумаге, как вчера.
     */
    (void)v; (void)pos; (void)value;
    (void)move_range;   /* уберите, когда начнёте её вызывать */
    return false;
}

static bool vec_erase(struct Vec *v, size_t pos)
{
    /* TODO */
    (void)v; (void)pos;
    return false;
}

static void vec_print(const struct Vec *v)
{
    printf("[");
    for (size_t i = 0; i < v->len; i++)
        printf(i == 0 ? "%d" : ", %d", v->data[i]);
    printf("]\n");
}

/* Разбор одной команды. Формат команд — в условии. */
static bool step(struct Vec *v, char cmd)
{
    if (cmd == '+') {
        int value;
        if (scanf("%d", &value) != 1) return false;
        printf("+ %d\n", value);
        if (!vec_push(v, value)) { printf("  памяти нет\n"); return false; }
        printf("  len %zu cap %zu\n", v->len, v->cap);
    } else if (cmd == '-') {
        int out;
        printf("-\n");
        if (vec_pop(v, &out))
            printf("  снято %d, len %zu cap %zu\n", out, v->len, v->cap);
        else
            printf("  вектор пуст\n");
    } else if (cmd == 'i') {
        long long pos; int value;
        if (scanf("%lld %d", &pos, &value) != 2) return false;
        printf("i %lld %d\n", pos, value);
        if (pos < 0 || !vec_insert(v, (size_t)pos, value))
            printf("  позиция за пределами\n");
        else
            printf("  len %zu cap %zu\n", v->len, v->cap);
    } else if (cmd == 'd') {
        long long pos;
        if (scanf("%lld", &pos) != 1) return false;
        printf("d %lld\n", pos);
        if (pos < 0 || !vec_erase(v, (size_t)pos))
            printf("  позиция за пределами\n");
        else
            printf("  len %zu cap %zu\n", v->len, v->cap);
    } else if (cmd == '?') {
        printf("?\n  ");
        vec_print(v);
    } else if (cmd == '*') {
        long long count;
        if (scanf("%lld", &count) != 1) return false;
        printf("* %lld\n", count);
        size_t g0 = stat_growths, c0 = stat_copied;
        verbose = false;
        for (long long i = 0; i < count; i++)
            if (!vec_push(v, (int)i)) break;
        verbose = true;
        printf("  len %zu cap %zu, переездов %zu, перенесено %zu\n",
               v->len, v->cap, stat_growths - g0, stat_copied - c0);
    } else if (cmd == '=') {
        printf("=\n");
        printf("  переездов: %zu\n", stat_growths);
        printf("  перенесено при росте: %zu\n", stat_copied);
        printf("  сдвинуто при вставках и удалениях: %zu\n", stat_moved);
    } else {
        printf("неизвестная команда: %c\n", cmd);
    }
    return true;
}

/* ── самопроверка ────────────────────────────────────────────────────────
   expect_* сравнивают то, что вернули ваши функции, с тем, что должно быть.
   Ожидаемые значения здесь — часть условия.*/
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

static void expect_vec(const char *what, const struct Vec *v,
                       const int *want, size_t n)
{
    checks_run++;
    if (v->len != n) {
        checks_failed++;
        printf("  ОШИБКА  %s: длина %zu, ожидалась %zu\n", what, v->len, n);
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (v->data[i] == want[i])
            continue;
        checks_failed++;
        printf("  ОШИБКА  %s\n", what);
        printf("            получилось:");
        for (size_t j = 0; j < n; j++) printf(" %d", v->data[j]);
        printf("\n            ожидалось: ");
        for (size_t j = 0; j < n; j++) printf(" %d", want[j]);
        printf("\n");
        return;
    }
    printf("  ok      %s\n", what);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

/* Краевые случаи. */
static void self_check_body(void)
{
    struct Vec v;
    int out = -12345;

    /* Пустой вектор. */
    vec_init(&v);
    expect_ll("после vec_init: len", (long long)v.len, 0);
    expect_ll("после vec_init: cap", (long long)v.cap, 0);
    expect_ll("после vec_init: data == NULL", v.data == NULL, 1);
    expect_ll("vec_pop на пустом", vec_pop(&v, &out), 0);
    expect_ll("vec_pop не тронул *out", out, -12345);
    expect_ll("vec_erase(0) на пустом", vec_erase(&v, 0), 0);
    expect_ll("vec_insert(5, x) на пустом", vec_insert(&v, 5, 1), 0);
    expect_ll("vec_insert(0, 7) на пустом", vec_insert(&v, 0, 7), 1);
    expect_ll("после неё len", (long long)v.len, 1);
    expect_ll("после неё cap", (long long)v.cap, 4);
    vec_free(&v);

    /* vec_free дважды подряд. */
    vec_init(&v);
    vec_push(&v, 1);
    vec_free(&v);
    vec_free(&v);
    expect_ll("после двух vec_free: len", (long long)v.len, 0);
    expect_ll("после двух vec_free: cap", (long long)v.cap, 0);
    expect_ll("после двух vec_free: data == NULL", v.data == NULL, 1);

    /* Рост от нулевой вместимости и дальше. */
    vec_init(&v);
    verbose = false;
    expect_ll("vec_push в пустой", vec_push(&v, 1), 1);
    expect_ll("cap после первого push", (long long)v.cap, 4);
    for (int i = 2; i <= 4; i++) vec_push(&v, i);
    expect_ll("cap при len == 4", (long long)v.cap, 4);
    vec_push(&v, 5);
    expect_ll("cap при len == 5", (long long)v.cap, 8);
    static const int five[] = { 1, 2, 3, 4, 5 };
    expect_vec("содержимое после пяти push", &v, five, 5);

    /* Вставка и удаление в середине. */
    expect_ll("vec_insert(0, 99)", vec_insert(&v, 0, 99), 1);
    static const int ins[] = { 99, 1, 2, 3, 4, 5 };
    expect_vec("после vec_insert(0, 99)", &v, ins, 6);

    expect_ll("vec_erase(2)", vec_erase(&v, 2), 1);
    static const int era[] = { 99, 1, 3, 4, 5 };
    expect_vec("после vec_erase(2)", &v, era, 5);

    expect_ll("vec_erase(5) за пределами", vec_erase(&v, 5), 0);
    expect_ll("длина не изменилась", (long long)v.len, 5);

    /* Вставка в конец: pos == len — законная позиция. */
    size_t at_end = v.len;
    expect_ll("vec_insert(len, 42)", vec_insert(&v, at_end, 42), 1);
    static const int app[] = { 99, 1, 3, 4, 5, 42 };
    expect_vec("после вставки в конец", &v, app, 6);
    expect_ll("vec_insert(len + 1, x) за пределами",
              vec_insert(&v, v.len + 1, 0), 0);

    /* Снятие с конца. */
    expect_ll("vec_pop", vec_pop(&v, &out), 1);
    expect_ll("vec_pop вернул последний", out, 42);
    expect_ll("cap после vec_pop не уменьшился", (long long)v.cap, 8);
    vec_free(&v);

    /* Амортизация: сто тысяч добавлений. */
    vec_init(&v);
    size_t g0 = stat_growths, c0 = stat_copied;
    for (int i = 0; i < 100000; i++)
        if (!vec_push(&v, i)) break;
    verbose = true;
    expect_ll("len после 100000 push", (long long)v.len, 100000);
    expect_ll("cap после 100000 push", (long long)v.cap, 131072);
    expect_ll("переездов", (long long)(stat_growths - g0), 16);
    expect_ll("перенесено при росте", (long long)(stat_copied - c0), 131068);
    if (v.len > 50000)
        expect_ll("значение в середине", v.data[50000], 50000);
    else
        expect_ll("вектор дорос до середины", 0, 1);
    vec_free(&v);
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

    struct Vec v;
    vec_init(&v);

    char cmd;
    while (scanf(" %c", &cmd) == 1)
        if (!step(&v, cmd))
            break;

    vec_free(&v);
    return 0;
}
