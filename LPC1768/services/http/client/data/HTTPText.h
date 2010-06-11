
/*
Copyright (c) 2010 Donatien Garnier (donatiengar [at] gmail [dot] com)
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef HTTP_TEXT_H
#define HTTP_TEXT_H

#include "../HTTPData.h"
#include "mbed.h"

#define DEFAULT_MAX_MEM_ALLOC 512 //Avoid out-of-memory problems

class HTTPText : public HTTPData //Simple Text I/O
{
public:
  HTTPText(const string& encoding = "text/html", int maxSize = DEFAULT_MAX_MEM_ALLOC);
  virtual ~HTTPText();
  
  const char* gets() const;
  void puts(const char* str);
  
  string& get();
  void set(const string& str);
  
  virtual void clear();
  
protected:
  virtual int read(char* buf, int len);
  virtual int write(const char* buf, int len);
  
  virtual string getDataType(); //Internet media type for Content-Type header
  virtual void setDataType(const string& type); //Internet media type from Content-Type header

  virtual bool getIsChunked(); //For Transfer-Encoding header
  virtual void setIsChunked(bool chunked); //From Transfer-Encoding header
  
  virtual int getDataLen(); //For Content-Length header
  virtual void setDataLen(int len); //From Content-Length header, or if the transfer is chunked, next chunk length
  
private:
  string m_buf;
  string m_encoding;
  int m_maxSize;
};

#endif
