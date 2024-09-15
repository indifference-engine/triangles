#include "../../../src/opaque_triangle.h"
#include "../../helpers/main.h"

int main(int argc, char **argv)
{
  (void)(argc);
  (void)(argv);

  float *const texture_opacities = read_file("inputs/viewport/opacities.data", 10, 8);
  float *const texture_reds = read_file("inputs/viewport/reds.data", 10, 8);
  float *const texture_greens = read_file("inputs/viewport/greens.data", 10, 8);
  float *const texture_blues = read_file("inputs/viewport/blues.data", 10, 8);

  float *const viewport_opacities = read_file("inputs/viewport/opacities.data", 192, 256);
  float *const viewport_reds = read_file("inputs/viewport/reds.data", 192, 256);
  float *const viewport_greens = read_file("inputs/viewport/greens.data", 192, 256);
  float *const viewport_blues = read_file("inputs/viewport/blues.data", 192, 256);
  float *const viewport_depths = read_file("inputs/viewport/depths.data", 192, 256);

  opaque_triangle(
      10, 8, texture_reds, texture_greens, texture_blues,
      -1.7f, 0.6f, 0.7f, 0.9f, 0.6f, 187.3f, 120.4f, 0.2f,
      1.8f, -2.3f, 0.84f, 0.67f, 0.92f, -20.5f, 21.8f, 0.6f,
      1.8f, 2.1f, 0.93f, 0.88f, 0.41f, -20.5f, 243.2f, 0.4f,
      192, 256, viewport_opacities, viewport_reds, viewport_greens, viewport_blues, viewport_depths);

  write_file(texture_opacities, "outputs/actual/texture/opacities.data", 10, 8);
  write_file(texture_reds, "outputs/actual/texture/reds.data", 10, 8);
  write_file(texture_greens, "outputs/actual/texture/greens.data", 10, 8);
  write_file(texture_blues, "outputs/actual/texture/blues.data", 10, 8);

  write_file(viewport_opacities, "outputs/actual/viewport/opacities.data", 192, 256);
  write_file(viewport_reds, "outputs/actual/viewport/reds.data", 192, 256);
  write_file(viewport_greens, "outputs/actual/viewport/greens.data", 192, 256);
  write_file(viewport_blues, "outputs/actual/viewport/blues.data", 192, 256);
  write_file(viewport_depths, "outputs/actual/viewport/depths.data", 192, 256);

  return 0;
}
