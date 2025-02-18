# Configure and Build
First, run the next script that generates the configure script.
```
./buildconf.sh
```
Now, execute configure to set the environment.
```
./configure --prefix=$GIAPI_ROOT/external/expat
```
Compile and install the library. 

```
make && make install
```
