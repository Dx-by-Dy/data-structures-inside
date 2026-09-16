/* Демо 11. Присваивание структуры копирует её целиком.
 *
 *   gcc -std=c11 -Wall -Wextra 11-struct-copy.c -o 11-struct-copy
 *   ./11-struct-copy
 *
 * До запуска: вчера b = a для массивов не компилировалось вовсе. Здесь такое
 * присваивание есть, и оно законно. Что окажется в b после изменения a?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

struct Point {
    int x;
    int y;
    int trail[4];   /* массив внутри структуры */
};

static void shift_by_value(struct Point p)
{
    p.x += 100;
    p.trail[0] = 100;
}

static void shift_by_pointer(struct Point *p)
{
    p->x += 100;
    p->trail[0] = 100;
}

static void show(const char *label, const struct Point *p)
{
    printf("%s: x = %3d, y = %3d, trail = %d %d %d %d\n",
           label, p->x, p->y, p->trail[0], p->trail[1], p->trail[2], p->trail[3]);
}

int main(void)
{
    struct Point a = { 1, 2, { 10, 20, 30, 40 } };
    struct Point b = a;              /* смотреть сюда */

    printf("sizeof a = %zu, адреса: &a = %p, &b = %p\n\n",
           sizeof a, (void *)&a, (void *)&b);

    show("a", &a);
    show("b", &b);

    a.x = 777;
    a.trail[0] = 777;
    printf("\nпосле изменения a:\n");
    show("a", &a);
    show("b", &b);

    printf("\nпередача в функцию по значению:\n");
    shift_by_value(a);
    show("a", &a);

    printf("\nпередача по указателю:\n");
    shift_by_pointer(&a);
    show("a", &a);

    return 0;
}
