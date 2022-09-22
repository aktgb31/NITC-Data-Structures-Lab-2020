#include<stdio.h>
#include<stdlib.h>

enum Color { Black = 66, Red = 82 };
typedef struct RB_TREE_NODE node;
typedef struct RB_TREE tree;


struct RB_TREE_NODE {
    int key; //Stores key
    int color;// Stores color of node
    struct RB_TREE_NODE* parent;
    struct RB_TREE_NODE* left;
    struct RB_TREE_NODE* right;
};

struct RB_TREE {
    struct RB_TREE_NODE* root;
};

// Function to create new RB tree node
node* CREATE_RB_TREE_NODE(int key) {
    node* temp = (node*)malloc(sizeof(node));
    temp->key = key;
    temp->left = NULL;
    temp->right = NULL;
    temp->parent = NULL;
    temp->color = Red;
    return temp;
}

/*Function to Right rotate subtree rooted with y
        y                        x
       / \      ----->          / \
      x   c                    a   y
     / \                          / \
    a   b                        b   c
*/
void RIGHTROTATE(tree* T, node* y)
{
    node* x = (y)->left;
    (y)->left = x->right;

    if (x->right != NULL)
        x->right->parent = (y);

    x->parent = (y)->parent;
    if (x->parent == NULL)
        T->root = (y);
    else if ((y) == (y)->parent->left)
        (y)->parent->left = x;
    else
        (y)->parent->right = x;
    x->right = (y);
    (y)->parent = x;
    //return x;
}

/*Function to Left rotate subtree rooted with x
        y                        x
       / \      <-----          / \
      x   c                    a   y
     / \                          / \
    a   b                        b   c
*/
void LEFTROTATE(tree* T, node* x)
{
    node* y = (x)->right;
    (x)->right = y->left;
    if (y->left != NULL)
        y->left->parent = (x);
    y->parent = (x)->parent;

    if ((x)->parent == NULL)
        T->root = y;
    else if ((x) == (x)->parent->left)
        (x)->parent->left = y;
    else
        (x)->parent->right = y;
    y->left = (x);
    (x)->parent = y;

}


//Function to insert node with'key' in RB Tree
void* INSERT(tree* T, int key)
{
    node* temp = CREATE_RB_TREE_NODE(key);
    node* up = NULL;
    node* down = T->root;
    while (down != NULL)
    {
        up = down;
        if (down->key > temp->key)
            down = down->left;

        else
            down = down->right;
    }
    temp->parent = up;
    if (up == NULL)
        T->root = temp;
    else if (up->key > temp->key)
        up->left = temp;
    else
        up->right = temp;


    // Recoloring and Rotation
    while (temp != T->root && temp->parent->color == Red)
    {
        if (temp->parent == temp->parent->parent->left)
        {
            node* x;
            x = temp->parent->parent->right;
            if (x != NULL)
            {
                if (x->color == Red)
                {
                    temp->parent->color = Black;
                    x->color = Black;
                    temp->parent->parent->color = Red;
                    temp = temp->parent->parent;
                }
            }
            else if (temp == temp->parent->right)
            {
                temp = temp->parent;
                LEFTROTATE(T, temp);
            }
            else
            {
                if (temp->parent != NULL)
                {
                    temp->parent->color = Black;
                    if (temp->parent->parent != NULL)
                    {
                        temp->parent->parent->color = Red;
                        RIGHTROTATE(T, temp->parent->parent);
                    }
                }
            }
        }
        else
        {
            node* x;
            x = temp->parent->parent->left;
            if (x != NULL)
            {
                if (x->color == Red)
                {
                    temp->parent->color = Black;
                    x->color = Black;
                    temp->parent->parent->color = Red;
                    temp = temp->parent->parent;
                }
            }
            else if (temp == temp->parent->left)
            {
                temp = temp->parent;
                RIGHTROTATE(T, temp);
            }
            else
            {
                if (temp->parent != NULL)
                {
                    temp->parent->color = Black;
                    if (temp->parent->parent != NULL)
                    {
                        temp->parent->parent->color = Red;
                        LEFTROTATE(T, temp->parent->parent);
                    }
                }
            }
        }
    }
    T->root->color = Black;
}

//Printing in parenthesis form
void PRINT(node* root)
{
    printf("( ");
    if (root != NULL)
    {
        printf("%d %c ", root->key, root->color);
        PRINT(root->left);
        PRINT(root->right);
    }
    printf(") ");
}

//Converts string of digits to numbers
int read(char* string)
{
    int neg = 1;
    int i = 0;
    int num = 0;
    if (string[i] == '-')
    {
        neg = -1;
        ++i;
    }
    for (;string[i] != '\0';i++)
        num = num * 10 + string[i] - '0';
    num *= neg;
    return num;
}


int main()
{
    tree T;
    T.root = NULL;
    char string[20];
    while (scanf("%s", string) && string[0] != 't')
    {
        int x = read(string);
        INSERT(&T, x);
        PRINT(T.root);
        printf("\n");
    }
    return 0;
}