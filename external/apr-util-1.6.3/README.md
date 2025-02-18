# Configure and Build
Execute configure to set the environment.
```
./configure --with-apr=$GIAPI_ROOT/external/apr --with-expat=$GIAPI_ROOT/external/expat --prefix=$GIAPI_ROOT/external/apr-util
```
Compile and install the library. 

```
make -j 4 && make install
```
