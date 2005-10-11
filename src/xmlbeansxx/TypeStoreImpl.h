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
#ifndef XMLBEANSXX_TYPESTOREIMPL_INCLUDED
#define XMLBEANSXX_TYPESTOREIMPL_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "TypeStore.h"
#include "QName.h"
#include "Mutex.h"
#include "Ptr.h"
#include "TextUtils.h"
//#include <list>
namespace xmlbeansxx {


class XobjBase_I;
BEGIN_CLASS(XobjBase, XobjBase_I)
END_CLASS()

class NamedPair {
    public:
    QName name;
    XobjBase value;
    NamedPair(const QName &name, const XobjBase &value): name(name), value(value) {}
};


class XobjBase_I: public TypeStore_I, public EnablePtrFromThis<XobjBase_I> {
private:
    static log4cxx::LoggerPtr LOG;

protected:
    SchemaType _schemaType;
    
public:
    typedef Vector<NamedPair>::type ContentsType;
    //typedef std::list<NamedPair> ContentsType;

    TypeStoreUser getUser();
    
    virtual Mutex &mutex() = 0;
    virtual XmlCursor new_cursor();
    
    void setSchemaType(const SchemaType &schemaType) { _schemaType = schemaType; }
    SchemaType getSchemaType() { return _schemaType; }
    
    /*
    virtual void store_text(const String &text);
    */
    virtual String fetch_text() = 0;
    virtual String fetch_text(SchemaType::WS_TYPE whitespaceRule) {
        if (whitespaceRule==SchemaType::WS_COLLAPSE) {
            return TextUtils::collapse(fetch_text());
        } else {
            return fetch_text();
        }
    }
    
    virtual TypeStoreUser change_type(const xmlbeansxx::SchemaType &newType) { _schemaType = newType; return getUser(); }
    virtual void copy_contents_from(const TypeStore &source);
    virtual TypeStoreUser find_attribute(const QName &name);
    virtual void remove_attribute(const QName &name);
    virtual void add_attribute(const QName &name, const TypeStoreUser &value);
    virtual TypeStoreUser add_attribute(const QName &name);
    virtual TypeStoreUser find_element(const QName &name,int index);
    virtual void find_all_elements(const QName &name, const Array<TypeStoreUser> &toFill);
    virtual int count_elements(const QName &name);
    virtual void add_element(const QName &name,const TypeStoreUser &value);
    virtual TypeStoreUser add_element(const QName &name);
    virtual void set_element(const QName &u,int index,const TypeStoreUser &value);
    virtual void remove_all_elements(const QName &name);
    virtual void remove_element(const QName &name,int index);

    virtual void visit_elements(const TypeStoreVisitor &visitor);

    virtual ContentsType &getAttributes() = 0;
    virtual ContentsType &getElements() = 0;
    virtual String &getValue() = 0;
};

class MixedContentStore_I: public XobjBase_I {
    private:
    Mutex _mutex;
    String _value;
    ContentsType _attributes, _elements;
    
    public:
    virtual Mutex &mutex() { return _mutex; }
    virtual ContentsType &getAttributes() { return _attributes; }
    virtual ContentsType &getElements() { return _elements; }
    virtual String &getValue() { return _value; }
    
    virtual void store_text(const String &text) { _value = text; }
    virtual String fetch_text() {
        return _value;
    }
};
BEGIN_CLASS(MixedContentStore, MixedContentStore_I)
END_CLASS()

class SimpleContentStore_I;
BEGIN_CLASS(SimpleContentStore, SimpleContentStore_I)
    static XobjBase_I::ContentsType emptyContents;
END_CLASS()

class SimpleContentStore_I: public XobjBase_I {
    private:
    static log4cxx::LoggerPtr LOG;
    Mutex _mutex;
    String _value;

    public:
    virtual Mutex &mutex() { return _mutex; }
    virtual ContentsType &getAttributes() { 
        BOOST_ASSERT(false);
        return SimpleContentStore::emptyContents; 
    }
    virtual ContentsType &getElements() { 
        BOOST_ASSERT(false);
        return SimpleContentStore::emptyContents; 
    }
    virtual String &getValue() { return _value; }
    
    virtual void store_text(const String &text) { _value = text; }
    virtual String fetch_text() {
        return _value;
    }
};

class TypeStoreImpl {
    public:
    static TypeStore create(const SchemaType &schemaType);
};

}
#endif //XMLBEANSXX_TYPESTOREIMPL_INCLUDED
