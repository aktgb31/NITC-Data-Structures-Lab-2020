#include <stdio.h>
#include <stdlib.h>

//Binary tree node and function to create empty binary tree
struct tnode
{
    int key;
    struct tnode* left, * right, * p;
};

struct tnode* createtnode(int item)
{
    struct tnode* temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->key = item;
    temp->left = temp->right = temp->p = NULL;
    return temp;
}
//--------------------------------------------------------------------------------

//Queue of tree nodes for level order traversal
struct qnode
{
    struct tnode* data;
    struct qnode* next;
};
struct queue
{
    struct qnode* tail, * head;
    int size;
};
struct qnode* create_qnode(struct tnode* k)
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
void enqueue(struct queue* q, struct tnode* k)
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
    q->size++;
}
void dequeue(struct queue* q)
{
    struct qnode* temp = q->head;
    q->head = q->head->next;
    if (q->head == NULL)
        q->tail = NULL;
    free(temp);
    q->size--;
}

struct tnode* queue_top(struct queue* q)
{
    return q->head->data;
}

//----------------------------------------------------------------

// Function to make balances bst from sorted array
struct tnode* make_bst(int* input, int i, int j)
{
    if (i > j)
        return NULL;

    int mid = ((i + j) / 2);
    struct tnode* root = createtnode(input[mid]);
    root->left = make_bst(input, i, mid - 1);
    root->right = make_bst(input, mid + 1, j);

    return root;
}

void PRINT(struct tnode* root)
{

    if (root != NULL)
    {
        printf("( ");
        printf("%d", root->key);
        printf(" ");
        PRINT(root->left);
        PRINT(root->right);
        printf(") ");
    }
    else
        printf("( ) ");
}

void PRINTLEVELSUM(struct tnode* root)
{
    struct queue q = { NULL, NULL, 0 };
    int level_size = 0;
    int sum = 0;
    enqueue(&q, root);
    while (!queue_empty(&q))
    {
        level_size = q.size;
        sum = 0;
        while (level_size)
        {
            struct tnode* p = queue_top(&q);
            sum += p->key;
            dequeue(&q);
            if (p->left)
                enqueue(&q, p->left);
            if (p->right)
                enqueue(&q, p->right);
            level_size--;
        }
        printf("%d ", sum);
    }
    printf("\n");
}
//----------------------------------------------------------------------

int main()
{
    struct tnode* root;
    root = NULL;

    int n;
    scanf("%d", &n);

    int* input = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        scanf("%d", input + i);
    }

    root = make_bst(input, 0, n - 1);

    PRINT(root);
    printf("\n");
    PRINTLEVELSUM(root);
    return 0;
}