// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//
#include "ngraph/op/roi_align.hpp"

#include <memory>

#include "intel_gpu/plugin/common_utils.hpp"
#include "intel_gpu/plugin/program.hpp"
#include "intel_gpu/primitives/roi_align.hpp"

namespace ov {
namespace intel_gpu {

namespace {

template <typename T>
cldnn::roi_align::PoolingMode from(T mode) {
    switch (mode) {
    case T::MAX:
        return cldnn::roi_align::PoolingMode::max;
    case T::AVG:
    default:
        return cldnn::roi_align::PoolingMode::avg;
    }
}

cldnn::roi_align::AlignedMode from(ngraph::op::v9::ROIAlign::AlignedMode mode) {
    switch (mode) {
    case ngraph::op::v9::ROIAlign::AlignedMode::HALF_PIXEL_FOR_NN:
        return cldnn::roi_align::AlignedMode::half_pixel_for_nn;
    case ngraph::op::v9::ROIAlign::AlignedMode::HALF_PIXEL:
        return cldnn::roi_align::AlignedMode::half_pixel;
    case ngraph::op::v9::ROIAlign::AlignedMode::ASYMMETRIC:
    default:
        return cldnn::roi_align::AlignedMode::asymmetric;
    }
}

void CreateROIAlignOp(Program& p, const std::shared_ptr<ngraph::op::v3::ROIAlign>& op) {
    p.ValidateInputs(op, {3});
    auto roi_align_prim = cldnn::roi_align(layer_type_name_ID(op),
                                           p.GetInputPrimitiveIDs(op),
                                           op->get_pooled_h(),
                                           op->get_pooled_w(),
                                           op->get_sampling_ratio(),
                                           op->get_spatial_scale(),
                                           from(op->get_mode()),
                                           cldnn::roi_align::AlignedMode::asymmetric,
                                           op->get_friendly_name());
    p.AddPrimitive(roi_align_prim);
    p.AddPrimitiveToProfiler(op);
}

void CreateROIAlignOp(Program& p, const std::shared_ptr<ngraph::op::v9::ROIAlign>& op) {
    p.ValidateInputs(op, {3});
    auto roi_align_prim = cldnn::roi_align(layer_type_name_ID(op),
                                           p.GetInputPrimitiveIDs(op),
                                           op->get_pooled_h(),
                                           op->get_pooled_w(),
                                           op->get_sampling_ratio(),
                                           op->get_spatial_scale(),
                                           from(op->get_mode()),
                                           from(op->get_aligned_mode()),
                                           op->get_friendly_name());
    p.AddPrimitive(roi_align_prim);
    p.AddPrimitiveToProfiler(op);
}

}  // anonymous namespace

REGISTER_FACTORY_IMPL(v3, ROIAlign);
REGISTER_FACTORY_IMPL(v9, ROIAlign);

}  // namespace intel_gpu
}  // namespace ov
