#ifndef BLENDED_TRIANGLE_H

#define BLENDED_TRIANGLE_H

/**
 * Draws a blended triangle.  The texture wraps at each edge and is sampled
 * using nearest-neighbor interpolation without perspective correction.
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
 * @param texture_reds The intensity of the red channel of each pixel within the
 *                     texture, row-major, starting from the top left corner.
 *                     Behavior is undefined if any are NaN, infinity, negative
 *                     infinity or unable to be cast to int.
 * @param texture_greens The intensity of the green channel of each pixel within
 *                       the texture, row-major, starting from the top left
 *                       corner.  Behavior is undefined if any are NaN,
 *                       infinity, negative infinity or unable to be cast to
 *                       int.
 * @param texture_blues The intensity of the blue channel of each pixel within
 *                      the texture, row-major, starting from the top left
 *                      corner.  Behavior is undefined if any are NaN, infinity,
 *                      negative infinity or unable to be cast to int.
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
 * @param a_red The intensity of the red channel of the triangle's first vertex.
 *              Behavior is undefined if NaN, infinity, negative infinity or
 *              unable to be cast to int.
 * @param a_green The intensity of the green channel of the triangle's first
 *                vertex.  Behavior is undefined if NaN, infinity, negative
 *                infinity or unable to be cast to int.
 * @param a_blue The intensity of the blue channel of the triangle's first
 *               vertex.  Behavior is undefined if NaN, infinity, negative
 *               infinity or unable to be cast to int.
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
 * @param b_red The intensity of the red channel of the triangle's second
 *              vertex.  Behavior is undefined if NaN, infinity, negative
 *              infinity or unable to be cast to int.
 * @param b_green The intensity of the green channel of the triangle's second
 *                vertex.  Behavior is undefined if NaN, infinity, negative
 *                infinity or unable to be cast to int.
 * @param b_blue The intensity of the blue channel of the triangle's second
 *               vertex.  Behavior is undefined if NaN, infinity, negative
 *               infinity or unable to be cast to int.
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
 * @param c_red The intensity of the red channel of the triangle's third vertex.
 *              Behavior is undefined if NaN, infinity, negative infinity or
 *              unable to be cast to int.
 * @param c_green The intensity of the green channel of the triangle's third
 *                vertex.  Behavior is undefined if NaN, infinity, negative
 *                infinity or unable to be cast to int.
 * @param c_blue The intensity of the blue channel of the triangle's third
 *               vertex.  Behavior is undefined if NaN, infinity, negative
 *               infinity or unable to be cast to int.
 * @param c_row The number of rows between the top of the viewport and the
 *              triangle's third vertex.  Behavior is undefined if NaN,
 *              infinity, negative infinity or unable to be cast to int.
 * @param c_column The number of columns between the left of the viewport and
 *                 the triangle's third vertex.  Behavior is undefined if NaN,
 *                 infinity, negative infinity or unable to be cast to int.
 * @param c_depth The depth of the triangle's third vertex, where greater values
 *                are further away.  Behavior is undefined if NaN, infinity,
 *                negative infinity or unable to be cast to int.
 * @param viewport_rows The height of the viewport, in rows.  Behavior is
 *                      undefined if less than 1.
 * @param viewport_columns The width of the viewport, in columns.  Behavior is
 *                         undefined if less than 1.
 * @param viewport_opacities The opacity of each pixel within the viewport,
 *                           row-major, starting from the top left corner, where
 *                           0 is fully transparent and 1 is fully opaque.
 *                           Behavior is undefined for values outside this
 *                           range, NaN, infinity or positive infinity.
 * @param viewport_reds The intensity of the red channel of each pixel within
 *                      the viewport, row-major, starting from the top left
 *                      corner.  Behavior is undefined if any are NaN, infinity,
 *                      negative infinity or unable to be cast to int.
 * @param viewport_greens The intensity of the red channel of each pixel within
 *                        the viewport, row-major, starting from the top left
 *                        corner.  Behavior is undefined if any are NaN,
 *                        infinity, negative infinity or unable to be cast to
 *                        int.
 * @param viewport_blues The intensity of the blue channel of each pixel within
 *                       the viewport, row-major, starting from the top left
 *                       corner.  Behavior is undefined if any are NaN,
 *                       infinity, negative infinity or unable to be cast to
 *                       int.
 * @param viewport_depths The depth of each pixel within the viewport, where
 *                        greater values are further away, row-major, starting
 *                        from the top left corner.  Behavior is undefined if
 *                        any are NaN, infinity, negative infinity or unable to
 *                        be cast to int.
 */
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
    float *const viewport_depths);

#endif
