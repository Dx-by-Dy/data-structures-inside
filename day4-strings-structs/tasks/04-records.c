/* Задача 4. Записи.  Условие — 04-records.md
 *
 *   make
 *   printf 'Anna 17 4.5\nBoris 18 3.9\n' | ./04-records age
 *   ./04-records --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но база пока
 * ничего не хранит. Ваша работа — шесть мест с TODO; main, разбор ввода,
 * печать таблицы и отчёт о памяти менять не нужно.
 *
 * Пока db_push возвращает false, программа печатает «запись не добавлена» на
 * каждую строку ввода — так и должно быть до первой правки. По той же причине
 * `make release` пока предупреждает про неинициализированные поля базы: как
 * только db_init заработает, предупреждение исчезнет. Обычный `make` молчит с
 * самого начала.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#define NAME_MAX_LEN 64

struct Person {
    char  *name;    /* строка в куче; владелец — эта запись */
    int    age;
    double score;
};

struct Db {
    struct Person *items;   /* блок в куче или NULL */
    size_t         len;     /* сколько записей занято */
    size_t         cap;     /* сколько помещается без переезда */
};

/* Копия строки в куче. Владелец результата — вызывающий. */
static char *my_strdup(const char *s)
{
    /* TODO: длина плюс байт, которого не видно. */
    (void)s;
    return NULL;
}

static void db_init(struct Db *db)
{
    /* TODO: пустая база — это NULL, 0, 0. */
    (void)db;
}

/* Освобождает всё, что выделила база: сначала строки, потом массив.
   После вызова структура описывает корректную пустую базу. */
static void db_free(struct Db *db)
{
    /* TODO
     * Порядок здесь важен — решите, какой, и будьте готовы объяснить, что
     * ломается при обратном.
     */
    (void)db;
}

/* Обеспечивает вместимость не меньше need записей. Единственное место, где
   вызывается realloc. */
static bool db_reserve(struct Db *db, size_t need)
{
    /* TODO
     * Удваивайте вместимость, пока её не хватает; нулевая при умножении на
     * два останется нулевой. Результат realloc кладите во временную
     * переменную.
     */
    (void)db; (void)need;
    return false;
}

/* Добавляет запись в конец. Имя копируется в кучу — посмотрите в main, что
   станет с буфером, из которого его прочитали, на следующей итерации. */
static bool db_push(struct Db *db, const char *name, int age, double score)
{
    /* TODO
     * Место просите у db_reserve, копию имени — у my_strdup.
     * Если одно из двух не получилось, база обязана остаться корректной и без
     * потерянных блоков.
     */
    (void)db; (void)name; (void)age; (void)score;
    (void)db_reserve;   /* уберите, когда начнёте её вызывать */
    return false;
}

/* Номер записи с таким именем либо -1, если такой нет. */
static long long db_find(const struct Db *db, const char *name)
{
    /* TODO: сравнивать имена надо по содержимому. */
    (void)db; (void)name;
    return -1;
}

/* Сортировка по ключу: 0 — по имени, 1 — по возрасту, 2 — по баллу.
   При равных ключах порядок записей сохраняется. */
static void db_sort(struct Db *db, int key)
{
    /* TODO
     * Меняются местами целые записи: структура присваивается одной строкой.
     * Строки при этом никуда не копируются — переезжают только указатели.
     */
    (void)db; (void)key;
}

/* ── выданное готовым ──────────────────────────────────────────────────── */

static void print_table(const struct Db *db)
{
    for (size_t i = 0; i < db->len; i++)
        printf("  %-10s %3d  %5.2f\n",
               db->items[i].name, db->items[i].age, db->items[i].score);
    if (db->len == 0)
        printf("  (пусто)\n");
}

/* Сколько байт занимают дыры внутри одной записи. */
static size_t holes_per_record(void)
{
    size_t fields = sizeof(char *) + sizeof(int) + sizeof(double);
    return sizeof(struct Person) - fields;
}

static void print_memory(const struct Db *db)
{
    size_t strings = 0;
    for (size_t i = 0; i < db->len; i++)
        strings += strlen(db->items[i].name) + 1;

    printf("память:\n");
    printf("  sizeof(struct Person) = %zu, сумма полей %zu, дыр %zu\n",
           sizeof(struct Person),
           sizeof(char *) + sizeof(int) + sizeof(double),
           holes_per_record());
    printf("  смещения: name %zu, age %zu, score %zu\n",
           offsetof(struct Person, name), offsetof(struct Person, age),
           offsetof(struct Person, score));
    printf("  массив: записей %zu при вместимости %zu = %zu байт\n",
           db->len, db->cap, db->cap * sizeof(struct Person));
    printf("  строки: %zu байт в %zu блоках\n", strings, db->len);
    printf("  блоков в куче всего: %zu\n", db->len + (db->items ? 1 : 0));
}

