# Configure and Build
First, regenerate the configure files.
```
autoreconf -fi
```
Next, execute configure to set the environment.
```
./configure --prefix=$GIAPI_ROOT/external/activemq-cpp --with-apr=$GIAPI_ROOT/external/apr --without-openssl
```
Compile and install the library. 

```
make && make install
```
