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
#include "XmlTypesGen.h"
#include "TypeStoreImpl.h"
#include "TypeStoreUser.h"
#include <log4cxx/logger.h>
#include "defs.h"
#include "Null.h"
#include "BeansException.h"
#include "Stack.h"
#include "Set.h"
#include "XmlBeans.h"
#include "StoreString.h"
#include "xml-fragment.h"
#include "SchemaTypeImpl.h"
namespace xmlbeansxx {

/*
 * This file contains implementation of Xml navigation, creation and saving.
 */

//log4cxx::LoggerPtr SimpleContentStore_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.SimpleContentStore");
log4cxx::LoggerPtr XobjBase_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.XobjBase");

namespace {
XobjBase createAttributeStore(const SchemaType &type) {
    XobjBase o = SimpleContentStore::New();
    o->setSchemaType(type);
    return o;
}

XobjBase createElementStore(const SchemaType &type) {
    XobjBase o = MixedContentStore::New();
    o->setSchemaType(type);
    return o;
}

}

class XmlSerializeVisitor {
    public:
    virtual ~XmlSerializeVisitor() {}
    virtual void begin() = 0;
    virtual void end() = 0;
    virtual void beginElement(const QName &name) = 0;
    virtual void beginElementContents() = 0;
    virtual void endElement(const QName &name) = 0;
    virtual void endElementQuick() = 0;
    virtual void attribute(const QName &name) = 0;
    virtual void attributeValue(const String &value) = 0;
    virtual void attributeValue(const QName &value) = 0;
    virtual void text(const String &value) = 0;
};

class NameVisitor {
    public:
    virtual ~NameVisitor() {}
    virtual void name(const QName &name) = 0;
};
    
typedef Map<StoreString, String, StoreString::Hash>::type NSMap;

class CalculateNamespacesVisitor: public NameVisitor {
    private:
    int counter;

    String nextLink() {
        counter ++;
        if (counter == 1) return "";
        else {
            String s;
            s.push_back('a' + counter - 1); 
            return s;
        }
    }
    
    public:
    NSMap nsMap;

    CalculateNamespacesVisitor(): counter(0) {
        nsMap[""] = "";
    }

    virtual void name(const QName &name) {
        if (nsMap.find(name.first) == nsMap.end()) {
            String link;
            if (name.first == XmlBeans::xs_ns()) {
                link = "xs";
            } else if (name.first == XmlBeans::xsi_ns()) {
                link = "xsi";
            } else {
                link = nextLink();
            }
            nsMap[name.first] = link;
        }
    }
};

class StdXmlSerializeVisitor: public XmlSerializeVisitor {
    private:
    static log4cxx::LoggerPtr LOG;
    
    bool isRoot; 
    std::ostream &output;
    NSMap namespaceMap;
    XmlOptions options;

    void outputQName(const QName &name) {
        String mapped = "NOT_MAPPED";
        if (namespaceMap.find(name.first) == namespaceMap.end()) {
            LOG4CXX_WARN(LOG, "namespace '" << name.first << "' not mapped");
        } else {
            mapped = namespaceMap[name.first];
        }
        if (mapped == "") {
            output << name.second;
        } else {
            output << mapped << ":" << name.second;
        }
    }

    public:
    StdXmlSerializeVisitor(std::ostream &output, NSMap &namespaceMap, const XmlOptions &options): output(output), namespaceMap(namespaceMap), options(options) {}
    
    virtual void begin() {
        isRoot = true;
        if (options->getSaveXmlDecl()) {
            output << "<?xml version='1.0' encoding='UTF-8'?>\n";
        }
    }

    virtual void end() {
    }

    virtual void beginElement(const QName &name) {
        output << "<";
        outputQName(name);
        if (isRoot) {
            //output xmlns declarations
            FOREACH(it, namespaceMap) {
                if (!(it->first == StoreString(""))) {
                    output << " xmlns";
                    if (it->second == "") {
                    } else {
                        output << ":";
                        output << it->second;
                    }
                    output << "='" << it->first << "'";
                }
            }
            isRoot = false;
        }
    }

    virtual void beginElementContents() {
        output << ">";
    }
    
    virtual void endElement(const QName &name) {
        output << "</";
        outputQName(name);
        output << ">";
    }
    
    virtual void endElementQuick() {
        output << "/>";
    }
    
    virtual void attribute(const QName &name) {
        output << " ";
        outputQName(name);
        output << "=";
    }
    
    virtual void attributeValue(const String &value) {
        output << "'";
        output << TextUtils::exchangeEntities(value);
        output << "'";
    }
    
