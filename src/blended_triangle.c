#include "blended_triangle.h"

static void blended_row(
    const int texture_rows,
    const int texture_columns,
    const float *const texture_opacities,
    const float *const texture_reds,
    const float *const texture_greens,
    const float *const texture_blues,
    const float row_accumulators[16],
    const int row,
    const int viewport_columns,
    float *const viewport_opacities,
    float *const viewport_reds,
    float *const viewport_greens,
    float *const viewport_blues,
    const float *const viewport_depths)
{
  float per_columns[7];
  float accumulators[7];

  const float first_column = row_accumulators[0];
  const float second_column = row_accumulators[8];

  per_columns[0] = row_accumulators[9] - row_accumulators[1];
  per_columns[1] = row_accumulators[10] - row_accumulators[2];
  per_columns[2] = row_accumulators[11] - row_accumulators[3];
  per_columns[3] = row_accumulators[12] - row_accumulators[4];
  per_columns[4] = row_accumulators[13] - row_accumulators[5];
  per_columns[5] = row_accumulators[14] - row_accumulators[6];
  per_columns[6] = row_accumulators[15] - row_accumulators[7];

  const float multiplicand = 1.0f / (second_column - first_column);
  per_columns[0] *= multiplicand;
  per_columns[1] *= multiplicand;
  per_columns[2] *= multiplicand;
  per_columns[3] *= multiplicand;
  per_columns[4] *= multiplicand;
  per_columns[5] *= multiplicand;
  per_columns[6] *= multiplicand;

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
    accumulators[6] = row_accumulators[7];
  }
  else
  {
    clamped_left_column = second_column < 0 ? ((int)second_column) - 1 : second_column;
    clamped_right_column = first_column < 0 ? ((int)first_column) - 1 : first_column;
    accumulators[0] = row_accumulators[9];
    accumulators[1] = row_accumulators[10];
    accumulators[2] = row_accumulators[11];
    accumulators[3] = row_accumulators[12];
    accumulators[4] = row_accumulators[13];
    accumulators[5] = row_accumulators[14];
    accumulators[6] = row_accumulators[15];
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
    accumulators[6] += per_columns[6] * negated_clamp_left_column;
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

      const float opacity = accumulators[3] * texture_opacities[texture_index];
      const float inverse_opacity = 1.0f - opacity;
      viewport_opacities[viewport_index] = 1.0f - (1.0f - viewport_opacities[viewport_index]) * inverse_opacity;
      viewport_reds[viewport_index] = viewport_reds[viewport_index] * inverse_opacity + accumulators[4] * texture_reds[texture_index] * opacity;
      viewport_greens[viewport_index] = viewport_greens[viewport_index] * inverse_opacity + accumulators[5] * texture_greens[texture_index] * opacity;
      viewport_blues[viewport_index] = viewport_blues[viewport_index] * inverse_opacity + accumulators[6] * texture_blues[texture_index] * opacity;
    }

    accumulators[0] += per_columns[0];
    accumulators[1] += per_columns[1];
    accumulators[2] += per_columns[2];
    accumulators[3] += per_columns[3];
    accumulators[4] += per_columns[4];
    accumulators[5] += per_columns[5];
    accumulators[6] += per_columns[6];
  }
}

