int my_atoi(const char *nptr) {
    int acum = 0;
    int factor = 1;
    
    if(*nptr == '-') {
        factor = -1;
        nptr++;
    }
    
    while((*nptr >= '0')&&(*nptr <= '9')) {
      acum = acum * 10;
      acum = acum + (*nptr - 48);
      nptr++;
    }
    return (factor * acum);
}