    virtual void attributeValue(const QName &value) {
        output << "'";
        outputQName(value);
        output << "'";
    }
    
    virtual void text(const String &value) {
        output << TextUtils::exchangeEntities(value);
    }
};

log4cxx::LoggerPtr StdXmlSerializeVisitor::LOG = log4cxx::Logger::getLogger("xmlbeansxx.StdXmlSerializeVisitor");

class XmlSerializer {
    public:
    XmlSerializeVisitor &visitor;

    XmlSerializer(XmlSerializeVisitor &visitor): visitor(visitor) {}
    
    private:
    void serializeRec(const QName &name, bool dumpXsiType, const XobjBase &target) {
        Lock lock(target->mutex());
        visitor.beginElement(name);
        SchemaType type = target->getSchemaType();

        if (dumpXsiType) {
            visitor.attribute(XmlBeans::xsi_type());
            visitor.attributeValue(type->getName());
        }

        
        FOREACH(it, target->getAttributes()) {
            visitor.attribute(it->name);
            visitor.attributeValue(it->value->fetch_text());
        }

        bool quickEnd = true;
        FOREACH(it, target->getElements()) {
            XobjBase value(it->value);
            if (value != Null()) {
                if (quickEnd) {
                    quickEnd = false;
                    visitor.beginElementContents();
                }
    
                SchemaProperty prop = type->getElementProperty(it->name);
                bool shallPrintXsiType = type != XmlObject::type && (prop != Null()) && value->getSchemaType() != prop->getType();
                serializeRec(it->name, shallPrintXsiType, value);
            }
        }

        String text = target->fetch_text();
        if (text != "") { 
            if (quickEnd) {
                quickEnd = false;
                visitor.beginElementContents();
            }
            visitor.text(text);
        }
        
        if (quickEnd) {
            visitor.endElementQuick();
        } else {
            visitor.endElement(name);
        }
    }
    public:
    void serialize(const XobjBase &target) {
        visitor.begin();
        Lock lock(target->mutex());
        VAL(contents, target->getElements());
        VAL(it, contents.begin());
        if (it != contents.end()) {
            serializeRec(it->name, false, it->value);
        }
        visitor.end();
    }
};

class VisitNames {
    NameVisitor &visitor;
    void visitRec(const XobjBase &target) {
        FOREACH(it, target->getAttributes()) {
           visitor.name(it->name); 
        }
        FOREACH(it, target->getElements()) {
           visitor.name(it->name); 
           if (it->value != Null()) {
               QName name = it->value->getSchemaType()->getName();
               if (name != QName()) {
                   visitor.name(name);
               }
               visitRec(it->value);
           }
        }
    }

    public:
    VisitNames(NameVisitor &visitor, const XobjBase &target): visitor(visitor) {
        visitRec(target);
        visitor.name(XmlBeans::xsi_type());
    }
};

class CorrectElementOrder {
    class Cmp {
        SchemaType st;
        public:
        Cmp(const XobjBase &target) {
            st = target->getSchemaType();
        }
        int value(const NamedPair &what) const {
            SchemaProperty prop = st->getElementProperty(what.name);
            if (prop == Null()) {
                return -1;
            } else {
                return cast<SchemaPropertyImpl>(prop)->order;
            }
        }
        bool operator()(const NamedPair &first, const NamedPair &second) const {
            return value(first) < value(second);
        }
    };
        
    void correctRec(const XobjBase &target) {
        Lock lock(target->mutex());
        XobjBase_I::ContentsType &elements(target->getElements());
        /*
        std::cout << "before sort:" << std::endl;
        FOREACH(it, elements) {
            std::cout << it->name->toString() << std::endl;
        }*/
        stable_sort(elements.begin(), elements.end(), Cmp(target)); 
        /*
        std::cout << "after sort:" << std::endl;
        FOREACH(it, elements) {
            std::cout << it->name->toString() << std::endl;
        }*/
        FOREACH(it, elements) {
            correctRec(it->value);
        }
    }
    public:
    CorrectElementOrder(const XobjBase &target) {
        correctRec(target);
    }
};

class Cur_I;
BEGIN_CLASS(Cur, Cur_I)
END_CLASS()
    
class Cur_I: public virtual XmlCursor_I {
    static log4cxx::LoggerPtr LOG;
    Mutex _mutex;
    Stack<XobjBase>::type stack;
    XobjBase target;
    
    public:    
    Cur_I(const XobjBase &target): target(target) {
    }

