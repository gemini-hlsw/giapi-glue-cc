# Introduction

The GIAPI C++ Language glue API  is one of the Language binding options used by
instrument code to integrate with Gemini.  The C++ API is implemented as a
shared library in C++. The API sits between the instrument code and the
Gemini Master Process (GMP).

The GIAPI C++ API library is included with any process that the builder creates
that must communicate with Gemini. Which processes in the instrument must link
with and use the C++ API is an instrument design decision.

The communication protocol between the C++ API and the GMP is encapsulated
within the GIAPI implementation.

Details about the usage of the API can be found in the Gemini Observatory
ICD50 - GIAPI C++ Language Glue API.

# Dependencies

The GIAPI C++ API now supports the C++20 standard. C++20 introduces significant
improvements over C++11, including enhanced performance, modern syntactic features,
and expanded standard library capabilities. The GIAPI library can be compiled with any
version of the GCC 10 compiler, which was among the first to fully implement C++20.
Currently, Gemini is using GIAPI on Rocky 9 with GCC 11.5.0.

The following are tools used to build the GIAPI C++ API:
|             | Rocky 9 |
|    :---:    |  :---:   |
|g++ (GCC)    | 11.5.0    |
|GNU Make     | 4.3     |
|GNU automake | 1.16.2   |
|cmake        | 3.26.5 |

# External libraries
The external libraries required by the GIAPI are:
* Apache Active MQ CMS (C++ Messaging system) version 3.9.5. [activemq](external/activemq-cpp-library-3.9.5)
* Apache Log4cxx version 1.3.1. [log4cxx](external/apache-log4cxx-1.3.1) 
* libCurl version 8.11.1. [curl](external/curl-8.11.1) 
* curlpp version 0.8.1 [curlpp](external/curlpp-0.8.1) 
* Apache Portable Runtime libraries (apr and apr-util). [apr](external/apr-1.7.5) and  [apr-util](external/apr-util-1.6.3)
* Expat version 2.6.4 [expat](external/expat-2.6.4) 

## Compile GIAPI-GLUE.
To compile GIAPI-GLUE from source code, it is necessary to first compile the external libraries 
located in the external directory. 

The steps performed from GEMINI to compile the library on Rocky 9 are described below. 

* Create the GIAPI_ROOT and BOOST_ROOT enviroment variables. It is possible executing the following command.
   ```
      source ./defineGiapiglueEnv.sh
   ```
* Compile the apr library. Follow the actions listed in [link](external/apr-1.7.5)
* Compile the expat library. Follow the actions listed in [link](external/expat-2.6.4) 
* Compile the apr-util library. Follow the actions listed in this [link](external/apr-util-1.6.3)
* Compile the log4cxx library. Follow the actions listed in this [link](external/apache-log4cxx-1.3.1) 
* Compile the activemq library. Follow the actions listed in this [link](external/activemq-cpp-library-3.9.5)
* Compile the libCurl library. Follow the actions listed in this [link](external/curl-8.11.1) 
* Compile the libCurlpp library. Follow the actions listed in this [link](external/curlpp-0.8.1)
* Compile the giapi-glue library. Execute the following commands.
  ```
  > cd $GIAPI_ROOT
  > make 
  > make install
  ``` 
* Finally, if you want to execute some examples provided by GEMINI, perform the following actions. 
  ```
  cd $GIAPI_ROOT/src/examples
  > make
  ```


# History
The first version of the giapi-glue dates from October 24, 2008. It was written by AN (Arturo Nunez). 
During these years it has been maintained mainly by Carlos Quiroz, Ignacio Arriagada and Fran. Ramos. . 

