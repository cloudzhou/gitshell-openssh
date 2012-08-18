#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>
#include "gitshell.h"
#include "log.h"
 
/* shared gs_auth_fingerprint */
const static char *AUTH_URL = "http://rpc.gitshell.com:9000/private/keyauth";
char *gs_auth_fingerprint = NULL;
char *gs_auth_pubkey = NULL;

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory! */ 
        printf("not enough memory (realloc returned NULL)\n");
        exit(EXIT_FAILURE);
    }
 
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
 
    return realsize;
}

char*
curl_get(CURL *curl_handle, char* url) {
 
    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);    /* will be grown as needed by the realloc above */
    chunk.size = 0;        /* no data at this point */ 
 
    /* specify URL to get */ 
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
 
    /* send all data to this function    */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
    /* some servers don't like requests that are made without a user-agent
         field, so we provide one */ 
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent-gs/1.0");

    /* set timeout */
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 5);
 
    /* get it! */ 
    curl_easy_perform(curl_handle);
 
    /* do not free memory
    if(chunk.memory)
        free(chunk.memory);
    */ 
    return chunk.memory;
}

char*
get_escape_url(CURL *curl_handle, const char *url) {
    char* escape_url = curl_easy_escape(curl_handle, url, 0);
    return escape_url;
}

char*
get_pubkey() {
    CURL *curl_handle = curl_easy_init();
    char *url_buf = malloc(512);
    memset(url_buf, 0, 512);
    char *escape_fingerprint = get_escape_url(curl_handle, gs_auth_fingerprint);
    snprintf(url_buf, 512, "%s/%s", AUTH_URL, escape_fingerprint);
    char *content = curl_get(curl_handle, url_buf);
    curl_free(escape_fingerprint);
    free(url_buf);
    curl_easy_cleanup(curl_handle);
    return content;
}

char*
get_git_command(const char *command) {
    CURL *curl_handle = curl_easy_init();
    char *url_buf = malloc(512);
    memset(url_buf, 0, 512);
    char *escape_command = get_escape_url(curl_handle, command);
    char *escape_fingerprint = get_escape_url(curl_handle, gs_auth_fingerprint);
    snprintf(url_buf, 512, "%s/%s/%s", AUTH_URL, escape_fingerprint, escape_command);
    char *content = curl_get(curl_handle, url_buf);
    curl_free(escape_command);
    curl_free(escape_fingerprint);
    free(url_buf);
    curl_easy_cleanup(curl_handle);
    return content;
}

void
curl_cleanup() {
    /* we're done with libcurl, so clean it up */ 
    curl_global_cleanup();
}

void
free_gs_key() {
    if(gs_auth_fingerprint) {
        free(gs_auth_fingerprint);
    }
    if(gs_auth_pubkey) {
        free(gs_auth_pubkey);
    }
}

