#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXELT 301

typedef struct elt
{
    int v;
} elt;

typedef struct node
{
    elt *value;
    struct node *next;
    struct node *prev;
} node;

node *head = NULL;
node *tail = NULL;
node *temphead;
node *templast;
node *newnode;

typedef struct partition
{
    node *addpart;
    struct partition *nextpart;
    struct partition *prevpart;
} partition;

int lenpart = 0;
int cntpart = 0;
partition *tempheadpart;
partition *newnodepart;

typedef struct Set
{
    partition *headpart;
    int size;
} Set;

typedef struct SearchResult
{
    elt *val;
    int found;
} SearchResult;

//------------  Set METHODS --------------

Set *buildSet(elt *e[], int n); // done
int len(Set *s);                // done
int isEmpty(Set *s);            // done

void put(elt *x, Set *s); // done
void rmv(elt *x, Set *s); // done

SearchResult search(elt *x, Set *s); // done
SearchResult next(elt *x, Set *s);   // done
SearchResult prev(elt *x, Set *s);   // done
SearchResult last(Set *s);
SearchResult first(Set *s);

// ---------------- INTERNAL METHODS -----------------

int cmp(elt *e1, elt *e2);
Set *BuildEmptySet();

//======== INTERNAL METHOD IMPLEMENTATION ==========

int cmp(elt *e1, elt *e2)
{
    return (e1->v) - (e2->v);
}

Set *BuildEmptySet()
{
    Set *s = (Set *)malloc(sizeof(Set));
    s->headpart = NULL;
    s->size = 0;
    return s;
}

// ---------------- Set Implmentation ------------------

int len(Set *s)
{
    return s->size;
}

int isEmpty(Set *s)
{
    int k = 0;
    if (s->size == 0)
        k = 1;
    return k;
}

Set *buildSet(elt *e[], int n)
{
    Set *s = BuildEmptySet();
    int i = 0;
    while (i < n)
    {
        put(e[i], s);
        i = i + 1;
    }
    return s;
}
SearchResult search(elt *x, Set *s)
{
    assert(x != NULL && s != NULL);
    SearchResult r;
    r.found = 0;
    r.val = NULL;
    tempheadpart = s->headpart;

    while (tempheadpart != NULL && ((cmp(tempheadpart->addpart->value, x) <= 0) && (tempheadpart->nextpart != s->headpart)))
    {
        tempheadpart = tempheadpart->nextpart;
    }

    if (tempheadpart != NULL && cmp(tempheadpart->addpart->value, x) >= 0)
    {
        temphead = tempheadpart->addpart;

        while (temphead != NULL && cmp(temphead->value, x) != 0)
        {
            temphead = temphead->prev;
        }

        if (temphead != NULL && cmp(temphead->value, x) == 0)
        {
            r.found = 1;
            r.val = temphead->value;
        }
    }

    return r;
}

void put(elt *x, Set *s)
{
    assert(x != NULL && s != NULL);
    SearchResult r = search(x, s);
    if (r.found == 0)
    {
        newnode = (node *)malloc(sizeof(node));
        newnode->value = x;
        newnode->prev = NULL;
        newnode->next = NULL;

        if (len(s) == 0)
        {
            head = tail = newnode;

            newnodepart = (partition *)malloc(sizeof(partition));
            newnodepart->addpart = head;
            newnodepart->nextpart = NULL;
            newnodepart->prevpart = NULL;

            // Assign address of created node to address part of partition node.
            s->headpart = newnodepart;

            // Initialize tempheadpart to newnodepart.
            tempheadpart = newnodepart;

            cntpart = cntpart + 1;
        }

        else if (cmp(x, head->value) < 0)
        {
            newnode->next = head;
            head->prev = newnode;
            head = newnode;
        }

        else if (cmp(x, tail->value) > 0)
        {
            newnode->prev = tail;
            tail->next = newnode;
            tail = newnode;
        }

        else
        {
            temphead = head;
            while (cmp(newnode->value, temphead->value) > 0)
            {
                temphead = temphead->next;
            }
            newnode->next = temphead;
            newnode->prev = temphead->prev;

            // Update the next pointer of the previous node.
            temphead->prev->next = newnode;

            temphead->prev = newnode;
        }

        s->size += 1;
        lenpart += 1;
        if (lenpart == MAXELT)
        {
            partition *secnewnode;
            secnewnode = (partition *)malloc(sizeof(partition));

            secnewnode->addpart = tail;
            secnewnode->nextpart = s->headpart;
            secnewnode->prevpart = tempheadpart;
            tempheadpart->nextpart = secnewnode;
            s->headpart->prevpart = secnewnode;
            tempheadpart = secnewnode;

            lenpart = 1;
        }
    }
}

