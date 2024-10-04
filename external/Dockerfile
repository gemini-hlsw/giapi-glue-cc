FROM rockylinux/rockylinux:8.4
RUN dnf -y group install "Development Tools" 
RUN dnf -y install  cmake
COPY . /home/software/giapi-glue-cc/external
USER root
# set common env variables for building
ENV GIAPI_ROOT=/home/software/giapi-glue-cc
ENV BOOST_ROOT=/home/software/giapi-glue-cc/external/boost

## curl library
RUN cd $GIAPI_ROOT/external/curl-7.21.6/ && \
    ./configure  --without-ssl --prefix=$GIAPI_ROOT/external/libcurl && \
    make -j 4 && make install

## curlpp
RUN mkdir $GIAPI_ROOT/external/curlpp-0.8.1/install && \ 
    cd $GIAPI_ROOT/external/curlpp-0.8.1 && cmake -S ./ -B ./install/ -DCMAKE_INSTALL_PREFIX=$GIAPI_ROOT/external/curlpp  && \
    cd $GIAPI_ROOT/external/curlpp-0.8.1/install && \
    make -j 4 && make install 


 # APR 
RUN cd $GIAPI_ROOT/external/apr-1.3.12 && \ 
    ./configure --prefix=$GIAPI_ROOT/external/apr && \
     make -j 4 && make install && ln -s $GIAPI_ROOT/external/apr-1.3.1 apr-1
# APR-util 
RUN  cd $GIAPI_ROOT/external/apr-util-1.3.10 && \
    ./configure --with-apr=$GIAPI_ROOT/external/apr \
    --prefix=$GIAPI_ROOT/external/apr-util && \
    make -j 4 && make install 


# ActiveMQ
RUN cd $GIAPI_ROOT/external/activemq-cpp-library-3.4.1 && \
    ./configure --prefix=$GIAPI_ROOT/external/activemq-cpp \
    --with-apr=$GIAPI_ROOT/external/apr \
    -with-apr-util=$GIAPI_ROOT/external/apr-util --without-openssl && \
    make -j 4 && make install

# log4cxx
RUN cd $GIAPI_ROOT/external/apache-log4cxx-0.11.0 && \
    ./configure --prefix=$GIAPI_ROOT/external/log4cxx \
    --with-apr=$GIAPI_ROOT/external/apr  \
    --with-apr-util=$GIAPI_ROOT/external/apr-util && \
    autoreconf --force --install && \
    make -j 4 && make install


