/*
    example/example9.cpp -- exception translation

    Copyright (c) 2016 Pim Schellart <P.Schellart@princeton.edu

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#include "example.h"
#include <pybind11/exception_translator.h>

// A type that should be raised as an exeption in Python
class MyError : public std::exception {
public:
    explicit MyError(const char * m) : message{m} {}
    virtual const char * what() const noexcept override {return message.c_str();}
private:
    std::string message = "";
};

// A type that should be translated to a standard Python exception
class MyError2 : public std::exception {
public:
    explicit MyError2(const char * m) : message{m} {}
    virtual const char * what() const noexcept override {return message.c_str();}
private:
    std::string message = "";
};

void something_that_throws1() {
    throw MyError("this error should go to a custom type");
}

void something_that_throws2() {
    throw MyError2("this error should go to a standard Python exception");
}

void something_that_throws3() {
    throw std::logic_error("this error should fall through to the standard handler");
}

void init_ex18(py::module &m) {
    py::class_<MyError> cls(m, "MyError");

    cls.def(py::init<const char *>())
        .def("what", &MyError::what)
        .def("__repr__", &MyError::what);

    // store custom exception type MyError and register new translator
    static auto my_exception_type = cls.ptr();
    py::register_exception_translator([](std::exception_ptr p) -> bool {
        try {
            if (p) {
                std::rethrow_exception(p);
            }
        } catch (const MyError &e) {
            // store current exception to ensure proper cleanup
            static auto current_exception = py::cast(MyError(e.what()));
            PyErr_SetObject(my_exception_type, current_exception.ptr()); 
            return true;
        } catch (...) {
            // rethrow is not allowed exit this scope as
            // this would break out the chain of registered
            // exception handlers early
        }
        return false;
    });

    // register new translator for MyError2
    // no need to store anything here because this type will
    // never by visible from Python
    py::register_exception_translator([](std::exception_ptr p) -> bool {
        try {
            if (p) {
                std::rethrow_exception(p);
            }
        } catch (const MyError2 &e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return true;
        } catch (...) {
            // rethrow is not allowed exit this scope as
            // this would break out the chain of registered
            // exception handlers early
        }
        return false;
    });

    m.def("something_that_throws1", &something_that_throws1);
    m.def("something_that_throws2", &something_that_throws2);
    m.def("something_that_throws3", &something_that_throws3);
}

