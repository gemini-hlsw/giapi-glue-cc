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

The GIAPI C++ API supports the C++11 standard. The C++11 is a major upgrade 
over C++98/03, with performance and convenience features that make it feel 
like a new language. The GIAPI library can be compiled from any version 
of the gcc 4.8.1 compiler, which was the first to implement the C++11 language.
Currently, Gemini is using the GIAPI on centos7 (gcc 4.8.5) and 
rocky 8 (gcc 8.5.0). 


The following are tools used to build the GIAPI C++ API:
|             | Centos 7 | Rocky 8|
|    :---:    |  :---:   | :---:  |
|g++ (GCC)    | 4.8.5    | 8.5.0  |
|GNU Make     | 3.82     | 4.2.1  |
|GNU automake | 1.16.1   | 1.16.1 |
|cmake        | 2.8.12.2 | 3.20.2 |

# External libraries
The external libraries required by the GIAPI are:
* Apache Active MQ CMS (C++ Messaging system) version 3.4.1. [activemq](external/activemq-cpp-library-3.4.1)
* Apache Log4cxx version 0.11.0. [log4cxx](external/apache-log4cxx-0.11.0) 
* libCurl version 7.21.6. [curl](external/curl-7.21.6) 
* curlpp version 0.8.1 [curlpp](external/curlpp-0.8.1) 
* Apache Portable Runtime libraries (apr and apr-util). [apr](external/apr-1.3.12) and  [apr-util](external/apr-util-1.3.10)

## Compile GIAPI-GLUE.
To compile GIAPI-GLUE from source code, it is necessary to first compile the external libraries 
located in the external directory. 

The steps performed from GEMINI to compile the library on CENTOS 7 and CENTOS 8 are described below. 

* Create the GIAPI_ROOT and BOOST_ROOT enviroment variables. It is possible executing the following command.
   ```
      source ./defineGiapiglueEnv.sh
   ```
* Compile the apr library. Follow the actions listed in [link](external/apr-1.3.12)
* Compile the apr-util library. Follow the actions listed in this [link](external/apr-util-1.3.10)
* Compile the activemq library. Follow the actions listed in this [link](external/activemq-cpp-library-3.4.1) 
* Compile the log4cxx library. Follow the actions listed in this [link](external/apache-log4cxx-0.11.0) 
* Compile the libCurl library. Follow the actions listed in this [link](external/curl-7.21.6) 
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

