#ifndef XMLBEANSXX_SIMPLEVALUE_INCLUDED
#define XMLBEANSXX_SIMPLEVALUE_INCLUDED
#include <string>
namespace xmlbeansxx {
class SimpleValue {
    public:
    virtual ~SimpleValue() {}

    /** Sets string content. */
	virtual void setSimpleContent(const std::string &c) = 0;
    
    /** Gets string content. */
	virtual std::string getSimpleContent() const = 0;
};
}
#endif //XMLBEANSXX_SIMPLEVALUE_INCLUDED
