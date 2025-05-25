#ifndef OPAQUE_TRIANGLE_DEPTH_H

#define OPAQUE_TRIANGLE_DEPTH_H

/**
 * Samples the depth of an opaque triangle at a specified screen coordinate.
 * @param a_row The number of rows between the top of the viewport and the
 *              triangle's first vertex.  Behavior is undefined if NaN,
 *              infinity, negative infinity or unable to be cast to int.
 * @param a_column The number of columns between the left of the viewport and
 *                 the triangle's first vertex.  Behavior is undefined if NaN,
 *                 infinity, negative infinity or unable to be cast to int.
 * @param a_depth The depth of the triangle's first vertex, where greater values
 *                are further away.  Behavior is undefined if NaN, infinity,
 *                negative infinity or unable to be cast to int.
 * @param b_row The number of rows between the top of the viewport and the
 *              triangle's second vertex.  Behavior is undefined if NaN,
 *              infinity, negative infinity or unable to be cast to int.
 * @param b_column The number of columns between the left of the viewport and
 *                 the triangle's second vertex.  Behavior is undefined if NaN,
 *                 infinity, negative infinity or unable to be cast to int.
 * @param b_depth The depth of the triangle's second vertex, where greater
 *                values are further away.  Behavior is undefined if NaN,
 *                infinity, negative infinity or unable to be cast to int.
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
    const float column);

#endif
