/*scanner.c*/

//
// Scanner for SimpleSQL programming language. The scanner
// reads the input stream and turns the characters into
// language Tokens, such as identifiers, keywords, and
// punctuation.
//
// Emma Scally
// Northwestern University
// CS 211, Winter 2023
//
// Contributing author: Prof. Joe Hummel
//

#include <ctype.h>   // isspace, isdigit, isalpha
#include <stdbool.h> // true, false
#include <stdio.h>
#include <string.h> // stricmp

#include "scanner.h"
#include "util.h"

//
// SimpleSQL keywords, in alphabetical order. Note that "static"
// means the array / variable is not accessible outside this file,
// which is intentional and good design.
//
static char *keywords[] = {"asc",  "avg",   "by",     "count",  "delete",
                           "desc", "from",  "inner",  "insert", "intersect",
                           "into", "join",  "like",   "limit",  "max",
                           "min",  "on",    "order",  "select", "set",
                           "sum",  "union", "update", "values", "where"};

static int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

// T.id = isKeyword(value);

//
// scanner_init
//
// Initializes line number, column number, and value before
// the start of the next input sequence.
//
void scanner_init(int *lineNumber, int *colNumber, char *value) {
  if (lineNumber == NULL || colNumber == NULL || value == NULL)
    panic("one or more parameters are NULL (scanner_init)");

  *lineNumber = 1;
  *colNumber = 1;
  value[0] = '\0'; // empty string ""
}

