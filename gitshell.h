#ifndef GITSHELL_H
#define GITSHELL_H

char*   get_pubkey();
char*   get_git_command(const char *);
void    curl_cleanup();
void    free_gs_key();

struct MemoryStruct {
    char *memory;
    size_t size;
};

#endif
