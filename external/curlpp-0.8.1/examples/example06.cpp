/*
 *    Copyright (c) <2002-2005> <Jean-Philippe Barrette-LaPierre>
 *    
 *    Permission is hereby granted, free of charge, to any person obtaining
 *    a copy of this software and associated documentation files 
 *    (curlpp), to deal in the Software without restriction, 
 *    including without limitation the rights to use, copy, modify, merge,
 *    publish, distribute, sublicense, and/or sell copies of the Software,
 *    and to permit persons to whom the Software is furnished to do so, 
 *    subject to the following conditions:
 *    
 *    The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *    
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 *    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 *    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
* \file
* WriteFunction option using functor as a callback.
* 
*/


#include <cstdlib>
#include <cstring>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#define MAX_FILE_LENGTH 20000

class WriterMemoryClass
{
public:
	// Helper Class for reading result from remote host
	WriterMemoryClass()
	{
		this->m_pBuffer = NULL;
		this->m_pBuffer = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
		this->m_Size = 0;
	};

	~WriterMemoryClass()
	{
		if (this->m_pBuffer)
			free(this->m_pBuffer);
	};

	void* Realloc(void* ptr, size_t size)
	{
		if(ptr)
			return realloc(ptr, size);
		else
			return malloc(size);
	};

	// Callback must be declared static, otherwise it won't link...
	size_t WriteMemoryCallback(char* ptr, size_t size, size_t nmemb)
	{
		// Calculate the real size of the incoming buffer
		size_t realsize = size * nmemb;

		// (Re)Allocate memory for the buffer
		m_pBuffer = (char*) Realloc(m_pBuffer, m_Size + realsize);

		// Test if Buffer is initialized correctly & copy memory
		if (m_pBuffer == NULL) {
			realsize = 0;
		}

		memcpy(&(m_pBuffer[m_Size]), ptr, realsize);
		m_Size += realsize;


		// return the real size of the buffer...
		return realsize;
	};


	void print() 
	{
		std::cout << "Size: " << m_Size << std::endl;
		std::cout << "Content: " << std::endl << m_pBuffer << std::endl;
	}

	// Public member vars
	char* m_pBuffer;
	size_t m_Size;
};


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		std::cerr << "Example 06: Wrong number of arguments" << std::endl 
			<< "Example 06: Usage: example06 url" 
			<< std::endl;
		return EXIT_FAILURE;
	}
	char *url = argv[1];

	try
	{
		curlpp::Easy request;

		WriterMemoryClass mWriterChunk;

		// Set the writer callback to enable cURL 
		// to write result in a memory area
		using namespace std::placeholders;
		curlpp::types::WriteFunctionFunctor functor = std::bind(&WriterMemoryClass::WriteMemoryCallback, &mWriterChunk, _1, _2, _3);

		curlpp::options::WriteFunction *test = new curlpp::options::WriteFunction(functor);
		request.setOpt(test);

		// Setting the URL to retrive.
		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::Verbose(true));
		request.perform();

		mWriterChunk.print();
	}
	
	catch ( curlpp::LogicError & e )
	{
		std::cout << e.what() << std::endl;
	}
	
	catch ( curlpp::RuntimeError & e )
	{
		std::cout << e.what() << std::endl;
	}
}
