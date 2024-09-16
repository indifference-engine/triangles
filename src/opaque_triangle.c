#include "opaque_triangle.h"

static void opaque_row(
    const int texture_rows,
    const int texture_columns,
    const float *const texture_reds,
    const float *const texture_greens,
    const float *const texture_blues,
    const float row_accumulators[14],
    const int row,
    const int viewport_columns,
    float *const viewport_opacities,
    float *const viewport_reds,
    float *const viewport_greens,
    float *const viewport_blues,
    float *const viewport_depths)
{
  float per_columns[6];
  float accumulators[6];

  const float first_column = row_accumulators[0];
  const float second_column = row_accumulators[7];

  per_columns[0] = row_accumulators[8] - row_accumulators[1];
  per_columns[1] = row_accumulators[9] - row_accumulators[2];
  per_columns[2] = row_accumulators[10] - row_accumulators[3];
  per_columns[3] = row_accumulators[11] - row_accumulators[4];
  per_columns[4] = row_accumulators[12] - row_accumulators[5];
  per_columns[5] = row_accumulators[13] - row_accumulators[6];

  const float multiplicand = 1.0f / (second_column - first_column);
  per_columns[0] *= multiplicand;
  per_columns[1] *= multiplicand;
  per_columns[2] *= multiplicand;
  per_columns[3] *= multiplicand;
  per_columns[4] *= multiplicand;
  per_columns[5] *= multiplicand;

  int clamped_left_column, clamped_right_column;

  if (first_column < second_column)
  {
    clamped_left_column = first_column < 0 ? ((int)first_column) - 1 : first_column;
    clamped_right_column = second_column < 0 ? ((int)second_column) - 1 : second_column;
    accumulators[0] = row_accumulators[1];
    accumulators[1] = row_accumulators[2];
    accumulators[2] = row_accumulators[3];
    accumulators[3] = row_accumulators[4];
    accumulators[4] = row_accumulators[5];
    accumulators[5] = row_accumulators[6];
  }
  else
  {
    clamped_left_column = second_column < 0 ? ((int)second_column) - 1 : second_column;
    clamped_right_column = first_column < 0 ? ((int)first_column) - 1 : first_column;
    accumulators[0] = row_accumulators[8];
    accumulators[1] = row_accumulators[9];
    accumulators[2] = row_accumulators[10];
    accumulators[3] = row_accumulators[11];
    accumulators[4] = row_accumulators[12];
    accumulators[5] = row_accumulators[13];
  }

  if (clamped_left_column < 0)
  {
    const float negated_clamp_left_column = -clamped_left_column;
    accumulators[0] += per_columns[0] * negated_clamp_left_column;
    accumulators[1] += per_columns[1] * negated_clamp_left_column;
    accumulators[2] += per_columns[2] * negated_clamp_left_column;
    accumulators[3] += per_columns[3] * negated_clamp_left_column;
    accumulators[4] += per_columns[4] * negated_clamp_left_column;
    accumulators[5] += per_columns[5] * negated_clamp_left_column;
    clamped_left_column = 0;
  }

  clamped_right_column = clamped_right_column < viewport_columns ? clamped_right_column : viewport_columns;

  const int left_index = row * viewport_columns + clamped_left_column;
  const int right_index = left_index + clamped_right_column - clamped_left_column;

  for (int viewport_index = left_index; viewport_index < right_index; viewport_index++)
  {
    const float source_depth = accumulators[0];

    if (source_depth < viewport_depths[viewport_index])
    {
      const float texture_row_float = texture_rows * accumulators[1];
      const int texture_row = texture_row_float < 0 ? ((int)texture_row_float) - 1 : texture_row_float;

      const int wrapped_texture_row = texture_row >= 0 ? texture_row % texture_rows : texture_rows + ((texture_row + 1) % texture_rows) - 1;

      const float texture_column_float = texture_columns * accumulators[2];
      const int texture_column = texture_column_float < 0 ? ((int)texture_column_float) - 1 : texture_column_float;

      const int wrapped_texture_column = texture_column >= 0 ? texture_column % texture_columns : texture_columns + ((texture_column + 1) % texture_columns) - 1;

      const int texture_index = wrapped_texture_row * texture_columns + wrapped_texture_column;

      viewport_depths[viewport_index] = source_depth;
      viewport_opacities[viewport_index] = 1.0f;
      viewport_reds[viewport_index] = accumulators[3] * texture_reds[texture_index];
      viewport_greens[viewport_index] = accumulators[4] * texture_greens[texture_index];
      viewport_blues[viewport_index] = accumulators[5] * texture_blues[texture_index];
    }

    accumulators[0] += per_columns[0];
    accumulators[1] += per_columns[1];
    accumulators[2] += per_columns[2];
    accumulators[3] += per_columns[3];
    accumulators[4] += per_columns[4];
    accumulators[5] += per_columns[5];
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
    float *const viewport_opacities,
    float *const viewport_reds,
    float *const viewport_greens,
    float *const viewport_blues,
    float *const viewport_depths)
{
  const float a[8] = {
      a_row,    // 0
      a_column, // 1
      a_depth,  // 2
      a_v,      // 3
      a_u,      // 4
      a_red,    // 5
      a_green,  // 6
      a_blue,   // 7
  };

  const float b[8] = {
      b_row,    // 0
      b_column, // 1
      b_depth,  // 2
      b_v,      // 3
      b_u,      // 4
      b_red,    // 5
      b_green,  // 6
      b_blue,   // 7
  };

  const float c[8] = {
      c_row,    // 0
      c_column, // 1
      c_depth,  // 2
      c_v,      // 3
      c_u,      // 4
      c_red,    // 5
      c_green,  // 6
      c_blue,   // 7
  };

  const float *top;
  const float *middle;
  const float *bottom;

  if (a_row <= b_row)
  {
    if (b_row <= c_row)
    {
      top = a;
      middle = b;
      bottom = c;
    }
    else if (a_row <= c_row)
    {
      top = a;
      middle = c;
      bottom = b;
    }
    else
    {
      top = c;
      middle = a;
      bottom = b;
    }
  }
  else if (a_row <= c_row)
  {
    top = b;
    middle = a;
    bottom = c;
  }
  else if (b_row <= c_row)
  {
    top = b;
    middle = c;
    bottom = a;
  }
  else
  {
    top = c;
    middle = b;
    bottom = a;
  }

  float deltas[16];
  deltas[0] = bottom[0] - top[0];
  deltas[1] = bottom[1] - top[1];
  deltas[2] = bottom[2] - top[2];
  deltas[3] = bottom[3] - top[3];
  deltas[4] = bottom[4] - top[4];
  deltas[5] = bottom[5] - top[5];
  deltas[6] = bottom[6] - top[6];
  deltas[7] = bottom[7] - top[7];
  deltas[8] = middle[0] - top[0];
  deltas[9] = middle[1] - top[1];
  deltas[10] = middle[2] - top[2];
  deltas[11] = middle[3] - top[3];
  deltas[12] = middle[4] - top[4];
  deltas[13] = middle[5] - top[5];
  deltas[14] = middle[6] - top[6];
  deltas[15] = middle[7] - top[7];

  const float multiplicand_a = 1.0f / deltas[0];
  const float short_row_delta = deltas[8];
  const float multiplicand_b = 1.0f / short_row_delta;

  float per_rows[14];
  per_rows[0] = deltas[1] * multiplicand_a;
  per_rows[1] = deltas[2] * multiplicand_a;
  per_rows[2] = deltas[3] * multiplicand_a;
  per_rows[3] = deltas[4] * multiplicand_a;
  per_rows[4] = deltas[5] * multiplicand_a;
  per_rows[5] = deltas[6] * multiplicand_a;
  per_rows[6] = deltas[7] * multiplicand_a;
  per_rows[7] = deltas[9] * multiplicand_b;
  per_rows[8] = deltas[10] * multiplicand_b;
  per_rows[9] = deltas[11] * multiplicand_b;
  per_rows[10] = deltas[12] * multiplicand_b;
  per_rows[11] = deltas[13] * multiplicand_b;
  per_rows[12] = deltas[14] * multiplicand_b;
  per_rows[13] = deltas[15] * multiplicand_b;

  float accumulators[14];

  const float top_row_float = top[0];
  const int top_row = top_row_float < 0 ? ((int)top_row_float) - 1 : top_row_float;
  const int clamped_top_row = top_row < 0 ? 0 : top_row;

  if (top_row < 0)
  {
    const float skipped_rows = -top_row;
    accumulators[0] = per_rows[0] * skipped_rows + top[1];
    accumulators[1] = per_rows[1] * skipped_rows + top[2];
    accumulators[2] = per_rows[2] * skipped_rows + top[3];
    accumulators[3] = per_rows[3] * skipped_rows + top[4];
    accumulators[4] = per_rows[4] * skipped_rows + top[5];
    accumulators[5] = per_rows[5] * skipped_rows + top[6];
    accumulators[6] = per_rows[6] * skipped_rows + top[7];
    accumulators[7] = per_rows[7] * skipped_rows + top[1];
    accumulators[8] = per_rows[8] * skipped_rows + top[2];
    accumulators[9] = per_rows[9] * skipped_rows + top[3];
    accumulators[10] = per_rows[10] * skipped_rows + top[4];
    accumulators[11] = per_rows[11] * skipped_rows + top[5];
    accumulators[12] = per_rows[12] * skipped_rows + top[6];
    accumulators[13] = per_rows[13] * skipped_rows + top[7];
  }
  else
  {
    accumulators[0] = top[1];
    accumulators[1] = top[2];
    accumulators[2] = top[3];
    accumulators[3] = top[4];
    accumulators[4] = top[5];
    accumulators[5] = top[6];
    accumulators[6] = top[7];
    accumulators[7] = top[1];
    accumulators[8] = top[2];
    accumulators[9] = top[3];
    accumulators[10] = top[4];
    accumulators[11] = top[5];
    accumulators[12] = top[6];
    accumulators[13] = top[7];
  }

  const float middle_row_float = middle[0];
  const int middle_row = middle_row_float < 0 ? ((int)middle_row_float) - 1 : middle_row_float;
  const int clamped_middle_row = middle_row < 0 ? 0 : (middle_row > viewport_rows ? viewport_rows : middle_row);

  for (int row = clamped_top_row; row < clamped_middle_row; row++)
  {
    opaque_row(
        texture_rows,
        texture_columns,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        row,
        viewport_columns,
        viewport_opacities,
        viewport_reds,
        viewport_greens,
        viewport_blues,
        viewport_depths);

    accumulators[0] += per_rows[0];
    accumulators[1] += per_rows[1];
    accumulators[2] += per_rows[2];
    accumulators[3] += per_rows[3];
    accumulators[4] += per_rows[4];
    accumulators[5] += per_rows[5];
    accumulators[6] += per_rows[6];
    accumulators[7] += per_rows[7];
    accumulators[8] += per_rows[8];
    accumulators[9] += per_rows[9];
    accumulators[10] += per_rows[10];
    accumulators[11] += per_rows[11];
    accumulators[12] += per_rows[12];
    accumulators[13] += per_rows[13];
  }

  deltas[0] = bottom[0] - middle[0];
  deltas[1] = bottom[1] - middle[1];
  deltas[2] = bottom[2] - middle[2];
  deltas[3] = bottom[3] - middle[3];
  deltas[4] = bottom[4] - middle[4];
  deltas[5] = bottom[5] - middle[5];
  deltas[6] = bottom[6] - middle[6];
  deltas[7] = bottom[7] - middle[7];

  const float multiplicand_c = 1.0f / deltas[0];
  per_rows[7] = deltas[1] * multiplicand_c;
  per_rows[8] = deltas[2] * multiplicand_c;
  per_rows[9] = deltas[3] * multiplicand_c;
  per_rows[10] = deltas[4] * multiplicand_c;
  per_rows[11] = deltas[5] * multiplicand_c;
  per_rows[12] = deltas[6] * multiplicand_c;
  per_rows[13] = deltas[7] * multiplicand_c;

  if (middle_row < 0)
  {
    const float skipped_rows = -middle_row;

    accumulators[7] = per_rows[7] * skipped_rows + middle[1];
    accumulators[8] = per_rows[8] * skipped_rows + middle[2];
    accumulators[9] = per_rows[9] * skipped_rows + middle[3];
    accumulators[10] = per_rows[10] * skipped_rows + middle[4];
    accumulators[11] = per_rows[11] * skipped_rows + middle[5];
    accumulators[12] = per_rows[12] * skipped_rows + middle[6];
    accumulators[13] = per_rows[13] * skipped_rows + middle[7];
  }
  else
  {
    accumulators[7] = middle[1];
    accumulators[8] = middle[2];
    accumulators[9] = middle[3];
    accumulators[10] = middle[4];
    accumulators[11] = middle[5];
    accumulators[12] = middle[6];
    accumulators[13] = middle[7];
  }

  const int bottom_row = *bottom;
  const int clamped_bottom_row = viewport_rows < bottom_row ? viewport_rows : bottom_row;

  for (int row = clamped_middle_row; row < clamped_bottom_row; row++)
  {
    opaque_row(
        texture_rows,
        texture_columns,
        texture_reds,
        texture_greens,
        texture_blues,
        accumulators,
        row,
        viewport_columns,
        viewport_opacities,
        viewport_reds,
        viewport_greens,
        viewport_blues,
        viewport_depths);

    accumulators[0] += per_rows[0];
    accumulators[1] += per_rows[1];
    accumulators[2] += per_rows[2];
    accumulators[3] += per_rows[3];
    accumulators[4] += per_rows[4];
    accumulators[5] += per_rows[5];
    accumulators[6] += per_rows[6];
    accumulators[7] += per_rows[7];
    accumulators[8] += per_rows[8];
    accumulators[9] += per_rows[9];
    accumulators[10] += per_rows[10];
    accumulators[11] += per_rows[11];
    accumulators[12] += per_rows[12];
    accumulators[13] += per_rows[13];
  }
}
