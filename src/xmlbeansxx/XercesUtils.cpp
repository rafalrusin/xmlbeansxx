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


#include "XercesUtils.h"
#include "defs.h"



namespace xmlbeansxx {

//---
XERCES_CPP_NAMESPACE_USE


LocalAttributeList::LocalAttributeList(AttributeList &al,Transcoder *tr):len(al.getLength()),list(new std::string[2*al.getLength()]) {
    FOR(i,len) {
        list[2*i]=tr->transcode(al.getName(i));
        list[2*i+1]=tr->transcode(al.getValue(i));
    }
}

std::string LocalAttributeList::getName(int i) {
    return list[2*i];
}
std::string LocalAttributeList::getValue(int i) {
    return list[2*i+1];
}

int LocalAttributeList::getLength() {
    return len;
}

LocalAttributeList::~LocalAttributeList() {
}

//---
BinStdInputStream::BinStdInputStream(std::istream &in): p(0),in(in) {}
unsigned int BinStdInputStream::curPos() const {
    return p;
}
unsigned int BinStdInputStream::readBytes(XMLByte *const toFill,const unsigned int maxToRead) {
    in.read((char *)toFill,maxToRead);
    p+=in.gcount();
    return in.gcount();
}

StdInputSource::StdInputSource(std::istream &in,MemoryManager* const manager)
        : InputSource("std",manager),in(in) {}

BinInputStream* StdInputSource::makeStream() const {
    BinStdInputStream* retStream = new (getMemoryManager()) BinStdInputStream(in);
    return retStream;
}


StdStringFormatTarget::StdStringFormatTarget(): s() {}

void StdStringFormatTarget::writeChars(const XMLByte *const toWrite, const unsigned int count, XMLFormatter *const formatter) {
    s.append((const char *)toWrite,count);
}

std::string StdStringFormatTarget::getString() const {
    return s;
}

void StdStringFormatTarget::reset() {
    s=std::string();
}


Transcoder::Transcoder(const char *encoding): t(), f(encoding,"1.0",&t) {}

std::string Transcoder::transcode(const XMLCh *s) {
    if (s==NULL) return std::string();
/*    
    char *t=XMLString::transcode(s);
    std::string r(t);
    XMLString::release(&t);
    return r;
*/    
    t.reset();
    f<<s;
    return t.getString();
}


}
