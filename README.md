# Triangles

C library to draw triangles.

## Usage

### Project Level

Your application's build pipeline will need to be configured to compile each C
file in the [src](./src) directory and recompile every C file should any H files
change.  Then, include each H file in the same directory to make its
corresponding function available.

### Assumptions

- The compilation environment supports C99.
- The texture being sampled from:
  - Has its origin in the top left corner.
  - Is row-major.
  - Is planar (not interleaved).
  - Has a red plane.
  - Has a green plane.
  - Has a blue plane.
  - Has an opacity plane (where required).
  - Uses 32-bit floats for all planes.
  - Describes more distant pixels using greater depth values.
  - Describes absolute transparency as an opacity of 0.
  - Describes absolute opacity as an opacity of 1.
  - Does not pre-multiply any plane by the opacity plane.
- The viewport being rendered to:
  - Has its origin in the top left corner.
  - Is row-major.
  - Is planar (not interleaved).
  - Has a red plane.
  - Has a green plane.
  - Has a blue plane.
  - Has an opacity plane.
  - Has a depth plane.
  - Uses 32-bit floats for all planes.
  - Describes more distant pixels using greater depth values.
  - Describes absolute transparency as an opacity of 0.
  - Describes absolute opacity as an opacity of 1.
  - Does not pre-multiply any plane by the opacity plane.
- No float arguments:
  - Are NaN.
  - Are infinity.
  - Are negative infinity.
  - Cannot be cast to `int` without undefined behavior (for example, very large
    values).

### Blend Modes

| Blend Mode | Color                                           | Opacity                          | Depth                     |
| ---------- | ----------------------------------------------- | -------------------------------- | ------------------------- |
| Opaque     | Overwritten.                                    | Overwritten with 1.              | Overwritten.              |
| Cutout     | Overwritten or unchanged.                       | Overwritten with 1 or unchanged. | Overwritten or unchanged. |
| Additive   | Summed ("linear dodge").                        | Unchanged.                       | Unchanged.                |
| Blended    | Mixed using opacity.                            | Mixed with opacity.              | Unchanged.                |

### Functions

| Name            | Blend Mode |
| --------------- | ---------- |
| `opaque_line`   | Opaque     |
| `cutout_line`   | Cutout     |
| `additive_line` | Additive   |
| `blended_line`  | Blended    |

## Tests

Execute `make` to run the test suite.

### Dependencies

- Make.
- MinGW-GCC for Windows, Clang for all other platforms.
- Bash.
