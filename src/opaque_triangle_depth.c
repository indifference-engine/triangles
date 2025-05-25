#include "opaque_triangle_depth.h"

static float opaque_triangle_depth_extracted(
    const float top_row,
    const float top_column,
    const float top_depth,
    const float middle_row,
    const float middle_column,
    const float middle_depth,
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
    float short_top_row, short_top_column, short_top_depth;
    float short_bottom_row, short_bottom_column, short_bottom_depth;

    if (row <= middle_row)
    {
      short_top_row = top_row;
      short_top_column = top_column;
      short_top_depth = top_depth;
      short_bottom_row = middle_row;
      short_bottom_column = middle_column;
      short_bottom_depth = middle_depth;
    }
    else
    {
      short_top_row = middle_row;
      short_top_column = middle_column;
      short_top_depth = middle_depth;
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

    return along_long_short * (along_short * short_bottom_depth + inverse_along_short * short_top_depth) + inverse_along_long_short * (along_long * bottom_depth + inverse_along_long * top_depth);
  }
}

float opaque_triangle_depth(
    const float a_row,
    const float a_column,
    const float a_depth,
    const float b_row,
    const float b_column,
    const float b_depth,
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
      return opaque_triangle_depth_extracted(
          a_row, a_column, a_depth,
          b_row, b_column, b_depth,
          c_row, c_column, c_depth,
          row, column);
    }
    else if (a_row <= c_row)
    {
      return opaque_triangle_depth_extracted(
          a_row, a_column, a_depth,
          c_row, c_column, c_depth,
          b_row, b_column, b_depth,
          row, column);
    }
    else
    {
      return opaque_triangle_depth_extracted(
          c_row, c_column, c_depth,
          a_row, a_column, a_depth,
          b_row, b_column, b_depth,
          row, column);
    }
  }
  else if (a_row <= c_row)
  {
    return opaque_triangle_depth_extracted(
        b_row, b_column, b_depth,
        a_row, a_column, a_depth,
        c_row, c_column, c_depth,
        row, column);
  }
  else if (b_row <= c_row)
  {
    return opaque_triangle_depth_extracted(
        b_row, b_column, b_depth,
        c_row, c_column, c_depth,
        a_row, a_column, a_depth,
        row, column);
  }
  else
  {
    return opaque_triangle_depth_extracted(
        c_row, c_column, c_depth,
        b_row, b_column, b_depth,
        a_row, a_column, a_depth,
        row, column);
  }
}