//
// scanner_nextToken
//
// Returns the next token in the given input stream, advancing the line
// number and column number as appropriate. The token's string-based
// value is returned via the "value" parameter. For example, if the
// token returned is an integer literal, then the value returned is
// the actual literal in string form, e.g. "123". For an identifer,
// the value is the identifer itself, e.g. "ID" or "title". For a
// string literal, the value is the contents of the string literal
// without the quotes.
//
struct Token scanner_nextToken(FILE *input, int *lineNumber, int *colNumber,
                               char *value) {
  if (input == NULL)
    panic("input stream is NULL (scanner_nextToken)");
  if (lineNumber == NULL || colNumber == NULL || value == NULL)
    panic("one or more parameters are NULL (scanner_nextToken)");

  struct Token T;


  //
  // repeatedly input characters one by one until a token is found:
  //
  while (true) {

    int c = fgetc(input);

    if (c == EOF) // no more input, return EOS:
    {
      T.id = SQL_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = '$';
      value[1] = '\0';

      return T;
    }

    // ASK ABT COLNUM

    else if (c == '\n') {
      *lineNumber += 1;
      *colNumber -= *colNumber;
      *colNumber +=1;

      continue;

    } else if (isspace(c)) {
      *colNumber += 1;
    }

    else if (c == '$') // this is also EOF / EOS
    {
  
      T.id = SQL_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;
      

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == ';') {
      T.id = SQL_SEMI_COLON;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber+=1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == '(') {
      T.id = SQL_LEFT_PAREN;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber += 1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == ')') {
      T.id = SQL_RIGHT_PAREN;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber += 1;


      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == '*') {
      T.id = SQL_ASTERISK;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber += 1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == '.') {
      T.id = SQL_DOT;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber += 1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == '#') {
      T.id = SQL_HASH;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber += 1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == ',') {
      T.id = SQL_COMMA;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber += 1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == '#') {
      T.id = SQL_HASH;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber += 1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == '=') {
      T.id = SQL_EQUAL;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber+=1;
      

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

    else if (c == '>') // could be > or >=
    {
      //
      // peek ahead to the next char:
      //
      c = fgetc(input);

      if (c == '=') {
        
        T.id = SQL_GTE;
        T.line = *lineNumber;
        T.col = *colNumber;

        value[0] = '>';
        value[1] = '=';
        value[2] = '\0';
        *colNumber+=2;

        return T;
      }
      ungetc(c, input);

      T.id = SQL_GT;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber+=1;

      value[0] = '>';
      value[1] = '\0';

      return T;
    }

    else if (c == '<') {

      c = fgetc(input);

      if (c == '=') {
        T.id = SQL_LTE;
        T.line = *lineNumber;
        T.col = *colNumber;
        *colNumber+=2;

        value[0] = '<';
        value[1] = '=';
        value[2] = '\0';

        return T;
      } else if (c == '>') {
        T.id = SQL_NOT_EQUAL;
        T.line = *lineNumber;
        T.col = *colNumber;
        *colNumber+=2;

        value[0] = '<';
        value[1] = '>';
        value[2] = '\0';
        

        return T;

      } else {
        ungetc(c, input);

        T.id = SQL_LT;
        T.line = *lineNumber;
        T.col = *colNumber;
        *colNumber += 1;

        value[0] = '<';
        value[1] = '\0';

        return T;
      }
    }

    else if (isdigit(c)) {

      int i = 0;
      value[i] = (char)c;

      while (true) {

        c = fgetc(input);
        i++;

        if (c == '.') {

          value[i] = (char)c;

          while (true) {

            c = fgetc(input);
            i++;

            if (!isdigit(c)) {
              ungetc(c, input);

              T.id = SQL_REAL_LITERAL;
              T.line = *lineNumber;
              T.col = *colNumber;
              *colNumber+=i;
              value[i] = '\0';
              return T;
            }
            value[i] = (char)c;
          }

        } else if (!isdigit(c)) {
          ungetc(c, input);
          T.id = SQL_INT_LITERAL;
          T.line = *lineNumber;
          T.col = *colNumber;
          *colNumber+=i;
          value[i] = '\0';
          return T;
        }
        value[i] = (char)c;
      }
    }

    else if (c == '-') {

      int i = 0;
      value[i] = (char)c;
      c = fgetc(input);
      i++;

      if (c == '-'){
        while(true){
          c = fgetc(input);
          if (c == '\n' || c == -1){
            *lineNumber +=1;
            *colNumber = 1;
            break;
          }
        } continue;
      }

      else if (isdigit(c)) {

    
      value[i] = (char)c;

      while (true) {

        c = fgetc(input);
        i++;

        if (c == '.') {

          value[i] = (char)c;

          while (true) {

            c = fgetc(input);
            i++;

            if (!isdigit(c)) {
              ungetc(c, input);

              T.id = SQL_REAL_LITERAL;
              T.line = *lineNumber;
              T.col = *colNumber;
              *colNumber+=i;
              value[i] = '\0';
              return T;
            }
            value[i] = (char)c;
          }

        } else if (!isdigit(c)) {
          ungetc(c, input);
          T.id = SQL_INT_LITERAL;
          T.line = *lineNumber;
          T.col = *colNumber;
          *colNumber+=i;
          value[i] = '\0';
          return T;
        }
        value[i] = (char)c;
      }
    }
      ungetc(c, input);
         T.id = SQL_UNKNOWN;
         T.line = *lineNumber;
         T.col = *colNumber;

        *colNumber+=i;
        

        value[i] = '\0';

        return T;
       
    }

      else if (c == '+') {

      int i = 0;
      value[i] = (char)c;
      c = fgetc(input);
      i++;

      if (isdigit(c)) {

    
      value[i] = (char)c;

      while (true) {

        c = fgetc(input);
        i++;

        if (c == '.') {

          value[i] = (char)c;

          while (true) {

            c = fgetc(input);
            i++;

            if (!isdigit(c)) {
              ungetc(c, input);

              T.id = SQL_REAL_LITERAL;
              T.line = *lineNumber;
              T.col = *colNumber;
              *colNumber+=i;
              value[i] = '\0';
              return T;
            }
            value[i] = (char)c;
          }

        } else if (!isdigit(c)) {
          ungetc(c, input);
          T.id = SQL_INT_LITERAL;
          T.line = *lineNumber;
          T.col = *colNumber;
          *colNumber+=i;
          value[i] = '\0';
          return T;
        }
        value[i] = (char)c;
      }
      
    }
        ungetc(c, input);
         T.id = SQL_UNKNOWN;
         T.line = *lineNumber;
         T.col = *colNumber;
        *colNumber+=i;
        value[i] = '\0';
        return T;
       
    }

    
      
      else if (c == '"'){
        
        int i = 0;

        while (true){

          c = fgetc(input);
          

          if (c == '"'){
           // ungetc(c, input);
            T.id = SQL_STR_LITERAL;
            T.line = *lineNumber;
            T.col = *colNumber;
            *colNumber+=(i+2);
            value[i] = '\0';
            return T;
          
            }

          if (c == -1 || c == '\n'){
            printf("**WARNING: string literal @ (%d, %d) not terminated properly.\n", *lineNumber, *colNumber);
            break;
          }
          
          value[i] = (char)c;
          i++;
        }
      }

    else if (c =='\'' ){
        int i = 0;
        while (true){
          c = fgetc(input);
          if (c == '\''){
           // ungetc(c, input);
            T.id = SQL_STR_LITERAL;
            T.line = *lineNumber;
            T.col = *colNumber;
            *colNumber+=(i+2);
            value[i] = '\0';
            return T;
            }

          if (c == -1 || c == '\n'){
            printf("**WARNING: string literal @ (%d, %d) not terminated properly.\n", *lineNumber, *colNumber);
            break;
          }
          value[i] = (char)c;
          i++;
        }
      }

    else if (isalpha(c) > 0) {
      int i = 0;
      value[i] = (char)c;
    
       
      while (isalnum(c)>0 || c == '_') {
         
        c = fgetc(input);
        i++;
         
         
         if (isalnum(c)==0 && c != '_') {
            ungetc(c, input);
            value[i]='\0';
            break;
           }
          
         
         value[i] = (char)c;
         
         
      }
      
        for (int i = 0; i < numKeywords; i++) {
             
        if (strcasecmp(keywords[i], value) == 0) {
               T.id = SQL_KEYW_ASC + i;
               T.col = *colNumber;
               T.line = *lineNumber;
               *colNumber+=strlen(keywords[i]);
               return T;
          
             }
        }

      T.id = SQL_IDENTIFIER;
      T.col = *colNumber;
      T.line = *lineNumber;
      *colNumber+=i;
      return T;
         
    }
    //
    // if we get here, then char denotes an UNKNOWN token:
    //
    else {
      T.id = SQL_UNKNOWN;
      T.line = *lineNumber;
      T.col = *colNumber;
      *colNumber+=1;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }
  }
}

// while

// if (strcasecmp(keywords[i], value==0))

//
// execution should never get here, return occurs
// from within loop
//