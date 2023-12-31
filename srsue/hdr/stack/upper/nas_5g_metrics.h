/**
 * Copyright 2013-2023 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#ifndef SRSUE_NAS_5G_METRICS_H
#define SRSUE_NAS_5G_METRICS_H

#include "nas_5gmm_state.h"

namespace srsue {

struct nas_5g_metrics_t {
  uint32_t              nof_active_pdu_sessions;
  mm5g_state_t::state_t state;
};

} // namespace srsue

#endif // SRSUE_NAS_5G_METRICS_H
