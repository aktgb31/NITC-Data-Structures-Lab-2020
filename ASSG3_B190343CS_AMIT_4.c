#include<stdio.h>
#include<stdlib.h>
typedef struct binomial_heap_node heapNode;
typedef struct BINOMIAL_HEAP Heap;

struct binomial_heap_node {
    int key, degree;
    struct binomial_heap_node* left_child, * right_sibling, * parent;
};

struct BINOMIAL_HEAP { struct binomial_heap_node* root; };


// Makes a new empty heap
Heap* MakeHeap() {
    Heap* temp = (Heap*)malloc(sizeof(Heap));
    temp->root = NULL;
    return temp;
}


// Returns minimum value inside the binomial heap
int Minimum(Heap* H)
{
    heapNode* temp = H->root;
    int value = 1e9;
    while (temp != NULL)
    {
        if (temp->key < value)
            value = temp->key;

        temp = temp->right_sibling;
    }
    return value;
}

// Links two binomial trees and makes n2 head of it
void BinomialLink(heapNode* n1, heapNode* n2)
{
    n1->parent = n2;
    n1->right_sibling = n2->left_child;
    n2->left_child = n1;
    ++n2->degree;
}

Heap* HeapMerge(Heap* H1, Heap* H2)
{
    Heap* New = MakeHeap();
    heapNode* new = New->root;
    heapNode* h1 = H1->root;
    heapNode* h2 = H2->root;

    while (h1 != NULL && h2 != NULL)
    {
        if (h1->degree <= h2->degree)
        {
            if (New->root == NULL)
            {
                New->root = h1;
                h1 = h1->right_sibling;
                new = New->root;
            }
            else
            {
                new->right_sibling = h1;
                h1 = h1->right_sibling;
                new = new->right_sibling;
            }

        }
        else
        {
            if (New->root == NULL)
            {
                New->root = h2;
                new = New->root;
                h2 = h2->right_sibling;
            }
            else
            {
                new->right_sibling = h2;
                h2 = h2->right_sibling;
                new = new->right_sibling;
            }
        }
    }

    while (h1 != NULL)
    {
        if (New->root == NULL)
        {
            New->root = h1;
            new = New->root;
            h1 = h1->right_sibling;
        }
        else
        {
            new->right_sibling = h1;
            h1 = h1->right_sibling;
            new = new->right_sibling;
        }

    }

    while (h2 != NULL)
    {
        if (New->root == NULL)
        {
            New->root = h2;
            new = New->root;
            h2 = h2->right_sibling;
        }
        else
        {
            new->right_sibling = h2;
            h2 = h2->right_sibling;
            new = new->right_sibling;
        }
    }

    return New;
}

// Returns a new heap which is union of H1 and H2
Heap* Union(Heap* H1, Heap* H2)
{
    Heap* H = MakeHeap();
    H = HeapMerge(H1, H2);

    free(H1);
    free(H2);
    if (H->root == NULL)
        return H;

    heapNode* previous = NULL;
    heapNode* current = H->root;
    heapNode* next = current->right_sibling;

    while (next != NULL)
    {
        if (current->degree != next->degree || (next->right_sibling != NULL && next->right_sibling->degree == current->degree))
        {
            previous = current;
            current = next;
        }
        else {
            if (current->key <= next->key)
            {
                current->right_sibling = next->right_sibling;
                BinomialLink(next, current); //make next as child of current
            }
            else
            {
                if (previous == NULL)
                    H->root = next;
                else
                    previous->right_sibling = next;

                BinomialLink(current, next);// make current as child of next
                current = next;
            }
        }
        next = current->right_sibling;
    }

    return H;
}

void heapInsert(Heap** H, int x)
{
    Heap* New = MakeHeap();
    heapNode* temp = (heapNode*)malloc(sizeof(heapNode));
    temp->parent = NULL;
    temp->right_sibling = NULL;
    temp->left_child = NULL;
    temp->key = x;
    temp->degree = 0;
    New->root = temp;

    //Need to change pointer here so use double pointers
    *H = Union(*H, New);
}

