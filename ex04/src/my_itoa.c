#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int int_len(int n) {
        char buffer[8192];
        return snprintf(buffer, sizeof buffer, "%i", n);
}

char* my_itoa(int nmb) {
        char * s;
        size_t l, len;
        bool fix_int_min = false;
        if (!nmb) {
                return "0";
        }

        if (-INT_MAX != INT_MIN && nmb == INT_MIN) {
                ++nmb;
                fix_int_min = true;
        }

        len = int_len(nmb);
        if (!(s = malloc(len + 1))) {
                return NULL;
        }
        if (nmb < 0) {
                s[0] = '-';
                nmb = -nmb;
        }
        s[l = len] = '\0';
        while (nmb) {
                s[--len] = (nmb % 10) + '0';
                nmb /= 10;
        }

        if (fix_int_min) {
                --l;
                while (s[l] == '9') {
                        s[l++] = 0;
                }
                if (s[l] == '-') {
                        // realloc +1 and write "-1[0....0]\0"
                } else {
                        ++s[l];
                }
        }

        return s;
}
