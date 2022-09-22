#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const int inf = 1e8;
struct fibonacciNode
{
    int key;
    int mark;
    int degree;
    struct fibonacciNode* left;
    struct fibonacciNode* right;
    struct fibonacciNode* parent;
    struct fibonacciNode* child;
};

struct fibonacciHeap
{
    struct fibonacciNode* min;
    int size;
};

struct fibonacciHeap* makefibonacciHeap(struct fibonacciNode* size)
{
    struct fibonacciHeap* h = (struct fibonacciHeap*)malloc(sizeof(struct fibonacciHeap));
    h->min = NULL;
    h->size = 0;
    return h;
}

struct fibonacciNode* create_fibonacciNode(int key)
{
    struct fibonacciNode* temp = (struct fibonacciNode*)malloc(sizeof(struct fibonacciNode));
    temp->child = NULL;
    temp->left = temp;
    temp->right = temp;
    temp->parent = NULL;
    temp->key = key;
    temp->mark = 0;
    temp->degree = 0;
    return temp;
}

void concatenate(struct fibonacciHeap* Heap, struct fibonacciNode* x)
{
    struct fibonacciNode* temp = Heap->min;
    do
    {
        if (x == temp)
            return;
        temp = temp->right;
    } while (temp != Heap->min);
    x->right = Heap->min;
    x->left = Heap->min->left;
    Heap->min->left = x;
    x->left->right = x;
}

void insert(struct fibonacciHeap* Heap, struct fibonacciNode* x)
{
    if (Heap->min == NULL)
    {
        Heap->min = x;
    }
    else
    {
        concatenate(Heap, x);
        if (x->key < Heap->min->key)
            Heap->min = x;
    }
    ++(Heap->size);
}

struct fibonacciNode* minimum(struct fibonacciHeap* Heap)
{
    return Heap->min;
}

void addChildren(struct fibonacciHeap* Heap, struct fibonacciNode* x)
{
    struct fibonacciNode* temp = x;

    do
    {
        struct fibonacciNode* y = x->right;
        concatenate(Heap, x);
        x->parent = NULL;
        x = y;
    } while (x != temp);
}

void link(struct fibonacciHeap* Heap, struct fibonacciNode* y, struct fibonacciNode* x)
{
    y->right->left = y->left;
    y->left->right = y->right;
    y->parent = x;
    if (y == Heap->min)
        Heap->min = y->right;
    if (x->child == NULL)
    {
        x->child = y;
        y->left = y;
        y->right = y;
    }
    else
    {
        y->right = x->child;
        y->left = x->child->left;
        x->child->left = y;
        y->left->right = y;
    }
    x->degree++;
    y->mark = 0;
}

