/* Задача 4. Словарь: разбор команд, печать и самопроверка.
 *
 * Этот файл выдан готовым, менять его не нужно. Ваша работа — hashmap.c.
 *
 *   make
 *   printf '+ one 1\n+ two 2\n#\n' | ./hashmap
 *   ./hashmap --check      встроенные краевые случаи
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

#define KEY_MAX 64

/* ── печать ─────────────────────────────────────────────────────────────── */

/* Обход словаря идёт по вектору записей, а не по корзинам: удалённые записи
   (key == NULL) пропускаются. */
static void print_map(const struct Map *m)
{
    printf("  {");
    bool first = true;
    for (size_t i = 0; i < m->entries.len; i++) {
        const struct Entry *e = &m->entries.data[i];
        if (e->key == NULL)
            continue;
        printf("%s%s: %ld", first ? "" : ", ", e->key, e->value);
        first = false;
    }
    printf("}\n");
}

static void print_stats(const struct Map *m)
{
    printf("  ключей %zu, записей в векторе %zu, корзин %zu\n",
           map_len(m), m->entries.len, m->index_cap);
    if (m->index_cap > 0)
        printf("  занято корзин: %.2f\n",
               (double)m->entries.len / (double)m->index_cap);
    printf("  просмотрено корзин: %zu\n", map_stat_probes);
    printf("  сравнений ключей: %zu\n", map_stat_compares);
    printf("  перестроек таблицы: %zu\n", map_stat_rehashes);
    printf("  переездов вектора: %zu, перенесено записей: %zu\n",
           vec_stat_growths, vec_stat_copied);
}

/* ── разбор команд ──────────────────────────────────────────────────────── */

static bool step(struct Map *m, char cmd)
{
    char key[KEY_MAX];
    long value;
    long count;

    switch (cmd) {
    case '+':
        if (scanf("%63s %ld", key, &value) != 2)
            return false;
        printf("+ %s %ld\n", key, value);
        if (!map_put(m, key, value))
            printf("  не хватило памяти\n");
        else
            printf("  ключей %zu\n", map_len(m));
        return true;

    case '?':
        if (scanf("%63s", key) != 1)
            return false;
        printf("? %s\n", key);
        if (map_get(m, key, &value))
            printf("  %ld\n", value);
        else
            printf("  ключа нет\n");
        return true;

    case '-':
        if (scanf("%63s", key) != 1)
            return false;
        printf("- %s\n", key);
        if (map_del(m, key))
            printf("  удалён, ключей %zu\n", map_len(m));
        else
            printf("  ключа нет\n");
        return true;

    case '#':
        printf("#\n");
        print_map(m);
        return true;

    case '=':
        printf("=\n");
        print_stats(m);
        return true;

    case '*':
        if (scanf("%ld", &count) != 1)
            return false;
        printf("* %ld\n", count);
        for (long i = 0; i < count; i++) {
            snprintf(key, sizeof key, "key%ld", i);
            if (!map_put(m, key, i)) {
                printf("  не хватило памяти\n");
                break;
            }
        }
        printf("  ключей %zu\n", map_len(m));
        return true;

    default:
        printf("неизвестная команда: %c\n", cmd);
        return true;
    }
}

/* ── самопроверка ────────────────────────────────────────────────────────
   expect_* сравнивают то, что вернули ваши функции, с тем, что должно быть.
   Проверяются свойства словаря, а не напечатанные строки: подогнать вывод,
   не написав словарь, не получится. */
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
    printf("  ОШИБКА  %s = \"%s\", ожидалось \"%s\"\n",
           what, got == NULL ? "(нет записи)" : got, want);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

/* Ключи в порядке вставки, склеенные через пробел: по этой строке видно и
   состав словаря, и порядок. */
static void order_of(const struct Map *m, char *out, size_t size)
{
    out[0] = '\0';
    size_t used = 0;
    for (size_t i = 0; i < m->entries.len; i++) {
        const struct Entry *e = &m->entries.data[i];
        if (e->key == NULL)
            continue;
        int written = snprintf(out + used, size - used,
                               used == 0 ? "%s" : " %s", e->key);
        if (written < 0 || (size_t)written >= size - used)
            return;
        used += (size_t)written;
    }
}

