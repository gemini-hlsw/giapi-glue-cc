# Configure and Build
Export PKG_CONFIG to set the environment.
```
export PKG_CONFIG_PATH=$GIAPI_ROOT/external/apr/lib/pkgconfig:$GIAPI_ROOT/external/apr-util/lib/pkgconfig:$GIAPI_ROOT/external/expat/lib/pkgconfig:$PKG_CONFIG_PATH
```
```
mkdir build && cd build
```

Execute the cmake.
```
cmake ../ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/log4cxx -DCMAKE_PREFIX_PATH="$GIAPI_ROOT/external/apr;$GIAPI_ROOT/external/apr-util;$GIAPI_ROOT/external/expat" -DCMAKE_CXX_STANDARD=14
```

Compile and install the library. 
```
make && make install
```
