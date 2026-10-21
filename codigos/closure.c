#include "closure.h"
#include "dfs.h"


void closure(DFS *dfs, const char *atrib_x) {
    char fecho[strlen(dfs->u) + 1];
    strcpy(fecho, atrib_x);

    for(int i = 0; i < dfs->tam_f; i++) {
        if(strstr(fecho, dfs->f[i].lhs) != NULL && strstr(fecho, dfs->f[i].rhs) == NULL) {
            strcat(fecho, dfs->f[i].rhs);
        }
    }

    printf("%s\n", fecho);
}