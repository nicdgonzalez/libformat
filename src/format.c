#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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
  int64_t copy_of_value = value;
  int8_t digits = count_digits(value);
  char *result = (char *) calloc(digits + 1, sizeof(char));
  char *_result = result;

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

static char * float_to_str(float value, int8_t precision) {
  // Ensure precision is between [0, 6].
  precision = set_precision(precision);
  // Convert the left side of the decimal into a string.
  char *lvalue = int_to_str((int64_t) value);
  // "rvalue" refers to the right side of the decimal.
  bool has_rvalue = (((float) (value - (int64_t) value)) != 0);
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
    ((value < 0) ? 1 : 0)
    // Allocate memory for the `rvalue`.
    // If (precision > 0), allocate +1 for the decimal point ('.').
    + ((precision > 0) ? (precision + 1) : 0)
    + str_len(lvalue)
  );

  char *result = (char *) calloc(result_length, sizeof(char));
  int64_t index = 0, jndex = 0;  // For incrementing dest and source.

  // If `value` is negative, append a negative sign.
  if (value < 0) {
    result[index++] = '-';
  }

  // Append the lvalue.
  while ((result[index] = lvalue[jndex++])) {
    index++;
  }

  if (has_rvalue) {
    result[index++] = '.';
    // Pads the result's right value with '0's
    while (precision-- > str_len(rvalue)) {
      result[index++] = '0';
    }
    jndex = 0;
    // Append the right value to `result`.
    while ((result[index++] = rvalue[jndex++])) {}
  }

  // Separated left and right values are no longer needed;
  // free the allocated memory from `int_to_str()`.
  free(lvalue);
  free(rvalue);

  return result;
}

// char * format(char **str, const char *fmt, ...) {}

int32_t main(int32_t argc, const char **argv) {
  const int32_t favorite_number = 7;
  char *string = int_to_str(favorite_number);
  // printf("Result: '%s'\n", string);
  free(string);
  // char *fmt = format(&format, "Hello, %s!", "World");

  string = float_to_str(7.023, set_precision(20));
  printf("float: '%s'\n", string);
  return 0;
}
