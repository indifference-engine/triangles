#include "opaque_triangle.h"

static float floor(float x)
{
  return x < 0 ? ((int)x) - 1 : (int)x;
}

static void copy_f32s(
    const float *const source,
    float *const destination,
    const int quantity)
{
  for (int index = 0; index < quantity; index++)
  {
    destination[index] = source[index];
  }
}

static void multiply_f32s_f32(
    const float *const multipliers,
    const float multiplicand,
    float *const products,
    const int quantity)
{
  for (int index = 0; index < quantity; index++)
  {
    products[index] = multipliers[index] * multiplicand;
  }
}

static void subtract_f32s_f32s(
    const float *const minuends,
    const float *const subtrahends,
    float *const differences,
    const int quantity)
{
  for (int index = 0; index < quantity; index++)
  {
    differences[index] = minuends[index] - subtrahends[index];
  }
}

static void multiply_add_f32s_f32_f32s(
    const float *const multipliers,
    const float multiplicand,
    const float *const addends,
    float *const results,
    const int quantity)
{
  for (int index = 0; index < quantity; index++)
  {
    results[index] = multipliers[index] * multiplicand + addends[index];
  }
}

static void add_f32s_f32s(
    const float *const augends,
    const float *const addends,
    float *const sums,
    const int quantity)
{
  for (int index = 0; index < quantity; index++)
  {
    sums[index] = augends[index] + addends[index];
  }
}

static void sort_top_to_bottom(
    float *const vertices,
    const int f32s_per_vertex,
    const float **const top,
    const float **const middle,
    const float **const bottom)
{
  float *const a = vertices;
  float *const b = &a[f32s_per_vertex];
  float *const c = &b[f32s_per_vertex];

  const float a_row = *a;
  const float b_row = *b;
  const float c_row = *c;

  if (a_row <= b_row)
  {
    if (b_row <= c_row)
    {
      *top = a;
      *middle = b;
      *bottom = c;
    }
    else if (a_row <= c_row)
    {
      *top = a;
      *middle = c;
      *bottom = b;
    }
    else
    {
      *top = c;
      *middle = a;
      *bottom = b;
    }
  }
  else if (a_row <= c_row)
  {
    *top = b;
    *middle = a;
    *bottom = c;
  }
  else if (b_row <= c_row)
  {
    *top = b;
    *middle = c;
    *bottom = a;
  }
  else
  {
    *top = c;
    *middle = b;
    *bottom = a;
  }
}

static void row(
    const int texture_rows,
    const int texture_columns,
    const float *const texture_reds,
    const float *const texture_greens,
    const float *const texture_blues,
    const float row_accumulators[14],
    const int camera_row,
    const int viewport_columns,
    float *const viewport_reds,
    float *const viewport_greens,
    float *const viewport_blues,
    float *const viewport_opacities,
    float *const viewport_depths)
{
  float per_columns[6];
  float accumulators[6];

  const float first_camera_column = row_accumulators[0];
  const float second_camera_column = row_accumulators[7];

  subtract_f32s_f32s(&row_accumulators[8], &row_accumulators[1], per_columns, 6);
  multiply_f32s_f32(per_columns, 1.0f / (second_camera_column - first_camera_column), per_columns, 6);

  int clamped_left_camera_column, clamped_right_camera_column;

  if (first_camera_column < second_camera_column)
  {
    clamped_left_camera_column = floor(first_camera_column);
    clamped_right_camera_column = floor(second_camera_column);
    copy_f32s(&row_accumulators[1], accumulators, 6);
  }
  else
  {
    clamped_left_camera_column = floor(second_camera_column);
    clamped_right_camera_column = floor(first_camera_column);
    copy_f32s(&row_accumulators[8], accumulators, 6);
  }

  if (clamped_left_camera_column < 0)
  {
    multiply_add_f32s_f32_f32s(per_columns, -clamped_left_camera_column, accumulators, accumulators, 5);
    clamped_left_camera_column = 0;
  }

  clamped_right_camera_column = clamped_right_camera_column < viewport_columns ? clamped_right_camera_column : viewport_columns;

  const int left_index = camera_row * viewport_columns + clamped_left_camera_column;
  const int right_index = left_index + clamped_right_camera_column - clamped_left_camera_column;

  for (int camera_index = left_index; camera_index < right_index; camera_index++)
  {
    const float source_depth = accumulators[0];

    if (source_depth < viewport_depths[camera_index])
    {

      const int texture_row = floor(texture_rows * accumulators[1]);

      const int wrapped_texture_row = texture_row >= 0 ? texture_row % texture_rows : texture_rows + ((texture_row + 1) % texture_rows) - 1;

      const int texture_column = floor(texture_columns * accumulators[2]);

      const int wrapped_texture_column = texture_column >= 0 ? texture_column % texture_columns : texture_columns + ((texture_column + 1) % texture_columns) - 1;

      const int texture_index = wrapped_texture_row * texture_columns + wrapped_texture_column;

      viewport_depths[camera_index] = source_depth;
      viewport_opacities[camera_index] = 1.0f;
      viewport_reds[camera_index] = accumulators[3] * texture_reds[texture_index];
      viewport_greens[camera_index] = accumulators[4] * texture_greens[texture_index];
      viewport_blues[camera_index] = accumulators[5] * texture_blues[texture_index];
    }

    add_f32s_f32s(accumulators, per_columns, accumulators, 6);
  }
}

