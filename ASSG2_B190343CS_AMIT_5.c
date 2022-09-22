#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Binary tree node and function to create empty binary tree
// key contains value stored inside current node
// count_left contains number of elements inside the left subtree of current node
// count contains number of elements inside the subtree of current node including curent node
// count_right contains number of elements inside the right subtree of current node

struct tnode
{
    int key;
    int count_left;
    int count_right;
    int count;
    struct tnode* left, * right, * p;
};

struct Binary_Tree
{
    struct tnode* root;
};

struct tnode* createtnode(int item)
{
    struct tnode* temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->key = item;
    temp->count_left = 0;
    temp->count_right = 0;
    temp->count = 1;
    temp->left = temp->right = temp->p = NULL;
    return temp;
}
//-----------------------------------------------------------------------------

//Stack to store characters
struct snode
{
    char data;
    struct snode* next;
};
struct stack
{
    struct snode* top;
};
struct snode* createsnode(char k)
{
    struct snode* temp = (struct snode*)malloc(sizeof(struct snode));
    temp->data = k;
    temp->next = NULL;
    return temp;
}
char stack_top(struct stack* s)
{
    return s->top->data;
}
int stack_empty(struct stack* s)
{
    return (s->top == NULL);
}
void push(struct stack* s, char k)
{
    struct snode* temp = createsnode(k);
    temp->next = s->top;
    s->top = temp;
}
void pop(struct stack* s)
{
    struct snode* temp = s->top;
    s->top = s->top->next;

    free(temp);
}
//------------------------------------------------------------------------------------

// Helper function for maketree
int findpartition(char* input, int start, int end)
{
    if (start > end)
        return -1;
    struct stack s;
    s.top = NULL;
    //  printf("Stack details\n");
    for (int i = start; i <= end; i++)
    {
        //  printf("stack top %c\n", s.top->data);
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
struct tnode* make_tree(char* input, int start, int end)
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
        num = (num) * 10 + (input[start] - '0');
    }
    if (negative)
        num *= -1;

    struct tnode* root;
    root = createtnode(num);

    int partition = -1;
    if (start + 1 <= end && input[start] == '(')
        partition = findpartition(input, start, end);

    if (partition != -1)
    {
        root->left = make_tree(input, start + 1, partition - 1);
        if (root->left != NULL)
        {
            root->left->p = root;                 //update parent
            root->count_left = root->left->count; //update count_left
            root->count += root->left->count;     //update count
        }
        root->right = make_tree(input, partition + 2, end - 1);
        if (root->right != NULL)
        {
            root->right->p = root;             //update parent
            root->count_right = root->right->count;//update count_right
            root->count += root->right->count; //update count
        }
    }
    return root;
}
//--------------------------------------------------------------------------------------

//function to erase space
void erase_space(char* input)
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
//-----------------------------------------------------------------------------------
//Searching a key in bst
struct tnode* SEARCH(struct tnode* T, int key)
{
    if (T == NULL)
        return NULL;
    if (key == T->key)
        return T;
    if (key < T->key)
        return SEARCH(T->left, key);
    else
        return SEARCH(T->right, key);
}

//-----------------------------------------------------------------------------------

//Minimum Value of node in bst
struct tnode* MIN_VALUE(struct tnode* T)
{
    struct tnode* temp = T;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

//Maximum Value of node in bst
struct tnode* MAX_VALUE(struct tnode* T)
{
    struct tnode* temp = T;
    while (temp->right != NULL)
    {
        temp = temp->right;
    }
    return temp;
}
//------------------------------------------------------------------------------
// KSMALLEST() returns k th smallest element
int KSMALLEST(struct tnode* root, int k)
{
    if (root == NULL || root->count < k)
        return -1;
    if (root->count_left + 1 == k)
        return root->key;
    if (root->count_left >= k)
        return KSMALLEST(root->left, k);

    return KSMALLEST(root->right, k - root->count_left - 1);
}

// KLARGEST() returns k th smallest element
int KLARGEST(struct tnode* root, int k)
{
    if (root == NULL || root->count < k)
        return -1;
    if (root->count_right + 1 == k)
        return root->key;
    if (root->count_right >= k)
        return KLARGEST(root->right, k);

    return KLARGEST(root->left, k - root->count_right - 1);
}
//inorder traversal of bst
void inorder(struct tnode* root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

//Successor of node with given key
struct tnode* SUCCESSOR(struct tnode* T, int key)
{
    struct tnode* temp = SEARCH(T, key);
    if (temp == NULL || T == NULL)
        return NULL;

    if (temp->right != NULL)
        return MIN_VALUE(temp->right);

    struct tnode* temp2 = temp->p;
    while (temp2 != NULL && temp == temp2->right)
    {
        temp = temp2;
        temp2 = temp2->p;
    }
    return temp2;
}

//predecessor of node with given key
struct tnode* PREDECESSOR(struct tnode* T, int key)
{
    struct tnode* temp = SEARCH(T, key);
    if (temp == NULL || T == NULL)
        return NULL;

    if (temp->left != NULL)
        return MAX_VALUE(temp->left);

    struct tnode* temp2 = temp->p;
    while (temp2 != NULL && temp == temp2->left)
    {
        temp = temp2;
        temp2 = temp2->p;
    }
    return temp2;
}

int main()
{
    struct tnode* T;
    T = NULL;
    char input[10000];
    scanf("%[^\n]%*c", input);
    erase_space(input);
    if (strlen(input) >= 3)
        T = make_tree(input, 1, strlen(input) - 2);

    char ch;
    while (scanf("%c", &ch) && ch != 'e')
    {
        switch (ch)
        {
        case 'l':
        {
            int k;
            scanf("%d", &k);
            printf("%d\n", KLARGEST(T, k));
        }
        break;
        case 's':
        {
            int k;
            scanf("%d", &k);
            printf("%d\n", KSMALLEST(T, k));
        }
        break;
        case 'r':
        {
            int k;
            scanf("%d", &k);
            struct tnode* temp = PREDECESSOR(T, k);
            if (temp == NULL)
                printf("-1\n");
            else
                printf("%d\n", temp->key);
        }
        break;
        case 'u':
        {
            int k;
            scanf("%d", &k);
            struct tnode* temp = SUCCESSOR(T, k);
            if (temp == NULL)
                printf("-1\n");
            else
                printf("%d\n", temp->key);
        }
        break;
        case 'i':
        {
            inorder(T);
            printf("\n");
        }
        break;
        }
    }
    return 0;
}