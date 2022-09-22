#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Binary tree node and function to create empty binary tree
// key contains value stored inside current node
// count_left contains number of elements inside the left subtree of current node
// count contains number of elements inside the subtree of current node including curent node

struct tnode
{
    int key;
    int count_left;
    int count;
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
    temp->count_left = 0;
    temp->count = 1;
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
//------------------------------------------------------------------------------------

// Helper function for maketree
int findpartition(char *input, int start, int end)
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
            root->left->p = root;                 //update parent
            root->count_left = root->left->count; //update count_left
            root->count += root->left->count;     //update count
        }
        root->right = make_tree(input, partition + 2, end - 1);
        if (root->right != NULL)
        {
            root->right->p = root;             //update parent
            root->count += root->right->count; //update count
        }
    }
    return root;
}
//--------------------------------------------------------------------------------------

//function to erase space
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
//-----------------------------------------------------------------------------------

// KSMALLEST() returns k th smallest element
int KSMALLEST(struct tnode *root, int k)
{
    if (root == NULL || root->count < k)
        return -1;
    if (root->count_left + 1 == k)
        return root->key;
    if (root->count_left >= k)
        return KSMALLEST(root->left, k);

    return KSMALLEST(root->right, k - root->count_left - 1);
}
int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    struct Binary_Tree T;
    T.root = NULL;
    char input[10000];
    scanf("%[^\n]%*c", input);
    erase_space(input);
    if (strlen(input) >= 3)
        T.root = make_tree(input, 1, strlen(input) - 2);

    int k;
    scanf("%d", &k);
    printf("%d\n", KSMALLEST(T.root, k));

    return 0;
}