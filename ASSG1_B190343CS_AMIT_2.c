#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
//-----------------------------------------------------------------------------

//Stack to store characters
struct snode
{
    char data;
    struct snode *next;
};
struct stack
{
    struct snode *top;
};
struct snode *createsnode(char k)
{
    struct snode *temp = (struct snode *)malloc(sizeof(struct snode));
    temp->data = k;
    temp->next = NULL;
    return temp;
}
char stack_top(struct stack *s)
{
    return s->top->data;
}
int stack_empty(struct stack *s)
{
    return (s->top == NULL);
}
void push(struct stack *s, char k)
{
    struct snode *temp = createsnode(k);
    temp->next = s->top;
    s->top = temp;
}
void pop(struct stack *s)
{
    struct snode *temp = s->top;
    s->top = s->top->next;

    free(temp);
}
//------------------------------------------------------------------------

//Queue of tree nodes for level order traversal
struct qnode
{
    struct tnode *data;
    struct qnode *next;
};
struct queue
{
    struct qnode *tail, *head;
    int size;
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
    q->size++;
}
void dequeue(struct queue *q)
{
    struct qnode *temp = q->head;
    q->head = q->head->next;
    if (q->head == NULL)
        q->tail = NULL;
    free(temp);
    q->size--;
}

struct tnode *queue_top(struct queue *q)
{
    return q->head->data;
}
//---------------------------------------------------------------------------------------

// Helper function for make tree
int findpartition(char *input, int start, int end)
{
    if (start > end)
        return -1;
    struct stack s;
    s.top = NULL;

    for (int i = start; i <= end; i++)
    {
        if (input[i] == '(')
            push(&s, input[i]);

        else if (input[i] == ')')
        {
            if (stack_top(&s) == '(')
            {
                pop(&s);
                if (stack_empty(&s))
                    return i;
            }
        }
    }
    return -1;
}

// function to construct tree
struct tnode *make_tree(char *input, int start, int end)
{
    if (start > end)
        return NULL;
    int num = 0;
    int negative = 0;

    //Check if the number is negative
    if (input[start] == '-')
    {
        negative = 1;
        start++;
    }
    for (; isdigit(input[start]) && start <= end; start++)
    {
        num = (num)*10 + (input[start] - '0');
    }
    if (negative)
        num *= -1;

    struct tnode *root;
    root = createtnode(num);

    int partition = -1;
    if (start + 1 <= end && input[start] == '(')
        partition = findpartition(input, start, end);

    if (partition != -1)
    {
        root->left = make_tree(input, start + 1, partition - 1);
        if (root->left != NULL)
        {
            root->left->p = root;
        }
        root->right = make_tree(input, partition + 2, end - 1);
        if (root->right != NULL)
        {
            root->right->p = root;
        }
    }
    return root;
}

//Helper function to erase space
void erase_space(char *input)
{
    int i = 0, j = 0;
    while (input[j] != '\0')
    {
        if (input[j] != ' ')
        {
            input[i] = input[j];
            i++;
        }
        j++;
    }
    input[i] = '\0';
}

//Searching the Binary tree
struct tnode *findtnode(struct tnode *root, int k)
{
    if (root != NULL)
    {
        if (root->key == k)
            return root;
        struct tnode *left = findtnode(root->left, k);
        struct tnode *right = findtnode(root->right, k);
        if (left != NULL)
            return left;
        else if (right != NULL)
            return right;
        else
            return NULL;
    }
    return NULL;
}

//Finding cousins
void findcousins(struct tnode *root, struct tnode *temp)
{
    if (temp == NULL || root == NULL || root == temp)
        printf("-1\n");
    else
    {
        struct queue q = {NULL, NULL, 0};
        int cousin_size = 0;
        int found = 0;

        enqueue(&q, root);
        while (!queue_empty(&q) && !found)
        {
            cousin_size = q.size;
            while (cousin_size)
            {
                struct tnode *p = queue_top(&q);
                dequeue(&q);
                if (p->left == temp || p->right == temp)
                    found = 1;
                else
                {
                    if (p->left)
                        enqueue(&q, p->left);

                    if (p->right)
                        enqueue(&q, p->right);
                }
                cousin_size--;
            }
        }
        if (queue_empty(&q))
            printf("-1");
        while (!queue_empty(&q))
        {
            printf("%d ", queue_top(&q)->key);
            dequeue(&q);
        }
        printf("\n");
    }
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output2.txt", "w", stdout);

    struct tnode *T;
    T = NULL;
    char input[10000];
    scanf("%[^\n]%*c", input);
    erase_space(input);
    if (strlen(input) >= 3)
        T = make_tree(input, 1, strlen(input) - 2);

    int k;
    scanf("%d", &k);
    findcousins(T, findtnode(T, k));
    return 0;
}