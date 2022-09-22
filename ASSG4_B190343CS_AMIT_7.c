//Disjoint Sets
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int max = 10001;
struct disjointSet
{
    int* set;
    int* rank;
    int findCount;
    int (*MakeSet)(struct disjointSet* d, int x);
    int (*Find)(struct disjointSet* d, int x);
    int (*Union)(struct disjointSet* d, int x, int y);
};

int makeSet(struct disjointSet* d, int x)
{
    if (d->set[x] != -1)
        return -1;
    else
    {
        d->set[x] = x;
        if (d->rank != NULL)  d->rank[x] = 1;
        return x;
    }
}

int simpleFind(struct disjointSet* d, int x)
{
    if (d->set[x] == -1)
        return -1;
    d->findCount++;
    if (x != d->set[x])
    {
        return simpleFind(d, d->set[x]);
    }
    return x;

}

int compressedFind(struct disjointSet* d, int x)
{
    if (d->set[x] == -1)
        return -1;
    d->findCount++;
    if (x != d->set[x])
    {
        d->set[x] = compressedFind(d, d->set[x]);

    }
    return d->set[x];
}

int simpleUnion(struct disjointSet* d, int x, int y)
{
    int l = d->Find(d, x);
    int r = d->Find(d, y);
    if (l == -1 || r == -1)
        return -1;
    if (l == r)
        return l;
    d->set[r] = l;
    return l;
}

int rankUnion(struct disjointSet* d, int x, int y)
{
    int l = d->Find(d, x);
    int r = d->Find(d, y);
    if (l == -1 || r == -1)
        return -1;
    if (l == r)
        return l;

    if (d->rank[l] >= d->rank[r])
    {
        d->set[r] = l;
        if (d->rank[l] == d->rank[r])
            d->rank[l]++;
        return l;
    }
    else
    {
        d->set[l] = r;
        return r;
    }

}

void initialize(struct disjointSet* d, int x)
{
    d->findCount = 0;
    d->set = (int*)malloc(max * sizeof(int));
    if (x == 1 || x == 3)
        d->rank = NULL;
    else
        d->rank = (int*)malloc(max * sizeof(int));
    for (int i = 0;i < max;i++)
    {
        d->set[i] = -1;
        if (d->rank != NULL) d->rank[i] = 0;
    }
    d->MakeSet = makeSet;
    if (x == 1 || x == 2)
        d->Find = simpleFind;
    else
        d->Find = compressedFind;
    if (x == 1 || x == 3)
        d->Union = simpleUnion;
    else
        d->Union = rankUnion;

}
int main()
{
    //freopen("input.txt", "r", stdin);
    struct disjointSet dsu[4];
    initialize(&dsu[0], 1);// no rank union no path compression
    initialize(&dsu[1], 2);// only ranked union
    initialize(&dsu[2], 3);// only path compression
    initialize(&dsu[3], 4);// both rank union and path compression

    char ch;
    while (scanf("%c", &ch) && ch != 's')
    {
        switch (ch)
        {
        case 'm':
        {int x;
        scanf("%d", &x);
        int ans = dsu[0].MakeSet(&dsu[0], x);
        ans = dsu[1].MakeSet(&dsu[1], x);
        ans = dsu[2].MakeSet(&dsu[2], x);
        ans = dsu[3].MakeSet(&dsu[3], x);
        printf("%d\n", ans);
        //printf("%d %d %d %d\n", dsu[0].MakeSet(&dsu[0], x), dsu[1].MakeSet(&dsu[1], x), dsu[2].MakeSet(&dsu[2], x), dsu[3].MakeSet(&dsu[3], x));
        }
        break;
        case 'u':
        {  int x, y;
        scanf("%d%d", &x, &y);
        printf("%d %d %d %d\n", dsu[0].Union(&dsu[0], x, y), dsu[1].Union(&dsu[1], x, y), dsu[2].Union(&dsu[2], x, y), dsu[3].Union(&dsu[3], x, y));
        //printf("count: %d %d %d %d\n", dsu[0].findCount, dsu[1].findCount, dsu[2].findCount, dsu[3].findCount);
        }
        break;
        case 'f':
        {  int x;
        scanf("%d", &x);
        printf("%d %d %d %d\n", dsu[0].Find(&dsu[0], x), dsu[1].Find(&dsu[1], x), dsu[2].Find(&dsu[2], x), dsu[3].Find(&dsu[3], x));
        // printf("count : % d % d % d % d\n", dsu[0].findCount, dsu[1].findCount, dsu[2].findCount, dsu[3].findCount);
        }
        break;
        default:
            break;
        }
    }

    printf("%d %d %d %d\n", dsu[0].findCount, dsu[1].findCount, dsu[2].findCount, dsu[3].findCount);
    return 0;
}
