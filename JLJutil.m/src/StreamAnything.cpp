// StreamAnything.cpp

//********************************************************************

#ifndef StreamAnything_cpp
#define StreamAnything_cpp

//********************************************************************

#include <jlj/util/StreamAnything.h>
#include <jlj/util/ChunkedOutputStream.h>
#include <jlj/util/ChunkedInputStream.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/SessionDirectory.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;
#include <jlj/util/zip/ZipUtility.h>
using namespace jlj::util::zip;
#include <jlj/io/FileInputStream.h>
#include <jlj/io/FileOutputStream.h>
#include <jlj/io/FileWriter.h>
#include <jlj/io/Base64.h>
using namespace jlj::io;
#include <jlj/core/platform/bigendian.h>
using namespace jlj::core::platform;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

const int StreamAnythingI::DEFAULTBUFFERSIZE = 128*1024;

//********************************************************************

StreamAnythingI::~StreamAnythingI() throw (IOException) 
{
	if (!!inputstream) inputstream->close();
	if (0 < temporaryfilepath->length()) 
	{
		::remove(constcharptr(temporaryfilepath->toUTF8String()));
	}
}

//********************************************************************

StreamAnythingI::StreamAnythingI(InputStream in) throw (IOException)
: inputstream(in)
{
	if (!inputstream) throw IOException(WITHDETAILS(L"Invalid inputstream."));
}

//********************************************************************

StreamAnythingI::StreamAnythingI(InputStream& in, 
								 Map<int, void*>& references) throw (IOException)
{
	try
	{
		int putback = sizeof(int);
		int tellg = in->tell() - putback;

		temporaryfilepath = SessionDirectory::getInstance()->getPath() + Uuid::randomUuid()->toString() + L".tmp";

		OutputStream outputstream = new FileOutputStreamI(temporaryfilepath);

		pipe(new ChunkedInputStreamI(in), outputstream);

		outputstream->close();
		inputstream = new FileInputStreamI(temporaryfilepath);

		references->put(tellg, (void*) this);
	}
	catch (const IOException&)
	{
		::remove(constcharptr(temporaryfilepath->toUTF8String()));
		throw;
	}
	catch (...)
	{
		::remove(constcharptr(temporaryfilepath->toUTF8String()));
		throw IOException(WITHDETAILS(L"Could not read from InputStream."));
	}
}

//********************************************************************

int StreamAnythingI::type() const 
{
	return Anything::ANY_STREAM;
}

//********************************************************************

InputStream StreamAnythingI::getInputStream() const throw (IOException) 
{
	return inputstream;
}

//********************************************************************

void StreamAnythingI::saveInputStreamToFileOutputStream(const String& filepath) throw (IOException)
{
	if (0 < temporaryfilepath->length()) 
	{
		// we rather "move/rename" the temporary file to the destination file

		inputstream->close();

		StringBuffer command;
#if defined(_WINDOWS_SOURCE)
		command->append(L"%COMSPEC% /c move /y \"");
#else
		command->append(L"mv -f \"");
#endif
		command->append(temporaryfilepath);
		command->append(L"\" \"");
		command->append(filepath);
		command->append(L"\"");
#if defined(_WINDOWS_SOURCE)
		command->append(L" > NUL ");
#else
		command->append(L" > /dev/null");
#endif

		if (0 != system(constcharptr(command->toUTF8String())))
		{
			throw IOException(WITHDETAILS(L"Could not execute : " + command->toString()));
		}

		temporaryfilepath = L"";
	}
	else
	{
		pipe(inputstream, new FileOutputStreamI(filepath));
		inputstream = new FileInputStreamI(temporaryfilepath);
	}
}

//********************************************************************

void StreamAnythingI::serialize__(OutputStream& os,
								  Map<void*, int>& references,
								  Anything) const throw (IOException)
{
	if (serializeReference(os, references, (void*) this))
	{
		int type = bigendian(Anything::ANY_STREAM);
		os->write((char*)&type, sizeof(int));

		pipe(inputstream, new ChunkedOutputStreamI(os));
	}
}

//********************************************************************

