# Configure and Build
Execute configure to set the environment.
```
./configure --with-apr=$GIAPI_ROOT/external/apr --with-expat=$GIAPI_ROOT/external/expat --prefix=$GIAPI_ROOT/external/apr-util CXXFLAGS="-std=c++14"
```
Compile and install the library. 

```
make && make install
```
