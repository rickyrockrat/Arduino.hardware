/*
  WString.h - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <avr/pgmspace.h>

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class StringSumHelper;

// The string class
class String
{
public:
	// constructors
	String(const char *cstr = "");
	String(const String &str);
	#ifdef __GXX_EXPERIMENTAL_CXX0X__
	String(String &&rval);
	String(StringSumHelper &&rval);
	#endif
	String(char c);
	String(unsigned char c);
	String(int, unsigned char base=10);
	String(unsigned int, unsigned char base=10);
	String(long, unsigned char base=10);
	String(unsigned long, unsigned char base=10);
	~String(void);

	// memory management
	unsigned char reserve(unsigned int size);
	inline unsigned int length(void) const {return len;}

	String & operator = (const String &rhs);
	String & operator = (const char *cstr);
	#ifdef __GXX_EXPERIMENTAL_CXX0X__
	String & operator = (String &&rval);
	String & operator = (StringSumHelper &&rval);
	#endif
	String & operator = (char c);

	// concat
	unsigned char concat(const String &str);
	unsigned char concat(const char *cstr);
	unsigned char concat(char c);
	unsigned char concat(unsigned char c)		{return concat((char)c);}
	unsigned char concat(int num);
	unsigned char concat(unsigned int num);
	unsigned char concat(long num);
	unsigned char concat(unsigned long num);
	String & operator += (const String &rhs)	{concat(rhs); return (*this);}
	String & operator += (const char *cstr)		{concat(cstr); return (*this);}
	String & operator += (char c)			{concat(c); return (*this);}
	String & operator += (unsigned char c)		{concat((char)c); return (*this);}
	String & operator += (int num)			{concat(num); return (*this);}
	String & operator += (unsigned int num)		{concat(num); return (*this);}
	String & operator += (long num)			{concat(num); return (*this);}
	String & operator += (unsigned long num)	{concat(num); return (*this);}

	// concatenate
	friend StringSumHelper & operator + (const StringSumHelper &lhs, const String &rhs);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, const char *cstr);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, char c);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, unsigned char c);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, int num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, unsigned int num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, long num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, unsigned long num);

	// comparison
	operator bool() const;
	int compareTo(const String &s) const;
	unsigned char equals(const String &s) const;
	unsigned char equals(const char *cstr) const;
	unsigned char operator == (const String &rhs) const {return equals(rhs);}
	unsigned char operator == (const char *cstr) const {return equals(cstr);}
	unsigned char operator != (const String &rhs) const {return !equals(rhs);}
	unsigned char operator != (const char *cstr) const {return !equals(cstr);}
	unsigned char operator <  (const String &rhs) const;
	unsigned char operator >  (const String &rhs) const;
	unsigned char operator <= (const String &rhs) const;
	unsigned char operator >= (const String &rhs) const;
	unsigned char equalsIgnoreCase(const String &s) const;
	unsigned char startsWith( const String &prefix) const;
	unsigned char startsWith(const String &prefix, unsigned int offset) const;
	unsigned char endsWith(const String &suffix) const;

	// character acccess
	char charAt(unsigned int index) const;
	void setCharAt(unsigned int index, char c);
	char operator [] (unsigned int index) const;
	char& operator [] (unsigned int index);
	void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index=0) const;
	void toCharArray(char *buf, unsigned int bufsize, unsigned int index=0) const
		{getBytes((unsigned char *)buf, bufsize, index);}

	// search
	int indexOf( char ch ) const;
	int indexOf( char ch, unsigned int fromIndex ) const;
	int indexOf( const String &str ) const;
	int indexOf( const String &str, unsigned int fromIndex ) const;
	int lastIndexOf( char ch ) const;
	int lastIndexOf( char ch, int fromIndex ) const;
	int lastIndexOf( const String &str ) const;
	int lastIndexOf( const String &str, int fromIndex ) const;
	String substring( unsigned int beginIndex ) const;
	String substring( unsigned int beginIndex, unsigned int endIndex ) const;

	// modification
	void replace(char find, char replace);
	void replace(const String& find, const String& replace);
	void toLowerCase(void);
	void toUpperCase(void);
	void trim(void);

	// parsing/conversion
	long toInt(void) const;

protected:
	char *buffer;	        // the actual char array
	unsigned int capacity;  // the array length minus one (for the '\0')
	unsigned int len;       // the String length (not counting the '\0')
	unsigned char flags;    // unused, for future features
protected:
	void init(void);
	void invalidate(void);
	unsigned char changeBuffer(unsigned int maxStrLen);
	unsigned char concat(const char *cstr, unsigned int length);

	// copy and move
	String & copy(const char *cstr, unsigned int length);
	#ifdef __GXX_EXPERIMENTAL_CXX0X__
	void move(String &rhs);
	#endif
};

class StringSumHelper : public String
{
public:
	StringSumHelper(const String &s) : String(s) {}
	StringSumHelper(const char *p) : String(p) {}
	StringSumHelper(char c) : String(c) {}
	StringSumHelper(unsigned char c) : String(c) {}
	StringSumHelper(int num) : String(num, 10) {}
	StringSumHelper(unsigned int num) : String(num, 10) {}
	StringSumHelper(long num) : String(num, 10) {}
	StringSumHelper(unsigned long num) : String(num, 10) {}
};

inline StringSumHelper operator + (const String &lhs, const String &rhs)
	{ return StringSumHelper(lhs) + rhs; }
	
inline StringSumHelper operator + (const String &lhs, const char *cstr)
	{ return StringSumHelper(lhs) + cstr; }
inline StringSumHelper operator + (const String &lhs, char c)
	{ return StringSumHelper(lhs) + c; }
inline StringSumHelper operator + (const String &lhs, unsigned char c)
	{ return StringSumHelper(lhs) + c; }
inline StringSumHelper operator + (const String &lhs, int num)
	{ return StringSumHelper(lhs) + num; }
inline StringSumHelper operator + (const String &lhs, unsigned int num)
	{ return StringSumHelper(lhs) + num; }
inline StringSumHelper operator + (const String &lhs, long num)
	{ return StringSumHelper(lhs) + num; }
inline StringSumHelper operator + (const String &lhs, unsigned long num)
	{ return StringSumHelper(lhs) + num; }
	
inline StringSumHelper operator + (const char *cstr, const String &rhs)
	{ return StringSumHelper(cstr) + rhs; }
inline StringSumHelper operator + (char c, const String &rhs)
	{ return StringSumHelper(c) + rhs; }
inline StringSumHelper operator + (unsigned char c, const String &rhs)
	{ return StringSumHelper(c) + rhs; }
inline StringSumHelper operator + (int num, const String &rhs)
	{ return StringSumHelper(num) + rhs; }
inline StringSumHelper operator + (unsigned int num, const String &rhs)
	{ return StringSumHelper(num) + rhs; }
inline StringSumHelper operator + (long num, const String &rhs)
	{ return StringSumHelper(num) + rhs; }
inline StringSumHelper operator + (unsigned long num, const String &rhs)
	{ return StringSumHelper(num) + rhs; }
	


#endif  // __cplusplus
#endif  // String_class_h