void opaque_triangle(
    const int texture_rows,
    const int texture_columns,
    const float *const texture_reds,
    const float *const texture_greens,
    const float *const texture_blues,
    const float a_v,
    const float a_u,
    const float a_red,
    const float a_green,
    const float a_blue,
    const float a_row,
    const float a_column,
    const float a_depth,
    const float b_v,
    const float b_u,
    const float b_red,
    const float b_green,
    const float b_blue,
    const float b_row,
    const float b_column,
    const float b_depth,
    const float c_v,
    const float c_u,
    const float c_red,
    const float c_green,
    const float c_blue,
    const float c_row,
    const float c_column,
    const float c_depth,
    const int viewport_rows,
    const int viewport_columns,
    float *const viewport_reds,
    float *const viewport_greens,
    float *const viewport_blues,
    float *const viewport_opacities,
    float *const viewport_depths)
{
  float vertices[] = {
      a_row,    // 0
      a_column, // 1
      a_depth,  // 2
      a_v,      // 3
      a_u,      // 4
      a_red,    // 5
      a_green,  // 6
      a_blue,   // 7
      b_row,    // 8
      b_column, // 9
      b_depth,  // 10
      b_v,      // 11
      b_u,      // 12
      b_red,    // 13
      b_green,  // 14
      b_blue,   // 15
      c_row,    // 16
      c_column, // 17
      c_depth,  // 18
      c_v,      // 19
      c_u,      // 20
      c_red,    // 21
      c_green,  // 22
      c_blue,   // 23
  };

  const float *top;
  const float *middle;
  const float *bottom;

  sort_top_to_bottom(vertices, 8, &top, &middle, &bottom);

  float deltas[18];
  subtract_f32s_f32s(bottom, top, deltas, 8);
  subtract_f32s_f32s(middle, top, &deltas[8], 8);

  float per_rows[17];
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], per_rows, 7);
  const float short_camera_row_delta = deltas[8];
  multiply_f32s_f32(&deltas[9], 1.0f / short_camera_row_delta, &per_rows[7], 7);

  float accumulators[17];

  const int top_camera_row = floor(top[0]);
  const int clamped_top_camera_row = top_camera_row < 0 ? 0 : top_camera_row;

  if (top_camera_row < 0)
  {
    const float skipped_rows = -top_camera_row;
    multiply_add_f32s_f32_f32s(per_rows, skipped_rows, &top[1], accumulators, 7);
    multiply_add_f32s_f32_f32s(&per_rows[7], skipped_rows, &top[1], &accumulators[7], 7);
  }
  else
  {
    copy_f32s(&top[1], accumulators, 7);
    copy_f32s(&top[1], &accumulators[7], 7);
  }

  const int middle_camera_row = floor(middle[0]);
  const int clamped_middle_camera_row = middle_camera_row < 0 ? 0 : (middle_camera_row > viewport_rows ? viewport_rows : middle_camera_row);

  for (int camera_row = clamped_top_camera_row; camera_row < clamped_middle_camera_row; camera_row++)
  {
    row(
        texture_rows,
        texture_columns,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        camera_row,
        viewport_columns,
        viewport_reds,
        viewport_greens,
        viewport_blues,
        viewport_opacities,
        viewport_depths);

    add_f32s_f32s(accumulators, per_rows, accumulators, 14);
  }

  subtract_f32s_f32s(bottom, middle, deltas, 8);
  multiply_f32s_f32(&deltas[1], 1.0f / deltas[0], &per_rows[7], 7);

  if (middle_camera_row < 0)
  {
    const float skipped_rows = -middle_camera_row;
    multiply_add_f32s_f32_f32s(&per_rows[7], skipped_rows, &middle[1], &accumulators[7], 7);
  }
  else
  {
    copy_f32s(&middle[1], &accumulators[7], 7);
  }

  const int bottom_camera_row = *bottom;
  const int clamped_bottom_camera_row = viewport_rows < bottom_camera_row ? viewport_rows : bottom_camera_row;

  for (int camera_row = clamped_middle_camera_row; camera_row < clamped_bottom_camera_row; camera_row++)
  {
    row(
        texture_rows,
        texture_columns,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        camera_row,
        viewport_columns,
        viewport_reds,
        viewport_greens,
        viewport_blues,
        viewport_opacities,
        viewport_depths);

    add_f32s_f32s(accumulators, per_rows, accumulators, 14);
  }
}
