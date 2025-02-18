FROM rockylinux/rockylinux:9
RUN dnf -y group install "Development Tools" 
RUN dnf -y install  cmake


COPY . /home/software/giapi-glue-cc/external
USER root
# set common env variables for building
ENV GIAPI_ROOT=/home/software/giapi-glue-cc
ENV BOOST_ROOT=/home/software/giapi-glue-cc/external/boost


## APR 
RUN cd $GIAPI_ROOT/external/apr-1.7.5 && \ 
    ./configure --prefix=$GIAPI_ROOT/external/apr && \
    make -j 4 && make install && ln -s $GIAPI_ROOT/external/apr-1.7.5 apr-1


## Expat
RUN cd $GIAPI_ROOT/external/expat-2.6.4 && \
    ./buildconf.sh && \
    ./configure --prefix=$GIAPI_ROOT/external/expat && \
    make -j 4 && make install


## APR-util 
RUN cd $GIAPI_ROOT/external/apr-util-1.6.3 && \
    ./configure --with-apr=$GIAPI_ROOT/external/apr \
    --with-expat=$GIAPI_ROOT/external/expat \
    --prefix=$GIAPI_ROOT/external/apr-util && \
    make -j 4 && make install 


## log4cxx
RUN cd $GIAPI_ROOT/external/apache-log4cxx-1.3.1 && \
    export PKG_CONFIG_PATH=$GIAPI_ROOT/external/apr/lib/pkgconfig:$GIAPI_ROOT/external/apr-util/lib/pkgconfig:$GIAPI_ROOT/external/expat/lib/pkgconfig:$PKG_CONFIG_PATH && \
    mkdir build && cd build && \
    cmake ../ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/log4cxx -DCMAKE_PREFIX_PATH="$GIAPI_ROOT/external/apr;$GIAPI_ROOT/external/apr-util;$GIAPI_ROOT/external/expat" && \
    make -j 4 && make install


## ActiveMQ
RUN cd $GIAPI_ROOT/external/activemq-cpp-library-3.9.5 && \
    autoreconf -fi && \
    ./configure --prefix=$GIAPI_ROOT/external/activemq-cpp \
    --with-apr=$GIAPI_ROOT/external/apr \
    --without-openssl && \
    make -j 4 && make install


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
