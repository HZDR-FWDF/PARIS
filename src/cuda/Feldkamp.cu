/*
 * CUDAFeldkamp.cu
 *
 *  Created on: 12.11.2015
 *      Author: Jan Stephan
 *
 *      This class is the concrete backprojection implementation for the Stage class. Implementation file.
 */

#include <array>
#include <cstddef>
#include <cmath>

#include <ddrf/Image.h>
#include <ddrf/cuda/Check.h>
#include <ddrf/cuda/Coordinates.h>
#include <ddrf/cuda/Launch.h>

#include "../common/Geometry.h"
#include "Feldkamp.h"
#include "FeldkampScheduler.h"

namespace ddafa
{
	namespace cuda
	{
		__global__ void init_volume(float* vol, std::size_t width, std::size_t height, std::size_t depth, std::size_t pitch)
		{
			auto x = ddrf::cuda::getX();
			auto y = ddrf::cuda::getY();
			auto z = ddrf::cuda::getZ();

			if((x < width) && (y < height) && (z < depth))
			{
				auto slice_pitch = pitch * height;
				auto slice = reinterpret_cast<char*>(vol) + z * slice_pitch;
				auto row = reinterpret_cast<float*>(slice + y * pitch);

				row[x] = 0.f;
			}
		}

		__global__ void backproject(float *vol, std::size_t vol_w, std::size_t vol_h, std::size_t vol_d, std::size_t vol_pitch,
									const float *proj, std::size_t proj_w, std::size_t proj_h, std::size_t proj_pitch,
									unsigned int i, float angle, float dist_src, float dist_det,
									std::uint32_t num_proj)
		{
			auto x = ddrf::cuda::getX();
			auto y = ddrf::cuda::getY();
			auto z = ddrf::cuda::getZ();

			if((x < vol_w) && (y < vol_h) && (z < vol_d))
			{
				auto slice_pitch = vol_pitch * vol_h;
				auto slice = reinterpret_cast<char*>(vol) + z * slice_pitch;
				auto row = reinterpret_cast<float*>(slice + y * vol_pitch);
				auto proj_row = reinterpret_cast<const float*>(reinterpret_cast<const char*>(proj) + y * proj_pitch); // FIXME: y is wrong here

				auto denominator = dist_src - x * cosf(angle) - y * sinf(angle);
				auto dist_so = dist_src + dist_det;
				auto u = (dist_so * (-x * sinf(angle) + y * cosf(angle))) / denominator;
				auto v = (dist_so * z) / denominator;
				auto w2 = dist_src / denominator;

				// this is wrong
				row[x] += (1.f / (2.f * M_PI * num_proj)) * proj_row[static_cast<std::uint32_t>(u) + static_cast<std::uint32_t>(v) * proj_w] * w2;
			}
		}


		Feldkamp::Feldkamp(const common::Geometry& geo)
		: scheduler_{FeldkampScheduler<float>::instance(geo)}
		, geo_(geo), input_num_{0u}, input_num_set_{false}
		{
			auto device_count = int{};
			ddrf::cuda::check(cudaGetDeviceCount(&device_count));
		}

		auto Feldkamp::process(input_type&& img) -> void
		{
		}

		auto Feldkamp::wait() -> output_type
		{
			return output_type{};
		}

		auto Feldkamp::set_input_num(std::uint32_t num) noexcept -> void
		{
			input_num_ = num;
			input_num_set_ = true;
		}
	}
}
