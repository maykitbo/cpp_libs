#pragma once

#include <vector>
#include <fstream>
#include <initializer_list>
#include <iostream>


// #include <cblas.h>

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>
#include <clBLAS.h>



template<class T>
class CLMatrix {
    size_t cols_, rows_;
    cl::Context context_;
    cl::Device default_device_;
    cl::Buffer matrix_;
    public:
        CLMatrix(size_t row, size_t col) : rows_(row), cols_(col),
                                        context_(CL_DEVICE_TYPE_GPU),
                                        default_device_(cl::Device::getDefault()),
                                        matrix_(cl::Buffer(context_, CL_MEM_READ_WRITE, sizeof(T) * row * col)) {}

        CLMatrix(size_t row, size_t col, const T *array) : CLMatrix(row, col) {
            cl::CommandQueue queue(context_, default_device_);
            queue.enqueueWriteBuffer(matrix_, CL_TRUE, 0, sizeof(T) * row * col, array);
        }

        friend void MulCl(CLMatrix &A, CLMatrix &B, CLMatrix &C) {
            std::ifstream kernelFile("openCL/mul.cl");
            std::string kernelCode(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));
            cl::Program::Sources sources({kernelCode});
            cl::Context context(CL_DEVICE_TYPE_GPU);
            cl::CommandQueue queue(context, context.getInfo<CL_CONTEXT_DEVICES>()[0]);
            cl::Program program(context, sources);
            program.build();
            cl::Kernel kernel(program, "matrixMul");
            kernel.setArg(0, A.matrix_);
            kernel.setArg(1, B.matrix_);
            kernel.setArg(2, C.matrix_);
            kernel.setArg(3, A.cols_);
            kernel.setArg(4, B.cols_);
            queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(A.rows_, B.cols_));
            queue.finish();
        }

        friend void MulClBlas(CLMatrix &A, CLMatrix &B, CLMatrix &C) {
            cl::CommandQueue queue(A.context_, A.default_device_);
            cl_device_id device_id = A.default_device_.get();
            cl_command_queue raw_queue = queue.get();
            clblasSetup();

            cl_mem a_buffer = A.matrix_(), b_buffer = B.matrix_(), c_buffer = C.matrix_();
            clblasSgemm(clblasRowMajor, clblasNoTrans, clblasNoTrans, A.rows_, B.cols_, A.cols_,
                        1.0f, a_buffer, 0, A.cols_, b_buffer, 0, B.cols_, 0.0f, c_buffer, 0, C.cols_, 1,
                        &raw_queue, 0, nullptr, nullptr);

            clblasTeardown();
            queue.finish();
        }

        std::vector<T> ToVector() {
            std::vector<T> vec(rows_ * cols_);
            cl::CommandQueue queue(context_, default_device_);
            queue.enqueueReadBuffer(matrix_, CL_TRUE, 0, sizeof(T) * vec.size(), vec.data());
            return vec;
        }

        void Print() {
            auto vec = ToVector();
            for (int k = 0; k < rows_; ++k) {
                for (int g = 0; g < cols_; ++g) {
                    std::cout << vec[cols_ * k + g] << ' ';
                }
                std::cout << '\n';
            }
        }
};

class CL {
    
};

