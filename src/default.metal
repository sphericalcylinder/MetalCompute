#include <metal_stdlib>
using namespace metal;

kernel void add_arrays(const device float* a [[buffer(0)]],
                       const device float* b [[buffer(1)]],
                       device float* c [[buffer(2)]],
                       uint i [[thread_position_in_grid]]) {
  c[i] = a[i] + b[i];
}

kernel void sub_arrays(const device float* a [[buffer(0)]],
                       const device float* b [[buffer(1)]],
                       device float* c [[buffer(2)]],
                       uint i [[thread_position_in_grid]]) {
  c[i] = a[i] + b[i];
}