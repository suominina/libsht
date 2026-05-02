#include <stdio.h>
#include <stdlib.h>

#include "libsht.h"

int main(void)
{
    char key[] = "key";
    char value[] = "value";

    lht_t *ht = lht_create();
    if (ht == NULL) {
        fprintf(stderr, "failed lht_create()\n");
        exit(EXIT_FAILURE);
    }
    
    if (lht_set(ht, key, value)) {
        fprintf(stderr, "failed lht_set()\n");
        exit(EXIT_FAILURE);
    }

    if (ht->entries[50341] == NULL) {
        fprintf(stderr, "wtf\n");
        exit(EXIT_FAILURE);
    }
    char *result = lht_get(ht, key);
    if (result != NULL) {
        printf("%s\n", result);
    }

    exit(EXIT_SUCCESS);
}
