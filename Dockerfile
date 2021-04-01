FROM giapi-glue-external:R15
COPY . /home/software/giapi-glue-cc
# set common env variables for building
ENV GIAPI_ROOT=/home/software/giapi-glue-cc
ENV BOOST_ROOT=/home/software/giapi-glue-cc/external/boost
WORKDIR /home/software/giapi-glue-cc
# build lib giapi-glue        
RUN make && make install && \ 
    echo ${GIAPI_ROOT}/external/apr/lib >> /etc/ld.so.conf && \
    echo ${GIAPI_ROOT}/external/apr-util/lib >> /etc/ld.so.conf && \
    echo ${GIAPI_ROOT}/external/activemq-cpp/lib >> /etc/ld.so.conf  && \
    echo ${GIAPI_ROOT}/external/log4cxx/lib >> /etc/ld.so.conf  && \
    echo ${GIAPI_ROOT}/external/cppunit/lib >> /etc/ld.so.conf  && \
    echo ${GIAPI_ROOT}/external/curlpp/lib >> /etc/ld.so.conf  && \
    echo ${GIAPI_ROOT}/install/lib >> /etc/ld.so.conf && \
    ldconfig
