#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

char *str_replace(char *orig, char *rep, char *with);

int main(){
    FILE *file_handle = fopen("links", "r");
    char src[256];
    char dest[256];

    int link = fscanf(file_handle, "%s -> %s", src, dest);
    if (link == EOF){
        return 1;
    }

    char *pwd = getenv("PWD");
    char *home = getenv("HOME");
    strcat(home, "/");
    int line = 0;
    while (link != EOF) {
        line++;
        // Handle the path1
        char path1[256];
        strcpy(path1, pwd);
        strcat(path1, "/");
        strcat(path1, src);
        // Handle the path2
        char path2[256];
        strcpy(path2, str_replace(dest, "~/", home));
        // Create the link
        int lns = symlink(path1, path2);
        if (lns == -1){
            printf(RED "%d %s: " RESET "%s -> %s\n" , line, strerror(errno), path1, path2);
        }
        else {
            printf(GRN "%s -> %s" RESET ,path1, path2);
        }
        link = fscanf(file_handle, "%s -> %s\n", src, dest);
    }
    return 0;

}
// You must free the result if result is non-NULL.
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
