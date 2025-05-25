#ifndef CUTOUT_TRIANGLE_DEPTH_H

#define CUTOUT_TRIANGLE_DEPTH_H

/**
 * Samples the depth of a triangle with cutout transparency at a specified
 * screen coordinate.  The texture wraps at each edge and is sampled using
 * nearest-neighbor interpolation without perspective correction.
 * @param texture_rows The height of the texture in rows.  Behavior is undefined
 *                     if less than 1.
 * @param texture_columns The width of the texture in columns.  Behavior is
 *                        undefined if less than 1.
 * @param texture_opacities The opacity of each pixel within the texture, row
 *                          major, starting from the top left corner., where 0
 *                          is fully transparent and 1 is fully opaque.
 *                          Behavior for values outside this range is undefined.
 *                          Behavior is undefined if NaN, infinity or negative
 *                          infinity.
 * @param a_v The V coordinate of the triangle's first vertex, where 0 is the
 *            top and 1 is the bottom.  Behavior is undefined if NaN, infinity,
 *            negative infinity or unable to be cast to int.
 * @param a_u The U coordinate of the triangle's first vertex, where 0 is the
 *            left and 1 is the right.  Behavior is undefined if NaN, infinity,
 *            negative infinity or unable to be cast to int.
 * @param a_opacity The opacity of the triangle's first vertex, where 0 is fully
 *                  transparent and 1 is fully opaque.  Behavior for values
 *                  outside this range is undefined.  Behavior is undefined if
 *                  NaN, infinity or negative infinity.
 * @param a_row The number of rows between the top of the viewport and the
 *              triangle's first vertex.  Behavior is undefined if NaN,
 *              infinity, negative infinity or unable to be cast to int.
 * @param a_column The number of columns between the left of the viewport and
 *                 the triangle's first vertex.  Behavior is undefined if NaN,
 *                 infinity, negative infinity or unable to be cast to int.
 * @param a_depth The depth of the triangle's first vertex, where greater values
 *                are further away.  Behavior is undefined if NaN, infinity,
 *                negative infinity or unable to be cast to int.
 * @param b_v The V coordinate of the triangle's second vertex, where 0 is the
 *            top and 1 is the bottom.  Behavior is undefined if NaN, infinity,
 *            negative infinity or unable to be cast to int.
 * @param b_u The U coordinate of the triangle's second vertex, where 0 is the
 *            left and 1 is the right.  Behavior is undefined if NaN, infinity,
 *            negative infinity or unable to be cast to int.
 * @param b_opacity The opacity of the triangle's second vertex, where 0 is
 *                  fully transparent and 1 is fully opaque.  Behavior for
 *                  values outside this range is undefined.  Behavior is
 *                  undefined if NaN, infinity or negative infinity.
 * @param b_row The number of rows between the top of the viewport and the
 *              triangle's second vertex.  Behavior is undefined if NaN,
 *              infinity, negative infinity or unable to be cast to int.
 * @param b_column The number of columns between the left of the viewport and
 *                 the triangle's second vertex.  Behavior is undefined if NaN,
 *                 infinity, negative infinity or unable to be cast to int.
 * @param b_depth The depth of the triangle's second vertex, where greater
 *                values are further away.  Behavior is undefined if NaN,
 *                infinity, negative infinity or unable to be cast to int.
 * @param c_v The V coordinate of the triangle's third vertex, where 0 is the
 *            top and 1 is the bottom.  Behavior is undefined if NaN, infinity,
 *            negative infinity or unable to be cast to int.
 * @param c_u The U coordinate of the triangle's third vertex, where 0 is the
 *            left and 1 is the right.  Behavior is undefined if NaN, infinity,
 *            negative infinity or unable to be cast to int.
 * @param c_opacity The opacity of the triangle's third vertex, where 0 is
 *                  fully transparent and 1 is fully opaque.  Behavior for
 *                  values outside this range is undefined.  Behavior is
 *                  undefined if NaN, infinity or negative infinity.
 * @param c_row The number of rows between the top of the viewport and the
 *              triangle's third vertex.  Behavior is undefined if NaN,
 *              infinity, negative infinity or unable to be cast to int.
 * @param c_column The number of columns between the left of the viewport and
 *                 the triangle's third vertex.  Behavior is undefined if NaN,
 *                 infinity, negative infinity or unable to be cast to int.
 * @param c_depth The depth of the triangle's third vertex, where greater values
 *                are further away.  Behavior is undefined if NaN, infinity,
 *                negative infinity or unable to be cast to int.
 * @param row The number of rows between the top of the viewport and the point
 *            to be sampled.  Behavior is undefined if NaN, infinity or negative
 *            infinity.
 * @param column The number of columns between the left of the viewport and the
 *               point to be sampled.  Behavior is undefined if NaN, infinity or
 *               negative infinity.
 * @return Positive infinity where the triangle does not cover the given
 *         co-ordinates, otherwise, the depth of the given triangle at the given
 *         co-ordinates.
 */
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
    const float column);

#endif
