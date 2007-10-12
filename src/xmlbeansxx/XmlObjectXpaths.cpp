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


#include <sstream>

#include <string>
#include "logger.h"
#include "XmlTypesGen.h"
#include "defs.h"
#include "BoostAssert.h"
#include "NSMap.h"
#include "QName.h"

namespace xmlbeansxx {








//---------------------------------------------------------------------------------------
// Geum
/**
 * xpath delimiter std::string
 */
static const char DELIM =  '/';

/**
 * CDATA prefix <pre><![CDATA[</pre>
 */
static const std::string CDATA_PREFIX = "<![CDATA[";

/**
 * CDATA suffix <pre>]]></pre>
 
 */
static const std::string CDATA_SUFFIX = "]]>";

//Xpath operations

using namespace std;



class NSMapXPath : public NSMap {
public:
	NSMapXPath() {}
	std::string setXPathNamespaces(const std::string& path) {
		std::stringstream ss(path);
		std::string s;
		
		ss >> s;
		while (s == "declare") {
			ss >> s;
			if(s != "namespace")
				throw BeansException("bad xpath: excepted 'namespace' keyword");
			ss >> s;
			setNS(s);
			ss >> s;
		}
		return s;
	}
private:
	void setNS(const std::string& ns) {
		int p=ns.find("=");
		if(p<0) addNamespace("",getURI(ns));
		else addNamespace(ns.substr(0,p),getURI(ns.substr(p+1)));
		
	}
	std::string getURI(const std::string& ns) {
		return ns.substr(1,ns.size()-2);
	}
};

LOGGER_PTR_SET(log,"xmlbeansxx.XPath");

class Path {
public:

	Path(const NSMap& ns,bool create=false):ns(ns),create(create){};

	bool isAttr(const string& name) {
		return name[0]=='@';
	}
    
	
	void addXmlObject(const XmlObject & o) {
		if(o) obj.push_back(o);
	}
	
	Path getPath(const std::string& path) {
		if(path.size()<=0) return *this;
		std::pair<std::string, std::string> part = getXPathPart(path);
		int size=obj.size();
		LOG4CXX_DEBUG(log,std::string("XmlObjects in buffer:") + TextUtils::intToString(size));

		if (size<=0) {
			LOG4CXX_DEBUG(log,"no more XmlObjects in buffer");
			return *this;
		}
		std::pair<std::string, std::string> pred = getXPathPredicat(part.first);

		Path p = doPart(pred.first).doPredicat(pred.second);
		
		return p.getPath(part.second);
	}
	
	pair<string,string> getXPathPart(const string& path) {
		int p=path.find('/');
		if(p<0)	return pair<string,string>(path,"");
		else	return pair<string,string>(path.substr(0,p),path.substr(p+1));
	}

	pair<string,string> getXPathPredicat(const string& part) {
		int p=part.find('[');
		if(p<0)	return pair<string,string>(part,"");
		else 	return pair<string,string>( part.substr(0,p), part.substr(p+1) );
	}
	
	pair<string,int> decomposeElem(const std::string& name) {
		int a,b;
		if ((a=name.find('['))!=-1) {
        		int pos=0;
			a++;
	        	b=name.find(']');
        		BOOST_ASSERT(b!=-1);
        		string n=name.substr(a,b-a);
	        	pos=atoi(n.c_str())-1;
        		return pair<string,int>(name.substr(0,a-1),pos);
	    	} else {
        		return pair<string,int>(name,0);
	    	}
	}
	
private:
	std::string _getQNameString(const QName& name){
		if(std::string(name.first).size()>0) return name.first + std::string(":") + name.second;
		return name->second;
	}
	std::string _getQNameStringWithStar(const std::string& part){
		if(part.find(":")==std::string::npos) return part;
		try {
			return _getQNameString(ns.getQName(part));
		} catch(...) {
			return part;
		}
	}
	std::vector<std::string> tokenize(const std::string& str,const std::string& t){
		std::vector<std::string> retu;
		std::string sub=str;
		std::string::size_type p;
		while((p = sub.find(t)) != std::string::npos) {
			LOG4CXX_DEBUG(log,"tokenize: " + sub.substr(0,p));
			retu.push_back(sub.substr(0,p));
			sub = sub.substr(p+t.size());
		}
		LOG4CXX_DEBUG(log,"tokenize: " + sub);
		retu.push_back(sub);		
		return retu;
	}
	
	bool matchQNameString(const std::string& qn,const std::vector<std::string>&  tab){
		BOOST_ASSERT(tab.size()>=1);
		if(tab.size() == 1) {
			if(tab[0] == qn) return true;
			else return false;
		}
		if(qn.find(tab.front())!=0) return false;
		std::string sub=qn;
		XMLBEANSXX_FOREACH(std::vector<std::string>::const_iterator,i,tab) {
//			LOG4CXX_DEBUG(log,"submatch: " + sub  + " ~= " + *i);
			int p = sub.find(*i);
			if(p<0) return false;
			sub = sub.substr(p+i->size());
		}
		if(sub.size()>0) {
		 	if(tab.back().size()>0) return false;
			else return true;
		 }
		return true;
		
	}