static void self_check_body(void)
{
    struct Map m;
    long value;
    char order[512];

    /* 1. Пустой словарь. Структура заранее замусорена: map_init обязан
       привести её в порядок, не глядя на прежнее содержимое. */
    memset(&m, 0xAA, sizeof m);
    map_init(&m);

    bool initialized = m.entries.data == NULL && m.entries.len == 0 &&
                       m.entries.cap == 0 && m.index == NULL &&
                       m.index_cap == 0 && m.live == 0;
    expect_ll("map_init: пустой вектор и ни одной корзины", initialized, 1);
    if (!initialized) {
        printf("  остальные проверки пропущены: дальше поля структуры\n"
               "  читаются напрямую, и читать в них нечего\n");
        return;
    }

    expect_ll("len пустого словаря", (long long)map_len(&m), 0);
    value = 777;
    expect_ll("get отсутствующего ключа", map_get(&m, "nope", &value), 0);
    expect_ll("и не тронул *out", value, 777);
    expect_ll("del отсутствующего ключа", map_del(&m, "nope"), 0);

    /* 2. Вставка, чтение, замена. */
    expect_ll("put(\"one\", 1)", map_put(&m, "one", 1), 1);
    expect_ll("put(\"two\", 2)", map_put(&m, "two", 2), 1);
    expect_ll("put(\"three\", 3)", map_put(&m, "three", 3), 1);
    expect_ll("len", (long long)map_len(&m), 3);
    expect_ll("get(\"two\")", map_get(&m, "two", &value) ? value : -1, 2);
    expect_ll("put(\"two\", 22) на существующий ключ", map_put(&m, "two", 22), 1);
    expect_ll("get(\"two\") после замены", map_get(&m, "two", &value) ? value : -1, 22);
    expect_ll("len после замены не изменилась", (long long)map_len(&m), 3);
    order_of(&m, order, sizeof order);
    expect_str("порядок вставки", order, "one two three");
    expect_ll("корзины появились", m.index_cap > 0, 1);

    /* 3. Ключ принадлежит словарю, а не тому, кто его передал. */
    char buffer[KEY_MAX] = "temporary";
    map_put(&m, buffer, 100);
    memset(buffer, 'z', sizeof buffer - 1);
    buffer[sizeof buffer - 1] = '\0';
    expect_ll("ключ пережил порчу чужого буфера",
              map_get(&m, "temporary", &value) ? value : -1, 100);
    expect_ll("и лежит по другому адресу",
              m.entries.data[3].key != buffer, 1);

    /* 4. Удаление. */
    expect_ll("del(\"two\")", map_del(&m, "two"), 1);
    expect_ll("get(\"two\") после удаления", map_get(&m, "two", &value), 0);
    expect_ll("len после удаления", (long long)map_len(&m), 3);
    expect_ll("повторный del(\"two\")", map_del(&m, "two"), 0);
    order_of(&m, order, sizeof order);
    expect_str("порядок оставшихся", order, "one three temporary");
    expect_ll("put(\"two\", 222) обратно", map_put(&m, "two", 222), 1);
    order_of(&m, order, sizeof order);
    expect_str("вернувшийся ключ встал в конец", order, "one three temporary two");

    /* 5. Три ключа одной корзины: средний удаляется, третий обязан
       по-прежнему находиться. */
    size_t cap = m.index_cap > 0 ? m.index_cap : 8;
    char same[3][KEY_MAX];
    size_t found = 0;
    size_t target = map_hash("one") & (cap - 1);
    for (int i = 0; found < 3 && i < 100000; i++) {
        char probe[KEY_MAX];
        snprintf(probe, sizeof probe, "w%d", i);
        if ((map_hash(probe) & (cap - 1)) == target)
            snprintf(same[found++], KEY_MAX, "%s", probe);
    }
    if (found == 3) {
        map_put(&m, same[0], 1);
        map_put(&m, same[1], 2);
        map_put(&m, same[2], 3);
        expect_ll("средний ключ корзины удалён", map_del(&m, same[1]), 1);
        expect_ll("первый ключ той же корзины на месте",
                  map_get(&m, same[0], &value) ? value : -1, 1);
        expect_ll("третий ключ той же корзины не потерялся",
                  map_get(&m, same[2], &value) ? value : -1, 3);
    } else {
        printf("  (трёх ключей одной корзины не нашлось — проверка пропущена)\n");
    }
    map_free(&m);

    /* 6. Рост. Тысяча ключей, потом каждый второй удаляется, потом тысяча
       новых. */
    map_init(&m);
    char key[KEY_MAX];
    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof key, "key%d", i);
        map_put(&m, key, i);
    }
    expect_ll("len после тысячи вставок", (long long)map_len(&m), 1000);

    long long missing = 0, wrong = 0;
    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof key, "key%d", i);
        if (!map_get(&m, key, &value))
            missing++;
        else if (value != i)
            wrong++;
    }
    expect_ll("ненайденных ключей", missing, 0);
    expect_ll("ключей с чужим значением", wrong, 0);
    expect_ll("корзин — степень двойки",
              m.index_cap > 0 && (m.index_cap & (m.index_cap - 1)) == 0, 1);
    expect_ll("занято меньше трёх четвертей корзин",
              m.entries.len * 4 < m.index_cap * 3, 1);
    expect_ll("первый ключ по-прежнему первый",
              m.entries.data[0].key != NULL &&
              strcmp(m.entries.data[0].key, "key0") == 0, 1);

    for (int i = 0; i < 1000; i += 2) {
        snprintf(key, sizeof key, "key%d", i);
        map_del(&m, key);
    }
    expect_ll("len после пятисот удалений", (long long)map_len(&m), 500);

    missing = 0;
    for (int i = 1; i < 1000; i += 2) {
        snprintf(key, sizeof key, "key%d", i);
        if (!map_get(&m, key, &value))
            missing++;
    }
    expect_ll("уцелевших ключей не потеряно", missing, 0);

    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof key, "new%d", i);
        map_put(&m, key, 1000 + i);
    }
    expect_ll("len после тысячи новых", (long long)map_len(&m), 1500);

    missing = 0;
    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof key, "new%d", i);
        if (!map_get(&m, key, &value) || value != 1000 + i)
            missing++;
    }
    expect_ll("новые ключи на месте", missing, 0);
    expect_ll("удалённые ключи не воскресли",
              map_get(&m, "key0", &value), 0);
    expect_ll("таблица не разрослась без причины", m.index_cap <= 8192, 1);
    map_free(&m);

    /* 7. map_free оставляет корректный пустой словарь. */
    map_free(&m);
    expect_ll("len после двух map_free", (long long)map_len(&m), 0);
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

    /* Структура обнулена до map_init, чтобы скелет можно было запустить, не
       написав ни одной функции. Ваш map_init всё равно обязан её заполнить:
       в --check он получает замусоренную структуру. */
    struct Map m = {{NULL, 0, 0}, NULL, 0, 0};
    map_init(&m);

    char cmd;
    while (scanf(" %c", &cmd) == 1)
        if (!step(&m, cmd))
            break;

    map_free(&m);
    return 0;
}
