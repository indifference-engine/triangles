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

  if (fclose(file) != 0)
  {
    fprintf(stderr, "Failed to close file \"%s\".\n", name);
    exit(1);
  }

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
    const float float_value = buffer[i];

    if (float_value != float_value)
    {
      temp[i] = 47;
    }
    else if (float_value == 1.0f / 0.0f)
    {
      temp[i] = 48;
    }
    else if (float_value == -1.0f / 0.0f)
    {
      temp[i] = 49;
    }
    else if (float_value > 1.0f)
    {
      temp[i] = 50;
    }
    else if (float_value < 0.0f)
    {
      temp[i] = 51;
    }
    else
    {
      const int int_value = float_value * 255.0f;

      if (int_value < 0)
      {
        temp[i] = 0;
      }
      else if (int_value > 255)
      {
        temp[i] = 255;
      }
      else
      {
        temp[i] = int_value;
      }
    }
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

  if (fclose(file) != 0)
  {
    fprintf(stderr, "Failed to close file \"%s\".\n", name);
    exit(1);
  }

  free(temp);
}