	Path doPredicat(const std::string& pred) {
		if(pred.size() <= 0) return *this;
		LOG4CXX_DEBUG(log,std::string("doPredicat: ") + pred);
		Path p(ns,create);
		stringstream s(pred);
		int pos=0; 
		s >> pos;
		LOG4CXX_DEBUG(log,std::string("selecting: ") + TextUtils::intToString(pos));		
		p.addXmlObject(obj[pos]);
		return p;
	}

	Path doPart(const std::string& part) {
		Path p(ns,create);

		LOG4CXX_DEBUG(log,"doPart: " + part);
		if (part.size()<=0) { 
			return *this;
		} else if (part == ".") {
			return *this;
		} else  {
			std::string elem_name;
			bool selectAttr = false;
			if(isAttr(part)) {
				selectAttr = true;
				elem_name = part.substr(1);
			} else 	elem_name = part;
			
			std::string partMatch = _getQNameStringWithStar(elem_name);
			std::vector<std::string> tab=tokenize(partMatch,"*");
			XMLBEANSXX_FOREACH(ObjType::iterator,i,obj) {
				Contents::Walker::ElemsType elems;
				if(selectAttr)	elems = Contents::Walker::getAttrObjects(*i);
				else		elems = Contents::Walker::getElems(*i);
				XMLBEANSXX_FOREACH(Contents::Walker::ElemsType::const_iterator,e,elems) {
					QName elemName=e->first;
					std::string elemMatch = _getQNameString(elemName);
					LOG4CXX_DEBUG(log,"element matching: "  + elemMatch + " ~= " + partMatch);
					
					if(matchQNameString(elemMatch, tab)) {
						LOG4CXX_DEBUG(log,"element matched: "   + elemMatch  + " ~= " + partMatch);
						try {
							p.addXmlObject(XmlObject(e->second));
						} catch(...) {}
					}
				}
			}	
		}
		return p;
	}
	
	typedef std::vector<XmlObject> ObjType;
	ObjType obj;

	const NSMap &ns;
	bool create;
	
	friend class XmlObject;
};



std::vector<XmlObject> XmlObject::selectPath(const std::string& path) {
	NSMapXPath ns;
	string s = ns.setXPathNamespaces(path);
	return selectPath(ns,s);
}

std::vector<XmlObject> XmlObject::selectPath(const NSMap& ns,const std::string& path) {
	LOG4CXX_DEBUG(log,"selectPath start:");
	LOG4CXX_DEBUG(log,"selectPath ns:"  + ns.toString());
	LOG4CXX_DEBUG(log,"selectPath xpath:"  + path);
	Path p(ns);
	p.addXmlObject(*this);
	Path retu = p.getPath(path);
	LOG4CXX_DEBUG(log,std::string("selectPath end: ")  + TextUtils::intToString(retu.obj.size()));
	return retu.obj;
}

const std::vector<XmlObject> XmlObject::selectPath(const std::string& path) const {
	NSMapXPath ns;
	string s = ns.setXPathNamespaces(path);
	return selectPath(ns,s);
}

const std::vector<XmlObject> XmlObject::selectPath(const NSMap& ns,const std::string& path) const {
	LOG4CXX_DEBUG(log,"selectPath start:");
	LOG4CXX_DEBUG(log,"selectPath ns:"  + ns.toString());
	LOG4CXX_DEBUG(log,"selectPath xpath:"  + path);
	Path p(ns);
	p.addXmlObject(*this);
	Path retu = p.getPath(path);
	LOG4CXX_DEBUG(log,std::string("selectPath end: ")  + TextUtils::intToString(retu.obj.size()));
	return retu.obj;
}


std::vector<XmlObject> XmlObject::cselectPath(const std::string& path) {
	NSMapXPath ns;
	string s = ns.setXPathNamespaces(path);
	return cselectPath(ns,s);
}


// Not implemented 
std::vector<XmlObject> XmlObject::cselectPath(const NSMap& ns,const std::string& path) {
	LOG4CXX_DEBUG(log,"cselectPath start:");
	LOG4CXX_DEBUG(log,"cselectPath ns:"  + ns.toString());
	LOG4CXX_DEBUG(log,"cselectPath xpath:"  + path);
/* 	Path p(ns,true);
	p.addXmlObject(*this);
	Path retu = p.getPath(path);
*/
	throw NotImplementedException("cselectPath not implemented.");
//	LOG4CXX_DEBUG(log,"cselectPath end: "  + retu.obj.size());	
//	return retu.obj;
}


}

