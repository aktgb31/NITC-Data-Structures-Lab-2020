#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int inf = 1e8;
struct node { int vertex; int weight; struct node* next; };
struct edge { int weight;int u;int v; };
struct Graph {
    int numVertices;
    int numEdges;
    struct node** list;
};

//--------------------------------------------------------------------------------
struct node* createNode(int vertex)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->vertex = vertex;
    temp->weight = 0;
    temp->next = NULL;
    return temp;
}
struct edge* createEdge(int w, int u, int v)
{
    struct edge* temp = (struct edge*)malloc(sizeof(struct edge));
    temp->u = u;
    temp->weight = w;
    temp->v = v;
    return temp;
}
void push(struct node** L, struct node* temp)
{
    if (*L == NULL)
    {
        *L = temp;
    }
    else
    {
        struct node* t = *L;
        while (t->next != NULL)
        {
            t = t->next;
        }
        t->next = temp;
    }
}
//---------------------------------------------------------------------------------
int primMST(struct Graph* adj)
{
    int ans = 0;
    int* vis = (int*)malloc(adj->numVertices * sizeof(int));
    int* dist = (int*)malloc(adj->numVertices * sizeof(int));

    for (int i = 0; i < adj->numVertices; i++)
        dist[i] = inf, vis[i] = 0;

    dist[0] = 0;

    for (int i = 0;i < adj->numVertices;i++)
    {
        int idx = -1, mx_dist = inf;
        //-------------------------------------------------
        for (int j = 0;j < adj->numVertices;j++)
        {
            if (!vis[j] && mx_dist > dist[j])
                idx = j, mx_dist = dist[j];
        }
        //-------------------------------------------------
        vis[idx] = 1;
        ans += dist[idx];
        struct node* temp = adj->list[idx];
        while (temp != NULL)
        {
            if (!vis[temp->vertex] && temp->weight < dist[temp->vertex])
                dist[temp->vertex] = temp->weight;
            temp = temp->next;
        }
    }

    return ans;
}
//----------------------------------------------------------------------------

//Quick Sort to sort edges according to weights
void partition(struct edge** A, int p, int* q, int r)
{
    *q = p - 1;
    struct edge* x = A[r];
    for (int j = p; j <= r - 1; j++)
    {
        if (A[j]->weight <= x->weight)
        {
            (*q)++;
            struct edge* temp = A[*q];
            A[*q] = A[j];
            A[j] = temp;
        }
    }
    (*q)++;
    struct edge* temp = A[*q];
    A[*q] = A[r];
    A[r] = temp;
}
void quick_sort(struct edge** A, int p, int r)
{
    if (p < r)
    {
        int q;
        partition(A, p, &q, r);
        quick_sort(A, p, q - 1);
        quick_sort(A, q + 1, r);
    }
}

struct disjointSet
{
    int* set;
    int* rank;
};

int compressedFind(struct disjointSet* d, int x)
{

    if (x != d->set[x])
    {
        d->set[x] = compressedFind(d, d->set[x]);
    }
    return d->set[x];
}
void rankUnion(struct disjointSet* d, int x, int y)
{
    int l = compressedFind(d, x);
    int r = compressedFind(d, y);
    if (d->rank[l] >= d->rank[r])
    {
        d->set[r] = l;
        if (d->rank[l] == d->rank[r])
            d->rank[l]++;
    }
    else
    {
        d->set[l] = r;
    }

}

void initialize(struct disjointSet* d, int size)
{

    d->set = (int*)malloc(size * sizeof(int));
    d->rank = (int*)malloc(size * sizeof(int));
    for (int i = 0;i < size;i++)
    {
        d->set[i] = i;
        d->rank[i] = 1;
    }
}
//----------------------------------------------------------------------------
int kruskalMST(struct Graph* adj)
{
    struct edge** edges = (struct edge**)malloc(sizeof(struct edge*) * adj->numEdges * 2);
    int j = 0;
    for (int i = 0;i < adj->numVertices;i++)
    {
        struct node* temp = adj->list[i];
        while (temp != NULL)
        {
            edges[j] = createEdge(temp->weight, i, temp->vertex);
            ++j;
            temp = temp->next;
        }
    }

    quick_sort(edges, 0, adj->numEdges * 2 - 1);
    struct disjointSet d;
    initialize(&d, adj->numVertices);

    int ans = 0;
    for (j = 0;j < 2 * adj->numEdges;j++)
    {
        if (compressedFind(&d, edges[j]->u) != compressedFind(&d, edges[j]->v))
        {
            rankUnion(&d, edges[j]->u, edges[j]->v);
            ans += edges[j]->weight;
        }
    }


    return ans;
}


//Awesome Function to extract numbers sequentially from a string
int extractInt(char* s)
{
    static int start = 0;
    if (s[start] == '\0')//because using fgets
    {
        start = 0;
        return -1;
    }
    int num = 0;
    int neg = 1;
    if (s[start] == '-')
        neg = -1, ++start;

    while (s[start] != ' ' && s[start] != '\n')//beacuse using fgets
    {
        num = num * 10 + (s[start] - '0');
        ++start;
    }
    ++start;
    num *= neg;
    //printf("%d: ", start);
    return num;
}

//------------------------------------------------------------------

int main()
{
    //freopen("input.txt", "r", stdin);
    char choice;
    scanf("%c\n", &choice);
    struct Graph adj;
    int n;
    scanf("%d\n", &n);
    adj.numVertices = n;
    adj.numEdges = 0;
    adj.list = (struct node**)malloc(n * sizeof(struct node*));
    for (int i = 0;i < n;i++)
    {
        int x;
        scanf("%d ", &x);
        char waste[100];
        int u;
        adj.list[x] = NULL;
        fgets(waste, 100, stdin);

        u = extractInt(waste);
        while (u != -1)
        {
            adj.numEdges++;
            push(&adj.list[x], createNode(u));
            u = extractInt(waste);
        }
    }
    adj.numEdges /= 2;// Edge= Degree/2
    for (int i = 0;i < n;i++)
    {
        int x;
        scanf("%d ", &x);
        char waste[100];
        int u;
        fgets(waste, 100, stdin);
        struct node* temp = adj.list[x];
        u = extractInt(waste);
        while (u != -1)
        {
            temp->weight = u;
            temp = temp->next;
            u = extractInt(waste);
        }
    }

    if (choice == 'a')
        printf("%d\n", primMST(&adj));
    else
        printf("%d\n", kruskalMST(&adj));
    return 0;
}