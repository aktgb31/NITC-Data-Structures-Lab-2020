#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SIZE = 256;

struct node
{
    char* word;
    struct node* next;
};
struct list
{
    struct node* head;
};
//Create node for the linkedlist
struct node* create_node(char* key)
{
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->word = (char*)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(temp->word, key);
    temp->next = NULL;
    return temp;
}


//Insert a node in the list such that list is sorted
void sorted_list_insert(struct list* L, struct node* temp)
{

    if (L->head == NULL)
    {
        L->head = temp;
    }
    else if (strcmp(temp->word, L->head->word) <= 0)
    {
        temp->next = L->head;
        L->head = temp;
    }
    else
    {
        struct node* t = L->head;
        while (t->next != NULL)
        {
            if (strcmp(temp->word, t->next->word) <= 0)
            {
                temp->next = t->next;
                t->next = temp;
                return;
            }
            t = t->next;
        }
        t->next = temp;
    }
}

struct Hash_Table
{
    struct list table[256];//Max size is fixed
};

struct Hash_Table HASHTABLE()
{
    struct Hash_Table T;
    for (int i = 0; i < SIZE; i++)
    {
        T.table[i].head = NULL;
    }
    return T;
}

int hash(char* string)
{
    int i = 0;
    int temp[26] = { 0 };
    while (string[i] != '\0')
    {
        temp[string[i] - 'a']++;
        i++;
    }
    i = 0;
    int index = 0;
    for (;i < 26;i++)
    {
        if (temp[i])
            index += (1 << i);
    }
    return index;
}

void INSERT(struct Hash_Table* T, char* string)
{
    int i = 0;
    int index = hash(string);
    struct node* temp = create_node(string);

    sorted_list_insert(&(T->table[index]), temp);
}

void PRINT(struct Hash_Table* T)
{
    for (int i = 0; i < SIZE; i++)
    {
        if (T->table[i].head != NULL)
        {
            struct node* temp = T->table[i].head;
            while (temp != NULL)
            {
                printf("%s ", temp->word);
                temp = temp->next;
            }
            printf("\n");
        }
    }
}


int main()
{
    struct Hash_Table T = HASHTABLE();
    int p = 0;
    scanf("%d", &p);
    char string[501];
    while (p--)
    {
        scanf("%s", string);
        INSERT(&T, string);
    }
    PRINT(&T);
    return 0;
}