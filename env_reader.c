#include "env_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** get_api_keys(const char **key, int key_count) {
    FILE* file;
    errno_t err;
    char err_msg[256];
    char **value = (char**)malloc(key_count * sizeof(char*));
    if (value == NULL) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return NULL;
    }

    if ((err = fopen_s(&file, ".env", "r")) != 0) {
        strerror_s(err_msg, sizeof(err_msg), err);
        fprintf(stderr, "cannot open file '%s': %s\n", ".env", err_msg);
        return NULL;
    } else {
        char line[256];

        while (fgets(line, sizeof(line), file)) {
            for (int i = 0; i < key_count; i++) {
                char* key_prefix = (char*)malloc((strlen(key[i]) + 2) * sizeof(char)); // Adding 2 for '=' and null character
                sprintf(key_prefix, "%s=", key[i]);
                if (strncmp(line, key_prefix, strlen(key_prefix)) == 0) {
                    value[i] =  _strdup(line + strlen(key_prefix));
                    if (value[i] == NULL) {
                        fprintf(stderr, "%s not found in .env file\n", key[i]);
                    }
                }
                free(key_prefix);
            }
        }

        fclose(file);

        return value;
    }
}
