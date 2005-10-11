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
#ifndef XMLBEANSXX_XMLOBJECTBASE_INCLUDED
#define XMLBEANSXX_XMLOBJECTBASE_INCLUDED
#include "BoostAssert.h"
#include <log4cxx/logger.h>
#include "Ptr.h"
#include <xmlbeansxx/String.h>
#include <xmlbeansxx/ValidationContext.h>
#include <xmlbeansxx/BeansException.h>
#include <xmlbeansxx/SchemaType.h>
#include "Mutex.h"
#include "TypeStoreUser.h"
#include "XmlOptions.h"
#include "XmlTypesGen.h"
#include <iostream>
#include "SimpleValue.h"
#include "TypeStoreUser.h"
#include "XmlTypesGen.h"
#include "TypeStore.h"
namespace xmlbeansxx {
class XmlObjectBase_I;
BEGIN_CLASS(XmlObjectBase, XmlObjectBase_I)
END_CLASS()

class XmlObjectBase_I: public virtual SimpleValue_I, public virtual TypeStoreUser_I, public virtual XmlObject_I, public EnablePtrFromThis<XmlObjectBase_I> {
private:
    static log4cxx::LoggerPtr LOG;

class ValueOutOfRangeValidationContext_I: public ValidationContext_I {
    public:
        virtual void invalid(const String &code, const String &args);
    };
BEGIN_CLASS(ValueOutOfRangeValidationContext, ValueOutOfRangeValidationContext_I)
END_CLASS()

public:
    static ValidationContext _voorVc;

protected:
    /** 
     * This should set the value of the type from text, 
     * or throw an XmlValueOutOfRangeException if it can't. 
     */
    virtual void set_text(const String &text) = 0;

     /** 
     * This should return the canonical string value of the primitive. 
     * Only called when non-nil. 
     */
    virtual String compute_text() = 0;
    
private:
    TypeStore _textsource;

public:
    virtual Mutex &mutex();
    virtual String build_text();
    virtual TypeStore get_store();
    virtual void attach_store(const TypeStore &store);
    virtual TypeStoreUser create_element_user(QName eltName, QName xsiType);
    virtual TypeStoreUser create_attribute_user(QName attrName);
    virtual SchemaType get_element_type(QName eltName, QName xsiType);
    virtual SchemaType get_attribute_type(QName attrName);

    virtual void check_orphaned();

    virtual XmlObject changeType(SchemaType newType);

    XmlCursor newCursorForce();
    XmlObject ensureStore();
    String xmlText(const XmlOptions &options);
    XmlOptions makeInnerOptions(const XmlOptions &options);
    virtual XmlCursor newCursor();
    virtual XmlObject copy();

    virtual void save(std::ostream &out, const XmlOptions &options = XmlOptions::New());
    virtual String toString(const XmlOptions &options = XmlOptions::New());
    
    virtual boolean valueEquals(const XmlObject &obj);
    virtual int valueHashCode();
    
    virtual String getStringValue();
    virtual void setStringValue(const String &value);
    virtual boolean getBooleanValue();
    virtual void setBooleanValue(const boolean &value);
    virtual byte getByteValue();
    virtual void setByteValue(const byte &value);
    virtual short getShortValue();
    virtual void setShortValue(const short &value);
    virtual int getIntValue();
    virtual void setIntValue(const int &value);
    virtual Long getLongValue();
    virtual void setLongValue(const Long &value);
    virtual BigInteger getBigIntegerValue();
    virtual void setBigIntegerValue(const BigInteger &value);
    virtual BigDecimal getBigDecimalValue();
    virtual void setBigDecimalValue(const BigDecimal &value);
    virtual float getFloatValue();
    virtual void setFloatValue(const float &value);
    virtual double getDoubleValue();
    virtual void setDoubleValue(const double &value);
    virtual Array<byte> getByteArrayValue();
    virtual void setByteArrayValue(const Array<byte> &value);
    virtual StringEnumAbstractBase getEnumValue();
    virtual void setEnumValue(const StringEnumAbstractBase &value);
    virtual Calendar getCalendarValue();
    virtual void setCalendarValue(const Calendar &value);
    virtual Date getDateValue();
    virtual void setDateValue(const Date &value);
    virtual GDate getGDateValue();
    virtual void setGDateValue(const GDate &value);
    virtual GDuration getGDurationValue();
    virtual void setGDurationValue(const GDuration &value);
    virtual QName getQNameValue();
    virtual void setQNameValue(const QName &value);
    virtual List getListValue();
    virtual void setListValue(const List &value);
    virtual Object getObjectValue();
    virtual void setObjectValue(const Object &value);

private:
    XmlCursor getSerializeCursor();
protected:
    String get_store_text();
    void set_store_text(const String &text);
};

}
#endif //XMLBEANSXX_XMLOBJECTBASE_INCLUDED
