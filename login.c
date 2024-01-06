#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "util.c"

#define FN "timesheet.csv" /* Filename */
#define LEN(X) ((int)sizeof(X) / sizeof(X[0]))

/**
 * @brief return current date
 *
 * @return pointer to date
 */
char *
date()
{
  char *date = malloc(sizeof(char[26]));
  time_t t;
  struct tm tm;
  t = time(NULL);
  tm = *localtime(&t);

  sprintf(date, "%d/%d/%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  return date;
}

/**
 * @brief return current time
 *
 * @return pointer to time
 */
char *
curtime()
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *ct = malloc(sizeof(char[20])), *per = malloc(sizeof(char[5]));

  /* convert time to 12 hour format */
  per = tm.tm_hour >= 12 ? "PM" : "AM";
  tm.tm_hour > 12 ? tm.tm_hour %= 12 : tm.tm_hour;

  /* format and return */
  sprintf(ct, "%02d:%02d%s", tm.tm_hour, tm.tm_min, per);

  return ct;
}

int
main(int argc, char *argv[])
{
  FILE *ts; /* timesheet file pointer */
  int sz, line;

  /* make sure the file exists */
  if (access(FN, F_OK)) { /* we check if the file exists */
    fprintf(stderr, "File doesn't exist, attemting to make a new one.\n");

    FILE *fc; /* file creation file stream */
    fc = fopen(FN, "w");
    fclose(fc);
  }

  ts = fopen(FN, "rw+");
  fseek(ts, 0, SEEK_END);
  sz = ftell(ts);
  fseek(ts, 0, SEEK_SET);

  if (sz == 0)
    fprintf(ts, "id,name,total days in,day in...");

  while (1) {
    char *buff = malloc(sizeof(char) * 50), file[100][1000];
    int curline, i;

    /* read id from stdin */
    if (fgets(buff, sizeof(buff), stdin) == NULL)
      continue;

    /* replace newline with string end */
    if (buff[strlen(buff) - 1] == '\n')
      buff[strlen(buff) - 1] = '\0';

    /* trigger error if char in input */
    for (i = 0; i < strlen(buff); i++)
      if (!isdigit(buff[i])) {
        strcpy(buff, "\0");
        break;
      }

    if (strcmp(buff, "\0") == 0) {
      printf("Impropper input\n");
      continue;
    }

    /* read file into array */
    curline = 1;
    while (fgets(file[curline], sizeof(file[curline]), ts) != NULL)
      curline++;

    /* find line that contains the id# */
    line = -1;
    for (curline = 1; curline < LEN(file); curline++) {
      if (strstr(file[curline], buff)) {
        line = curline;
        break;
      }
    }

    if (line == -1) {
      printf("You're not in the timesheet\n");
      continue;
    }
    free(buff);

    /* skip if user already logged in today */
    if (strstr(file[line], date())) {
      printf("You've already logged in today\n");
      continue;
    }

    /* remove everything from file */
    ftruncate(fileno(ts), 0);

    /* increment day */
    /* HACK: the magic numbers would be better if dynamically allocated */
    char replacement[1024], final[2000], *tok;

    /* get contents of third column (should be the days in) and save the rest
         * for later */
    strcpy(replacement, strtok(file[line], ","));
    strcat(replacement, concat(",", strtok(NULL, ",")));
    tok = strtok(NULL, ",");

    sprintf(final, "%s,%d,", replacement, atoi(tok) + 1);

    /* grab all the remaining bits of info */
    while ((tok = strtok(NULL, ",")) != NULL) {
      /* replace newline with string end */
      if (tok[strlen(tok) - 1] == '\n')
        tok[strlen(tok) - 1] = '\0';
      if (tok[strlen(tok) - 1] != ',')
        strcat(final, concat(tok, ","));
    }
    /* tack the current date onto the end */
    strcat(final, date());
    strcat(final, concat(" ", curtime()));

    /* add a newline, and write back to the array */
    strcat(final, "\n");
    strcpy(file[line], final);

    /* write back to file */
    for (curline = 1; curline < LEN(file); curline++) {
      fseek(ts, 0, SEEK_END);
      fprintf(ts, "%s", file[curline]);
    }

    /* sync all changes onto disk */
    fsync(fileno(ts));
  }
  fclose(ts);

  return 0;
}
