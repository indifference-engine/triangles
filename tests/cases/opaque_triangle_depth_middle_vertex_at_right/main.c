#include "../../../src/opaque_triangle_depth.h"
#include "../../helpers/main.h"

int main(int argc, char **argv)
{
  (void)(argc);
  (void)(argv);

  float viewport_depths[256 * 192];

  for (int row = 0; row < 192; row++)
  {
    for (int column = 0; column < 256; column++)
    {
      viewport_depths[row * 256 + column] = opaque_triangle_depth(
          96.21f, 243.2f, 0.2f,
          20.5f, 12.4f, 0.6f,
          182.4f, 12.4f, 0.4f,
          row, column);
    }
  }

  write_file(viewport_depths, "outputs/actual/viewport/depths.data", 192, 256);

  return 0;
}
