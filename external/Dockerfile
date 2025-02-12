FROM rockylinux/rockylinux:9
RUN dnf -y group install "Development Tools" 
RUN dnf -y install  cmake

# Package necessary for the compilation of APR-util external library.
RUN dnf -y install expat-devel

COPY . /home/software/giapi-glue-cc/external
USER root
# set common env variables for building
ENV GIAPI_ROOT=/home/software/giapi-glue-cc
ENV BOOST_ROOT=/home/software/giapi-glue-cc/external/boost

## curl library
RUN cd $GIAPI_ROOT/external/curl-8.11.1/ && \
    autoreconf -fi && \
    ./configure --without-libpsl --without-ssl --prefix=$GIAPI_ROOT/external/libcurl && \
    make -j 4 && make install

## curlpp
RUN cd $GIAPI_ROOT/external/curlpp-0.8.1/ && \
    export PKG_CONFIG_PATH=$GIAPI_ROOT/external/libcurl/lib/pkgconfig:$PKG_CONFIG_PATH && \
    pkg-config --cflags --libs libcurl && \
    cmake -S ./ -B ./install/ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/curlpp \
    -DCMAKE_INSTALL_LIBDIR=lib && \
    cd install && \
    make -j 4 && make install


 # APR 
RUN cd $GIAPI_ROOT/external/apr-1.7.5 && \ 
    ./configure --prefix=$GIAPI_ROOT/external/apr && \
     make -j 4 && make install && ln -s $GIAPI_ROOT/external/apr-1.7.5 apr-1


# APR-util 
RUN cd $GIAPI_ROOT/external/apr-util-1.6.3 && \
    ./configure --with-apr=$GIAPI_ROOT/external/apr \
    --prefix=$GIAPI_ROOT/external/apr-util && \
    make -j 4 && make install 


# ActiveMQ
RUN cd $GIAPI_ROOT/external/activemq-cpp-library-3.9.5 && \
    ./configure --prefix=$GIAPI_ROOT/external/activemq-cpp \
    --with-apr=$GIAPI_ROOT/external/apr \
    --with-apr-util=$GIAPI_ROOT/external/apr-util --without-openssl && \
    make -j 4 && make install

# log4cxx
RUN cd $GIAPI_ROOT/external/apache-log4cxx-1.3.1 && \
    PKG_CONFIG_PATH=$GIAPI_ROOT/external/apr/lib/pkgconfig:$GIAPI_ROOT/external/apr-util/lib/pkgconfig \
    cmake -S ./ -B ./install/ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/log4cxx \
    -DAPR_ROOT=$GIAPI_ROOT/external/apr \
    -DAPRUTIL_ROOT=$GIAPI_ROOT/external/apr-util \
    -DCMAKE_CXX_STANDARD=20 && \
    cd install && \
    make -j 4 && make install
