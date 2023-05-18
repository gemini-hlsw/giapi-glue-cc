# Overview
These are the steps  that GEMINI uses to compile this library on CENTOS 7 and ROCKY 8. However, depending on the linux platform you will have to execute different actions or commands.
# Configure and Build
Execute configure to set the environment.
```
./configure --prefix=$GIAPI_ROOT/external/apr
```
Compile and install the library. 

```
make && make install
```
