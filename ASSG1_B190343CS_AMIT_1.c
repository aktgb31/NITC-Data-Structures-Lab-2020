#include <stdio.h>
#include <stdlib.h>

//Binary tree node and function to create empty binary tree
struct tnode
{
    int key;
    struct tnode *left, *right, *p;
};
struct Binary_Tree
{
    struct tnode *root;
};
struct tnode *createtnode(int item)
{
    struct tnode *temp = (struct tnode *)malloc(sizeof(struct tnode));
    temp->key = item;
    temp->left = temp->right = temp->p = NULL;
    return temp;
}

//Queue of tree nodes for level order traversal
struct qnode
{
    struct tnode *data;
    struct qnode *next;
};
struct queue
{
    struct qnode *tail, *head;
};
struct qnode *create_qnode(struct tnode *k)
{
    struct qnode *temp = (struct qnode *)malloc(sizeof(struct qnode));
    temp->data = k;
    temp->next = NULL;
    return temp;
}
int queue_empty(struct queue *q)
{
    return (q->head == NULL);
}
void enqueue(struct queue *q, struct tnode *k)
{
    struct qnode *temp = create_qnode(k);
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
void dequeue(struct queue *q)
{
    struct qnode *temp = q->head;
    q->head = q->head->next;
    if (q->head == NULL)
        q->tail = NULL;
    free(temp);
}

struct tnode *queue_top(struct queue *q)
{
    return q->head->data;
}
//------------------------------------------------------------
//Binary tree related functions as per given problem

void INSERT(struct Binary_Tree *T, int key)
{
    static struct queue q = {NULL, NULL};
    if (T->root == NULL)
    {
        struct tnode *temp = createtnode(key);
        temp->p = T->root;
        T->root = temp;
        return;
    }
    if (queue_empty(&q))
        enqueue(&q, T->root);

    //Level order traversal
    while (!queue_empty(&q))
    {
        struct tnode *temp = queue_top(&q);
        if (temp->left && temp->right)
        {
            enqueue(&q, temp->left);
            enqueue(&q, temp->right);
            dequeue(&q);
        }
        else if (temp->left == NULL)
        {
            temp->left = createtnode(key);
            temp->left->p = temp;
            return;
        }
        else
        {
            temp->right = createtnode(key);
            temp->right->p = temp;
            return;
        }
    }
}

void PRINT(struct tnode *root)
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

//Main function
int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    struct Binary_Tree T;
    T.root = NULL;
    char ch;
    while (scanf("%c", &ch) && ch != 'e')
    {
        switch (ch)
        {
        case 'i':
        {
            int k;
            scanf("%d", &k);
            INSERT(&T, k);
        }
        break;
        case 'p':
        {
            PRINT(T.root);
            printf("\n");
        }
        break;
        default:
            break;
        }
    }
    return 0;
}