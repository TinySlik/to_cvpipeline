cmake_minimum_required (VERSION 2.8.0)

add_library(paramserv  SHARED
                         mongoose.c 
                         parameterserver.cpp
                         urldecode.cpp
                         )
