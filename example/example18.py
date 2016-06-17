#!/usr/bin/env python
from __future__ import print_function
import sys
sys.path.append('.')

import example

try:
    example.something_that_throws1()
except example.MyError as e:
    print(type(e), e)

try:
    example.something_that_throws2()
except Exception as e:
    print(type(e), e)

try:
    example.something_that_throws3()
except Exception as e:
    print(type(e), e)

try:
    example.something_that_throws4()
except example.MyError as e:
    print(type(e), e)

try:
    example.something_that_throws5()
except Exception as e:
    print(type(e), e)
