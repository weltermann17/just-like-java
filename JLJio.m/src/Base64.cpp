// Base64.cpp

//********************************************************************

#ifndef Base64_cpp
#define Base64_cpp

//********************************************************************

#include <jlj/io/Base64.h>

//********************************************************************

#include <memory.h>
#include <string.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

Base64::~Base64() 
{
	delete [] valuetochar;
	delete [] chartovalue;
}

//********************************************************************

Base64::Base64() 
: linelength(72)
, valuetochar(new char[64])
, chartovalue(new int[256])
{
	strcpy(lineseparator, "\n");
	int i;
	for (i = 0; i <= 25; i++)
		valuetochar[i] = (char)('A' + i);
	for (i = 0; i <= 25; i++)
		valuetochar[i + 26] = (char)('a' + i);
	for (i = 0; i <= 9; i++)
		valuetochar[i + 52] = (char)('0' + i);
	valuetochar[62] = '+';
	valuetochar[63] = '/';	

	for (i = 0; i < 256; i++)
	{
		chartovalue[i] = Base64::IGNORE64;  
	}
	for (i = 0; i < 64; i++)
	{
		chartovalue[valuetochar[i]] = i;
	}
	chartovalue['='] = Base64::PAD64;

}

//********************************************************************

bool Base64::encode(UTF8String& out, char* in, int length)
{
	int lslen = strlen(lineseparator);

	int outputlength = ((length + 2) / 3) * 4;

	if (0 != linelength)
	{
		int lines = ((outputlength + linelength - 1) / linelength) - 1;
		if (0 < lines )
		{
			outputlength += lines * lslen; 
		}
	}

	int sblen = 0;
	char* sb = new char[outputlength + 1];

	int linepos = 0;

	int len = (length / 3) * 3;
	int leftover = length - len;
	for (int i = 0; i < len; i += 3)
	{
		linepos += 4;
		if (linepos > linelength)
		{
			if (0 != linelength)
			{
				memcpy(sb + sblen, lineseparator, lslen * sizeof(char));
				sblen += lslen;
			}
			linepos = 4;
		}

		int combined = in[i + 0] & 0xff;
		combined <<= 8;
		combined |= in[i + 1] & 0xff;
		combined <<= 8;
		combined |= in[i + 2] & 0xff;

		int c3 = combined & 0x3f;
		combined >>= 6; 
		int c2 = combined & 0x3f;
		combined >>= 6;
		int c1 = combined & 0x3f;
		combined >>= 6;
		int c0 = combined & 0x3f;

		append(sb, sblen, valuetochar[c0]);
		append(sb, sblen, valuetochar[c1]);
		append(sb, sblen, valuetochar[c2]);
		append(sb, sblen, valuetochar[c3]);
	}

	switch (leftover)
	{
	case 0:
	default:
		{
			break;
		}
	case 1:
		{
			linepos += 4;
			if (linepos > linelength)
			{
				if (0 != linelength)
				{
					memcpy(sb + sblen, lineseparator, lslen * sizeof(char));
					sblen += lslen;
				}
				linepos = 4;
			}			
			char remains[3];
			remains[0] = in[len];
			remains[1] = '\0';
			remains[2] = '\0';
			UTF8String recurs;
			encode(recurs, remains, 3);
			const char* rec = constcharptr(recurs);
			append(sb, sblen, rec[0]);
			append(sb, sblen, rec[1]);
			append(sb, sblen, L'=');
			append(sb, sblen, L'=');
			break;
		}		
	case 2:
		{
			linepos += 4;
			if (linepos > linelength)
			{
				if (linelength != 0)
				{
					memcpy(sb + sblen, lineseparator, lslen * sizeof(char));
					sblen += lslen;
				}
				linepos = 4;
			}
			char remains[3];
			remains[0] = in[len];
			remains[1] = in[len + 1];
			remains[2] = '\0';
			UTF8String recurs;
			encode(recurs, remains, 3);
			const char* rec = constcharptr(recurs);
			append(sb, sblen, rec[0]);
			append(sb, sblen, rec[1]);
			append(sb, sblen, rec[2]);
			append(sb, sblen, '=');
			break;
		}
	} 

	append(sb, sblen, '\0');
	out = UTF8String(sb, 0, sblen - 1);

	delete [] sb;

	return true;
}

//********************************************************************

bool Base64::decode(char*& out, int& length, const UTF8String& in)
{
	length = (in->length() / 4) * 3;
	out = new char[length];

	int cycle = 0;

	int combined = 0;

	int j = 0;
	int len = in->length();
	int dummies = 0;
	const char* p = constcharptr(in);

	for (int i = 0; i < len; i++)
	{		
		wchar_t c = p[i];
		int value  = (c <= 255) ? chartovalue[c] : Base64::IGNORE64;

		switch (value)
		{
		case Base64::IGNORE64:
			{
				break;
			}		
		case Base64::PAD64:
			{
				value = 0;
				dummies++;
				// fall through, no break
			}
		default:
			{
				switch ( cycle )
				{
				case 0:
					{
						combined = value;
						cycle = 1;
						break;
					}
				case 1:
					{
						combined <<= 6;
						combined |= value;
						cycle = 2;
						break;
					}
				case 2:
					{
						combined <<= 6;
						combined |= value;
						cycle = 3;
						break;
					}
				case 3:
					{
						combined <<= 6;
						combined |= value;
						out[j + 2] = (char)combined;
						combined >>= 8;
						out[j + 1] = (char)combined;
						combined >>= 8;
						out[j] = (char)combined;
						j += 3;
						cycle = 0;
						break;
					}
				}
				break;
			}
		}
	} 
	if (0 != cycle) return false;

	j -= dummies;
	length = j;

	return true;
}

//********************************************************************

int Base64::getLineLength() const
{
	return linelength;
}

//********************************************************************

void Base64::setLineLength(int l) 
{
	if (40 < l) linelength = ((l / 4) + 1) * 4;
}

//********************************************************************

UTF8String Base64::getLineSeparator() const
{
	return UTF8String(lineseparator);
}

//********************************************************************

void Base64::setLineSeparator(const UTF8String& s) 
{
	strcpy(lineseparator, constcharptr(s));
}

//********************************************************************

void Base64::append(char* dest, int& len, char src) const
{
	dest[len++] = src;
}

//********************************************************************

void* Base64::operator new(size_t)
{
	return 0;
}

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
