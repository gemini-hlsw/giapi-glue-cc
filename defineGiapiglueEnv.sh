
# This bash file defines the environment variable for GIAPI_GLUE. The 
export GIAPI_ROOT=`pwd | sed -E 's/(src|test).*//'`
export BOOST_ROOT=${GIAPI_ROOT}/external/boost
export LD_LIBRARY_PATH=${GIAPI_ROOT}/external/apr/lib:${GIAPI_ROOT}/external/apr-util/lib:${GIAPI_ROOT}/external/activemq-cpp/lib:${GIAPI_ROOT}/external/log4cxx/lib64:${GIAPI_ROOT}/external/cppunit/lib:${GIAPI_ROOT}/external/curlpp/lib:${GIAPI_ROOT}/external/curlpp/lib64:${GIAPI_ROOT}/external/libcurl/lib:${GIAPI_ROOT}/external/libcurl/lib64:${GIAPI_ROOT}/install/lib
