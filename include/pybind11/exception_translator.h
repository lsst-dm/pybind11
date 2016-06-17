/*
    pybind11/exception_translator.h: Translators for custom exceptions

    Copyright (c) 2016 Pim Schellart <P.Schellart@princeton.edu>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#pragma once

#include "pybind11.h"
#include <forward_list>
#include <iostream>

NAMESPACE_BEGIN(pybind11)

template <typename ExceptionTranslator>
void register_exception_translator(ExceptionTranslator translator) {
    detail::get_internals().registered_exception_translators.push_front(std::move(translator));
}

//NAMESPACE_BEGIN(detail)
//NAMESPACE_END(detail)

NAMESPACE_END(pybind11)

