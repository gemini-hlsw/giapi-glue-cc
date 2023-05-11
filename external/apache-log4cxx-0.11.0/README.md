# Overview
These are the steps  that GEMINI uses to compile this library on CENTOS 7 and ROCKY 8. However, depending on the linux platform you will have to execute different actions or commands. 

# Configure and Build

## Centos 7 Actions
Install the automake-1.16.1-95.1.noarch.rpm (this is not necessary for rocky 8, because rocky 8 has for default installed the automake).
```
sudo yum install automake-1.16.1-95.1.noarch.rpm
```
Execute configure to set the environment. 
```
./configure --prefix=$GIAPI_ROOT/external/log4cxx --with-apr=$GIAPI_ROOT/external/apr --with-apr-util=$GIAPI_ROOT/external/apr-util
```
Execute the autoreconf. Autoreconf is a Autotool which is used to create automatically buildable source code for Unix-like systems.
   -f, --force : This option is used to consider all files obsolete.
```
autoreconf --force --install 
```
Finally compile and install
```
make && make install
```
## Rocky 8 Actions
Execute configure to set the environment. 
```
./configure --prefix=$GIAPI_ROOT/external/log4cxx --with-apr=$GIAPI_ROOT/external/apr --with-apr-util=$GIAPI_ROOT/external/apr-util
```

Try to compile and install the library. If you see the following error *error configure.ac:47: error: required file './compile' not found* you have to execute the automake command (see bellow statement).
```
make && make install
```

The automake is a tool used for automatically generating Makefile.in files compliant with the set GNU Coding Standards. 
   -- add-missing : Adds the standard missing files to the package.
```
automake --add-missing
```
Finally compile and install
```
make && make install
```

