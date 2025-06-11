#include <metal_stdlib>
using namespace metal;

kernel void add_arrays(const device float* a [[buffer(0)]],
                       const device float* b [[buffer(1)]],
                       device float* c [[buffer(2)]],
                       uint i [[thread_position_in_grid]]) {
  
  c[i] = a[i] + b[i];
}

kernel void both(const device float* a [[buffer(0)]],
                 texture2d<float, access::write> b [[texture(0)]],
                  uint2 gid [[thread_position_in_grid]]) {
  
  float sum = a[gid.y] + a[gid.x];
  b.write(sum, gid);
}

kernel void dim_add_textures(const texture1d<float, access::read> a [[texture(0)]],
                             const texture2d<int, access::read> b [[texture(1)]],
                             texture3d<float, access::write> c [[texture(2)]],
                             uint3 gid [[thread_position_in_grid]]) {

  c.write(a.read(uint(gid.x)) + (vec<float, 4>)b.read(uint2(gid.x, gid.y)), gid);
}
