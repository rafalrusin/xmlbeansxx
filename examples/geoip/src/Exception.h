#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>

class Exception: public std::exception {
public:
	std::string msg;
	virtual ~Exception() throw() {}
	Exception(std::string msg): msg(msg) {}
	virtual const char * what() const throw() {
		return msg.c_str();
	}
};

#endif /*EXCEPTION_H_*/
