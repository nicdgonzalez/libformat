#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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
  return (precision >= 0 ? precision : (precision <= 6 ? precision : 6));
}

int64_t str_length(char *str) {
  int64_t length = 0;
  while ((*str++ != '\0')) length++;
  return length;
}

static char * float_to_str(float value, int8_t precision) {
  // We'll need to allocate an extra byte for the '-' if the value is
  // negative so note that here before modifying value in any way.
  bool is_negative = (value < 0);
  // If `value` doesn't have a decimal point value (x.123456), we can
  // skip where we convert that part to a string.
  bool has_decimal_value = (((float) (value - (int64_t) value)) != 0);

  char *decimal_digits = NULL;
  int64_t index;
  char *temp;

  if (has_decimal_value) {
    // Value has a decimal point value; convert it to a string:
    precision = set_precision(precision);
    decimal_digits = (char *) calloc(precision, sizeof(char));
    index = 0;

    // Take the value beyond the decimal point and multiply by
    // (10^precision) so it will be in the form of a regular integer.
    // Then we can perform the helper `int_to_str` function to get the
    // decimal value as a string.
    int decimal_as_int = (
      (value - (int64_t) value)
      * (multiply_by_10_to_the_nth(10, (precision - 1)))
    );
    char *decimal_str = int_to_str(decimal_as_int);
    // We can't properly free `decimal_str` if we modify the pointer
    // so we'll modify a pointer of a pointer instead.
    temp = decimal_str;

    // Pad the decimal string with '0's to maintain the proper precision.
    while (precision-- > str_length(decimal_str)) {
      decimal_digits[index++] = '0';
    }
    // Take the content of `decimal_str` and append it to `decimal_digits`.
    while (precision--) {
      decimal_digits[index++] = *temp++;
    }
    // We no longer need `decimal_str`; free the allocated memory.
    free(decimal_str);
  }

  char *real_value = int_to_str((int64_t) value);

  int8_t result_size = (
    (is_negative ? 1 : 0)  // additional byte for negative sign (if negative).
    + (has_decimal_value ? (str_length(decimal_digits) + 1) : 0)
    + str_length(real_value)
  );

  printf("%hhd\n", result_size);

  char *result = (char *) calloc(result_size, sizeof(char));
  index = 0;
  int64_t index2 = 0;

  if (is_negative) {
    result[index++] = '-';
  }

  while ((result[index] = real_value[index2++])) {
    index++;
  }

  index2 = 0;
  if (has_decimal_value) {
    result[index++] = '.';
    while ((result[index++] = decimal_digits[index2++])) {}
    free(decimal_digits);
  }

  free(real_value);
  return result;
}

// char * format(char **str, const char *fmt, ...) {}

int32_t main(int32_t argc, const char **argv) {
  const int32_t favorite_number = 7;
  char *string = int_to_str(favorite_number);
  // printf("Result: '%s'\n", string);
  free(string);
  // char *fmt = format(&format, "Hello, %s!", "World");

  string = float_to_str(7.023456, set_precision(6));
  printf("float: '%s'\n", string);
  return 0;
}
