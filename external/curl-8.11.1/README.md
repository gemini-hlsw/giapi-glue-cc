# Configure and Build
First, regenerate the configure files.
```
autoreconf -fi
```
Next, execute configure to set the environment.
```
./configure --without-libpsl --without-ssl --prefix=$GIAPI_ROOT/external/libcurl
```
Compile and install the library. 

```
make && make install
```
