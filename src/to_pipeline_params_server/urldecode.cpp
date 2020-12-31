#include "urldecode.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "urldecode.h"

char *urlDecode(const char *str) {
    int d = 0; 
    char *dStr = (char *)(malloc(strlen(str) + 1));
    char eStr[] = "00"; 
    strcpy(dStr, str);

    while(!d) {
        d = 1;
        for(size_t i = 0; i < strlen(dStr); ++i) {
            if(dStr[i] == '%') {
                if(dStr[i+1] == 0)
                    return dStr;
                if(isxdigit(dStr[i+1]) && isxdigit(dStr[i+2])) {
                    d = 0;
                    eStr[0] = dStr[i+1];
                    eStr[1] = dStr[i+2];
                    long int x = strtol(eStr, nullptr, 16);
                    memmove(&dStr[i+1], &dStr[i+3], strlen(&dStr[i+3])+1);
                    dStr[i] = x;
                }
            }
        }
    }
    return dStr;
}
