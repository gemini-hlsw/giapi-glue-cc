# Configure and Build
Execute configure to set the environment.
```
./configure --prefix=$GIAPI_ROOT/external/apr CXXFLAGS="-std=c++14"
```
Compile and install the library. 

```
make && make install && ln -s $GIAPI_ROOT/external/apr-1.7.5 apr-1
```
