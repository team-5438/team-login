#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(const char *fmt, ...) {

  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
    fputc(' ', stderr);
    perror(NULL);
  } else
    fputc('\n', stderr);
  exit(1);
}

char
*concat(const char *s1, const char *s2)
{
  char *res = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(res, s1);
  strcat(res, s2);

  return res;
  free(res);
}
