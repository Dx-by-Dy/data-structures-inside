/* Задача 3. Множество на открытой адресации.  Условие — 03-probe.md
 *
 *   make
 *   printf '+ one\n+ two\n#\n? one\n- one\n#\n=\n' | ./03-probe
 *   ./03-probe --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но множество
 * пока ничего не хранит. Ваша работа — пять функций, помеченных TODO; main,
 * печать и set_hash менять не нужно.
 *
 * Таблица здесь не растёт: шестнадцать слотов, и всё. Рост будет в задаче 4.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SET_CAP  16
#define WORD_MAX 32

#define SLOT_EMPTY 0   /* слот никогда не был занят */
#define SLOT_USED  1   /* в слоте лежит ключ */
#define SLOT_DEAD  2   /* ключ отсюда удалён */

struct Set {
    char   keys[SET_CAP][WORD_MAX];
    int    state[SET_CAP];
    size_t live;   /* сколько ключей лежит */
    size_t dead;   /* сколько надгробий */
};

/* Просмотренные слоты: увеличивают его ваши функции. */
static size_t stat_probes = 0;

/* Хеш ключа. Дан готовым — это FNV-1a из задачи 1. */
static uint32_t set_hash(const char *key)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)key; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

static void set_init(struct Set *s)
{
    /* TODO: пустая таблица — это шестнадцать пустых слотов. */
    (void)s;
}

/* Находит слот ключа. Возвращает его номер и через *found сообщает, лежит ли
   ключ в таблице.
 *
 * Если ключ найден — номер слота, в котором он лежит. Если нет — номер слота,
 * в который его следует положить, либо SET_CAP, если положить некуда.
 *
 * Считает stat_probes: каждый просмотренный слот — единица.
 */
static size_t set_find(const struct Set *s, const char *key, bool *found)
{
    /* TODO
     * Начинать со слота set_hash(key) % SET_CAP и идти вперёд по кругу.
     *
     * SLOT_EMPTY заканчивает поиск. SLOT_DEAD поиск не заканчивает, но годится
     * для вставки.
     *
     * Пустых слотов может не остаться вовсе — проверка на этот случай в
     * --check есть, и завершиться поиск обязан в любом случае.
     */
    (void)s; (void)key;
    *found = false;
    return SET_CAP;
}

/* Добавляет ключ. Если он уже есть, ничего не меняется. false — места нет. */
static bool set_add(struct Set *s, const char *key)
{
    /* TODO: длина ключа не больше WORD_MAX - 1, за этим следит main. */
    (void)s; (void)key;
    /* Строка нужна только скелету: пока set_find никем не вызвана,
       компилятор предупреждает, что она написана зря. Уберите её, когда
       функция понадобится. */
    (void)set_find;
    return false;
}

static bool set_contains(const struct Set *s, const char *key)
{
    /* TODO */
    (void)s; (void)key;
    return false;
}

/* Удаляет ключ. false, если его не было. */
static bool set_erase(struct Set *s, const char *key)
{
    /* TODO */
    (void)s; (void)key;
    return false;
}

/* ── печать ─────────────────────────────────────────────────────────────── */

static void print_map(const struct Set *s)
{
    for (size_t i = 0; i < SET_CAP; i++) {
        printf("  %2zu ", i);
        if (s->state[i] == SLOT_USED)
            printf("%-12s корзина %zu\n", s->keys[i],
                   (size_t)(set_hash(s->keys[i]) % SET_CAP));
        else if (s->state[i] == SLOT_DEAD)
            printf("надгробие\n");
        else
            printf(".\n");
    }
}

static void print_stats(const struct Set *s)
{
    printf("  ключей %zu, надгробий %zu, свободно %zu\n",
           s->live, s->dead, SET_CAP - s->live - s->dead);
    printf("  просмотрено слотов: %zu\n", stat_probes);
}

/* ── самопроверка ───────────────────────────────────────────────────────── */

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

/* Три ключа, которым досталась одна корзина: подбираются перебором. */
static void three_in_one_bucket(char out[3][WORD_MAX])
{
    size_t found = 0;
    size_t target = set_hash("one") % SET_CAP;
    for (int i = 0; found < 3 && i < 100000; i++) {
        char probe[WORD_MAX];
        snprintf(probe, sizeof probe, "w%d", i);
        if (set_hash(probe) % SET_CAP == target)
            snprintf(out[found++], WORD_MAX, "%s", probe);
    }
}