void rmv(elt *x, Set *s)
{
    assert(x != NULL && s != NULL);
    SearchResult r = search(x, s);
    if (r.found == 1)
    {
        temphead = head;
        tempheadpart = s->headpart;
        if (len(s) == 1)
        {
            head = NULL;
            s->headpart = NULL;
            free(temphead);
            free(tempheadpart);
        }

        else
        {
            while (temphead->value != x)
            {
                temphead = temphead->next;
            }
            if (temphead->value == x)
            {
                elt *temp;
                while (temphead->next != NULL)
                {
                    temp = temphead->value;
                    temphead->value = temphead->next->value;
                    temphead->next->value = temp;

                    temphead = temphead->next;
                }
                if (cmp(tail->value, x) == 0)
                {
                    while (tempheadpart->nextpart != s->headpart)
                    {
                        tempheadpart = tempheadpart->nextpart;
                    }

                    if (tail == tempheadpart->addpart)
                    {
                        tempheadpart->prevpart->nextpart = s->headpart;
                        free(tempheadpart);
                        templast = tail;
                        tail = tail->prev;
                        tail->next = NULL;
                        free(templast);
                    }
                    else
                    {
                        templast = tail;
                        tail = tail->prev;
                        tail->next = NULL;
                        free(templast);
                    }
                }
            }
        }
        s->size = s->size - 1;
    }
}

SearchResult next(elt *x, Set *s)
{
    assert(x != NULL && s != NULL);
    SearchResult r = search(x, s);
    if (r.found == 1)
    {
        r.found = 0;
        tempheadpart = s->headpart;
        if (cmp(tempheadpart->addpart->value, x) == 0)
        {
            r.found = 1;
            r.val = tempheadpart->addpart->next->value;
        }
        else
        {
            while (cmp(tempheadpart->addpart->value, x) < 0)
            {
                tempheadpart = tempheadpart->nextpart;
            }
            temphead = tempheadpart->addpart;
            while (cmp(temphead->value, x) != 0)
            {
                temphead = temphead->prev;
            }
            if (cmp(temphead->value, x) == 0 && temphead->next != NULL)
            {
                r.found = 1;
                r.val = temphead->next->value;
            }
        }
    }
    return r;
}
SearchResult prev(elt *x, Set *s)
{
    assert(x != NULL && s != NULL);
    SearchResult r = search(x, s);
    if (r.found == 1)
    {
        r.found = 0;
        tempheadpart = s->headpart;
        if (cmp(tail->value, x) == 0)
        {
            r.found = 1;
            r.val = tail->prev->value;
        }
        else
        {
            while (cmp(tempheadpart->addpart->value, x) < 0)
            {
                tempheadpart = tempheadpart->nextpart;
            }

            temphead = tempheadpart->addpart;
            while (cmp(temphead->value, x) != 0)
            {
                temphead = temphead->prev;
            }

            if (cmp(temphead->value, x) == 0 && temphead->prev != NULL)
            {
                r.found = 1;
                r.val = temphead->prev->value;
            }
        }
    }
    return r;
}

SearchResult last(Set *s)
{
    assert(s != NULL);
    SearchResult r;
    r.found = 0;
    if (len(s) > 0)
    {
        r.found = 1;
        r.val = tail->value;
    }
    return r;
}

