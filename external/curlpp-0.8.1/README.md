# Configure and Build
Export PKG_CONFIG to set the environment.
```
export PKG_CONFIG_PATH=$GIAPI_ROOT/external/libcurl/lib/pkgconfig:$PKG_CONFIG_PATH
```
Execute the pkg-config.
```
pkg-config --cflags --libs libcurl
```

Execute the cmake.
```
cmake -S ./ -B ./install/ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/curlpp -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_CXX_STANDARD=14 && cd install
```

Compile and install the library. 
```
make && make install
```