    void dump() {
        LOG4CXX_DEBUG(LOG, "dump");
        FOREACH(it, target->getAttributes()) {
            LOG4CXX_DEBUG(LOG, "ATTR " << it->name.toString() << " -> " << it->value.ptr.get());
        }
        FOREACH(it, target->getElements()) {
            LOG4CXX_DEBUG(LOG, "ELEM " << it->name.toString() << " -> " << it->value.ptr.get());
        }
    }
    
    virtual Mutex &mutex() { return _mutex; }
    virtual XmlCursor newCursor() {
        LOG4CXX_ERROR(LOG, "Not implemented");
        throw NotImplementedException();
    }
    
    virtual XmlObject getObject() {
        Lock lock(target->mutex());
        return cast<XmlObject>(target->getUser());
    }
    
    virtual void dispose() {
        target = Null();
    }
    
    virtual String xmlText(const XmlOptions &options) {
        std::ostringstream ss;
        save(ss, options);
        return ss.str();
    }
    virtual void save(std::ostream &output, const XmlOptions &options) {
        Lock lock(target->mutex());
        {
            CorrectElementOrder correct(target);
        }
        SchemaType st = target->getSchemaType();
        if (!st->isDocumentType()) {
            definitions::XmlFragmentDocument doc(definitions::XmlFragmentDocument::Factory::newInstance());
            doc->cgetXmlFragment()->setElement(cast<XmlObject>(target->getUser()));
            doc->save(output, options);
        } else {
            CalculateNamespacesVisitor nsVisitor; 
            VisitNames(nsVisitor, target);
    
            StdXmlSerializeVisitor xmlVisitor(output, nsVisitor.nsMap, options);
            XmlSerializer xmlSerializer(xmlVisitor); 
            xmlSerializer.serialize(target);
        }
    }
    virtual bool isAttr() {
        LOG4CXX_ERROR(LOG, "Not implemented");
        throw NotImplementedException();
    }
    
    virtual bool toNextSibling() {
        LOG4CXX_ERROR(LOG, "Not implemented");
        throw NotImplementedException();
    }
    
    virtual void insertChars(const String &text) {
        Lock lock(target->mutex());
        SchemaType st = target->getSchemaType();
        if (st->getContentType() != SchemaType::ELEMENT_CONTENT) {
            target->getValue() += text;
        }
    }
    
    virtual void beginElement(const QName &name) {
        BOOST_ASSERT(target != Null());
        Lock lock(target->mutex());
        SchemaType type = target->getSchemaType();
        BOOST_ASSERT(type != Null());
        XobjBase n;
        SchemaProperty childProp = type->getElementProperty(name);
        if (childProp != Null()) {
            n = createElementStore(childProp->getType());
        } else {
            n = createElementStore(XmlObject::type);
        }
        target->getElements().push_back(NamedPair(name, n));
        //XobjBase n = cast<XobjBase>(target->add_element(name)->get_store());
        push();
        target = n;
    }
    
    virtual void insertAttributeWithValue(const QName &name, const String &value) {
        Lock lock(target->mutex());
        SchemaType type = target->getSchemaType();
        XobjBase n;
        SchemaProperty childProp = type->getAttributeProperty(name);
        if (childProp != Null()) {
            n = createAttributeStore(childProp->getType());
        } else {
            n = createAttributeStore(XmlAnySimpleType::type);
        }
        n->store_text(value);
        target->getAttributes().push_back(NamedPair(name, n));
        //target->add_attribute(name)->get_store()->store_text(value);
    }

    virtual void insertAttributeWithValue(const QName &name, const QName &value) {
        Lock lock(target->mutex());
        if (name == XmlBeans::xsi_type()) {
            SchemaType type = XmlBeans::getContextTypeLoader()->findType(value);
            if (type == Null()) {
                LOG4CXX_ERROR(LOG, "Error");
                throw XmlException("xsi-type '" + value->toString() + "' not found in context");
            }
            target->setSchemaType(type);
        } else throw IllegalStateException();
    }

    virtual void push() {
        stack.push(target);
    }
    