static int self_check(void)
{
    struct Set s;
    char key[WORD_MAX];

    /* 1. Пустая таблица. Структура заранее замусорена. */
    memset(&s, 0xAA, sizeof s);
    set_init(&s);
    expect_ll("ключей в пустой таблице", (long long)s.live, 0);
    expect_ll("надгробий в пустой таблице", (long long)s.dead, 0);
    expect_ll("contains в пустой таблице", set_contains(&s, "nope"), 0);
    expect_ll("erase в пустой таблице", set_erase(&s, "nope"), 0);

    /* 2. Добавление и повторное добавление. */
    expect_ll("add(\"one\")", set_add(&s, "one"), 1);
    expect_ll("contains(\"one\")", set_contains(&s, "one"), 1);
    expect_ll("contains(\"two\")", set_contains(&s, "two"), 0);
    set_add(&s, "one");
    expect_ll("тот же ключ второй раз не удвоил счётчик", (long long)s.live, 1);

    /* 3. Три ключа одной корзины. */
    char same[3][WORD_MAX];
    three_in_one_bucket(same);
    set_init(&s);
    set_add(&s, same[0]);
    set_add(&s, same[1]);
    set_add(&s, same[2]);
    expect_ll("три ключа одной корзины легли", (long long)s.live, 3);
    expect_ll("средний удалён", set_erase(&s, same[1]), 1);
    expect_ll("надгробие появилось", (long long)s.dead, 1);
    expect_ll("первый на месте", set_contains(&s, same[0]), 1);
    expect_ll("третий не потерялся", set_contains(&s, same[2]), 1);
    expect_ll("удалённого нет", set_contains(&s, same[1]), 0);
    expect_ll("повторное удаление", set_erase(&s, same[1]), 0);
    expect_ll("ключ вернулся", set_add(&s, same[1]), 1);
    expect_ll("надгробие занято, а не добавлен слот", (long long)s.dead, 0);
    expect_ll("ключей снова три", (long long)s.live, 3);

    /* 4. Таблица заполняется целиком. */
    set_init(&s);
    long long added = 0;
    for (int i = 0; i < SET_CAP; i++) {
        snprintf(key, sizeof key, "full%d", i);
        added += set_add(&s, key) ? 1 : 0;
    }
    expect_ll("шестнадцать ключей поместились", added, 16);
    expect_ll("свободных слотов не осталось", (long long)s.live, SET_CAP);
    expect_ll("семнадцатый не поместился", set_add(&s, "extra"), 0);
    expect_ll("поиск отсутствующего в полной таблице завершился",
              set_contains(&s, "extra"), 0);

    long long all_found = 0;
    for (int i = 0; i < SET_CAP; i++) {
        snprintf(key, sizeof key, "full%d", i);
        all_found += set_contains(&s, key) ? 1 : 0;
    }
    expect_ll("все шестнадцать находятся", all_found, 16);

    expect_ll("освободили один слот", set_erase(&s, "full7"), 1);
    expect_ll("и новый ключ поместился", set_add(&s, "extra"), 1);

    /* 5. Таблица из одних надгробий. */
    set_init(&s);
    for (int i = 0; i < SET_CAP; i++) {
        snprintf(key, sizeof key, "full%d", i);
        set_add(&s, key);
    }
    for (int i = 0; i < SET_CAP; i++) {
        snprintf(key, sizeof key, "full%d", i);
        set_erase(&s, key);
    }
    expect_ll("надгробий стало шестнадцать", (long long)s.dead, SET_CAP);
    expect_ll("поиск в таблице без пустых слотов завершился",
              set_contains(&s, "nobody"), 0);
    expect_ll("вставка в такую таблицу работает", set_add(&s, "nobody"), 1);

    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

/* ── разбор команд ──────────────────────────────────────────────────────── */

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    /* Таблица обнулена до set_init, чтобы скелет можно было запустить, не
       написав ни одной функции. Ваш set_init всё равно обязан её заполнить:
       в --check он получает замусоренную структуру. */
    struct Set s;
    memset(&s, 0, sizeof s);
    set_init(&s);

    char cmd;
    char key[WORD_MAX];
    while (scanf(" %c", &cmd) == 1) {
        switch (cmd) {
        case '+':
            if (scanf("%31s", key) != 1)
                return 0;
            printf("+ %s\n", key);
            if (set_add(&s, key))
                printf("  ключей %zu\n", s.live);
            else
                printf("  места нет, ключей %zu\n", s.live);
            break;
        case '?':
            if (scanf("%31s", key) != 1)
                return 0;
            printf("? %s\n", key);
            printf("  %s\n", set_contains(&s, key) ? "есть" : "нет");
            break;
        case '-':
            if (scanf("%31s", key) != 1)
                return 0;
            printf("- %s\n", key);
            if (set_erase(&s, key))
                printf("  удалён, ключей %zu\n", s.live);
            else
                printf("  ключа нет, ключей %zu\n", s.live);
            break;
        case '#':
            printf("#\n");
            print_map(&s);
            break;
        case '=':
            printf("=\n");
            print_stats(&s);
            break;
        default:
            printf("неизвестная команда: %c\n", cmd);
            break;
        }
    }
    return 0;
}
