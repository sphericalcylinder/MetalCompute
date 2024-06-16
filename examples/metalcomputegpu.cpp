#include "MTLComputeGPU.hpp"

int main() {

    // Create gpu class
    MTLCompute::GPU<float> gpu("default", "add_arrays");

    // Create arrays
    vec<float> array1(10, 1.0);
    vec<float> array2(10, 2.0);
    vec<float> array3(10);

    // Load arrays
    gpu.loadArray(array1, 0);
    gpu.loadArray(array2, 1);
    gpu.loadArray(array3, 2);

    // Run kernel
    gpu.runKernel();

    // Get array
    array3 = gpu.getArray(2);

    // Print result
    for (int i = 0; i < 10; i++) {
        std::cout << array1[i] << " + " << array2[i] << " = " << array3[i] << std::endl;
    }

    return 0;
}