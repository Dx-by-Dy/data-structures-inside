/* Задача 4. Сдвиг с перекрытием.  Условие — 04-shift.md
 *
 *   make
 *   printf '8\n1 2 3 4 5 6 7 8\n2 3 1 99\n' | ./04-shift
 *   ./04-shift --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все пять
 * функций пока ничего не делают. Ваша работа — их тела; main и печать менять
 * не нужно.
 *
 * Функции из <string.h> в этой задаче запрещены: memmove вы пишете сами.
 */
#include <stdio.h>
#include <string.h>   /* только ради strcmp в main; в ваших функциях его быть не должно */

#define CAP 64

/* Копирует n элементов из src в dst циклом от начала к концу. */
static void copy_forward(int *dst, const int *src, size_t n)
{
    /* TODO */
    (void)dst; (void)src; (void)n;
}

/* Копирует n элементов из src в dst циклом от конца к началу.
   Идиома обратного цикла по size_t, в которой нет вычитания из нуля:
       for (size_t i = n; i-- > 0; ) ... */
static void copy_backward(int *dst, const int *src, size_t n)
{
    /* TODO */
    (void)dst; (void)src; (void)n;
}

/* Копирует n элементов корректно при любом взаимном расположении участков.
   Ровно одно сравнение указателей и два вызова. */
static void move_range(int *dst, const int *src, size_t n)
{
    /* TODO */
    (void)dst; (void)src; (void)n;
}

/* Удаляет элемент на позиции pos. Возвращает новую длину.
   Копировать ничего не надо: один вызов move_range. */
static size_t delete_at(int *a, size_t n, size_t pos)
{
    /* TODO */
    (void)a; (void)pos;
    return n;
}

/* Вставляет value на позицию pos. Возвращает новую длину, а если места нет
   или позиция за пределами — прежнюю. */
static size_t insert_at(int *a, size_t n, size_t cap, size_t pos, int value)
{
    /* TODO */
    (void)a; (void)cap; (void)pos; (void)value;
    return n;
}

static void print_row(const char *label, const int *a, size_t n)
{
    printf("%s", label);
    for (size_t i = 0; i < n; i++)
        printf(" %d", a[i]);
    printf("\n");
}

/* Шесть опытов над одним и тем же исходным массивом. Перед каждым рабочий
   массив восстанавливается из нетронутой копии — иначе сравнивать нечего. */
