#include<stdio.h>
#include<stdlib.h>

typedef struct AVL_TREE_NODE node;
struct AVL_TREE_NODE {
    int key;
    int count;
    int height;
    struct AVL_TREE_NODE* left;
    struct AVL_TREE_NODE* right;
};

// Funtion that returns height of node
int height(node* temp)
{
    if (temp == NULL)
        return 0;
    return temp->height;
}

// Function to get maximum of two integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Function to create new AVL tree node
node* CREATE_AVL_TREE_NODE(int key) {
    node* temp = (node*)malloc(sizeof(node));
    temp->key = key;
    temp->left = NULL;
    temp->right = NULL;
    temp->count = 1;
    temp->height = 1;
}

/*Function to Right rotate subtree rooted with y
        y                        x
       / \      ----->          / \
      x   c                    a   y
     / \                          / \
    a   b                        b   c
*/
node* RIGHTROTATE(node* y)
{
    node* x = y->left;
    node* b = x->right;

    x->right = y;
    y->left = b;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

/*Function to Left rotate subtree rooted with x
        y                        x
       / \      <-----          / \
      x   c                    a   y
     / \                          / \
    a   b                        b   c
*/
node* LEFTROTATE(node* x)
{
    node* y = x->right;
    node* b = y->left;

    y->left = x;
    x->right = b;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

//Function to get balance factor of tree rooted at temp
int GET_BALANCE_FACTOR(node* temp)
{
    if (temp == NULL)
        return 0;
    return height(temp->left) - height(temp->right);
}

//Function to insert node with'key' in A
node* INSERT(node* A, int key)
{
    if (A == NULL)
        return CREATE_AVL_TREE_NODE(key);

    if (key < A->key)
    {
        A->left = INSERT(A->left, key);
    }
    else if (key > A->key)
    {
        A->right = INSERT(A->right, key);
    }
    else
    {
        ++(A->count);
        return A;
    }

    A->height = max(height(A->left), height(A->right)) + 1;

    int balance = GET_BALANCE_FACTOR(A);

    //Left Left heavy
    if (balance > 1 && key < A->left->key)
    {
        return RIGHTROTATE(A);
    }

    //Left Right Heavy
    if (balance > 1 && key > A->left->key)
    {
        A->left = LEFTROTATE(A->left);
        return(RIGHTROTATE(A));
    }

    //Right Right heavy
    if (balance < -1 && key > A->right->key)
    {
        return(LEFTROTATE(A));
    }

    //Right Left heavy
    if (balance < -1 && key < A->right->key)
    {
        A->right = RIGHTROTATE(A->right);
        return(LEFTROTATE(A));
    }

    return A;
}

//inorder traversal
void inorder(node* A)
{
    if (A == NULL)
        return;
    inorder(A->left);
    for (int j = 0;j < A->count;j++)
        printf("%d ", A->key);
    inorder(A->right);
}

int main()
{
    node* A = NULL;
    int n;
    scanf("%d", &n);
    while (n--)
    {
        int x;
        scanf("%d", &x);
        A = INSERT(A, x);
    }
    inorder(A);
    printf("\n");
}