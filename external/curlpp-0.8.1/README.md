# Overview
These are the steps  that GEMINI uses to compile this library on CENTOS 7 and ROCKY 8. However, depending on the linux platform you will have to execute different actions or commands.

# Configure and Build

## Centos 7 Actions
Execute cmake to set the environment to the compilation. 
```
cmake ./ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/curlpp
```
Compile and install
```
make && make install
```

## Rocky 8 Actions
Execute cmake and create the install folder where the environment setup will be done. 
```
mkdir install
cmake -S ./ -B ./install/ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/curlpp
cd ./install
```
Compile and install
```
make && make install
```


