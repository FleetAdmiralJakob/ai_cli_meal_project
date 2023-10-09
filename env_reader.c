#include "env_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_api_keys(const char* key) {
    FILE* file;
    errno_t err;
    char err_msg[256];
    char* key_prefix = (char*)malloc((strlen(key) + 2) * sizeof(char)); // Adding 2 for '=' and null character
    sprintf(key_prefix, "%s=", key);
    char* value = NULL;

    if ((err = fopen_s(&file, ".env", "r")) != 0) {
        strerror_s(err_msg, sizeof(err_msg), err);
        fprintf(stderr, "cannot open file '%s': %s\n", ".env", err_msg);
        free(key_prefix);
        return NULL;
    } else {
        char line[256];

        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, key_prefix, strlen(key_prefix)) == 0) {
                value = _strdup(line + strlen(key_prefix));
                break;
            }
        }

        fclose(file);
        free(key_prefix);

        if (value == NULL) {
            fprintf(stderr, "%s not found in .env file\n", key);
        }

        return value;
    }
}
