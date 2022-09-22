//Negative Cycle Detection in a directed graph
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int inf = 1e9;
struct edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int vertexCount;
    int edgeCount;
    struct edge* edges;
};

int isNegativeCycle(struct Graph* g)
{
    int* dist = (int*)malloc(sizeof(int) * g->vertexCount);
    for (int i = 0;i < g->vertexCount;i++)
        dist[i] = inf;

    dist[0] = 0;

    for (int i = 0;i < g->vertexCount - 1;i++)
    {
        for (int j = 0;j < g->edgeCount;j++)
        {
            if (dist[g->edges[j].src] != inf && dist[g->edges[j].src] + g->edges[j].weight < dist[g->edges[j].dest])
                dist[g->edges[j].dest] = dist[g->edges[j].src] + g->edges[j].weight;
        }
    }

    for (int j = 0;j < g->edgeCount;j++)
    {
        if (dist[g->edges[j].src] != inf && dist[g->edges[j].src] + g->edges[j].weight < dist[g->edges[j].dest])
            return 1;
    }
    return -1;
}
int main()
{
    freopen("input.txt", "r", stdin);
    int n, m;
    struct Graph g;
    scanf("%d%d", &n, &m);
    g.vertexCount = n;
    g.edgeCount = m;
    g.edges = (struct edge*)malloc(m * sizeof(struct edge));
    for (int i = 0;i < m;i++)
    {
        scanf("%d%d%d", &g.edges[i].src, &g.edges[i].dest, &g.edges[i].weight);
    }

    printf("%d\n", isNegativeCycle(&g));
    free(g.edges);
    return 0;
}