void StreamAnythingI::write__(Writer& writer, 
							  int level,
							  Map<void*, int>& references) const throw (IOException)
{
	char* buffer = 0;

	try
	{
		if (this->writeReference(writer, level, references, (void*) this))
		{	
			inputstream->reset();

			writer->write(L"\n<![CDATA[");

			FileWriter filewriter;
			bool isfilewriter = writer->instanceOf(filewriter);

			buffer = new char[DEFAULTBUFFERSIZE]; 
			int pos = 0;
			int n = 0;

			while (0 < (n = inputstream->read(buffer, pos, DEFAULTBUFFERSIZE - pos)))
			{
				pos += n;
				if (pos >= DEFAULTBUFFERSIZE)
				{
					UTF8String encoded;
					Base64().encode(encoded, buffer, pos);
					isfilewriter ? filewriter->write(encoded) : writer->write(encoded);
					writer->write(L"\n");
					pos = 0;
				}
			}
			if (pos > 0)
			{
				UTF8String encoded;
				Base64().encode(encoded, buffer, pos);
				isfilewriter ? filewriter->write(encoded) : writer->write(encoded);
			}

			writer->write(L"\n]]>\n");
			indent(writer, level);
			writer->write(L"</anything>\n");

			delete [] buffer;
		}
	}
	catch (const IOException&)
	{
		delete [] buffer;
		throw;
	}
}

//********************************************************************

void StreamAnythingI::writeJSON__(UTF8StringBuffer& stringbuffer) const throw (IOException)
{
	char* buffer = 0;
	char* readbuffer = 0;

	try
	{
		inputstream->reset();
		buffer = new char[DEFAULTBUFFERSIZE]; 
		int t = inputstream->tell();
		int readbuffersize = t > DEFAULTBUFFERSIZE ? t : DEFAULTBUFFERSIZE;
		readbuffer = new char[readbuffersize]; 
		int pos = 0;
		int n = 0;
		int total = 0;
		while (0 < (n = inputstream->read(buffer, pos, DEFAULTBUFFERSIZE - pos)))
		{
			pos += n;
			if (pos >= DEFAULTBUFFERSIZE)
			{
				if (readbuffersize < total + pos)
				{
					char* oldbuffer = readbuffer;
					readbuffersize *= 2;
					readbuffer = new char[readbuffersize];
					memcpy(readbuffer, oldbuffer, total);
					delete [] oldbuffer;
				}
				memcpy(readbuffer  + total, buffer, pos);
				total += pos;
				pos = 0;
			}
		}
		if (pos > 0)
		{
			if (readbuffersize < total + pos)
			{
				char* oldbuffer = readbuffer;
				readbuffersize *= 2;
				readbuffer = new char[readbuffersize];
				memcpy(readbuffer, oldbuffer, total);
				delete [] oldbuffer;
			}
			memcpy(readbuffer  + total, buffer, pos);
			total += pos;
		}
		delete [] buffer;
		int compressedtotal = 0;
		ZipUtility().zipBuffer(readbuffer, compressedtotal, readbuffer, total);
		Base64 base64;
		base64.setLineSeparator("");
		UTF8String encoded;
		base64.encode(encoded, readbuffer, compressedtotal);
		delete [] readbuffer;

		stringbuffer->append("{\"encoded\":\"base64\",\"compression\":\"zip\",\"decodedAndUncompressedSize\":");
		stringbuffer->append(total);
		stringbuffer->append(",\"decodedAndCompressedSize\":");
		stringbuffer->append(compressedtotal);
		stringbuffer->append(",\"content\":\"");
		stringbuffer->append(encoded);
		stringbuffer->append("\"}");
	}
	catch (const IOException&)
	{
		delete [] buffer;
		delete [] readbuffer;
		throw;
	}
}

//********************************************************************

Object StreamAnythingI::clone() const 
{
	return new StreamAnythingI(inputstream);
}

//********************************************************************

int StreamAnythingI::size() const
{
	return streamedbytes;
}

//********************************************************************

void StreamAnythingI::pipe(InputStream in, 
						   OutputStream out) const throw (IOException)
{
	char* buffer = 0;

	try
	{
		in->reset();

		buffer = new char[DEFAULTBUFFERSIZE]; 

		int total = 0;
		int pos = 0;
		int n = 0;

		while (0 < (n = in->read(buffer, pos, DEFAULTBUFFERSIZE - pos)))
		{
			pos += n;
			if (pos >= DEFAULTBUFFERSIZE)
			{
				out->write(buffer, 0, pos);
				total += pos;
				pos = 0;
			}
		}
		if (pos > 0)
		{
			total += pos;
			out->write(buffer, 0, pos);
		}

		delete [] buffer;

		streamedbytes = total;
	}
	catch (const IOException&)
	{
		delete [] buffer;
		throw;
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