void blended_triangle(
    const int texture_rows,
    const int texture_columns,
    const float *const texture_opacities,
    const float *const texture_reds,
    const float *const texture_greens,
    const float *const texture_blues,
    const float a_v,
    const float a_u,
    const float a_opacity,
    const float a_red,
    const float a_green,
    const float a_blue,
    const float a_row,
    const float a_column,
    const float a_depth,
    const float b_v,
    const float b_u,
    const float b_opacity,
    const float b_red,
    const float b_green,
    const float b_blue,
    const float b_row,
    const float b_column,
    const float b_depth,
    const float c_v,
    const float c_u,
    const float c_opacity,
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
  float top[9];
  float middle[9];
  float bottom[9];

  if (a_row <= b_row)
  {
    if (b_row <= c_row)
    {
      top[0] = a_row;
      top[1] = a_column;
      top[2] = a_depth;
      top[3] = a_v;
      top[4] = a_u;
      top[5] = a_opacity;
      top[6] = a_red;
      top[7] = a_green;
      top[8] = a_blue;
      middle[0] = b_row;
      middle[1] = b_column;
      middle[2] = b_depth;
      middle[3] = b_v;
      middle[4] = b_u;
      middle[5] = b_opacity;
      middle[6] = b_red;
      middle[7] = b_green;
      middle[8] = b_blue;
      bottom[0] = c_row;
      bottom[1] = c_column;
      bottom[2] = c_depth;
      bottom[3] = c_v;
      bottom[4] = c_u;
      bottom[5] = c_opacity;
      bottom[6] = c_red;
      bottom[7] = c_green;
      bottom[8] = c_blue;
    }
    else if (a_row <= c_row)
    {
      top[0] = a_row;
      top[1] = a_column;
      top[2] = a_depth;
      top[3] = a_v;
      top[4] = a_u;
      top[5] = a_opacity;
      top[6] = a_red;
      top[7] = a_green;
      top[8] = a_blue;
      middle[0] = c_row;
      middle[1] = c_column;
      middle[2] = c_depth;
      middle[3] = c_v;
      middle[4] = c_u;
      middle[5] = c_opacity;
      middle[6] = c_red;
      middle[7] = c_green;
      middle[8] = c_blue;
      bottom[0] = b_row;
      bottom[1] = b_column;
      bottom[2] = b_depth;
      bottom[3] = b_v;
      bottom[4] = b_u;
      bottom[5] = b_opacity;
      bottom[6] = b_red;
      bottom[7] = b_green;
      bottom[8] = b_blue;
    }
    else
    {
      top[0] = c_row;
      top[1] = c_column;
      top[2] = c_depth;
      top[3] = c_v;
      top[4] = c_u;
      top[5] = c_opacity;
      top[6] = c_red;
      top[7] = c_green;
      top[8] = c_blue;
      middle[0] = a_row;
      middle[1] = a_column;
      middle[2] = a_depth;
      middle[3] = a_v;
      middle[4] = a_u;
      middle[5] = a_opacity;
      middle[6] = a_red;
      middle[7] = a_green;
      middle[8] = a_blue;
      bottom[0] = b_row;
      bottom[1] = b_column;
      bottom[2] = b_depth;
      bottom[3] = b_v;
      bottom[4] = b_u;
      bottom[5] = b_opacity;
      bottom[6] = b_red;
      bottom[7] = b_green;
      bottom[8] = b_blue;
    }
  }
  else if (a_row <= c_row)
  {
    top[0] = b_row;
    top[1] = b_column;
    top[2] = b_depth;
    top[3] = b_v;
    top[4] = b_u;
    top[5] = b_opacity;
    top[6] = b_red;
    top[7] = b_green;
    top[8] = b_blue;
    middle[0] = a_row;
    middle[1] = a_column;
    middle[2] = a_depth;
    middle[3] = a_v;
    middle[4] = a_u;
    middle[5] = a_opacity;
    middle[6] = a_red;
    middle[7] = a_green;
    middle[8] = a_blue;
    bottom[0] = c_row;
    bottom[1] = c_column;
    bottom[2] = c_depth;
    bottom[3] = c_v;
    bottom[4] = c_u;
    bottom[5] = c_opacity;
    bottom[6] = c_red;
    bottom[7] = c_green;
    bottom[8] = c_blue;
  }
  else if (b_row <= c_row)
  {
    top[0] = b_row;
    top[1] = b_column;
    top[2] = b_depth;
    top[3] = b_v;
    top[4] = b_u;
    top[5] = b_opacity;
    top[6] = b_red;
    top[7] = b_green;
    top[8] = b_blue;
    middle[0] = c_row;
    middle[1] = c_column;
    middle[2] = c_depth;
    middle[3] = c_v;
    middle[4] = c_u;
    middle[5] = c_opacity;
    middle[6] = c_red;
    middle[7] = c_green;
    middle[8] = c_blue;
    bottom[0] = a_row;
    bottom[1] = a_column;
    bottom[2] = a_depth;
    bottom[3] = a_v;
    bottom[4] = a_u;
    bottom[5] = a_opacity;
    bottom[6] = a_red;
    bottom[7] = a_green;
    bottom[8] = a_blue;
  }
  else
  {
    top[0] = c_row;
    top[1] = c_column;
    top[2] = c_depth;
    top[3] = c_v;
    top[4] = c_u;
    top[5] = c_opacity;
    top[6] = c_red;
    top[7] = c_green;
    top[8] = c_blue;
    middle[0] = b_row;
    middle[1] = b_column;
    middle[2] = b_depth;
    middle[3] = b_v;
    middle[4] = b_u;
    middle[5] = b_opacity;
    middle[6] = b_red;
    middle[7] = b_green;
    middle[8] = b_blue;
    bottom[0] = a_row;
    bottom[1] = a_column;
    bottom[2] = a_depth;
    bottom[3] = a_v;
    bottom[4] = a_u;
    bottom[5] = a_opacity;
    bottom[6] = a_red;
    bottom[7] = a_green;
    bottom[8] = a_blue;
  }

  float deltas[18];
  deltas[0] = bottom[0] - top[0];
  deltas[1] = bottom[1] - top[1];
  deltas[2] = bottom[2] - top[2];
  deltas[3] = bottom[3] - top[3];
  deltas[4] = bottom[4] - top[4];
  deltas[5] = bottom[5] - top[5];
  deltas[6] = bottom[6] - top[6];
  deltas[7] = bottom[7] - top[7];
  deltas[8] = bottom[8] - top[8];
  deltas[9] = middle[0] - top[0];
  deltas[10] = middle[1] - top[1];
  deltas[11] = middle[2] - top[2];
  deltas[12] = middle[3] - top[3];
  deltas[13] = middle[4] - top[4];
  deltas[14] = middle[5] - top[5];
  deltas[15] = middle[6] - top[6];
  deltas[16] = middle[7] - top[7];
  deltas[17] = middle[8] - top[8];

  const float multiplicand_a = 1.0f / deltas[0];
  const float short_row_delta = deltas[9];
  const float multiplicand_b = 1.0f / short_row_delta;

  float per_rows[16];
  per_rows[0] = deltas[1] * multiplicand_a;
  per_rows[1] = deltas[2] * multiplicand_a;
  per_rows[2] = deltas[3] * multiplicand_a;
  per_rows[3] = deltas[4] * multiplicand_a;
  per_rows[4] = deltas[5] * multiplicand_a;
  per_rows[5] = deltas[6] * multiplicand_a;
  per_rows[6] = deltas[7] * multiplicand_a;
  per_rows[7] = deltas[8] * multiplicand_a;
  per_rows[8] = deltas[10] * multiplicand_b;
  per_rows[9] = deltas[11] * multiplicand_b;
  per_rows[10] = deltas[12] * multiplicand_b;
  per_rows[11] = deltas[13] * multiplicand_b;
  per_rows[12] = deltas[14] * multiplicand_b;
  per_rows[13] = deltas[15] * multiplicand_b;
  per_rows[14] = deltas[16] * multiplicand_b;
  per_rows[15] = deltas[17] * multiplicand_b;

  float accumulators[16];

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
    accumulators[7] = per_rows[7] * skipped_rows + top[8];
    accumulators[8] = per_rows[8] * skipped_rows + top[1];
    accumulators[9] = per_rows[9] * skipped_rows + top[2];
    accumulators[10] = per_rows[10] * skipped_rows + top[3];
    accumulators[11] = per_rows[11] * skipped_rows + top[4];
    accumulators[12] = per_rows[12] * skipped_rows + top[5];
    accumulators[13] = per_rows[13] * skipped_rows + top[6];
    accumulators[14] = per_rows[14] * skipped_rows + top[7];
    accumulators[15] = per_rows[15] * skipped_rows + top[8];
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
    accumulators[7] = top[8];
    accumulators[8] = top[1];
    accumulators[9] = top[2];
    accumulators[10] = top[3];
    accumulators[11] = top[4];
    accumulators[12] = top[5];
    accumulators[13] = top[6];
    accumulators[14] = top[7];
    accumulators[15] = top[8];
  }

  const float middle_row_float = middle[0];
  const int middle_row = middle_row_float < 0 ? ((int)middle_row_float) - 1 : middle_row_float;
  const int clamped_middle_row = middle_row < 0 ? 0 : (middle_row > viewport_rows ? viewport_rows : middle_row);

  for (int row = clamped_top_row; row < clamped_middle_row; row++)
  {
    blended_row(
        texture_rows,
        texture_columns,
        texture_opacities,
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
    accumulators[14] += per_rows[14];
    accumulators[15] += per_rows[15];
  }

  deltas[0] = bottom[0] - middle[0];
  deltas[1] = bottom[1] - middle[1];
  deltas[2] = bottom[2] - middle[2];
  deltas[3] = bottom[3] - middle[3];
  deltas[4] = bottom[4] - middle[4];
  deltas[5] = bottom[5] - middle[5];
  deltas[6] = bottom[6] - middle[6];
  deltas[7] = bottom[7] - middle[7];
  deltas[8] = bottom[8] - middle[8];

  const float multiplicand_c = 1.0f / deltas[0];
  per_rows[8] = deltas[1] * multiplicand_c;
  per_rows[9] = deltas[2] * multiplicand_c;
  per_rows[10] = deltas[3] * multiplicand_c;
  per_rows[11] = deltas[4] * multiplicand_c;
  per_rows[12] = deltas[5] * multiplicand_c;
  per_rows[13] = deltas[6] * multiplicand_c;
  per_rows[14] = deltas[7] * multiplicand_c;
  per_rows[15] = deltas[8] * multiplicand_c;

  if (middle_row < 0)
  {
    const float skipped_rows = -middle_row;

    accumulators[8] = per_rows[8] * skipped_rows + middle[1];
    accumulators[9] = per_rows[9] * skipped_rows + middle[2];
    accumulators[10] = per_rows[10] * skipped_rows + middle[3];
    accumulators[11] = per_rows[11] * skipped_rows + middle[4];
    accumulators[12] = per_rows[12] * skipped_rows + middle[5];
    accumulators[13] = per_rows[13] * skipped_rows + middle[6];
    accumulators[14] = per_rows[14] * skipped_rows + middle[7];
    accumulators[15] = per_rows[15] * skipped_rows + middle[8];
  }
  else
  {
    accumulators[8] = middle[1];
    accumulators[9] = middle[2];
    accumulators[10] = middle[3];
    accumulators[11] = middle[4];
    accumulators[12] = middle[5];
    accumulators[13] = middle[6];
    accumulators[14] = middle[7];
    accumulators[15] = middle[8];
  }

  const int bottom_row = *bottom;
  const int clamped_bottom_row = viewport_rows < bottom_row ? viewport_rows : bottom_row;

  for (int row = clamped_middle_row; row < clamped_bottom_row; row++)
  {
    blended_row(
        texture_rows,
        texture_columns,
        texture_opacities,
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
    accumulators[14] += per_rows[14];
    accumulators[15] += per_rows[15];
  }
}
