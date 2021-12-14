// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "itt.hpp"

#include "ngraph/op/cosh.hpp"
#include "ngraph/op/multiply.hpp"
#include "ngraph/op/sinh.hpp"

#include "ngraph/runtime/host_tensor.hpp"
#include "ngraph/runtime/reference/sinh.hpp"

using namespace std;
using namespace ngraph;

constexpr NodeTypeInfo op::Sinh::type_info;

op::Sinh::Sinh(const Output<Node>& arg)
    : UnaryElementwiseArithmetic(arg)
{
    constructor_validate_and_infer_types();
}

bool ngraph::op::v0::Sinh::visit_attributes(AttributeVisitor& visitor)
{
    NGRAPH_OP_SCOPE(v0_Sinh_visit_attributes);
    return true;
}

shared_ptr<Node> op::Sinh::clone_with_new_inputs(const OutputVector& new_args) const
{
    NGRAPH_OP_SCOPE(v0_Sinh_clone_with_new_inputs);
    check_new_args_count(this, new_args);
    return make_shared<Sinh>(new_args.at(0));
}

namespace sinhop
{
    template <element::Type_t ET>
    inline bool evaluate(const HostTensorPtr& arg0, const HostTensorPtr& out, const size_t count)
    {
        using T = typename element_type_traits<ET>::value_type;
        runtime::reference::sinh<T>(arg0->get_data_ptr<ET>(), out->get_data_ptr<ET>(), count);
        return true;
    }

    bool evaluate_sinh(const HostTensorPtr& arg0, const HostTensorPtr& out, const size_t count)
    {
        bool rc = true;
        out->set_unary(arg0);

        switch (arg0->get_element_type())
        {
            NGRAPH_TYPE_CASE(evaluate_sinh, boolean, arg0, out, count);
            NGRAPH_TYPE_CASE(evaluate_sinh, i32, arg0, out, count);
            NGRAPH_TYPE_CASE(evaluate_sinh, i64, arg0, out, count);
            NGRAPH_TYPE_CASE(evaluate_sinh, u32, arg0, out, count);
            NGRAPH_TYPE_CASE(evaluate_sinh, u64, arg0, out, count);
            NGRAPH_TYPE_CASE(evaluate_sinh, f16, arg0, out, count);
            NGRAPH_TYPE_CASE(evaluate_sinh, f32, arg0, out, count);
        default: rc = false; break;
        }
        return rc;
    }
} // namespace sinhop

bool op::Sinh::evaluate(const HostTensorVector& outputs, const HostTensorVector& inputs) const
{
    NGRAPH_OP_SCOPE(v0_Sinh_evaluate);
    return sinhop::evaluate_sinh(inputs[0], outputs[0], shape_size(get_output_shape(0)));
}

bool op::Sinh::has_evaluate() const
{
    NGRAPH_OP_SCOPE(v0_Sinh_has_evaluate);
    switch (get_input_element_type(0))
    {
    case ngraph::element::boolean:
    case ngraph::element::i32:
    case ngraph::element::i64:
    case ngraph::element::u32:
    case ngraph::element::u64:
    case ngraph::element::f16:
    case ngraph::element::f32: return true;
    default: break;
    }
    return false;
}