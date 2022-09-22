#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Binary tree node and function to create empty binary tree
// key contains value stored inside current node
// sum contains sum of values inside the subtree of current node including current node
// min contains minimum value of the key of nodes inside subtree of current node including current node
// max  contains minimum value of the key of nodes inside subtree of current node including current node
// isbst denotes if subtree of current node including current node forms a BST

struct tnode
{
    int key;
    int sum;
    int min;
    int max;
    int isbst;
    struct tnode *left, *right, *p;
};

struct tnode *createtnode(int item)
{
    struct tnode *temp = (struct tnode *)malloc(sizeof(struct tnode));
    temp->key = item;
    temp->sum = item;
    temp->max = item;
    temp->min = item;
    temp->isbst = 1;
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
            root->left->p = root;            //update parent
            root->sum += root->left->sum;    //update sum
            if (root->left->max > root->max) //update max
                root->max = root->left->max;
            if (root->left->min < root->min) //update min
                root->min = root->left->min;
            if (root->key >= root->left->max) //update isbst
                root->isbst &= root->left->isbst;
            else
                root->isbst = 0;
        }
        root->right = make_tree(input, partition + 2, end - 1);
        if (root->right != NULL)
        {
            root->right->p = root;            //update parent
            root->sum += root->right->sum;    //update sum
            if (root->right->max > root->max) //update max
                root->max = root->right->max;
            if (root->right->min < root->min) //update min
                root->min = root->right->min;
            if (root->key <= root->right->min) //update isbst
                root->isbst &= root->right->isbst;
            else
                root->isbst = 0;
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

// answer function returns number of nodes with subtree sum=k and and subtree is a bst
int answer(struct tnode *root, int k)
{
    if (root != NULL)
    {
        return (root->sum == k && root->isbst) + answer(root->left, k) + answer(root->right, k);
    }
    return 0;
}

//---------------------------------------------------------------------------------

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
    int Answer = answer(T, k);
    if (Answer == 0)
        Answer = -1;
    printf("%d\n", Answer);
    return 0;
}