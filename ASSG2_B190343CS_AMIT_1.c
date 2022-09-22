#include <stdio.h>
#include <stdlib.h>

struct Hash_Table
{
    int* table;
    int size;
    int c1;
    int c2;
    int (*hash)(struct Hash_Table* T, int i, int k);
};

int quadratic_hash(struct Hash_Table* T, int i, int k)
{
    int val;
    val = (k % T->size + T->c1 * i + T->c2 * i * i) % T->size;
    return val;
}

int double_hash(struct Hash_Table* T, int i, int k)
{
    static int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 };
    static int R = -1;
    if (R == -1)
    {
        for (int j = 0; j < 25; j++)
            if (primes[j] < T->size)
                R = primes[j];
    }

    int val;
    val = (k % T->size + i * (R - (k % R))) % T->size;
    return val;
}

struct Hash_Table HASHTABLE(int m, char type)
{
    struct Hash_Table T;
    T.size = m;
    T.table = (int*)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
        T.table[i] = -2; //Empty space denoted by -2
    switch (type)
    {
    case 'a':
        scanf("%d%d", &T.c1, &T.c2);
        T.hash = quadratic_hash;
        break;
    case 'b':
        T.hash = double_hash;
        break;
    default:
        T.hash = NULL;
    }
    return T;
}

void INSERT(struct Hash_Table* T, int k)
{
    int i = 0;
    int index = T->hash(T, i, k);
    while (i < T->size && (T->table[index] >= 0))
    {
        index = T->hash(T, i, k);
        i++;
    }
    T->table[index] = k;
}

int SEARCH(struct Hash_Table* T, int k)
{
    int i = 0;
    int index = T->hash(T, i, k);
    while (i < T->size && T->table[index] != -2)
    {
        if (T->table[index] == k)
            return 1;
        index = T->hash(T, i, k);
        i++;
    }
    return -1;
}

void DELETE(struct Hash_Table* T, int k)
{
    int i = 0;
    int index = T->hash(T, i, k);
    while (i < T->size && T->table[index] != k)
    {
        index = T->hash(T, i, k);
        i++;
    }
    T->table[index] = -1;
}

void PRINT(struct Hash_Table* T)
{
    for (int i = 0; i < T->size; i++)
        if (T->table[i] != -1 && T->table[i] != -2)
            printf("%d (%d)\n", i, T->table[i]);
        else
            printf("%d ()\n", i);
}
int main()
{
    char type;
    scanf("%c", &type);
    int table_size;
    scanf("%d", &table_size);
    struct Hash_Table T = HASHTABLE(table_size, type);
    char choice;
    while (scanf("%c", &choice) && choice != 't')
    {
        switch (choice)
        {
        case 'i':
        {
            int key;
            scanf("%d", &key);
            INSERT(&T, key);
        }
        break;
        case 's':
        {
            int key;
            scanf("%d", &key);
            printf("%d\n", SEARCH(&T, key));
        }
        break;
        case 'd':
        {
            int key;
            scanf("%d", &key);
            DELETE(&T, key);
        }
        break;
        case 'p':
        {
            PRINT(&T);
        }
        break;
        default:
            break;
        }
    }
    free(T.table);
    return 0;
}
