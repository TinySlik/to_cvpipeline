cmake_minimum_required (VERSION 3.1)

add_library(paramserv  SHARED
                         mongoose.c 
                         parameterserver.cpp
                         urldecode.cpp
                         )
