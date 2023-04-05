__kernel void matrixMul(__global float* A, __global float* B, __global float* C, const int K, const int N) {
    int i = get_global_id(0);
    int j = get_global_id(1);
    float sum = 0;
    for (int k = 0; k < K; k++) {
        sum += A[i * K + k] * B[k * N + j];
    }
    C[i * N + j] = sum;
}
