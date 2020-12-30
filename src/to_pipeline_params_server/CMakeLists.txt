add_definitions(-DPARAMSSERV_LIBRARY)

add_library_qt(paramsserver
        parameterserver.cpp
        urldecode.cpp
        mongoose.c
    )
