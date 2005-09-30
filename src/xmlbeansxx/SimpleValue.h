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
#ifndef XMLBEANSXX_SIMPLEVALUE_INCLUDED
#define XMLBEANSXX_SIMPLEVALUE_INCLUDED
#include "BoostAssert.h"
#include <string>
#include "CoreTypes.h"
#include "Array.h"
#include "Ptr.h"
namespace xmlbeansxx {
class StringAccessor_I {
    public:
    virtual ~StringAccessor_I() {}
    virtual String getStringValue() = 0;
    virtual void setStringValue(const String &value) = 0;
};
class BooleanAccessor_I {
    public:
    virtual ~BooleanAccessor_I() {}
    virtual boolean getBooleanValue() = 0;
    virtual void setBooleanValue(const boolean &value) = 0;
};
class ByteAccessor_I {
    public:
    virtual ~ByteAccessor_I() {}
    virtual byte getByteValue() = 0;
    virtual void setByteValue(const byte &value) = 0;
};
class ShortAccessor_I {
    public:
    virtual ~ShortAccessor_I() {}
    virtual short getShortValue() = 0;
    virtual void setShortValue(const short &value) = 0;
};
class IntAccessor_I {
    public:
    virtual ~IntAccessor_I() {}
    virtual int getIntValue() = 0;
    virtual void setIntValue(const int &value) = 0;
};
class LongAccessor_I {
    public:
    virtual ~LongAccessor_I() {}
    virtual Long getLongValue() = 0;
    virtual void setLongValue(const Long &value) = 0;
};
class BigIntegerAccessor_I {
    public:
    virtual ~BigIntegerAccessor_I() {}
    virtual BigInteger getBigIntegerValue() = 0;
    virtual void setBigIntegerValue(const BigInteger &value) = 0;
};
class BigDecimalAccessor_I {
    public:
    virtual ~BigDecimalAccessor_I() {}
    virtual BigDecimal getBigDecimalValue() = 0;
    virtual void setBigDecimalValue(const BigDecimal &value) = 0;
};
class FloatAccessor_I {
    public:
    virtual ~FloatAccessor_I() {}
    virtual float getFloatValue() = 0;
    virtual void setFloatValue(const float &value) = 0;
};
class DoubleAccessor_I {
    public:
    virtual ~DoubleAccessor_I() {}
    virtual double getDoubleValue() = 0;
    virtual void setDoubleValue(const double &value) = 0;
};
class ByteArrayAccessor_I {
    public:
    virtual ~ByteArrayAccessor_I() {}
    virtual Array<byte> getByteArrayValue() = 0;
    virtual void setByteArrayValue(const Array<byte> &value) = 0;
};
class EnumAccessor_I {
    public:
    virtual ~EnumAccessor_I() {}
    virtual StringEnumAbstractBase getEnumValue() = 0;
    virtual void setEnumValue(const StringEnumAbstractBase &value) = 0;
};
class CalendarAccessor_I {
    public:
    virtual ~CalendarAccessor_I() {}
    virtual Calendar getCalendarValue() = 0;
    virtual void setCalendarValue(const Calendar &value) = 0;
};
class DateAccessor_I {
    public:
    virtual ~DateAccessor_I() {}
    virtual Date getDateValue() = 0;
    virtual void setDateValue(const Date &value) = 0;
};
class GDateAccessor_I {
    public:
    virtual ~GDateAccessor_I() {}
    virtual GDate getGDateValue() = 0;
    virtual void setGDateValue(const GDate &value) = 0;
};
class GDurationAccessor_I {
    public:
    virtual ~GDurationAccessor_I() {}
    virtual GDuration getGDurationValue() = 0;
    virtual void setGDurationValue(const GDuration &value) = 0;
};
class QNameAccessor_I {
    public:
    virtual ~QNameAccessor_I() {}
    virtual QName getQNameValue() = 0;
    virtual void setQNameValue(const QName &value) = 0;
};
class ListAccessor_I {
    public:
    virtual ~ListAccessor_I() {}
    virtual List getListValue() = 0;
    virtual void setListValue(const List &value) = 0;
};
class ObjectAccessor_I {
    public:
    virtual ~ObjectAccessor_I() {}
    virtual Object getObjectValue() = 0;
    virtual void setObjectValue(const Object &value) = 0;
};
class SimpleValue_I: 
    public virtual StringAccessor_I, 
    public virtual BooleanAccessor_I, 
    public virtual ByteAccessor_I, 
    public virtual ShortAccessor_I, 
    public virtual IntAccessor_I, 
    public virtual LongAccessor_I, 
    public virtual BigIntegerAccessor_I, 
    public virtual BigDecimalAccessor_I, 
    public virtual FloatAccessor_I, 
    public virtual DoubleAccessor_I, 
    public virtual ByteArrayAccessor_I, 
    public virtual EnumAccessor_I, 
    public virtual CalendarAccessor_I, 
    public virtual DateAccessor_I, 
    public virtual GDateAccessor_I, 
    public virtual GDurationAccessor_I, 
    public virtual QNameAccessor_I, 
    public virtual ListAccessor_I, 
    public virtual ObjectAccessor_I
{};
BEGIN_CLASS(SimpleValue, SimpleValue_I)
END_CLASS()

    


template <typename T, typename Q>
struct AccessorTraits{};


template <typename T>
struct AccessorTraits<T, const char *> {
    void set(const T& obj, const String &value) {
        obj->setStringValue(value);
    }
    String get(const T& obj) {
        return obj->getStringValue();
    }
};

template <typename T>
struct AccessorTraits<T, String> {
    void set(const T& obj, const String &value) {
        obj->setStringValue(value);
    }
    String get(const T& obj) {
        return obj->getStringValue();
    }
};
template <typename T>
struct AccessorTraits<T, boolean> {
    void set(const T& obj, const boolean &value) {
        obj->setBooleanValue(value);
    }
    boolean get(const T& obj) {
        return obj->getBooleanValue();
    }
};
template <typename T>
struct AccessorTraits<T, byte> {
    void set(const T& obj, const byte &value) {
        obj->setByteValue(value);
    }
    byte get(const T& obj) {
        return obj->getByteValue();
    }
};
template <typename T>
struct AccessorTraits<T, short> {
    void set(const T& obj, const short &value) {
        obj->setShortValue(value);
    }
    short get(const T& obj) {
        return obj->getShortValue();
    }
};
template <typename T>
struct AccessorTraits<T, int> {
    void set(const T& obj, const int &value) {
        obj->setIntValue(value);
    }
    int get(const T& obj) {
        return obj->getIntValue();
    }
};
/*
template <typename T>
struct AccessorTraits<T, Long> {
    void set(const T& obj, const Long &value) {
        obj->setLongValue(value);
    }
    Long get(const T& obj) {
        return obj->getLongValue();
    }
};*/
template <typename T>
struct AccessorTraits<T, BigInteger> {
    void set(const T& obj, const BigInteger &value) {
        obj->setBigIntegerValue(value);
    }
    BigInteger get(const T& obj) {
        return obj->getBigIntegerValue();
    }
};
template <typename T>
struct AccessorTraits<T, BigDecimal> {
    void set(const T& obj, const BigDecimal &value) {
        obj->setBigDecimalValue(value);
    }
    BigDecimal get(const T& obj) {
        return obj->getBigDecimalValue();
    }
};
template <typename T>
struct AccessorTraits<T, float> {
    void set(const T& obj, const float &value) {
        obj->setFloatValue(value);
    }
    float get(const T& obj) {
        return obj->getFloatValue();
    }
};
template <typename T>
struct AccessorTraits<T, double> {
    void set(const T& obj, const double &value) {
        obj->setDoubleValue(value);
    }
    double get(const T& obj) {
        return obj->getDoubleValue();
    }
};
template <typename T>
struct AccessorTraits<T, Array<byte> > {
    void set(const T& obj, const Array<byte> &value) {
        obj->setByteArrayValue(value);
    }
    Array<byte> get(const T& obj) {
        return obj->getByteArrayValue();
    }
};
template <typename T>
struct AccessorTraits<T, StringEnumAbstractBase> {
    void set(const T& obj, const StringEnumAbstractBase &value) {
        obj->setEnumValue(value);
    }
    StringEnumAbstractBase get(const T& obj) {
        return obj->getEnumValue();
    }
};
template <typename T>
struct AccessorTraits<T, Calendar> {
    void set(const T& obj, const Calendar &value) {
        obj->setCalendarValue(value);
    }
    Calendar get(const T& obj) {
        return obj->getCalendarValue();
    }
};
template <typename T>
struct AccessorTraits<T, Date> {
    void set(const T& obj, const Date &value) {
        obj->setDateValue(value);
    }
    Date get(const T& obj) {
        return obj->getDateValue();
    }
};
template <typename T>
struct AccessorTraits<T, GDate> {
    void set(const T& obj, const GDate &value) {
        obj->setGDateValue(value);
    }
    GDate get(const T& obj) {
        return obj->getGDateValue();
    }
};
template <typename T>
struct AccessorTraits<T, GDuration> {
    void set(const T& obj, const GDuration &value) {
        obj->setGDurationValue(value);
    }
    GDuration get(const T& obj) {
        return obj->getGDurationValue();
    }
};
template <typename T>
struct AccessorTraits<T, QName> {
    void set(const T& obj, const QName &value) {
        obj->setQNameValue(value);
    }
    QName get(const T& obj) {
        return obj->getQNameValue();
    }
};
template <typename T>
struct AccessorTraits<T, List> {
    void set(const T& obj, const List &value) {
        obj->setListValue(value);
    }
    List get(const T& obj) {
        return obj->getListValue();
    }
};
template <typename T>
struct AccessorTraits<T, Object> {
    void set(const T& obj, const Object &value) {
        obj->setObjectValue(value);
    }
    Object get(const T& obj) {
        return obj->getObjectValue();
    }
};




template <typename T, typename Q>
T newInstance(Q value) {
    T obj = T::Factory::newInstance();
    AccessorTraits<T, Q> trait;
    trait.set(obj, value);
    return obj;
}

}
#endif //XMLBEANSXX_SIMPLEVALUE_INCLUDED