static const char *key_name(int key)
{
    if (key == 0) return "имени";
    if (key == 1) return "возрасту";
    return "баллу";
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

/* Порядок имён в базе одной строкой: "Anna,Boris,Vera". */
static void expect_order(const char *what, const struct Db *db,
                         const char *want)
{
    char got[256] = "";
    size_t used = 0;

    for (size_t i = 0; i < db->len && used + 1 < sizeof got; i++) {
        const char *name = db->items[i].name;
        if (i > 0 && used + 1 < sizeof got)
            got[used++] = ',';
        for (size_t k = 0; name != NULL && name[k] != '\0'
                           && used + 1 < sizeof got; k++)
            got[used++] = name[k];
    }
    got[used] = '\0';

    expect_str(what, got, want);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

static void self_check_body(void)
{
    struct Db db;

    /* Пустая база. */
    db_init(&db);
    expect_ll("после db_init: len", (long long)db.len, 0);
    expect_ll("после db_init: cap", (long long)db.cap, 0);
    expect_ll("после db_init: items == NULL", db.items == NULL, 1);
    expect_ll("db_find в пустой базе", db_find(&db, "Anna"), -1);
    db_sort(&db, 0);
    expect_ll("db_sort пустой базы не сломал len", (long long)db.len, 0);

    /* my_strdup. */
    char source[] = "Anna";
    char *copy = my_strdup(source);
    expect_str("my_strdup(\"Anna\")", copy, "Anna");
    expect_ll("копия — новый блок", copy != NULL && copy != source, 1);
    if (copy != NULL) {
        copy[0] = 'J';
        expect_ll("запись в копию не задела оригинал", source[0] == 'A', 1);
    } else {
        expect_ll("запись в копию не задела оригинал", 0, 1);
    }
    free(copy);

    char *empty = my_strdup("");
    expect_str("my_strdup(\"\")", empty, "");
    free(empty);

    /* Заполнение. Имя каждый раз лежит в одном и том же буфере — если db_push
       не скопировала его, все записи окажутся одинаковыми. */
    char buffer[NAME_MAX_LEN];

    strcpy(buffer, "Vera");  db_push(&db, buffer, 17, 5.0);
    strcpy(buffer, "Anna");  db_push(&db, buffer, 18, 4.5);
    strcpy(buffer, "Boris"); db_push(&db, buffer, 17, 3.9);

    expect_ll("после трёх db_push: len", (long long)db.len, 3);
    expect_ll("вместимость не меньше длины", db.cap >= db.len, 1);
    expect_order("порядок при добавлении", &db, "Vera,Anna,Boris");
    expect_ll("имя записи — своя копия, а не буфер",
              db.len == 3 && db.items[0].name != buffer, 1);
    expect_ll("возраст первой записи", db.len == 3 ? db.items[0].age : -1, 17);

    /* Поиск. */
    expect_ll("db_find(\"Anna\")", db_find(&db, "Anna"), 1);
    expect_ll("db_find(\"Vera\")", db_find(&db, "Vera"), 0);
    expect_ll("db_find(\"Xenia\")", db_find(&db, "Xenia"), -1);
    expect_ll("db_find различает регистр", db_find(&db, "anna"), -1);

    /* Сортировка по каждому из трёх ключей. */
    db_sort(&db, 0);
    expect_order("по имени", &db, "Anna,Boris,Vera");
    expect_ll("после сортировки db_find(\"Vera\")", db_find(&db, "Vera"), 2);

    db_sort(&db, 2);
    expect_order("по баллу", &db, "Boris,Anna,Vera");

    db_sort(&db, 1);
    expect_order("по возрасту, при равных — прежний порядок",
                 &db, "Boris,Vera,Anna");

    /* Строки пережили сортировку: проверяется содержимое, а не адреса. */
    expect_str("имя первой записи после трёх сортировок",
               db.len == 3 ? db.items[0].name : NULL, "Boris");
    expect_ll("балл первой записи",
              db.len == 3 && db.items[0].score == 3.9, 1);

    db_free(&db);
    db_free(&db);
    expect_ll("после двух db_free: len", (long long)db.len, 0);
    expect_ll("после двух db_free: cap", (long long)db.cap, 0);
    expect_ll("после двух db_free: items == NULL", db.items == NULL, 1);

    /* Рост на тысяче записей. */
    db_init(&db);
    for (int i = 0; i < 1000; i++) {
        char name[NAME_MAX_LEN];
        snprintf(name, sizeof name, "имя%d", i);
        if (!db_push(&db, name, i % 100, i / 10.0))
            break;
    }
    expect_ll("len после 1000 db_push", (long long)db.len, 1000);
    expect_ll("вместимость не меньше длины", db.cap >= db.len, 1);
    expect_str("имя последней записи",
               db.len == 1000 ? db.items[999].name : NULL, "имя999");
    expect_ll("db_find(\"имя500\")", db_find(&db, "имя500"), 500);
    db_free(&db);
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

    int key = 0;
    if (argc > 1) {
        if (strcmp(argv[1], "name") == 0)       key = 0;
        else if (strcmp(argv[1], "age") == 0)   key = 1;
        else if (strcmp(argv[1], "score") == 0) key = 2;
        else {
            fprintf(stderr, "ключ: name, age или score\n");
            return 1;
        }
    }

    struct Db db;
    db_init(&db);

    char name[NAME_MAX_LEN];
    int age;
    double score;

    while (scanf("%63s %d %lf", name, &age, &score) == 3)
        if (!db_push(&db, name, age, score))
            printf("запись не добавлена: %s\n", name);

    printf("прочитано записей: %zu\n", db.len);
    print_table(&db);

    db_sort(&db, key);
    printf("\nотсортировано по %s:\n", key_name(key));
    print_table(&db);

    if (db.len > 0) {
        const char *wanted = db.items[db.len - 1].name;
        printf("\nпоиск \"%s\": запись %lld\n", wanted, db_find(&db, wanted));
    }
    printf("поиск \"Xenia\": %lld\n", db_find(&db, "Xenia"));

    printf("\n");
    print_memory(&db);

    db_free(&db);
    return 0;
}