void consolidate(struct fibonacciHeap* Heap)
{
    int d;
    int count = log(Heap->size) / log(2) + 1;
    struct fibonacciNode** A = (struct fibonacciNode**)malloc(count * sizeof(struct fibonacciNode*));
    struct fibonacciNode* w = Heap->min, * x, * y, * temp;
    for (int i = 0; i < count; i++)
        A[i] = NULL;
    while (1)
    {
        x = w;
        d = x->degree;
        while (A[d] != NULL && d < count)
        {
            y = A[d];
            if (x->key > y->key)
            {
                temp = x;
                x = y;
                y = temp;
            }
            link(Heap, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
        if (w->parent == NULL)
            w = w->right;
        else
        {
            while (w->parent != NULL)
                w = w->parent;
            w = w->right;
        }
        if (w == Heap->min)
            break;
    }
    Heap->min = NULL;
    for (int i = 0; i < count; i++)
    {
        if (A[i] != NULL)
        {
            if (Heap->min == NULL)
            {
                Heap->min = A[i];
            }
            else
            {
                concatenate(Heap, A[i]);
                if (A[i]->key < Heap->min->key)
                    Heap->min = A[i];
            }
        }
    }
}

struct fibonacciNode* extract_min(struct fibonacciHeap* Heap)
{
    struct fibonacciNode* temp = Heap->min;
    if (temp != NULL)
    {
        if (temp->child != NULL)
        {
            addChildren(Heap, temp->child);
            temp->child = NULL;
        }
        temp->right->left = temp->left;
        temp->left->right = temp->right;
        if (temp == temp->right)
            Heap->min = NULL;
        else
        {
            Heap->min = temp->right;
            consolidate(Heap);
        }
        --Heap->size;
    }
    return temp;
}

void cut(struct fibonacciHeap* Heap, struct fibonacciNode* x, struct fibonacciNode* y)
{
    if (x == y->child)
    {
        if (x == x->right)
        {
            y->child = NULL;
            x->parent = NULL;
        }
        else
        {
            y->child = x->right;
            x->parent = NULL;
            x->right->left = x->left;
            x->left->right = x->right;
            x->right = x;
            x->left = x;
        }
    }
    else
    {
        x->parent = NULL;
        x->right->left = x->left;
        x->left->right = x->right;
        x->right = x;
        x->left = x;
    }
    y->degree--;
    concatenate(Heap, x);
    x->mark = 0;
}

void cascadingCut(struct fibonacciHeap* Heap, struct fibonacciNode* y)
{
    struct fibonacciNode* z = y->parent;
    if (z != NULL)
    {
        if (y->mark == 0)
            y->mark = 1;
        else
        {
            cut(Heap, y, z);
            cascadingCut(Heap, z);
        }
    }
}

//Function to decrease key
int decrease_key(struct fibonacciHeap* Heap, struct fibonacciNode* x, int k)
{
    if (x == NULL || k > x->key)
        return -1;
    x->key = k;
    struct fibonacciNode* y = x->parent;
    if (y != NULL && x->key < y->key)
    {
        cut(Heap, x, y);
        cascadingCut(Heap, y);
    }
    if (x->key < Heap->min->key)
        Heap->min = x;
    return k;
}

//Function to delete a node in fibonacci heap
void delete(struct fibonacciHeap* Heap, struct fibonacciNode* x)
{
    if (x != NULL)
        printf("%d\n", x->key);
    else
    {
        printf("-1\n");
        return;
    }
    decrease_key(Heap, x, -inf);
    extract_min(Heap);
}

//-----------------------------------------------------------------------------------
//Circular Queue
struct queueNode
{
    struct fibonacciNode* key;
    struct queueNode* next;
};

struct circularQueue
{
    struct queueNode* front;
    struct queueNode* back;
};

struct queueNode* create_queueNode(struct fibonacciNode* key)
{
    struct queueNode* temp;
    temp = (struct queueNode*)malloc(sizeof(struct queueNode));
    temp->next = NULL;
    temp->key = key;
    return temp;
}

void push(struct circularQueue* q, struct queueNode* temp)
{
    if (q->front == NULL)
        q->front = q->back = temp;
    else
    {
        q->back->next = temp;
        q->back = temp;
    }
}

void pop(struct circularQueue* q)
{
    if (q->front == NULL)
        return;
    if (q->front == q->back)
        q->front = q->back = NULL;
    else
        q->front = q->front->next;
}
//--------------------------------------------------------------------------------

// Function to search a node with given key in Heap
struct fibonacciNode* findFibonacciNode(struct fibonacciHeap* Heap, int i)
{
    if (Heap->min == NULL)
        return NULL;

    struct circularQueue q;
    q.front = NULL;
    q.back = NULL;
    struct fibonacciNode* curr = Heap->min;
    struct fibonacciNode* prev = curr;
    do
    {
        push(&q, create_queueNode(curr));
        if (curr->key == i)
            return curr;
        curr = curr->right;
        if (curr == prev)
        {
            do
            {
                if (&q.front == NULL)
                    break;
                curr = q.front->key->child;
                prev = curr;
                pop(&q);
            } while (curr == NULL);
        }
    } while (curr != NULL);
    return NULL;
}

//Function to print root list of heap
void print(struct fibonacciHeap* Heap)
{
    if (Heap->min == NULL)
        return;
    struct fibonacciNode* temp = Heap->min;
    printf("%d ", temp->key);
    temp = temp->right;
    while (temp != Heap->min)
    {
        printf("%d ", temp->key);
        temp = temp->right;
    }
    printf("\n");
}


int main()
{
    // freopen("input.txt", "r", stdin);
    char ch;

    struct fibonacciHeap* Heap = makefibonacciHeap(NULL);
    while (scanf("%c", &ch) && ch != 'e')
    {
        switch (ch)
        {
        case 'i':
        {int n;
        scanf("%d", &n);
        insert(Heap, create_fibonacciNode(n));
        }
        break;
        case 'm':
        {struct fibonacciNode* temp = minimum(Heap);
        if (temp != NULL)
            printf("%d\n", temp->key);
        else
            printf("-1\n");
        }
        break;
        case 'x':
        {   struct fibonacciNode* temp = extract_min(Heap);
        if (temp != NULL)
            printf("%d\n", temp->key);
        else
            printf("-1\n");}
        break;
        case 'r':
        {int  x, k;
        scanf("%d%d", &x, &k);
        printf("%d\n", decrease_key(Heap, findFibonacciNode(Heap, x), k));
        }
        break;
        case 'd':
        {int k;
        scanf("%d", &k);
        delete(Heap, findFibonacciNode(Heap, k));
        }
        break;
        case 'p': {print(Heap);}
                break;
        default:break;
        }
    }
    return 0;
}