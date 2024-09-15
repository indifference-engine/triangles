#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

float *read_file(const char *const name, const int rows, const int columns)
{
  uint8_t *const temp = malloc(sizeof(uint8_t) * rows * columns);

  if (temp == NULL)
  {
    fprintf(stderr, "Failed to allocate memory for a buffer copy.\n");
    exit(1);
  }

  FILE *file = fopen(name, "rb");

  if (file == NULL)
  {
    fprintf(stderr, "Failed to open file \"%s\".\n", name);
    exit(1);
  }

  if (fread(temp, sizeof(uint8_t), rows * columns, file) != (size_t)(rows * columns))
  {
    fprintf(stderr, "Failed to read file \"%s\".\n", name);
    exit(1);
  }

  fclose(file);

  float *const output = malloc(sizeof(float) * 192 * 256);

  if (output == NULL)
  {
    fprintf(stderr, "Failed to allocate memory for a buffer.\n");
    exit(1);
  }

  for (int i = 0; i < rows * columns; i++)
  {
    output[i] = temp[i] / 255.0f;
  }

  free(temp);

  return output;
}

void write_file(const float *const buffer, const char *const name, const int rows, const int columns)
{
  uint8_t *const temp = malloc(sizeof(uint8_t) * rows * columns);

  if (temp == NULL)
  {
    fprintf(stderr, "Failed to allocate memory for a buffer copy.\n");
    exit(1);
  }

  for (int i = 0; i < rows * columns; i++)
  {
    temp[i] = buffer[i] * 255.0f;
  }

  FILE *file = fopen(name, "wb");

  if (file == NULL)
  {
    fprintf(stderr, "Failed to open file \"%s\".\n", name);
    exit(1);
  }

  if (fwrite(temp, sizeof(uint8_t), rows * columns, file) != (size_t)(rows * columns))
  {
    fprintf(stderr, "Failed to write file \"%s\".\n", name);
    exit(1);
  }

  fclose(file);

  free(temp);
}
