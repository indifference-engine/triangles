#include "../../../src/cutout_triangle_depth.h"
#include "../../helpers/main.h"

int main(int argc, char **argv)
{
  (void)(argc);
  (void)(argv);

  float *const texture_opacities = read_file("inputs/texture/opacities.data", 10, 8);
  float viewport_depths[256 * 192];

  for (int row = 0; row < 192; row++)
  {
    for (int column = 0; column < 256; column++)
    {
      viewport_depths[row * 256 + column] = cutout_triangle_depth(
          10, 8, texture_opacities,
          -1.7f, 0.6f, 0.35f, 96.21f, 243.2f, 0.2f,
          1.8f, -2.3f, 0.65f, 20.5f, -12.4f, 0.6f,
          1.8f, 2.1f, 0.81f, 182.4f, -12.4f, 0.4f,
          row, column);
    }
  }

  write_file(texture_opacities, "outputs/actual/texture/opacities.data", 10, 8);
  write_file(viewport_depths, "outputs/actual/viewport/depths.data", 192, 256);

  return 0;
}
