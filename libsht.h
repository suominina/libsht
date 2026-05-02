#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100000

typedef struct entry_t {
    char *key;
    char *value;
    struct entry_t *next; /* for hash collision */
} entry_t;

typedef struct lht_t {
    entry_t **entries;
} lht_t; 


lht_t *lht_create();
int lht_set(lht_t *ht, const char *key, const char *value);
char *lht_get(lht_t* ht, const char *key);
/* TODO: implement these */
int lht_remove(lht_t* ht, const char *key);
int lht_free(lht_t* ht);


/* functions below are being used internaly */
unsigned int ht_hash(const char *key);
entry_t *ht_pair(const char *key, const char *value);

unsigned int ht_hash(const char *key)
{
    unsigned long int value = 0;
    unsigned int key_len = strlen(key);

    for (unsigned int i = 0; i < key_len; i++) {
        value = value * 37 + key[i];
    }

    value = value % TABLE_SIZE;

    return value;
}

entry_t *ht_pair(const char *key, const char *value)
{
    entry_t *entry = malloc(sizeof(entry));
    if (entry == NULL) return entry;

    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    strncpy(entry->key, key, strlen(key));
    strncpy(entry->value, value, strlen(value));

    entry->next = NULL;

    return entry;
}

lht_t* lht_create()
{
    lht_t *ht = malloc(sizeof(lht_t));
    if (ht == NULL) { return ht; }

    ht->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);
    if (ht->entries == NULL) { return ht; }

    /* needed for lht_set() */
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->entries[i] = NULL;
    }

    return ht;
}

/* returns the slot if there were no entries */
int lht_set(lht_t *ht, const char *key, const char *value)
{
    unsigned int slot = ht_hash(key);
    if (ht == NULL) { 
        fprintf(stderr, "ht is null. error at %d in %s\n", __LINE__, __FILE__);
        return 1;
    }

    entry_t *prev = NULL;
    entry_t *entry = ht->entries[slot];
    /* entry == NULL means the slot is empty. set a new pair and return */
    if (entry == NULL) {
        if ((ht->entries[slot] = ht_pair(key, value)) == NULL) { 
            fprintf(stderr, "failed to ht_pair(). error at %d in %s\n", __LINE__, __FILE__);
            return 1; 
        }
        return 0;
    }

    /* reaching here means hash collision happened.
     * walk through each entry until either it reached the end 
     * or matching key is found */
    while (entry != NULL) {
        /* the same key found. replace the value */
        if (strncmp(entry->key, key, strlen(key)) == 0) {
            free(entry->value);
            entry->value = malloc(strlen(value) + 1);
            if (entry->value == NULL) {
                fprintf(stderr, "failed to malloc(). error at %d in %s\n", __LINE__, __FILE__);
                return 1;
            }

            strncpy(entry->value, value, strlen(value));
            return 0;
        }

        prev = entry;
        entry = prev->next;
    }

    prev->next = ht_pair(key, value);
    return 0;
}

char *lht_get(lht_t *ht, const char *key)
{
    unsigned int slot = ht_hash(key);
    if (ht->entries[slot] == NULL) {
        /* key not found */
        printf("not found\n");
        return NULL; 
    }

    entry_t *entry = ht->entries[slot];
    while (entry != NULL) {
        if (strncmp(entry->key, key, strlen(key)) == 0) {
            return entry->value;
        }

        entry = entry->next;
    }

    /* there were entries but no key match */
    return NULL;
}
