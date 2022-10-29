#include <stdio.h>

typedef struct node
{
    int val;
    struct node *next;
} node;

/* FIXME: this function is buggy. */
int ll_equal(const node *a, const node *b)
{
    while (a != NULL)
    {
        if (a->val != b->val)
            // Program received signal SIGSEGV, Segmentation fault.
            // 0x0000555555555185 in ll_equal (a=0x7fffffffdc70, b=0x0) at ll_equal.c:14
            // 14              if (a->val != b->val)
            // (gdb) info args
            // a = 0x7fffffffdc70
            // b = 0x0
            // FIX if (b && a->val != b->val)
            return 0;
        a = a->next;
        b = b->next;
    }
    /* lists are equal if a and b are both null */
    return a == b;
}

int main(int argc, char **argv)
{
    int i;
    node nodes[10];

    for (i = 0; i < 10; i++)
    {
        nodes[i].val = 0;
        nodes[i].next = NULL;
    }

    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[3];

    printf("equal test 1 result = %d\n", ll_equal(&nodes[0], &nodes[0]));
    printf("equal test 2 result = %d\n", ll_equal(&nodes[0], &nodes[2]));

    return 0;
}
