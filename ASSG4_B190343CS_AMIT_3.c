#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int inf = 1e8;
struct node { int vertex; int weight; struct node* next; };

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
int* disjktsra(struct Graph* adj, int src)
{
    int* vis = (int*)malloc(adj->numVertices * sizeof(int));
    int* dist = (int*)malloc(adj->numVertices * sizeof(int));

    for (int i = 0; i < adj->numVertices; i++)
        dist[i] = inf, vis[i] = 0;

    // struct node* temp = adj->list[src];
    // while (temp != NULL)
    // {
    //     dist[temp->vertex] = temp->weight;
    //     temp = temp->next;
    // }
    //temp = NULL;

    dist[src] = 0;

    for (int i = 0;i < adj->numVertices;i++)
    {
        int idx = -1, mn_dist = inf;
        //-------------------------------------------------
        for (int j = 0;j < adj->numVertices;j++)
        {
            if (!vis[j] && mn_dist > dist[j])
                idx = j, mn_dist = dist[j];
        }
        //-------------------------------------------------
        if (idx == -1)
            break;
        vis[idx] = 1;
        struct node* temp = adj->list[idx];
        while (temp != NULL)
        {
            if (!vis[temp->vertex] && temp->weight != inf)
            {
                if (temp->weight + mn_dist < dist[temp->vertex])
                    dist[temp->vertex] = temp->weight + mn_dist;
            }
            temp = temp->next;
        }
    }
    free(vis);
    return dist;
}
//----------------------------------------------------------------------------


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

void printGraphList(struct Graph* adj)
{
    for (int i = 0;i < adj->numVertices;i++)
    {
        struct node* temp = adj->list[i];
        printf("%p ", temp);
        printf("%d ", i);
        while (temp != NULL)
        {
            printf("-> { %d,%d }", temp->vertex, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

int main()
{
    freopen("input.txt", "r", stdin);

    struct Graph adj;
    int n;
    scanf("%d\n", &n);
    adj.numVertices = n;
    adj.numEdges = 0;
    adj.list = (struct node**)malloc(n * sizeof(struct node*));
    for (int i = 0;i < n;i++)
    {
        char waste[100];
        int u;
        fgets(waste, 100, stdin);
        int x = extractInt(waste);;
        adj.list[x] = NULL;
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
        char waste[100];
        int u;
        fgets(waste, 100, stdin);
        int x = extractInt(waste);
        struct node* temp = adj.list[x];
        u = extractInt(waste);
        while (u != -1)
        {
            temp->weight = u;
            temp = temp->next;
            u = extractInt(waste);
        }
    }
    //  printGraphList(&adj);
    char choice[4];
    int* dist = NULL;
    while (scanf("%s", choice) && strcmp(choice, "stop") != 0)
    {
        if (strcmp(choice, "apsp") == 0)
        {
            int src;
            scanf("%d", &src);
            dist = disjktsra(&adj, src);
            for (int i = 0;i < adj.numVertices;i++)
                if (dist[i] != inf)
                    printf("%d ", dist[i]);
                else
                    printf("INF ");
            printf("\n");
            free(dist);
        }
        if (strcmp(choice, "sssp") == 0)
        {
            int src, dest;
            scanf("%d%d", &src, &dest);
            dist = disjktsra(&adj, src);
            if (dist[dest] != inf)
                printf("%d\n", dist[dest]);
            else
                printf("UNREACHABLE\n");
            free(dist);
        }
    }
    return 0;
}