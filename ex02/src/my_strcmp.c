int my_strcmp(char *s1, char *s2) {
    const char *p1 = (const char *)s1;
    const char *p2 = (const char *)s2;

    while (*p1 != '\0') {
        if (*p2 == '\0') return  1;
        if (*p2 > *p1)   return -1;
        if (*p1 > *p2)   return  1;
        p1++;
        p2++;
    }

    if (*p2 != '\0') return -1;

    return 0;
}
