#include<stdio.h>
#include<stdlib.h>

typedef struct AVL_TREE_NODE node;
struct AVL_TREE_NODE {
    int key;
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

// Function to search for a node with key k in A, and return a pointer to the node
node* SEARCH(node* A, int key)
{
    if (A == NULL)
        return NULL;
    if (key == A->key)
        return A;
    if (key < A->key)
        return SEARCH(A->left, key);
    else
        return SEARCH(A->right, key);
}

//Function to delete node with 'key' in A
node* DELETE(node* A, int key)
{
    if (A == NULL)
        return NULL;

    if (key < A->key)
        A->left = DELETE(A->left, key);

    else if (key > A->key)
        A->right = DELETE(A->right, key);

    else
    {
        if (A->left == NULL || A->right == NULL)
        {
            node* temp = A->left;
            if (temp == NULL)
                temp = A->right;

            if (temp == NULL)
            {
                temp = A;
                A = NULL;
            }
            else
                *A = *temp;
            free(temp);
        }
        else
        {
            node* temp = A->right;
            while (temp->left != NULL)
            {
                temp = temp->left;
            }
            A->key = temp->key;
            A->right = DELETE(A->right, temp->key);
        }
    }
    if (A == NULL)
        return A;

    A->height = max(height(A->left), height(A->right)) + 1;

    int balance = GET_BALANCE_FACTOR(A);

    //Left Left heavy
    if (balance > 1 && GET_BALANCE_FACTOR(A->left) >= 0)
    {
        return RIGHTROTATE(A);
    }

    //Left Right Heavy
    if (balance > 1 && GET_BALANCE_FACTOR(A->left) < 0)
    {
        A->left = LEFTROTATE(A->left);
        return(RIGHTROTATE(A));
    }

    //Right Right heavy
    if (balance < -1 && GET_BALANCE_FACTOR(A->right) <= 0)
    {
        return(LEFTROTATE(A));
    }

    //Right Left heavy
    if (balance < -1 && GET_BALANCE_FACTOR(A->right) >= 0)
    {
        A->right = RIGHTROTATE(A->right);
        return(LEFTROTATE(A));
    }
    return A;
}

//Funtion to print the balance factor of the node with k as key in the tree A
void GETBALANCE(node* A, int key)
{
    node* temp = SEARCH(A, key);
    if (temp != NULL)
        printf("%d", GET_BALANCE_FACTOR(temp));
    else
        printf("FALSE");
}

//Function to print tree in parenthesis form
void PRINTTREE(node* A)
{
    if (A != NULL)
    {
        printf("( ");
        printf("%d", A->key);
        printf(" ");
        PRINTTREE(A->left);
        PRINTTREE(A->right);
        printf(") ");
    }
    else
        printf("( ) ");
}

int main()
{
    node* A = NULL;
    char ch;
    while (scanf("%c", &ch) && ch != 'e')
    {
        switch (ch)
        {
        case 'i':
        {
            int k;
            scanf("%d", &k);
            A = INSERT(A, k);
        }
        break;
        case 'd':
        {
            int k;
            scanf("%d", &k);
            if (SEARCH(A, k) == NULL)
                printf("FALSE\n");
            else
            {
                A = DELETE(A, k);
                printf("%d\n", k);
            }
        }
        break;
        case 's':
        {
            int k;
            scanf("%d", &k);
            if (SEARCH(A, k) != NULL)
                printf("TRUE\n");
            else
                printf("FALSE\n");
        }
        break;
        case 'b':
        {
            int k;
            scanf("%d", &k);
            GETBALANCE(A, k);
            printf("\n");
        }
        break;
        case 'p':
        {
            PRINTTREE(A);
            printf("\n");
        }
        break;
        }
    }
}