SearchResult first(Set *s)
{
    assert(s != NULL);
    SearchResult r;
    r.found = 0;
    if (len(s) > 0)
    {
        r.found = 1;
        r.val = s->headpart->addpart->value;
    }
    return r;
}

void testBuildSet()
{
    elt *e1[10000];
    int n = 0;
    int i = -10000;
    elt *e2[10000];

    while (i < 9999)
    {
        elt *var1 = (elt *)malloc(sizeof(elt));
        elt *var2 = (elt *)malloc(sizeof(elt));
        assert(var1 != NULL && var2 != NULL);
        var1->v = i;
        var2->v = i + 1;
        e1[n] = var1;
        e2[n] = var2;
        n++;
        i = i + 2;
    }
    Set *s = buildSet(e1, n);

    i = 0;
    while (i < n)
    {
        SearchResult r1 = search(e1[i], s);
        assert(r1.found == 1);
        SearchResult r2 = search(e2[i], s);
        assert(r2.found == 0);
        i = i + 1;
    }
    i = 0;
    while (i < n)
    {
        free(e1[i]);
        free(e2[i]);
        i = i + 1;
    }
    free(s);
}

void testPut()
{
    elt *e1[10000];
    int n = 0;
    int i = -10000;

    elt *dummy[2];
    Set *s = buildSet(dummy, 0); // empty set
    assert(isEmpty(s) > 0);

    while (i < 9999)
    {
        elt *var = (elt *)malloc(sizeof(elt));
        assert(var != NULL);
        var->v = i;
        SearchResult before = search(var, s);
        put(var, s);
        SearchResult after = search(var, s);
        assert(before.found == 0);
        assert(after.found == 1);
        e1[n] = var;
        i = i + 2;
        n = n + 1;
    }
    i = 0;
    while (i < n)
    {
        free(e1[i]);
        i = i + 1;
    }
    free(s);
}

void testRmv()
{
    elt *e1[10000];
    int n = 0;
    int i = -10000;

    while (i < 9999)
    {
        elt *var1 = (elt *)malloc(sizeof(elt));
        assert(var1 != NULL);
        var1->v = i;
        e1[n] = var1;
        i = i + 2;
        n = n + 1;
    }
    Set *s = buildSet(e1, n);
    assert(len(s) == n);

    i = 0;
    while (i < n)
    {
        SearchResult before = search(e1[i], s);
        rmv(e1[i], s);
        SearchResult after = search(e1[i], s);
        assert(before.found == 1);
        assert(after.found == 0);
        i = i + 1;
    }
    assert(isEmpty(s) > 0);
    i = 0;
    while (i < n)
    {
        free(e1[i]);
        i = i + 1;
    }
    free(s);
}

void testPrev()
{
    elt *dummy[2];
    Set *s = buildSet(dummy, 0); // empty set
    assert(isEmpty(s) > 0);

    elt *e1[10000];
    int n = 0, i = 1, j = 8192;

    elt *temp = (elt *)malloc(sizeof(elt));
    e1[n] = temp;
    n++;
    elt *var1 = (elt *)malloc(sizeof(elt));
    var1->v = j;
    put(var1, s);
    e1[n] = var1;
    n++;
    SearchResult r = prev(var1, s);
    assert(r.found == 0);

    while (i < j)
    {
        elt *var2 = (elt *)malloc(sizeof(elt));
        var2->v = i;
        put(var2, s);
        e1[n] = var2;
        n++;
        r = prev(var1, s);
        assert(r.found == 1 && (r.val)->v == i);
        int k = j - 1;
        while (k > i)
        {
            temp->v = k;
            SearchResult t = search(temp, s);
            assert(t.found == 0);
            k--;
        }

        i = i << 1;
    }

    while (j > 1)
    {
        temp->v = j;
        r = prev(temp, s);
        assert(r.found == 1);
        assert((r.val)->v == (j >> 1));
        j = j >> 1;
    }
    i = 0;
    while (i < n)
    {
        free(e1[i]);
        i = i + 1;
    }
    free(s);
}

