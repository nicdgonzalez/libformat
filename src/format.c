#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

static int64_t multiply_by_10_to_the_nth(int64_t value, int8_t n) {
  while (n-- > 0) {
    value *= 10;
  }
  return value;
}

static int8_t count_digits(int64_t value) {
  int8_t digits = 1;  // The largest number in C has twenty digits.
  while ((value /= 10) != 0) {
    digits++;
  }
  return digits;
}

static char * int_to_str(int64_t value) {
  bool is_negative = false;
  if (value < 0) {
    is_negative = true;
    value *= -1;
  }
  int64_t copy_of_value = value;
  int8_t digits = count_digits(value);
  char *result = (char *) calloc((digits + 1), sizeof(char));
  char *_result = result;

  if (is_negative) {
    *_result++ = '-';
  }

  while (digits != 0) {
    if ((copy_of_value / 10) != 0) {
      copy_of_value /= 10;
      continue;
    }
    *_result++ = ('0' + copy_of_value);
    // Remove the leftmost digit from `value`.
    value -= multiply_by_10_to_the_nth(copy_of_value, --digits);
    copy_of_value = value;
  }
  return result;
}

int8_t set_precision(int8_t precision) {
  // Set `value` equal to lower or upper bound if value falls out of range.
  return (precision >= 0 ? (precision <= 6 ? precision : 6) : 0);
}

int64_t str_len(char *str) {
  int64_t length = 0;
  while ((*str++ != '\0')) {
    length++;
  }
  return length;
}

static char * double_to_str(float value, int8_t precision) {
  bool is_negative = false;
  if (value < 0) {
    is_negative += true;
    value *= -1;
  }
  // Ensure precision is between [0, 6].
  precision = set_precision(precision);
  // Convert the left side of the decimal into a string.
  char *lvalue = int_to_str((int64_t) value);
  // "rvalue" refers to the right side of the decimal.
  // bool has_rvalue = (((float) (value - (int64_t) value)) != 0);
  bool has_rvalue = (precision > 0);
  char *rvalue = NULL;
  if (has_rvalue) {
    rvalue = int_to_str(
      // zero out the left value and leave the right value.
      (value - (int64_t) value)
      // Move the right value to the left side so it is an integer.
      * (multiply_by_10_to_the_nth(10, (precision - 1)))
    );
  }

  int8_t result_length = (
    // if `value` is negative; allocate memory for the sign.
    ((is_negative) ? 1 : 0)
    // Allocate memory for the `rvalue`.
    // If (precision > 0), allocate +1 for the decimal point ('.').
    + ((precision > 0) ? (precision + 1) : 0)
    + str_len(lvalue)
  );

  char *result = (char *) calloc(result_length, sizeof(char));
  int64_t index = 0, j = 0;  // For incrementing dest and source.

  // If `value` is negative, append a negative sign.
  if (is_negative) {
    result[index++] = '-';
  }

  // Append the lvalue.
  while ((result[index] = lvalue[j++])) {
    index++;
  }

  if (has_rvalue) {
    result[index++] = '.';
    // Pads the result's right value with '0's
    while (precision-- > str_len(rvalue)) {
      result[index++] = '0';
    }
    j = 0;
    // Append the right value to `result`.
    while ((result[index++] = rvalue[j++])) {}
  }

  // Separated left and right values are no longer needed;
  // free the allocated memory from `int_to_str()`.
  free(lvalue);
  free(rvalue);

  return result;
}

