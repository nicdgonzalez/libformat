#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static int64_t multiply_by_10_to_the_nth(int64_t value, int8_t n) {
  while (n-- > 0) value *= 10;
  return value;
}

static int8_t count_digits(int64_t value) {
  int8_t digits = 1;  // The largest number in C has twenty digits.
  while ((value /= 10) != 0) digits++;
  return digits;
}

static char * int_to_str(int64_t value) {
  int64_t copy_of_value = value;
  int8_t digits = count_digits(value);
  char *result = (char *) calloc(digits, sizeof(char));
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


// char * format(char **str, const char *fmt, ...) {}

int32_t main(int32_t argc, const char **argv) {
  const int32_t favorite_number = 7;
  char *string = int_to_str(favorite_number);
  printf("Result: '%s'\n", string);
  free(string);
  // char *fmt = format(&format, "Hello, %s!", "World");
  return 0;
}
