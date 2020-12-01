#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 502
#define MAX_WORD_LENGTH 105

// Copyright 2020 Paul Chung

// Turns a given string into all lowercase letters
// Params:
//  - word:  desired string to put into lowercase
// Returns lowercased word
char *strlow(char *word);

// Prints all lines in a file that match the pattern
// Params:
//  - file:              the file to check for results
//  - pattern:           the pattern to look for in each line
//  - ignore_case:       whether to ignore case when using the pattern
//  - print_line_number: whether to print the line number for each matching line
void print_matches_in_file(FILE *file, char *pattern,
                           bool ignore_case, bool print_line_number);

// Print the usage of the program
// Params:
//  - program_name: the name of the program being run (argv[0])
void usage(char *program_name);

// Main entry point of the function
// Parses the input into flags, pattern, and files
// Params:
//  - argc: The number of arguments provided
//  - argv: The list of arguments provided
// Returns the exit status
int main(int argc, char **argv);

// Turns pattern into lowercase in the instance of -i
char *strlow(char *word) {
  for (int i = 0; *(word+i); ++i) {
    *(word+i) = tolower(*(word+i));
  }
  return word;
}

// Prints all the matches in a file
void print_matches_in_file(FILE *file, char *pattern,
                           bool ignore_case, bool print_line_number) {
  char temp[512];
  char copy_temp[512];
  char copy_pattern[512];
  int line_num = 1;
  // Loops through the entire file, looking for the pattern
  while (fgets(temp, MAX_LINE_LENGTH, file) != NULL) {
    // strcpy(copy_temp, temp);
    // strcpy(copy_pattern, pattern);
    snprintf(copy_temp, MAX_WORD_LENGTH, "%s", temp);
    snprintf(copy_pattern, MAX_WORD_LENGTH, "%s", pattern);
    // Handles the 4 different cases of -i and -n the user can input
    if ((strstr(temp, pattern)) != NULL) {
      if (print_line_number == true && ignore_case == false) {
        printf("(%d) %s", line_num, temp);
      }
    }
    if (ignore_case == true && print_line_number == false) {
      strlow(copy_temp);
      strlow(copy_pattern);
      if (strstr(copy_temp, copy_pattern) != NULL) {
        printf("%s", temp);
      }
    }
    if (ignore_case == true && print_line_number == true) {
      strlow(copy_temp);
      strlow(copy_pattern);
      if (strstr(copy_temp, copy_pattern) != NULL) {
        printf("(%d) %s", line_num, temp);
      }
    }
    if (ignore_case == false && print_line_number == false) {
      if (strstr(temp, pattern) != NULL) {
        printf("%s", temp);
      }
    }
    line_num++;
  }
}

// Prints out the usage of the program
void usage(char *program_name) {
    printf("Usage: %s [OPTIONS] pattern file...\n", program_name);
}

// Checks through the given arguments to find options -i and/or -n
// even for the special case in which both -i and -n.
// Finds starting place for files in given command line.
// Checks to see if arguments passed are valid
// Stores the different file types into an array
// Searches for pattern in the given file(s)
int main(int argc, char **argv) {
  bool checkCaps = false;
  bool printLines = false;
  int index = 1;

// Loops through the given arguments to find options -i and/or -n
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      checkCaps = true;
      index = 2;
    }
    if (strcmp(argv[i], "-n") == 0) {
      printLines = true;
      index =2;
    }
  }

// Special case in which both -i and -n
  if (printLines == true && checkCaps == true) {
    index = 3;
  }

  char* fileLibrary[15][15];
  char *searchString = argv[index];

// Finds starting place for files in given command line
  int startingIndex = 2;
  if (index == 2) {
    startingIndex = 3;
  } else if (index == 3) {
    startingIndex = 4;
  }
  int totalFiles = argc - startingIndex;

  // Checks to see if arguments passed are valid
  if (searchString == NULL || totalFiles < 1) {
    usage(argv[0]);
    return 1;
  }

// Stores the different file types into an array
  for (int i = startingIndex; i < argc; i++) {
    fileLibrary[i][i] = argv[i];
  }

  if (sizeof(searchString) >= MAX_WORD_LENGTH) {
    fprintf(stderr, "%s\n", "STRING needs to be less than 100 characters");
    return 2;
  }

// Searches for the pattern in the given file(s)
  int indx = startingIndex;
  for (int i = 0; i < totalFiles; i++) {
    char *filename = fileLibrary[indx][indx];
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
      printf("%s:\n", filename);
      print_matches_in_file(file, searchString, checkCaps, printLines);
    } else {
      fprintf(stderr, "%s %s\n", "Could not open file:",
        fileLibrary[indx][indx]);
    }
    indx++;
  }
}
