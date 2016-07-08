#!/usr/bin/env python
from __future__ import print_function
import sys
sys.path.append('.')

import example
import inspect

print("Exception type bases:")
print(inspect.getmro(example.MyException))
print(inspect.getmro(example.MyBaseException))
print(inspect.getmro(example.MyDerivedException1))
print("")

print("Can we catch a MyException as it's own type?")
try:
    example.throws1()
except example.MyException as e:
    print(type(e), e.what())
print("")

print("Can we catch a MyException through Python Exception?")
try:
    example.throws1()
except Exception as e:
    print(type(e), e)
print("")

print("Can we raise a MyException in Python?")
try:
    raise example.MyException("raised from Python")
except Exception as e:
    print(type(e), e)
print("")

print("Can we translate to standard Python exceptions?")
try:
    example.throws2()
except Exception as e:
    print(type(e), e)
print("")

print("Can we handle unknown exceptions?")
try:
    example.throws3()
except Exception as e:
    print(type(e), e)
print("")

print("Can we delegate to another handler by rethrowing?")
try:
    example.throws4()
except Exception as e:
    print(type(e), e)
print("")

print("Can we fall-through to the default handler?")
try:
    example.throws_logic_error()
except Exception as e:
    print(type(e), e)
print("")

print("Can we catch a MyBaseException as it's own type?")
try:
    example.throws_base()
except example.MyBaseException as e:
    print(type(e), e.what())
print("")

print("Can we catch a MyBaseException through Python Exception?")
try:
    example.throws_base()
except Exception as e:
    print(type(e), e)
print("")

print("Can we catch a MyDerivedException1 as it's own type?")
try:
    example.throws_derived1()
except example.MyDerivedException1 as e:
    print(type(e), e.what())
print("")

print("Can we catch a MyDerivedException1 through Python Exception?")
try:
    example.throws_derived1()
except Exception as e:
    print(type(e), e)
print("")

print("Can we catch a MyDerivedException1 through MyBaseException?")
try:
    example.throws_derived1()
except example.MyBaseException as e:
    print(type(e), e)
print("")

print("Can we catch a MyDerivedException2 through Python Exception?")
try:
    example.throws_derived2()
except Exception as e:
    print(type(e), e)
print("")

print("Can we catch a MyDerivedException2 through MyBaseException?")
try:
    example.throws_derived2()
except example.MyBaseException as e:
    print(type(e), e)
print("")