int extractMin(Heap** H)
{
    if ((*H)->root == NULL)
        return -1;
    heapNode* prev = NULL;
    heapNode* curr = (*H)->root;
    heapNode* next = curr->right_sibling;
    int x = Minimum(*H);

    while (next != NULL)
    {
        if (curr->key == x)
        {
            break;
        }
        prev = curr;
        curr = next;
        next = next->right_sibling;
    }
    if (prev == NULL)
    {
        (*H)->root = next;
    }
    else
    {
        prev->right_sibling = next;
    }
    curr->right_sibling = NULL;
    heapNode* temp = curr;
    curr = curr->left_child;
    free(temp);

    Heap* new = MakeHeap();
    new->root = curr;

    //Reversing the sibling order of child nodes of deleted node
    prev = NULL;
    next = NULL;
    while (curr != NULL)
    {
        curr->parent = NULL;
        next = curr->right_sibling;
        curr->right_sibling = prev;
        prev = curr;
        curr = next;
    }
    new->root = prev;
    //Need to change pointer here so use double pointers
    *H = Union(*H, new);
    return x;
}

heapNode* heapSearch(heapNode* H, int key)
{
    if (H == NULL)
        return NULL;

    if (H->key == key)
        return H;

    heapNode* temp = heapSearch(H->left_child, key);
    if (temp != NULL)
        return temp;
    temp = heapSearch(H->right_sibling, key);
    return temp;

}
int decreaseKey(Heap* H, int x, int k)
{
    heapNode* target = heapSearch(H->root, x);
    if (target == NULL)
        return -1;

    target->key -= k;

    heapNode* up = target->parent;
    while (up != NULL && target->key < up->key)
    {
        int temp = target->key;
        target->key = up->key;
        up->key = temp;
        target = up;
        up = up->parent;
    }
    return x - k;
}

void delete(Heap** H, int x)
{
    if (decreaseKey(*H, x, 1e9) == -1)
        printf("-1\n");
    else
    {
        extractMin(H);
        printf("%d\n", x);
    }

}
//-------------------------------------------------------------------------------------------------

struct listNode
{
    heapNode* data;
    struct listNode* next;
};
struct queue
{
    struct listNode* tail, * head;
};
struct listNode* create_listNode(heapNode* k)
{
    struct listNode* temp = (struct listNode*)malloc(sizeof(struct listNode));
    temp->data = k;
    temp->next = NULL;
    return temp;
}
int queue_empty(struct queue* q)
{
    return (q->head == NULL);
}
void enqueue(struct queue* q, heapNode* k)
{
    struct listNode* temp = create_listNode(k);
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
    struct listNode* temp = q->head;
    q->head = q->head->next;
    temp->next = NULL;
    if (q->head == NULL)
        q->tail = NULL;
    free(temp);

}
heapNode* queue_top(struct queue* q)
{
    return q->head->data;
}
//-------------------------------------------------------------------------
void printTree(heapNode* H)
{
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->tail = q->head = NULL;
    printf("%d ", H->key);
    enqueue(q, H->left_child);
    while (!queue_empty(q))
    {
        heapNode* temp = queue_top(q);
        while (temp != NULL)
        {
            printf("%d ", temp->key);
            enqueue(q, temp->left_child);
            temp = temp->right_sibling;
        }
        dequeue(q);
    }
    free(q);
}

void printHeap(Heap* H)
{
    heapNode* temp = H->root;
    while (temp != NULL)
    {
        printTree(temp);
        temp = temp->right_sibling;
    }
    printf("\n");
}

int main()
{

    struct BINOMIAL_HEAP* H = (Heap*)malloc(sizeof(Heap));
    H->root = NULL;
    char ch;
    while (scanf("%c", &ch) && ch != 'e')
    {
        switch (ch)
        {
        case 'i':
        {int x;
        scanf("%d", &x);
        heapInsert(&H, x);
        }
        break;

        case 'd':
        {int x;
        scanf("%d", &x);
        delete(&H, x);}
        break;

        case 'p':
        {
            printHeap(H);
        }
        break;

        case 'm':
        {printf("%d\n", Minimum(H));}
        break;

        case 'x':
        {printf("%d\n", extractMin(&H));}
        break;

        case 'r':
        {int y, z;
        scanf("%d%d", &y, &z);
        if (z > y)
            printf("-1\n");
        else
            printf("%d\n", decreaseKey(H, y, z));}
        break;

        default:
            break;
        }
    }
    return 0;
}