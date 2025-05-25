#include "cutout_triangle_depth.h"

static float cutout_triangle_depth_extracted(
    const int texture_rows,
    const int texture_columns,
    const float *const texture_opacities,
    const float top_v,
    const float top_u,
    const float top_opacity,
    const float top_row,
    const float top_column,
    const float top_depth,
    const float middle_v,
    const float middle_u,
    const float middle_opacity,
    const float middle_row,
    const float middle_column,
    const float middle_depth,
    const float bottom_v,
    const float bottom_u,
    const float bottom_opacity,
    const float bottom_row,
    const float bottom_column,
    const float bottom_depth,
    const float row,
    const float column)
{
  if (row < top_row)
  {
    return 1.0f / 0.0f;
  }
  else if (row >= bottom_row)
  {
    return 1.0f / 0.0f;
  }
  else
  {
    float short_top_v, short_top_u, short_top_opacity, short_top_row, short_top_column, short_top_depth;
    float short_bottom_v, short_bottom_u, short_bottom_opacity, short_bottom_row, short_bottom_column, short_bottom_depth;

    if (row <= middle_row)
    {
      short_top_v = top_v;
      short_top_u = top_u;
      short_top_opacity = top_opacity;
      short_top_row = top_row;
      short_top_column = top_column;
      short_top_depth = top_depth;
      short_bottom_v = middle_v;
      short_bottom_u = middle_u;
      short_bottom_opacity = middle_opacity;
      short_bottom_row = middle_row;
      short_bottom_column = middle_column;
      short_bottom_depth = middle_depth;
    }
    else
    {
      short_top_v = middle_v;
      short_top_u = middle_u;
      short_top_opacity = middle_opacity;
      short_top_row = middle_row;
      short_top_column = middle_column;
      short_top_depth = middle_depth;
      short_bottom_v = bottom_v;
      short_bottom_u = bottom_u;
      short_bottom_opacity = bottom_opacity;
      short_bottom_row = bottom_row;
      short_bottom_column = bottom_column;
      short_bottom_depth = bottom_depth;
    }

    const float along_long = (row - top_row) / (bottom_row - top_row);
    const float inverse_along_long = 1.0f - along_long;
    const float along_short = (row - short_top_row) / (short_bottom_row - short_top_row);
    const float inverse_along_short = 1.0f - along_short;

    const float long_column = along_long * bottom_column + inverse_along_long * top_column;
    const float short_column = along_short * short_bottom_column + inverse_along_short * short_top_column;

    if (long_column <= short_column)
    {
      if (column < long_column)
      {
        return 1.0f / 0.0f;
      }

      if (column >= short_column)
      {
        return 1.0 / 0.0f;
      }
    }
    else
    {
      if (column < short_column)
      {
        return 1.0f / 0.0f;
      }

      if (column >= long_column)
      {
        return 1.0 / 0.0f;
      }
    }

    const float along_long_short = (column - long_column) / (short_column - long_column);
    const float inverse_along_long_short = 1.0f - along_long_short;

    const float texture_row_float = texture_rows * along_long_short * (along_short * short_bottom_v + inverse_along_short * short_top_v) + inverse_along_long_short * (along_long * bottom_v + inverse_along_long * top_v);
    const int texture_row = texture_row_float < 0 ? ((int)texture_row_float) - 1 : texture_row_float;
    const int wrapped_texture_row = texture_row >= 0 ? texture_row % texture_rows : texture_rows + ((texture_row + 1) % texture_rows) - 1;

    const float texture_column_float = texture_columns * along_long_short * (along_short * short_bottom_u + inverse_along_short * short_top_u) + inverse_along_long_short * (along_long * bottom_u + inverse_along_long * top_u);
    const int texture_column = texture_column_float < 0 ? ((int)texture_column_float) - 1 : texture_column_float;
    const int wrapped_texture_column = texture_column >= 0 ? texture_column % texture_columns : texture_columns + ((texture_column + 1) % texture_columns) - 1;

    if (texture_opacities[wrapped_texture_row * texture_columns + wrapped_texture_column] * along_long_short * (along_short * short_bottom_opacity + inverse_along_short * short_top_opacity) + inverse_along_long_short * (along_long * bottom_opacity + inverse_along_long * top_opacity) > 0.5f)
    {
      return along_long_short * (along_short * short_bottom_depth + inverse_along_short * short_top_depth) + inverse_along_long_short * (along_long * bottom_depth + inverse_along_long * top_depth);
    }

    return 1.0f / 0.0f;
  }
}

float cutout_triangle_depth(
    const int texture_rows,
    const int texture_columns,
    const float *const texture_opacities,
    const float a_v,
    const float a_u,
    const float a_opacity,
    const float a_row,
    const float a_column,
    const float a_depth,
    const float b_v,
    const float b_u,
    const float b_opacity,
    const float b_row,
    const float b_column,
    const float b_depth,
    const float c_v,
    const float c_u,
    const float c_opacity,
    const float c_row,
    const float c_column,
    const float c_depth,
    const float row,
    const float column)
{

  if (a_row <= b_row)
  {
    if (b_row <= c_row)
    {
      return cutout_triangle_depth_extracted(
          texture_rows, texture_columns, texture_opacities,
          a_v, a_u, a_opacity, a_row, a_column, a_depth,
          b_v, b_u, b_opacity, b_row, b_column, b_depth,
          c_v, c_u, c_opacity, c_row, c_column, c_depth,
          row, column);
    }
    else if (a_row <= c_row)
    {
      return cutout_triangle_depth_extracted(
          texture_rows, texture_columns, texture_opacities,
          a_v, a_u, a_opacity, a_row, a_column, a_depth,
          c_v, c_u, c_opacity, c_row, c_column, c_depth,
          b_v, b_u, b_opacity, b_row, b_column, b_depth,
          row, column);
    }
    else
    {
      return cutout_triangle_depth_extracted(
          texture_rows, texture_columns, texture_opacities,
          c_row, c_column, c_depth, c_v, c_u, c_opacity,
          a_row, a_column, a_depth, a_v, a_u, a_opacity,
          b_row, b_column, b_depth, b_v, b_u, b_opacity,
          row, column);
    }
  }
  else if (a_row <= c_row)
  {
    return cutout_triangle_depth_extracted(
        texture_rows, texture_columns, texture_opacities,
        b_v, b_u, b_opacity, b_row, b_column, b_depth,
        a_v, a_u, a_opacity, a_row, a_column, a_depth,
        c_v, c_u, c_opacity, c_row, c_column, c_depth,
        row, column);
  }
  else if (b_row <= c_row)
  {
    return cutout_triangle_depth_extracted(
        texture_rows, texture_columns, texture_opacities,
        b_v, b_u, b_opacity, b_row, b_column, b_depth,
        c_v, c_u, c_opacity, c_row, c_column, c_depth,
        a_v, a_u, a_opacity, a_row, a_column, a_depth,
        row, column);
  }
  else
  {
    return cutout_triangle_depth_extracted(
        texture_rows, texture_columns, texture_opacities,
        c_v, c_u, c_opacity, c_row, c_column, c_depth,
        b_v, b_u, b_opacity, b_row, b_column, b_depth,
        a_v, a_u, a_opacity, a_row, a_column, a_depth,
        row, column);
  }
}
