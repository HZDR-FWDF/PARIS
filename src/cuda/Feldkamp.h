#ifndef CUDA_FELDKAMP_H_
#define CUDA_FELDKAMP_H_

#include <atomic>
#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include <ddrf/Image.h>
#include <ddrf/Queue.h>
#include <ddrf/Volume.h>
#include <ddrf/cuda/DeviceMemoryManager.h>
#include <ddrf/cuda/HostMemoryManager.h>
#include <ddrf/cuda/Memory.h>

#include "../common/Geometry.h"

#include "FeldkampScheduler.h"

namespace ddafa
{
	namespace cuda
	{
		class Feldkamp
		{
			public:
				using input_type = ddrf::Image<ddrf::cuda::DeviceMemoryManager<float>>;
				using output_type = ddrf::Volume<ddrf::cuda::HostMemoryManager<float>>;
				using volume_type = ddrf::Volume<ddrf::cuda::DeviceMemoryManager<float>>;

			public:
				Feldkamp(const common::Geometry& geo, const std::string& angles);
				auto process(input_type&&) -> void;
				auto wait() -> output_type;
				auto set_input_num(std::uint32_t) noexcept -> void;

			private:
				auto parse_angles(const std::string&) -> void;
				auto create_volumes(int) -> void;
				auto processor(int) -> void;
				auto merge_volumes() -> void;

			protected:
				~Feldkamp();

			private:
				std::map<int, ddrf::Queue<input_type>> map_imgs_;
				ddrf::Queue<output_type> results_;
				int devices_;
				bool done_;

				FeldkampScheduler<float> scheduler_;
				common::Geometry geo_;

				float dist_sd_;
				FeldkampScheduler<float>::VolumeGeometry vol_geo_;

				std::uint32_t input_num_;
				std::atomic_bool input_num_set_;

				std::atomic_bool angle_tabs_created_;
				std::once_flag angle_flag_;
				std::vector<float> sin_tab_;
				std::vector<float> cos_tab_;

				std::uint32_t current_img_;
				float current_angle_;

				std::map<int, std::vector<volume_type>> volume_map_;

				std::map<int, std::thread> processor_threads_;
		};
	}
}


#endif /* CUDA_FELDKAMP_H_ */