void testNext()
{
    elt *dummy[2];
    Set *s = buildSet(dummy, 0); // empty set
    assert(isEmpty(s) > 0);

    elt *e1[10000];
    int n = 0, i = 1, j = 8192;

    elt *temp = (elt *)malloc(sizeof(elt));
    e1[n] = temp;
    n++;
    // Put only one element
    elt *var1 = (elt *)malloc(sizeof(elt));
    var1->v = i;
    put(var1, s);
    e1[n] = var1;
    n++;
    SearchResult r = next(var1, s);
    assert(r.found == 0);

    while (i < j)
    {
        // put another element >i
        elt *var2 = (elt *)malloc(sizeof(elt));
        var2->v = j;
        put(var2, s);
        e1[n] = var2;
        n++;
        // next(i) should be j now
        r = next(var1, s);
        assert(r.found == 1 && cmp(r.val, var2) == 0);
        int k = i + 1;
        while (k < j)
        {
            temp->v = k;
            SearchResult t = search(temp, s);
            assert(t.found == 0);
            k++;
        }

        // new j will be smaller than earlier j
        // so next(i) now should become lastest j
        j = j >> 1;
    }

    while (i < 8192)
    {
        temp->v = i;
        r = next(temp, s);
        assert(r.found == 1);
        assert((r.val)->v == (i << 1));
        i = i << 1;
    }
    i = 0;
    while (i < n)
    {
        free(e1[i]);
        i = i + 1;
    }
    free(s);
}

void testFirst()
{
    elt *dummy[2];
    Set *s = buildSet(dummy, 0); // empty set
    assert(isEmpty(s) > 0);

    elt *e1[10000];
    int n = 0, i = 8192;

    elt *temp = (elt *)malloc(sizeof(elt));
    e1[n] = temp;
    n++;
    SearchResult r = first(s);
    assert(r.found == 0);

    while (i >= 1)
    {
        elt *var2 = (elt *)malloc(sizeof(elt));
        var2->v = i;
        put(var2, s);
        e1[n] = var2;
        n++;
        r = first(s);
        assert(r.found == 1 && (r.val)->v == i);
        int k = 1;
        while (k < i)
        {
            temp->v = k;
            SearchResult t = search(temp, s);
            assert(t.found == 0);
            k++;
        }
        i = i >> 1;
    }
    i = 0;
    while (i < n)
    {
        free(e1[i]);
        i = i + 1;
    }
    free(s);
}

void testLast()
{
    elt *dummy[2];
    Set *s = buildSet(dummy, 0); // empty set
    assert(isEmpty(s) > 0);

    elt *e1[10000];
    int n = 0, i = 1;

    elt *temp = (elt *)malloc(sizeof(elt));
    e1[n] = temp;
    n++;
    SearchResult r = last(s);
    assert(r.found == 0);

    while (i <= 8192)
    {
        elt *var2 = (elt *)malloc(sizeof(elt));
        var2->v = i;
        put(var2, s);
        e1[n] = var2;
        n++;
        r = last(s);
        assert(r.found == 1 && (r.val)->v == i);
        int k = 9999;
        while (k > i)
        {
            temp->v = k;
            SearchResult t = search(temp, s);
            assert(t.found == 0);
            k--;
        }
        i = i << 1;
    }
    i = 0;
    while (i < n)
    {
        free(e1[i]);
        i = i + 1;
    }
    free(s);
}

int main(void)
{
    testBuildSet();
    printf("test1 done\n");
    testPut();
    printf("test2 done\n");
    testRmv();
    printf("test3 done\n");
    testNext();
    printf("test4 done\n");
    testPrev();
    printf("test5 done\n");
    testFirst();
    printf("test6 done\n");
    testLast();
    printf("test7 done\n");
}
