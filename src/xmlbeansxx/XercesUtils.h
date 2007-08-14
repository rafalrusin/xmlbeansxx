/*
    Copyright 2004-2005 TouK s.c.
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
 
    http://www.apache.org/licenses/LICENSE-2.0
 
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */


#ifndef _XMLBEANSXX_XERCESUTILS_H_
#define _XMLBEANSXX_XERCESUTILS_H_
/**
 * Xerces-c utilities (not in public headers)
 */
#include "BoostAssert.h"
#include <iostream>
#include <string>
#include <boost/scoped_array.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/framework/XMLFormatter.hpp>



namespace xmlbeansxx {


/** Provides a xerces input stream to read from std::istream */
class BinStdInputStream: public xercesc::BinInputStream {
    int p;
    std::istream &in;
public:
    BinStdInputStream(std::istream &in);
    unsigned int curPos() const;
    unsigned int readBytes(XMLByte *const toFill,const unsigned int maxToRead);
};

/** Provides a xerces input source that uses std::istream reader */
class StdInputSource: public xercesc::InputSource {
    std::istream &in;
public:
    StdInputSource(std::istream &in,XERCES_CPP_NAMESPACE::MemoryManager* const manager = xercesc::XMLPlatformUtils::fgMemoryManager);
    xercesc::BinInputStream* makeStream() const;
};

/** Provides a xerces XMLFormatTarget that writes to String. */
class StdStringFormatTarget: public xercesc::XMLFormatTarget {
private:
    std::string s;
public:
    StdStringFormatTarget();

    virtual void writeChars(const XMLByte *const toWrite, const unsigned int count, xercesc::XMLFormatter *const formatter);
    std::string getString() const;
    void reset();
};

/** Provides transcoding of xerces const XMLCh * string to String in given encoding. */
class Transcoder {
private:
    StdStringFormatTarget t;
    xercesc::XMLFormatter f;

public:
    /**
     * @param encoding is a name of destination encoding. Eg. 'UTF-8'.
     */
    Transcoder(const char *encoding);
    std::string transcode(const XMLCh *s);
};


}

#endif

