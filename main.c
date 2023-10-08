#include <stdio.h>

#include <curl/curl.h>

#define MAX_LENGTH 200

int main() {
    char meal[MAX_LENGTH / 2];
    char *encoded_meal;
    char spoonacularurl[MAX_LENGTH];

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

    printf("Okay! Let me find you a recipe for %s!", meal);
    if(curl) {
        encoded_meal = curl_easy_escape(curl, meal, 0);
        sprintf(spoonacularurl, "https://api.spoonacular.com/recipes/complexSearch?query=%s", encoded_meal);
        curl_easy_setopt(curl, CURLOPT_URL, spoonacularurl);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_free(encoded_meal);
    }


    curl_global_cleanup();
    return 0;
}
