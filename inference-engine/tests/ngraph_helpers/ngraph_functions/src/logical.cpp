// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <memory>
#include <ngraph/opsets/opset3.hpp>
#include "ngraph_functions/utils/ngraph_helpers.hpp"

namespace ngraph {
namespace builder {

std::shared_ptr<ngraph::Node> makeLogical(const ngraph::Output<Node> &in0,
                                          const ngraph::Output<Node> &in1,
                                          ngraph::helpers::LogicalTypes logicalType) {
    switch (logicalType) {
        case ngraph::helpers::LogicalTypes::LOGICAL_AND:
            return std::make_shared<ngraph::opset3::LogicalAnd>(in0, in1);
        case ngraph::helpers::LogicalTypes::LOGICAL_OR:
            return std::make_shared<ngraph::opset3::LogicalOr>(in0, in1);
        case ngraph::helpers::LogicalTypes::LOGICAL_NOT:
            return std::make_shared<ngraph::opset3::LogicalNot>(in0);
        case ngraph::helpers::LogicalTypes::LOGICAL_XOR:
            return std::make_shared<ngraph::opset3::LogicalXor>(in0, in1);
        default: {
            throw std::runtime_error("Incorrect type of Logical operation");
        }
    }
}

}  // namespace builder
}  // namespace ngraph