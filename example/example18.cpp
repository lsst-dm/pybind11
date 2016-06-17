/*
    example/example9.cpp -- exception translation

    Copyright (c) 2016 Pim Schellart <P.Schellart@princeton.edu

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#include "example.h"
#include <pybind11/exception_translator.h>

// A type that should be raised as an exeption in Python
class MyException : public std::exception {
public:
    explicit MyException(const char * m) : message{m} {}
    virtual const char * what() const noexcept override {return message.c_str();}
private:
    std::string message = "";
};

// A type that should be translated to a standard Python exception
class MyException2 : public std::exception {
public:
    explicit MyException2(const char * m) : message{m} {}
    virtual const char * what() const noexcept override {return message.c_str();}
private:
    std::string message = "";
};

// A type that is not derived from std::exception (and is thus unknown)
class MyException3 {
public:
    explicit MyException3(const char * m) : message{m} {}
    virtual const char * what() const noexcept {return message.c_str();}
private:
    std::string message = "";
};

// A type that should be translated to MyException1
// and delegated to its exception translator
class MyException4 : public std::exception {
public:
    explicit MyException4(const char * m) : message{m} {}
    virtual const char * what() const noexcept override {return message.c_str();}
private:
    std::string message = "";
};

// Base class in exception hierarchy
class MyBaseException : public std::exception {
public:
    explicit MyBaseException(const char * m) : message{m} {}
    virtual const char * what() const noexcept override {return message.c_str();}
private:
    std::string message = "";
};

// Derived type to be raised as an exeption in python
class MyDerivedException1 : public MyBaseException {
    using MyBaseException::MyBaseException;
};

// Derived class to be handled by base class case
class MyDerivedException2 : public MyBaseException {
    using MyBaseException::MyBaseException;
};

void throws1() {
    throw MyException("this error should go to a custom type");
}

void throws2() {
    throw MyException2("this error should go to a standard Python exception");
}

void throws3() {
    throw MyException3("this error cannot be translated");
}

void throws4() {
    throw MyException4("this error is rethrown");
}

void throws_logic_error() {
    throw std::logic_error("this error should fall through to the standard handler");
}

void throws_base() {
    throw MyBaseException("base");
}

void throws_derived1() {
    throw MyDerivedException1("derived 1");
}

void throws_derived2() {
    throw MyDerivedException2("derived 2");
}

void init_ex18(py::module &m) {
    py::class_<MyException> cls(m, "MyException");

    cls.def(py::init<const char *>())
        .def("what", &MyException::what)
        .def("__repr__", &MyException::what)
        .def("__str__", &MyException::what); // Needed for Exception inheritance to work

    py::class_<MyBaseException> clsMyBaseException(m, "MyBaseException");

    clsMyBaseException.def(py::init<const char *>())
        .def("what", &MyBaseException::what)
        .def("__repr__", &MyBaseException::what)
        .def("__str__", &MyBaseException::what);

    py::class_<MyDerivedException1> clsMyDerivedException1(m, "MyDerivedException1", py::base<MyBaseException>());

    clsMyDerivedException1.def(py::init<const char *>())
        .def("what", &MyDerivedException1::what)
        .def("__repr__", &MyDerivedException1::what)
        .def("__str__", &MyDerivedException1::what);

    // store custom exception type MyException and register new translator
    static auto my_exception_type = cls.ptr();
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const MyException &e) {
            // store current exception to ensure proper cleanup
            static py::object current_exception;
            current_exception = py::cast(MyException(e.what()));
            PyErr_SetObject(my_exception_type, current_exception.ptr()); 
        }
    });

    // register new translator for MyException2
    // no need to store anything here because this type will
    // never by visible from Python
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const MyException2 &e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
    });

    // register new translator for MyException4
    // which will catch it and delegate to the previously registered
    // translator for MyException1 by throwing a new exception
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const MyException4 &e) {
            throw MyException(e.what());
        }
    });

    // register new translator with multiple catch clauses to handle
    // derived exceptions. Explicitly don't catch MyDerivedException2 which
    // should be handled as a MyBaseException
    static auto MyBaseExceptionType = clsMyBaseException.ptr();
    static auto MyDerivedException1Type = clsMyDerivedException1.ptr();

    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const MyDerivedException1 &e) {
            std::cout<<"caught MyDerivedException1 "<<e.what()<<std::endl;
            static py::object current_exception;
            current_exception = py::cast(MyDerivedException1(e.what()));
            PyErr_SetObject(MyDerivedException1Type, current_exception.ptr()); 
        } catch (const MyBaseException &e) {
            std::cout<<"caught MyBaseException"<<std::endl;
            static py::object current_exception;
            current_exception = py::cast(MyBaseException(e.what()));
            PyErr_SetObject(MyBaseExceptionType, current_exception.ptr()); 
        }
    });

    m.def("throws1", &throws1);
    m.def("throws2", &throws2);
    m.def("throws3", &throws3);
    m.def("throws4", &throws4);
    m.def("throws_logic_error", &throws_logic_error);
    m.def("throws_base", &throws_base);
    m.def("throws_derived1", &throws_derived1);
    m.def("throws_derived2", &throws_derived2);
}

