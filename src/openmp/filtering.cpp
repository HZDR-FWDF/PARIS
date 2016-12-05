/*
 * This file is part of the ddafa reconstruction program.
 *
 * Copyright (C) 2016 Helmholtz-Zentrum Dresden-Rossendorf
 *
 * ddafa is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ddafa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ddafa. If not, see <http://www.gnu.org/licenses/>.
 *
 * Date: 05 December 2016
 * Authors: Jan Stephan <j.stephan@hzdr.de>
 */

#include <fftw3.h>

#include "backend.h"

namespace ddafa
{
    namespace openmp
    {
        namespace fft
        {
            auto make_forward_plan(int rank, int* n, int batch_size,
                                float* in, int* inembed, int istride, int idist,
                                complex_type* out, int* onembed, int ostride, int odist) -> forward_plan_type
            {
                return fftwf_plan_many_dft_r2c(rank, n, batch_size,
                                              in, inembed, istride, idist,
                                              out, onembed, ostride, odist,
                                              FFTW_MEASURE | FFTW_PRESERVE_INPUT);
            }

            auto make_inverse_plan(int rank, int* n, int batch_size,
                                complex_type* in, int* inembed, int istride, int idist,
                                float* out, int* onembed, int ostride, int odist) -> inverse_plan_type
            {
                return fftwf_plan_many_dft_c2r(rank, n, batch_size,
                                              in, inembed, istride, idist,
                                              out, onembed, ostride, odist,
                                              FFTW_MEASURE | FFTW_DESTROY_INPUT);
            }
        }
    }
}
