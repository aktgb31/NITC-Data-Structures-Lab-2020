#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Graph {
    int size;
    int** adj;
};

//==================================================================
// Queue for BFS
struct qnode
{
    int data;
    struct qnode* next;
};
struct queue
{
    struct qnode* tail, * head;
};
struct qnode* create_qnode(int k)
{
    struct qnode* temp = (struct qnode*)malloc(sizeof(struct qnode));
    temp->data = k;
    temp->next = NULL;
    return temp;
}
int queue_empty(struct queue* q)
{
    return (q->head == NULL);
}
void enqueue(struct queue* q, int k)
{
    struct qnode* temp = create_qnode(k);
    if (q->head == NULL)
    {
        q->head = q->tail = temp;
    }
    else
    {
        q->tail->next = temp;
        q->tail = temp;
    }
}
void dequeue(struct queue* q)
{
    struct qnode* temp = q->head;
    q->head = q->head->next;
    temp->next = NULL;
    if (q->head == NULL)
        q->tail = NULL;
    free(temp);

}
int queue_top(struct queue* q)
{
    return q->head->data;
}
//==================================================================

void bfs(struct Graph* g, int i)
{
    int vis[g->size];
    memset(vis, 0, sizeof(vis));

    struct queue q;
    q.head = NULL;
    q.tail = NULL;
    vis[i] = 1;
    enqueue(&q, i);
    while (!queue_empty(&q))
    {
        int u = queue_top(&q);
        dequeue(&q);
        printf("%d ", u);

        for (int v = 0;v < g->size;v++)
        {
            if (g->adj[u][v] && !vis[v])
            {
                enqueue(&q, v);
                vis[v] = 1;
            }
        }
    }
}

void dfs(struct Graph* g, int u, int* vis)
{
    vis[u] = 1;
    printf("%d ", u);
    for (int v = 0;v < g->size;v++)
    {
        if (g->adj[u][v] && !vis[v])
        {
            dfs(g, v, vis);
        }
    }
}

void dfsUtil(struct Graph* g, int i)
{
    int vis[g->size];
    memset(vis, 0, sizeof(vis));
    dfs(g, i, vis);
}
int main()
{
    //freopen("input.txt", "r", stdin);
    int n, m;
    scanf("%d%d", &n, &m);
    struct Graph g;
    g.size = n;
    g.adj = (int**)malloc(n * sizeof(int*));
    for (int i = 0;i < n;i++)
        g.adj[i] = (int*)calloc(n, sizeof(int));

    for (int i = 0;i < m;i++)
    {
        int u, v;
        scanf("%d%d", &u, &v);
        g.adj[u][v] = 1;
        g.adj[v][u] = 1;
    }
    int source;
    scanf("%d", &source);
    bfs(&g, source);
    printf("\n");
    dfsUtil(&g, source);
    printf("\n");
    return 0;
}