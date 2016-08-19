/*
 * This file is part of the ddafa reconstruction program.
 *
 * Copyright (C) 2016 Helmholtz-Zentrum Dresden-Rossendorf
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved by
 * the European Commission - subsequent version of the EUPL (the "Licence");
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the Licence is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the Licence for the specific language governing permissions and
 * limitations under the Licence.
 *
 * Date: 18 August 2016
 * Authors: Jan Stephan
 */

#ifndef DDAFA_WEIGHTING_STAGE_H_
#define DDAFA_WEIGHTING_STAGE_H_

#include <atomic>
#include <functional>
#include <map>
#include <utility>

#include <boost/lockfree/spsc_queue.hpp>

#include <ddrf/cuda/memory.h>

#include "metadata.h"

namespace ddafa
{
    class weighting_stage
    {
        private:
            using device_allocator = ddrf::cuda::device_allocator<float, ddrf::memory_layout::pointer_2D>;
            using pool_allocator = ddrf::pool_allocator<float, ddrf::memory_layout::pointer_2D, device_allocator>;
            using smart_pointer = typename pool_allocator::smart_pointer;

        public:
            using input_type = std::pair<smart_pointer, projection_metadata>;
            using output_type = std::pair<smart_pointer, projection_metadata>;

        public:
            weighting_stage(std::uint32_t n_row, std::uint32_t n_col,
                            float l_px_row, float l_px_col,
                            float delta_s, float delta_t,
                            float d_so, float d_od) noexcept;

            auto run() -> void;
            auto set_input_function(std::function<input_type(void)> input) noexcept -> void;
            auto set_output_function(std::function<void(output_type)> output) noexcept -> void;

        private:
            auto process(int);

        private:
            std::function<input_type(void)> input_;
            std::function<void(output_type)> output_;

            float h_min_;
            float v_min_;
            float d_sd_;

            std::map<int, std::queue<input_type>> input_map_;
            std::atomic_flag lock_;
    };
}



#endif /* DDAFA_WEIGHTING_STAGE_H_ */