    virtual bool pop() {
        if (stack.empty()) {
            return false; 
        } else {    
            target = stack.top();
            stack.pop();
            return true;
        }
    }
};

log4cxx::LoggerPtr Cur_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.Cur");


XmlCursor XobjBase_I::new_cursor() {
    return Cur::New(shared_from_this());
}
    
void XobjBase_I::copy_contents_from(const TypeStore &source) {
    LOG4CXX_ERROR(LOG, "Not implemented");
    throw NotImplementedException();
}


TypeStoreUser XobjBase_I::find_attribute(const QName &name) {
    FOREACH(it, getAttributes()) {
        if (name == it->name) {
            if (it->value == Null()) {
                return Null();
            } else {
                return it->value->getUser();
            }
        }
    }
    return Null();
}
    
void XobjBase_I::remove_attribute(const QName &name) {
    ContentsType &attributes(getAttributes());
    FOREACH(it, attributes) {
        if (name == it->name) {
            attributes.erase(it);
            return;
        }
    }
}

void XobjBase_I::add_attribute(const QName &name, const TypeStoreUser &value) {
    if (value != Null()) {
        getAttributes().push_back(NamedPair(name, cast<XobjBase>(value->get_store())));
    }
}

TypeStoreUser XobjBase_I::add_attribute(const QName &name) {
    TypeStoreUser user = getUser()->create_attribute_user(name);
    XobjBase store = createAttributeStore(user->get_schema_type());
    user->attach_store(store);
    getAttributes().push_back(NamedPair(name, store));
    return user;
}

TypeStoreUser XobjBase_I::find_element(const QName &name,int index) {
    FOREACH(it, getElements()) {
        if (it->name == name) {
            index --;
            if (index == -1) {
                return it->value == Null() ? TypeStoreUser() : it->value->getUser();
            }
        }
    }
    return Null();
}

void XobjBase_I::find_all_elements(const QName &name, const Array<TypeStoreUser> &toFill) {
    FOREACH(it, getElements()) {
        if (it->name == name) {
            if (it->value == Null()) {
                toFill.push_back(Null());
            } else {
                toFill.push_back(it->value->getUser());
            }
        }
    }
}

int XobjBase_I::count_elements(const QName &name) {
    ContentsType &elements(getElements());
    int count = 0;
    FOREACH(it, elements) {
        if (it->name == name) {
            count ++;
        }
    }
    return count;
}

void XobjBase_I::add_element(const QName &name,const TypeStoreUser &value) {
    if (value != Null()) {
        getElements().push_back(NamedPair(name, cast<XobjBase>(value->get_store())));
    }
}

TypeStoreUser XobjBase_I::add_element(const QName &name) {
    TypeStoreUser user = getUser()->create_element_user(name, QName());
    XobjBase store = createElementStore(user->get_schema_type());
    user->attach_store(store);
    getElements().push_back(NamedPair(name, store));
    return user;
}

void XobjBase_I::set_element(const QName &name,int index,const TypeStoreUser &value) {
    ContentsType &elements(getElements());
    FOREACH(it, elements) {
        if (it->name == name) {
            if (index == 0) {
                it->value = Null();
                if (value != Null()) {
                    it->value = cast<XobjBase>(value->get_store());
                }
                return;
            }
            index --;
        }
    }
    FOR(i,index) {
        add_element(name, Null());
    }
    add_element(name, value);
}

void XobjBase_I::remove_all_elements(const QName &name) {
    int p=0,p2=0;
    int ile=0;
    ContentsType &elements(getElements());
    while (p<int(elements.size())) {
        if (p!=p2)
            elements[p2] = elements[p];
        if (elements[p].name == name) {
            elements[p].name = Null();
            elements[p].value = Null();
            ile++;
            p++;
        } else {
            p++;
            p2++;
        }
    }
    FOR(i,ile) elements.pop_back();
}

void XobjBase_I::remove_element(const QName &name,int index) {
    ContentsType &elements(getElements());
    FOREACH(it, elements) {
        if (it->name == name) {
            index --;
            if (index == -1) {
                elements.erase(it);
                return;
            }
        }
    }
}
    
void XobjBase_I::visit_elements(const TypeStoreVisitor &visitor) {
    LOG4CXX_ERROR(LOG, "Not implemented");
    throw NotImplementedException();
}

TypeStoreUser XobjBase_I::getUser() {
    //This should be changed, TypeStoreUser should be instantiated from itself, not from TypeStore
    VAL(st, cast<SchemaTypeImpl>(getSchemaType()));
    VAL(user, cast<TypeStoreUser>(st->createRawXmlObject()));
    user->attach_store(TypeStore(shared_from_this()));
    return user;
}


TypeStore TypeStoreImpl::create(const SchemaType &schemaType) {
    MixedContentStore s = MixedContentStore::New();
    s->setSchemaType(schemaType);
    return s;
}

XobjBase_I::ContentsType SimpleContentStore::emptyContents;
log4cxx::LoggerPtr SimpleContentStore_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.SimpleContentStore");

}