// printf-style string formatting (This function was mostly written by Github Copilot.)
// I need to go back and redo it with a better understanding of how it works.
char * format(char **str, const char *fmt, ...) {
  int64_t str_size = str_len((char *) fmt);
  *str = (char *) calloc(str_size, sizeof(char));
  int64_t index = 0;
  va_list args;
  va_start(args, fmt);
  int64_t precision = 6;
  char *src = (char *) fmt;
  while (*src != '\0') {
    if (*src != '%') {
      (*str)[index++] = *src++;
      continue;
    }
    // If the next character is a '%', find the format specifier.
    else {
      if (*(src + 1) == '%') {
        (*str)[index++] = *src++;
        continue;
      }
      else if (*(src + 1) == 'd') {
        char *value = int_to_str(va_arg(args, int64_t));
        int64_t j = 0;
        while ((value[j] != '\0')) {
          (*str)[index++] = value[j++];
        }
        free(value);
        src += 2;
        continue;
      }
      else if (*(src + 1) == 'f') {
        char *value = double_to_str(va_arg(args, double), precision);
        int64_t j = 0;
        while ((value[j] != '\0')) {
          (*str)[index++] = value[j++];
        }
        free(value);
        src += 2;
        continue;
      }
      else if (*(src + 1) == 's') {
        char *value = va_arg(args, char *);
        int64_t j = 0;
        while ((value[j] != '\0')) {
          (*str)[index++] = value[j++];
        }
        src += 2;
        continue;
      }
      else if (*(src + 1) == 'c') {
        (*str)[index++] = va_arg(args, int64_t);
        src += 2;
        continue;
      }
      // else if (*(src + 1) == '.') {
      //   if ((*(src + 2) >= '0') || (*(src + 2) <= 9)) {
      //     precision = *(src + 2) - '0';
      //     char *value = double_to_str(va_arg(args, double), precision);
      //     int64_t j = 0;
      //     while ((value[j] != '\0')) {
      //       (*str)[index++] = value[j++];
      //     }
      //     free(value);
      //     src += 4;
      //   }
      //   else {
      //     (*str)[index++] = *src++;
      //   }
      // }
      else {
        (*str)[index++] = *src++;
        continue;
      }
    }
  }
  return *str;
}

int32_t main(int32_t argc, const char **argv) {
  char *fmt = format(&fmt, "Hello, %s! My favorite number is %f.", "World", 7.0702);
  printf("fmt: %s\n", fmt);
  return 0;
}

/**
 * TODO:
 * Fix the following:
 *  - [ ] `format()` does not handle the case where the format specifier is
 *    - [ ] Add support for %c.
 *    - [ ] Add support for %d.
 *    - [ ] Add support for %f.
 *    - [ ] Add support for %s.
 *    - [ ] Add support for %x.
 *    - [ ] Add support for %o.
 *    - [ ] Add support for %b.
 *    - [ ] Add support for %p.
 *    - [ ] Add support for %e.
 *    - [ ] Add support for %g.
 *    - [ ] Add support for %n.
 *    - [ ] Add support for %u.
 *    - [ ] Add support for %.*f.
 *    - [ ] Add support for %*.*f.
 *    - [ ] Add support for %*f.
 *    - [ ] Add support for %*.*s.
 *    - [ ] Add support for %*s.
 *    - [ ] Add support for %*.*d.
 *    - [ ] Add support for %*d.
 *    - [ ] Add support for %*.*x.
 *    - [ ] Add support for %*x.
 *    - [ ] Add support for %*.*o.
 *    - [ ] Add support for %*o.
 *    - [ ] Add support for %*.*b.
 *    - [ ] Add support for %*b.
 *    - [ ] Add support for %*.*e.
 *    - [ ] Add support for %*e.
 *    - [ ] Add support for %*.*g.
 *    - [ ] Add support for %*g.
 *    - [ ] Add support for %*.*n.
 *    - [ ] Add support for %*n.
 *    - [ ] Add support for %*.*u.
 *    - [ ] Add support for %*u.
 *    - [ ] Add support for %*.*p.
 *    - [ ] Add support for %*p.
 *    - [ ] Add support for %*.*c.
 *    - [ ] Add support for %*c.
 * 
 * Fix double_to_str()'s incorrect rvalue.
 * Find random symbol at after final character of fmt.
 */
