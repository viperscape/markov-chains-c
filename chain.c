#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int read_words(char* filename, char*** words_) {
    FILE *fp = fopen(filename, "r");

    char** words = NULL;
    int words_len = 0;

    while (!feof(fp)) {
        char buf[256];
        int buf_len = 0;

        int ch = 0;
        while(ch != EOF) {
            ch = fgetc(fp);
            if ((ch != ' ') && (ch != '\n') && (ch != '\t') && (ch != EOF)) {
                buf[buf_len] = ch;
                buf_len++;
            }
            else if (buf_len > 0) {
                buf[buf_len] = '\0';

                char* w = malloc(buf_len+1);
                if (w) {
                    strcpy(w, buf);
                    
                    char** ws = realloc(words, (words_len + 1) * sizeof(char*));
                    if (ws) {
                        words = ws;
                        words[words_len] = w;
                        words_len++;

                        buf[0] = '\0';
                        buf_len = 0;
                    }
                }
            }
        }
    }

    *words_ = words;
    return words_len;
}



int find_suffix_names(char** words, int words_len, char* pfx, char** sfx_) {
    char* sfx = malloc(1); // choice of suffixes
    sfx[0] = '\0';
    int sfx_len = 0;

    for (int i = 0; i < words_len; i++) {
        int match = -1;

        // find matching prefix
        for (int n=0; n<strlen(words[i]); n++) {
            if (strlen(words[i]) - n > strlen(pfx)) {
                for (int m=0; m<strlen(pfx); m++) {

                    if (tolower(words[i][n+m]) != tolower(pfx[m])) {
                        match = -1;
                        break;
                    }
                    else match = n;
                }
            }

            if (match > -1) break;
        }
        
        if ((match > -1) && (strlen(words[i]) > match+strlen(pfx))) {
            char* s = realloc(sfx, sfx_len + 2);
            if (s) {
                sfx = s;
                sfx[sfx_len] = words[i][match+strlen(pfx)];
                sfx[sfx_len+1] = '\0';
                sfx_len++;
            }
        }
    }

    *sfx_ = sfx;
    return sfx_len;
}

void build_chain_names (char** words, int words_len, char** chain_, int max_len, char* word) {
    char* chain = NULL;
    int chain_len = 0;

    // pick a random letter to start at
    int len = strlen(word) - 4;
    if (len < 1) len = 1;
    int rand_n = rand() % len;
    
    char pfx[4] = ""; // order of 3
    pfx[0] = word[rand_n];
    pfx[1] = word[rand_n+1];
    pfx[2] = word[rand_n+2];

    // start with prefix
    chain_len = 3;
    char* cs = realloc(chain, chain_len+1);
    if (cs == NULL) return;
    chain = cs;
    strcpy(chain, pfx);

    while(chain_len < max_len) {
        char* sfx = NULL;
        int n = find_suffix_names(words, words_len, pfx, &sfx);

        if (n > 0) {
            int rand_n = (rand() % n) + 1;
            char rand_c = sfx[rand_n-1]; //pick random choice of suffic position
            free(sfx);

            char* cs = realloc(chain, chain_len+2);
            if (cs == NULL) return;

            chain = cs;
            chain[chain_len] = rand_c;
            chain[chain_len+1] = '\0';
            chain_len++;

            // shift prefix along
            pfx[0] = pfx[1];
            pfx[1] = pfx[2];
            pfx[2] = rand_c;
        }
        else break; // no more suffixes available in word list
    }

    if (chain_len > 0) chain[0] = toupper(chain[0]);
    *chain_ = chain;
}

void names_test(char* filename, int len) {
    char** words = NULL;
    int words_len = read_words(filename, &words);

    // pick a random word to start with
    int rand_n = (rand() % words_len) + 1;
    if (rand_n +1 > words_len) rand_n -= 2;

    if (words) {
        char* chain = NULL;
      
        build_chain_names(words, words_len, &chain, len, words[rand_n]);
        printf("%s\n", chain);

        for (int i = 0; i < words_len; i++) {
            free(words[i]);
        }
        free(words);

        if (chain) free(chain);
    }
}