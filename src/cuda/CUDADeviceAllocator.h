/*
 * CUDADeviceAllocator.h
 *
 *  Created on: 12.01.2016
 *      Author: Jan Stephan
 *
 *      Custom allocator for allocating GPU memory using CUDA's memory management functions.
 */

#ifndef CUDADEVICEALLOCATOR_H_
#define CUDADEVICEALLOCATOR_H_

#include <cstddef>

#include "CUDAAssert.h"

namespace ddafa
{
	namespace impl
	{
		template <typename T>
		class CUDADeviceAllocator
		{
			public:
				using value_type = T;
				using pointer = T*;
				using const_pointer = const T*;
				using reference = T&;
				using const_reference = const T&;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;

			public:
				pointer allocate(size_type n)
				{
					void *p;
					assertCuda(cudaMalloc(&p, n * sizeof(value_type)));
					return static_cast<pointer>(p);
				}

				void deallocate(pointer p, size_type)
				{
					assertCuda(cudaFree(p));
				}

			protected:
				~CUDADeviceAllocator() = default;
		};
	}
}

#endif /* CUDADEVICEALLOCATOR_H_ */
