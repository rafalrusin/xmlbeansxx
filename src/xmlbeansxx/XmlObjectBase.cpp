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
#include "XmlObjectBase.h"
#include <log4cxx/logger.h>
#include <xmlbeansxx/Tracer.h>
#include <xmlbeansxx/defs.h>
#include <xmlbeansxx/XmlTypesGen.h>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <xmlbeansxx/TextUtils.h>
#include <xmlbeansxx/SchemaProperty.h>
#include "TypeStore.h"
#include "BeansException.h"
#include "xml-fragment.h"
#include "Null.h"
#include "XmlBeans.h"

using namespace std;

//Disable tracing
#undef TRACER
#define TRACER(logger, msg)


namespace xmlbeansxx {
log4cxx::LoggerPtr XmlObjectBase_I::LOG = log4cxx::Logger::getLogger("xmlbeansxx.XmlObjectBase");

void XmlObjectBase_I::ValueOutOfRangeValidationContext_I::invalid(const String &code, const String &args) {
    throw XmlValueOutOfRangeException( code, args );
}
ValidationContext XmlObjectBase_I::_voorVc(new ValueOutOfRangeValidationContext_I());

Mutex &XmlObjectBase_I::mutex() {
    return get_store()->mutex();
}

TypeStore XmlObjectBase_I::get_store() {
    return _textsource;
}

void XmlObjectBase_I::attach_store(const TypeStore &store) {
    _textsource = store;
}

TypeStoreUser XmlObjectBase_I::create_element_user(QName eltName, QName xsiType) {
    return cast<TypeStoreUser>(XmlBeans::getContextTypeLoader()->newInstance(get_element_type(eltName, xsiType), XmlOptions::New()));
}

TypeStoreUser XmlObjectBase_I::create_attribute_user(QName attrName) {
    return cast<TypeStoreUser>(XmlBeans::getContextTypeLoader()->newInstance(get_attribute_type(attrName), XmlOptions::New()));
}

SchemaType XmlObjectBase_I::get_element_type(QName eltName, QName xsiType) {
    SchemaType type = XmlBeans::getContextTypeLoader()->findType(xsiType);
    if (type == Null()) {
        SchemaProperty prop = get_schema_type()->getElementProperty(eltName);
        if (prop == Null()) {
            return XmlObject::type;
        } else {
            return prop->getType();
        }
    } else {
        return type;
    }
}

SchemaType XmlObjectBase_I::get_attribute_type(QName attrName) {
    SchemaProperty prop = get_schema_type()->getAttributeProperty(attrName);
    if (prop == Null()) {
        return XmlAnySimpleType::type;
    } else {
        return prop->getType();
    }
}


XmlObject XmlObjectBase_I::copy() {
    /*
    VAL(store1, get_store());
    VAL(obj2, cast<XmlObjectBase>(get_schema_type()->createXmlObject()));
    VAL(store2, obj2->get_store());
    Lock lock(mutex()), lock2(obj2->mutex());

    obj2->setStringValue(getStringValue());
    store2->copy_contents_from(store1);
    return cast<XmlObject>(obj2);
    */
    return Null();
}


String XmlObjectBase_I::build_text() {
    return compute_text();
}

XmlCursor XmlObjectBase_I::getSerializeCursor() {
    Lock lock(mutex());
    return get_store()->new_cursor();
}

void XmlObjectBase_I::save(std::ostream &output, const XmlOptions &options) {
    return getSerializeCursor()->save(output, options);
}

String XmlObjectBase_I::toString(const XmlOptions &options) {
    return getSerializeCursor()->xmlText(options);
}

XmlObject XmlObjectBase_I::changeType(SchemaType newType) {
    if (newType == Null())
        throw IllegalArgumentException( "Invalid type (null)" );

    /*
    if ((_flags & FLAG_STORE) == 0) {
        throw
        new IllegalStateException(
            "XML Value Objects cannot have thier type changed" );
    }*/

    Lock lock(mutex());
    check_orphaned();
    return cast<XmlObject>(get_store()->change_type(newType));
}

XmlCursor XmlObjectBase_I::newCursorForce() {
    Lock lock(mutex());
    return ensureStore()->newCursor();
}

XmlObject XmlObjectBase_I::ensureStore() {
    return cast<XmlObject>(shared_from_this());
}

String XmlObjectBase_I::xmlText(const XmlOptions &options) {
    XmlCursor cur = newCursorForce();
    return cur->xmlText(makeInnerOptions(options));
}

XmlOptions XmlObjectBase_I::makeInnerOptions(const XmlOptions &options) {
    return options;
}

XmlCursor XmlObjectBase_I::newCursor() {
    /*
    if ((_flags & FLAG_STORE) == 0)
        throw new IllegalStateException("XML Value Objects cannot create cursors");
        */
    check_orphaned();
    return get_store()->new_cursor();
}

void XmlObjectBase_I::check_orphaned() {}

boolean XmlObjectBase_I::valueEquals(const XmlObject &obj) {
    LOG4CXX_ERROR(LOG, "Not implemented");
    throw NotImplementedException();
    return true;
}

int XmlObjectBase_I::valueHashCode() {
    LOG4CXX_ERROR(LOG, "Not implemented");
    throw NotImplementedException();
    return 0;
}


String XmlObjectBase_I::getStringValue() {
    return get_store_text();
}
void XmlObjectBase_I::setStringValue(const String &value) {
    set_store_text(value);
}
boolean XmlObjectBase_I::getBooleanValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
void XmlObjectBase_I::setBooleanValue(const boolean &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
byte XmlObjectBase_I::getByteValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
void XmlObjectBase_I::setByteValue(const byte &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
short XmlObjectBase_I::getShortValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return short();
}
void XmlObjectBase_I::setShortValue(const short &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
int XmlObjectBase_I::getIntValue() {
    BigInteger v=getBigIntegerValue();
    if (v.fits_sint_p()) {
        return v.get_si();
    } else {
        throw IllegalArgumentException(String("Cannot convert decimal='") + get_store_text() + String("' to int."));
        return 0;
    }
}
void XmlObjectBase_I::setIntValue(const int &value) {
    setBigIntegerValue(value);
}
Long XmlObjectBase_I::getLongValue() {
    return getBigIntegerValue();
}
void XmlObjectBase_I::setLongValue(const Long &value) {
    setBigIntegerValue(value);
}
BigInteger XmlObjectBase_I::getBigIntegerValue() {
    String v=TextUtils::collapse(get_store_text());
    if (!TextUtils::checkInteger(v))
        throw XmlIllegalValueException("xs:integer",v);
    return BigInteger(v);
}
void XmlObjectBase_I::setBigIntegerValue(const BigInteger &value) {
    std::ostringstream ss;
    ss << value;
    set_store_text(ss.str());
}
BigDecimal XmlObjectBase_I::getBigDecimalValue() {
    String v=TextUtils::collapse(get_store_text());
    if (!TextUtils::checkDecimal(v))
        throw XmlIllegalValueException("xs:decimal",v);
    return BigDecimal(v);
}
void XmlObjectBase_I::setBigDecimalValue(const BigDecimal &value) {
    std::ostringstream ss;
    ss << std::fixed;
    int fd = get_schema_type()->getFractionDigits();
    if (fd!=-1) {
        ss.precision(fd);
    }
    ss << value;
    set_store_text(ss.str());
}
float XmlObjectBase_I::getFloatValue() {
    std::istringstream iss(get_store_text());
    float floatVal;
    iss >> floatVal;
    if (iss.fail()) {
        throw IllegalArgumentException(String("Cannot convert value='")
                                        + get_store_text() + String("' to float."));
        return 0.0;
    } else {
        return floatVal;
    }
}
void XmlObjectBase_I::setFloatValue(const float &value) {
    set_store_text(TextUtils::floatToString(value));
}
double XmlObjectBase_I::getDoubleValue() {
    std::istringstream iss(get_store_text());
    double doubleVal;
    iss >> doubleVal;
    if (iss.fail()) {
        throw IllegalArgumentException(String("Cannot convert value='")
                                        + get_store_text() + String("' to double."));
        return 0.0;
    } else {
        return doubleVal;
    }
}
void XmlObjectBase_I::setDoubleValue(const double &value) {
    set_store_text(TextUtils::doubleToString(value));
}
Array<byte> XmlObjectBase_I::getByteArrayValue() {
    return TextUtils::base64Decode(get_store_text());
}
void XmlObjectBase_I::setByteArrayValue(const Array<byte> &value) {
    set_store_text(TextUtils::base64Encode(value));
}
StringEnumAbstractBase XmlObjectBase_I::getEnumValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return StringEnumAbstractBase();
}
void XmlObjectBase_I::setEnumValue(const StringEnumAbstractBase &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
Calendar XmlObjectBase_I::getCalendarValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return Calendar();
}
void XmlObjectBase_I::setCalendarValue(const Calendar &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
Date XmlObjectBase_I::getDateValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return Date();
}
void XmlObjectBase_I::setDateValue(const Date &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
GDate XmlObjectBase_I::getGDateValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return GDate();
}
void XmlObjectBase_I::setGDateValue(const GDate &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
GDuration XmlObjectBase_I::getGDurationValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return GDuration();
}
void XmlObjectBase_I::setGDurationValue(const GDuration &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
QName XmlObjectBase_I::getQNameValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return QName();
}
void XmlObjectBase_I::setQNameValue(const QName &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
List XmlObjectBase_I::getListValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return List();
}
void XmlObjectBase_I::setListValue(const List &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}
Object XmlObjectBase_I::getObjectValue() {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
    return Object();
}
void XmlObjectBase_I::setObjectValue(const Object &value) {
    LOG4CXX_ERROR(LOG, "Exception");
    throw XmlValueOutOfRangeException();
}

String XmlObjectBase_I::get_store_text() {
    Lock lock(mutex());
    return get_store()->fetch_text(get_schema_type()->getWhitespaceRule());
}

void XmlObjectBase_I::set_store_text(const String &text) {
    Lock lock(mutex());
    get_store()->store_text(text);
}


}
