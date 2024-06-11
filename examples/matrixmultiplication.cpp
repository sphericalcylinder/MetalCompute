#include "MTLCompute.hpp"

int main() {

    // Create new GPU device
    MTL::Device *gpu = MTL::CreateSystemDefaultDevice();

    // Load metal library and matrix multiplication function
    MTLCompute::Kernel kernel(gpu, "default.metallib");
    kernel.useFunction("matrix_mult");

    // set the matrix size to 800x800
    size_t texsize = 800;

    // Create 3 2D textures for 2 input matrices and an output matrix
    MTLCompute::Texture2D<float> a(gpu, texsize, texsize, MTLCompute::TextureType::float32);
    MTLCompute::Texture2D<float> b(gpu, texsize, texsize, MTLCompute::TextureType::float32);
    MTLCompute::Texture2D<float> c(gpu, texsize, texsize, MTLCompute::TextureType::float32);

    // Create 2D vectors to store the data for the input matrices
    std::vector<std::vector<float>> adata(texsize, std::vector<float>(texsize));
    std::vector<std::vector<float>> bdata(texsize, std::vector<float>(texsize));

    // Seed the random generator
    srand(time(NULL));

    // Fill the input matrices with random float data
    for (int i = 0; i < texsize; i++) {
        for (int j = 0; j < texsize; j++) {
            adata[i][j] = (rand() % 1000)+(rand()/(float)RAND_MAX);
            bdata[i][j] = (rand() % 1000)+(rand()/(float)RAND_MAX);
        }
    }

    a = adata;
    b = bdata;

    // Create a command manager
    MTLCompute::CommandManager<float> commandmanager(gpu, &kernel);

    // Load the textures into the command manager
    commandmanager.loadTexture(a, 0);
    commandmanager.loadTexture(b, 1);
    commandmanager.loadTexture(c, 2);

    // Print status and start timer
    std::cout << "start gpu compute" << std::endl;
    auto starttime = std::chrono::high_resolution_clock::now();

    // Run the kernel in a seperate thread
    commandmanager.dispatch();

    // Stop timer and print status
    auto endtime = std::chrono::high_resolution_clock::now();
    std::cout << "done gpu compute" << std::endl;

    // Print the total elapsed time
    std::chrono::duration<double> elapsedgputime = endtime - starttime;
    std::cout << "Total elapsed time: " << elapsedgputime.count() << " s\n\n";

    // Get the data from the output texture
    std::vector<std::vector<float>> cdata = c.getData();

    // Create a new vector for the cpu computed data
    std::vector<std::vector<float>> finaldata(texsize, std::vector<float>(texsize));

    // Print status and start timer
    std::cout << "start cpu compute" << std::endl;
    starttime = std::chrono::high_resolution_clock::now();

    // Compute the matrix multiplication on the cpu
    for (int i = 0; i < texsize; i++) {
        for (int j = 0; j < texsize; j++) {
            finaldata[i][j] = 0;
            for (int k = 0; k < texsize; k++) {
                finaldata[i][j] += adata[i][k] * bdata[k][j];
            }

        }
    }

    // Stop timer and print status
    endtime = std::chrono::high_resolution_clock::now();
    std::cout << "done cpu compute" << std::endl;

    // Print the total elapsed time
    std::chrono::duration<double> elapsedcputime = endtime - starttime;
    std::cout << "Total elapsed time: " << elapsedcputime.count() << " s\n";

    // Check if the data is correct
    for (int i = 0; i < texsize; i++) {
        for (int j = 0; j < texsize; j++) {
            if (cdata[i][j] != finaldata[i][j]) {
                std::cout << "Error: " << cdata[i][j] << " != " << finaldata[i][j] << std::endl;
            }
        }
    }

}