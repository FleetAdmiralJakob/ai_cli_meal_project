#include "env_reader.h"
#include <stdio.h>
#include <string.h>

char* get_api_key() {
    FILE* file;
    errno_t err;
    char err_msg[256];
    char* key = "API_KEY=";
    char* value = NULL;

    if ((err = fopen_s(&file, ".env", "r")) != 0) {
        strerror_s(err_msg, sizeof(err_msg), err);
        fprintf(stderr, "cannot open file '%s': %s\n",
                ".env", err_msg);
        return NULL;
    } else {
        char line[256];

        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, key, strlen(key)) == 0) {
                value = _strdup(line + strlen(key));
                break;
            }
        }

        fclose(file);

        if (value == NULL) {
            fprintf(stderr, "API_KEY not found in .env file\n");
        }

        return value;
    }
}
