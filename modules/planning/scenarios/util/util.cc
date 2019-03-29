/******************************************************************************
 * Copyright 2019 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/planning/scenarios/util/util.h"

#include "modules/common/util/map_util.h"
#include "modules/map/pnc_map/path.h"
#include "modules/planning/common/planning_context.h"

namespace apollo {
namespace planning {
namespace scenario {
namespace util {

using hdmap::PathOverlap;
using perception::TrafficLight;

/*
 * @brief: read signal info
 */
TrafficLight GetSignal(const std::string& traffic_light_id) {
  const auto* result = common::util::FindPtrOrNull(
      PlanningContext::GetScenarioInfo()->traffic_lights, traffic_light_id);

  if (result == nullptr) {
    TrafficLight traffic_light;
    traffic_light.set_id(traffic_light_id);
    traffic_light.set_color(TrafficLight::UNKNOWN);
    traffic_light.set_confidence(0.0);
    traffic_light.set_tracking_time(0.0);
    return traffic_light;
  }
  return *result;
}

hdmap::PathOverlap* GetOverlapOnReferenceLine(
    const ReferenceLineInfo& reference_line_info,
    const std::string& overlap_id,
    const ReferenceLineInfo::OverlapType& overlap_type) {
  if (overlap_type == ReferenceLineInfo::STOP_SIGN) {
    const auto& stop_sign_overlaps =
        reference_line_info.reference_line().map_path().stop_sign_overlaps();
    for (const auto& stop_sign_overlap : stop_sign_overlaps) {
      if (stop_sign_overlap.object_id == overlap_id) {
        return const_cast<hdmap::PathOverlap*>(&stop_sign_overlap);
      }
    }
  } else if (overlap_type == ReferenceLineInfo::SIGNAL) {
    const auto& traffic_light_overlaps =
        reference_line_info.reference_line().map_path().signal_overlaps();
    for (const auto& traffic_light_overlap : traffic_light_overlaps) {
      if (traffic_light_overlap.object_id == overlap_id) {
        return const_cast<hdmap::PathOverlap*>(&traffic_light_overlap);
      }
    }
  }

  return nullptr;
}

}  // namespace util
}  // namespace scenario
}  // namespace planning
}  // namespace apollo
