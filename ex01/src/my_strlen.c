unsigned int my_strlen(char *str) {
  unsigned i = 0;
  for(i = 0;*(str + i) != '\0'; i++);
  return i;
}
