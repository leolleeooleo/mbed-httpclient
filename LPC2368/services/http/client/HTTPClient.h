
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

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

class HTTPData;

#include "if/net/net.h"
#include "api/TCPSocket.h"
#include "api/DNSRequest.h"
#include "HTTPData.h"
#include "mbed.h"

#include <string>
using std::string;

#include <map>
using std::map;

enum HTTPResult
{
  HTTP_OK,
  HTTP_PROCESSING,
  HTTP_PARSE, //URI Parse error
  HTTP_DNS, //Could not resolve name
  HTTP_PRTCL, //Protocol error
  HTTP_NOTFOUND, //404 Error
  HTTP_REFUSED, //403 Error
  HTTP_ERROR, //xxx error
  HTTP_TIMEOUT, //Connection timeout
  HTTP_CONN //Connection error
};



class HTTPClient : protected NetService
{
public:
  HTTPClient();
  virtual ~HTTPClient();
  
  void basicAuth(const char* user, const char* password); //Basic Authentification
  
  //High Level setup functions
  HTTPResult get(const char* uri, HTTPData* pDataIn); //Blocking
  HTTPResult get(const char* uri, HTTPData* pDataIn, void (*pMethod)(HTTPResult)); //Non blocking
  template<class T> 
  HTTPResult get(const char* uri, HTTPData* pDataIn, T* pItem, void (T::*pMethod)(HTTPResult)) //Non blocking
  {
    setOnResult(pItem, pMethod);
    doGet(uri, pDataIn);
    return HTTP_PROCESSING;
  }
  
  HTTPResult post(const char* uri, const HTTPData& dataOut, HTTPData* pDataIn); //Blocking
  HTTPResult post(const char* uri, const HTTPData& dataOut, HTTPData* pDataIn, void (*pMethod)(HTTPResult)); //Non blocking
  template<class T> 
  HTTPResult post(const char* uri, const HTTPData& dataOut, HTTPData* pDataIn, T* pItem, void (T::*pMethod)(HTTPResult)) //Non blocking  
  {
    setOnResult(pItem, pMethod);
    doPost(uri, dataOut, pDataIn);
    return HTTP_PROCESSING;
  }
  
  void doGet(const char* uri, HTTPData* pDataIn);  
  void doPost(const char* uri, const HTTPData& dataOut, HTTPData* pDataIn); 
  
  void setOnResult( void (*pMethod)(HTTPResult) );
  class CDummy;
  template<class T> 
  //Linker bug : Must be defined here :(
  void setOnResult( T* pItem, void (T::*pMethod)(HTTPResult) )
  {
    m_pCb = NULL;
    m_pCbItem = (CDummy*) pItem;
    m_pCbMeth = (void (CDummy::*)(HTTPResult)) pMethod;
  }
  
  void setTimeout(int ms);
  
  virtual void poll(); //Called by NetServices
  
  int getHTTPResponseCode();
  void setRequestHeader(const string& header, const string& value);
  string& getResponseHeader(const string& header);
  void resetRequestHeaders();
  
protected:
  void resetTimeout();
  
  void init();
  void close();
  
  void setup(const char* uri, HTTPData* pDataOut, HTTPData* pDataIn); //Setup request, make DNS Req if necessary
  void connect(); //Start Connection
  
  int  tryRead(); //Read data and try to feed output
  void readData(); //Data has been read
  void writeData(); //Data has been written & buf is free
  
  void onTCPSocketEvent(TCPSocketEvent e);
  void onDNSReply(DNSReply r);
  void onResult(HTTPResult r); //Called when exchange completed or on failure
  void onTimeout(); //Connection has timed out
  
private:
  HTTPResult blockingProcess(); //Called in blocking mode, calls Net::poll() until return code is available

  bool readHeaders(); //Called first when receiving data
  bool writeHeaders(); //Called to create req
  int readLine(char* str, int maxLen, bool* pIncomplete = NULL);
  
  enum HTTP_METH
  {
    HTTP_GET,
    HTTP_POST,
    HTTP_HEAD
  };
  
  HTTP_METH m_meth;
  
  CDummy* m_pCbItem;
  void (CDummy::*m_pCbMeth)(HTTPResult);
  
  void (*m_pCb)(HTTPResult);
  
  TCPSocket* m_pTCPSocket;
  map<string, string> m_reqHeaders;
  map<string, string> m_respHeaders;
  
  Timer m_watchdog;
  int m_timeout;
  
  DNSRequest* m_pDnsReq;
  
  Host m_server;
  string m_path;
  
  bool m_closed;
  
  enum HTTPStep
  {
   // HTTP_INIT,
    HTTP_WRITE_HEADERS,
    HTTP_WRITE_DATA,
    HTTP_READ_HEADERS,
    HTTP_READ_DATA,
    HTTP_READ_DATA_INCOMPLETE,
    HTTP_DONE,
    HTTP_CLOSED
  };
  
  HTTPStep m_state;
  
  HTTPData* m_pDataOut;
  HTTPData* m_pDataIn;
  
  bool m_dataChunked; //Data is encoded as chunks
  int m_dataPos; //Position in data
  int m_dataLen; //Data length
  char* m_buf;
  char* m_pBufRemaining; //Remaining
  int m_bufRemainingLen; //Data length in m_pBufRemaining
  
  int m_httpResponseCode;
  
  HTTPResult m_blockingResult; //Result if blocking mode
  
};

//Including data containers here for more convenience
#include "data/HTTPFile.h"
#include "data/HTTPStream.h"
#include "data/HTTPText.h"
#include "data/HTTPMap.h"

#endif
