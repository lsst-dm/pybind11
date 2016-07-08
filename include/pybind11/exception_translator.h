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

struct exception {};

void register_python_exception() {
    auto tindex = std::type_index(typeid(exception));
    auto &internals = detail::get_internals();
    if (internals.registered_types_cpp.find(tindex) == internals.registered_types_cpp.end()) {
        detail::type_info *tinfo = new detail::type_info();
        tinfo->type = (PyTypeObject *) PyExc_Exception;
        tinfo->type_size = sizeof(exception);
        tinfo->init_holder = nullptr;
        internals.registered_types_cpp[tindex] = tinfo;
    }
}

template <typename ExceptionTranslator>
void register_exception_translator(ExceptionTranslator&& translator) {
    detail::get_internals().registered_exception_translators.push_front(std::forward<ExceptionTranslator>(translator));
}

NAMESPACE_END(pybind11)