static void run(const int *orig, size_t n, size_t k, size_t del, size_t ins, int value)
{
    int work[CAP];

    print_row("исходный:        ", orig, n);

    printf("\nсдвиг вправо на %zu (dst выше src):\n", k);
    copy_forward(work, orig, n);  copy_forward(work + k, work, n - k);
    print_row("  copy_forward:  ", work, n);
    copy_forward(work, orig, n);  copy_backward(work + k, work, n - k);
    print_row("  copy_backward: ", work, n);
    copy_forward(work, orig, n);  move_range(work + k, work, n - k);
    print_row("  move_range:    ", work, n);

    printf("\nсдвиг влево на %zu (dst ниже src):\n", k);
    copy_forward(work, orig, n);  copy_forward(work, work + k, n - k);
    print_row("  copy_forward:  ", work, n);
    copy_forward(work, orig, n);  copy_backward(work, work + k, n - k);
    print_row("  copy_backward: ", work, n);
    copy_forward(work, orig, n);  move_range(work, work + k, n - k);
    print_row("  move_range:    ", work, n);

    printf("\nчерез move_range делаются pop и insert:\n");
    copy_forward(work, orig, n);
    size_t len = delete_at(work, n, del);
    char label[64];
    snprintf(label, sizeof label, "  delete_at(%zu):     ", del);
    print_row(label, work, len);

    len = insert_at(work, len, CAP, ins, value);
    snprintf(label, sizeof label, "  insert_at(%zu, %d): ", ins, value);
    print_row(label, work, len);
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

static void expect_arr(const char *what, const int *got, const int *want, size_t n)
{
    checks_run++;
    for (size_t i = 0; i < n; i++) {
        if (got[i] == want[i])
            continue;
        checks_failed++;
        printf("  ОШИБКА  %s\n", what);
        printf("            получилось:");
        for (size_t j = 0; j < n; j++) printf(" %d", got[j]);
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

/* Восстанавливает рабочий массив своим циклом, а не вашими функциями: иначе
   сломанный copy_forward испортил бы и саму проверку. */
static void restore(int *dst, const int *src, size_t n)
{
    for (size_t i = 0; i < n; i++)
        dst[i] = src[i];
}

/* Краевые случаи. Испорченные результаты здесь такие же ожидаемые, как
   правильные: copy_forward на перекрытии обязан испортить массив ровно так,
   а не как-нибудь ещё. */
static int self_check(void)
{
    static const int src8[]  = { 1, 2, 3, 4, 5, 6, 7, 8 };
    static const int fwd_r[] = { 1, 2, 1, 2, 1, 2, 1, 2 };  /* вправо, forward  */
    static const int bwd_r[] = { 1, 2, 1, 2, 3, 4, 5, 6 };  /* вправо, backward */
    static const int fwd_l[] = { 3, 4, 5, 6, 7, 8, 7, 8 };  /* влево,  forward  */
    static const int bwd_l[] = { 7, 8, 7, 8, 7, 8, 7, 8 };  /* влево,  backward */
    static const int del3[]  = { 1, 2, 3, 5, 6, 7, 8 };
    static const int ins1[]  = { 1, 99, 2, 3, 5, 6, 7, 8 };
    static const int keep2[] = { 111, 222 };

    int w[CAP];

    /* Непересекающиеся участки: все три функции обязаны дать одно и то же. */
    int dst[8];
    copy_forward(dst, src8, 8);
    expect_arr("copy_forward без перекрытия", dst, src8, 8);
    for (size_t i = 0; i < 8; i++) dst[i] = 0;
    copy_backward(dst, src8, 8);
    expect_arr("copy_backward без перекрытия", dst, src8, 8);
    for (size_t i = 0; i < 8; i++) dst[i] = 0;
    move_range(dst, src8, 8);
    expect_arr("move_range без перекрытия", dst, src8, 8);

    /* Сдвиг вправо на 2: dst выше src. */
    restore(w, src8, 8); copy_forward(w + 2, w, 6);
    expect_arr("вправо, copy_forward", w, fwd_r, 8);
    restore(w, src8, 8); copy_backward(w + 2, w, 6);
    expect_arr("вправо, copy_backward", w, bwd_r, 8);
    restore(w, src8, 8); move_range(w + 2, w, 6);
    expect_arr("вправо, move_range", w, bwd_r, 8);

    /* Сдвиг влево на 2: dst ниже src. */
    restore(w, src8, 8); copy_forward(w, w + 2, 6);
    expect_arr("влево, copy_forward", w, fwd_l, 8);
    restore(w, src8, 8); copy_backward(w, w + 2, 6);
    expect_arr("влево, copy_backward", w, bwd_l, 8);
    restore(w, src8, 8); move_range(w, w + 2, 6);
    expect_arr("влево, move_range", w, fwd_l, 8);

    /* k == 0 и k == n: сдвигать нечего, массив обязан остаться прежним. */
    restore(w, src8, 8); move_range(w + 0, w, 8);
    expect_arr("k == 0", w, src8, 8);
    restore(w, src8, 8); move_range(w + 8, w, 0);
    expect_arr("k == n", w, src8, 8);

    /* n == 0: ни одна из трёх функций не должна тронуть ни байта. */
    restore(w, keep2, 2);
    copy_forward(w, src8, 0);
    copy_backward(w, src8, 0);
    move_range(w, src8, 0);
    expect_arr("n == 0 (обратный цикл не должен считать от n - 1)", w, keep2, 2);

    /* delete_at */
    restore(w, src8, 8);
    expect_ll("delete_at(3) вернул длину", (long long)delete_at(w, 8, 3), 7);
    expect_arr("delete_at(3) результат", w, del3, 7);

    restore(w, src8, 8);
    expect_ll("delete_at последнего вернул длину", (long long)delete_at(w, 8, 7), 7);
    expect_arr("delete_at последнего результат", w, src8, 7);

    restore(w, src8, 8);
    expect_ll("delete_at(8) за пределами", (long long)delete_at(w, 8, 8), 8);
    expect_arr("delete_at(8) массив не тронут", w, src8, 8);

    /* insert_at */
    restore(w, del3, 7);
    expect_ll("insert_at(1, 99) вернул длину",
              (long long)insert_at(w, 7, CAP, 1, 99), 8);
    expect_arr("insert_at(1, 99) результат", w, ins1, 8);

    restore(w, src8, 3);
    expect_ll("insert_at в конец (pos == n)",
              (long long)insert_at(w, 3, CAP, 3, 99), 4);
    static const int tail4[] = { 1, 2, 3, 99 };
    expect_arr("insert_at в конец результат", w, tail4, 4);

    restore(w, src8, 3);
    expect_ll("insert_at(4) за пределами при длине 3",
              (long long)insert_at(w, 3, CAP, 4, 99), 3);

    /* Места больше нет: длина обязана остаться прежней. */
    int full[CAP];
    for (size_t i = 0; i < CAP; i++) full[i] = (int)i;
    expect_ll("insert_at при n == cap",
              (long long)insert_at(full, CAP, CAP, 0, -1), CAP);

    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    long long got_n;
    if (scanf("%lld", &got_n) != 1 || got_n < 0 || got_n > CAP) {
        fprintf(stderr, "длина должна быть от 0 до %d\n", CAP);
        return 1;
    }
    size_t n = (size_t)got_n;

    int orig[CAP];
    for (size_t i = 0; i < n; i++) {
        if (scanf("%d", &orig[i]) != 1) {
            fprintf(stderr, "не хватило чисел\n");
            return 1;
        }
    }

    long long k, del, ins;
    int value;
    if (scanf("%lld %lld %lld %d", &k, &del, &ins, &value) != 4
        || k < 0 || (size_t)k > n || del < 0 || ins < 0) {
        fprintf(stderr, "нужны k, позиция удаления, позиция вставки и значение\n");
        return 1;
    }

    run(orig, n, (size_t)k, (size_t)del, (size_t)ins, value);
    return 0;
}
