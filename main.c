#include <stdio.h>

#include <curl/curl.h>
#include <jansson.h>

#include "env_reader.h"

#define MAX_LENGTH 300

struct MemoryStruct {
    char *memory;
    size_t size;
    json_t *json;
    json_error_t json_err;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main() {
    char meal[MAX_LENGTH / 4];
    char *encoded_meal;

    char spoonacularurl[MAX_LENGTH];

    char* api_key = get_api_keys("API_KEY");

    struct MemoryStruct response_data;
    response_data.memory = malloc(1);
    response_data.size = 0;
    response_data.json = NULL;

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    printf("\n");
    printf("HH     HH            IIIIIIIIII\n");
    printf("HH     HH                II    \n");
    printf("HH     HH                II    \n");
    printf("HHHHHHHHH                II    \n");
    printf("HH     HH                II    \n");
    printf("HH     HH                II    \n");
    printf("HH     HH            IIIIIIIIII\n");
    printf("Welcome to the AI Recipe Provider in the CLI! Please enter a meal you would like to eat:\n");

    fgets(meal, MAX_LENGTH, stdin);
    // This strips the newline character from the end of the string
    meal[strcspn(meal, "\n")] = 0;

    printf("\nOkay! Let me find you a recipe for %s!", meal);
    // TODO: Loading animation

    if(curl) {
        encoded_meal = curl_easy_escape(curl, meal, 0);
        sprintf(spoonacularurl, "https://api.spoonacular.com/recipes/complexSearch?apiKey=%s&query=%s", api_key, encoded_meal);
        free(api_key);
        curl_free(encoded_meal);

        curl_easy_setopt(curl, CURLOPT_URL, spoonacularurl);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_data);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if(res == CURLE_OK) {
            response_data.json = json_loads(response_data.memory, 0, &(response_data.json_err));
            if(!response_data.json) {
                /* parse error! */
                printf("Parse error on line %d: %s\n", response_data.json_err.line, response_data.json_err.text);
            } else {
                json_t *results, *element;
                size_t index;

                results = json_object_get(response_data.json, "results");
                json_array_foreach(results, index, element)
                {
                    // printf("Pls choose one of the following recipes:\n");
                    // TODO: This has to be a selector
                    // printf("\n%s\n", json_string_value(json_object_get(element, "title")));
                }
                json_decref(response_data.json);
            }
        } else {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
    }

    curl_global_cleanup();
    return 0;
}
