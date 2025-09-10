// test.cu
_global__ void kernel() {}

int main() {
    kernel<<<1,1>>>();  // launch 1 block of 1 thread
    return 0;
}