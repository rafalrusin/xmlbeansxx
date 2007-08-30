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
#include "geum_types.h"
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

class Path{
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
		if (obj.size()<=0) {
			LOG4CXX_DEBUG(log,"no more XmlObjects in buffer");
			return *this;
		}
		Path p = doPart(part.first);
		return p.getPath(part.second);
	}
	
	pair<string,string> getXPathPart(const string& path){
		int p=path.find('/');
		if(p<0)	return pair<string,string>(path,"");
		else	return pair<string,string>(path.substr(0,p),path.substr(p+1));
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
		return name->first + std::string(":") + name->second;
	}
	std::string _getQNameStringWithStar(const std::string& part){
		if(part.find(":")==-1) return part;
		try {
			return _getQNameString(ns.getQName(part));
		} catch(...) {
			return part;
		}
	}
	std::vector<std::string> tokenize(const std::string& str,const std::string& t){
		std::vector<std::string> retu;
		std::string sub=str;
		int p;
		while((p = sub.find(t)) != -1) {
			LOG4CXX_DEBUG(log,"tokenize: " + sub.substr(0,p));
			retu.push_back(sub.substr(0,p));
			sub = sub.substr(p+t.size());
		}
		LOG4CXX_DEBUG(log,"tokenize: " + sub);
		retu.push_back(sub);		
		return retu;
	}
	
	bool matchQNameString(const std::string& qn,const std::vector<std::string>&  tab){
		BOOST_ASSERT(tab.size()>1);
		if(qn.find(tab.front())!=0) return false;
		std::string sub=qn;
		FOREACH(i,tab) {
			LOG4CXX_DEBUG(log,"submatch: " << sub  << " ~= " << *i);
			int p = sub.find(*i);
			if(p<0) return false;
			sub = sub.substr(p+i->size());
		}
		if(sub.size()>0) {
		 	if(tab.back().size()>0) return false;
			else true;
		 }
		return true;
		
	}


	Path doPart(const std::string& part) {
		Path p(ns,create);

		LOG4CXX_DEBUG(log,"doPart: " + part);
		if (part.size()<=0) { 
			return *this;
		} else if (part == ".") {
			return *this;
		} else if (part.find("*")!=-1) {
			std::string partMatch = _getQNameStringWithStar(part);
			VAL(tab,tokenize(partMatch,"*"));
			FOREACH(i,obj) {
				FOREACH(e,i->getSchemaType()->elements) {
					QName elemName=e->first;
					std:string elemMatch = _getQNameString(elemName);
					LOG4CXX_DEBUG(log,"element matching: "  << elemMatch << " ~= " << partMatch);
					
					if(matchQNameString(elemMatch, tab)) {
						LOG4CXX_DEBUG(log,"element matched: "   << elemMatch  << " ~= " << partMatch);
						try {
							LOG4CXX_DEBUG(log,"creating: "   << create);
							if(create && !Contents::Walker::isSetElem(*i,elemName)) {
								LOG4CXX_DEBUG(log,"creating element: "   << elemName.toString());
								Contents::Walker::cgetElem(*i,elemName);
							}
							
							for(int k=0;k<Contents::Walker::countElems(*i,elemName);k++)
								p.addXmlObject(Contents::Walker::getElem(*i,elemName,k));
						
						} catch(...) {}
					}
				}
			}	
		} else {
			pair<string,int> e=decomposeElem(part);
			QName name;
			if (isAttr(e.first))
				name = ns.getQName(e.first.substr(1));
			else	name = ns.getQName(e.first);

		
			if (isAttr(e.first)) {
				FOREACH(i,obj) {
					try {
						p.addXmlObject(Contents::Walker::getAttr(*i,name));
					} catch(...) {}
//					p.addXmlObject(Contents::Walker::getAttr(*i,name));
				}
			} else {
				FOREACH(i,obj) {
					try {
						if(create) 	p.addXmlObject(Contents::Walker::cgetElem(*i,name));
						else		p.addXmlObject(Contents::Walker::getElem(*i,name));
					} catch(...) {}
//					p.addXmlObject(Contents::Walker::getElem(*i,name,e.second));
				}
			}
		}
		return p;
	}
	
	std::vector<XmlObject> obj;

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
	Path p(ns);
	p.addXmlObject(*this);
	Path retu = p.getPath(path);
	return retu.obj;
}

const std::vector<XmlObject> XmlObject::selectPath(const std::string& path) const {
	NSMapXPath ns;
	string s = ns.setXPathNamespaces(path);
	return selectPath(ns,s);
}

const std::vector<XmlObject> XmlObject::selectPath(const NSMap& ns,const std::string& path) const {
	Path p(ns);
	p.addXmlObject(*this);
	Path retu = p.getPath(path);
	return retu.obj;
}


std::vector<XmlObject> XmlObject::cselectPath(const std::string& path) {
	NSMapXPath ns;
	string s = ns.setXPathNamespaces(path);
	return cselectPath(ns,s);
}

std::vector<XmlObject> XmlObject::cselectPath(const NSMap& ns,const std::string& path) {
	Path p(ns,true);
	p.addXmlObject(*this);
	Path retu = p.getPath(path);
	return retu.obj;
}


}


/*
ContentsPtr XmlObject::walkXpath(const string xpath,string &lastName,bool createElems) {
    int delim=xpath.find('/');
    if (delim==0) {
        return walkXpath(xpath.substr(1),lastName,createElems);
    } else if (delim==-1) {
        lastName=xpath;
        return contents;
    } else {
        string t=xpath.substr(0,delim);

        pair<string,int> e=decomposeElem(t);
	ContentsPtr sub;
	if(createElems)	sub=Contents::Walker::cgetElem(*this,e.first,e.second);
	else		sub=Contents::Walker::getElem(*this,e.first,e.second);

        if (sub==NULL) return ContentsPtr();
        return XmlObject(sub).walkXpath(xpath.substr(delim+1),lastName,createElems);
    }
}

bool XmlObject::isAttr(const string name) {
    return name[0]=='@';
}
pair<string,int> XmlObject::decomposeElem(const string name) {
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

std::string XmlObject::getXpathValue(const string xpath) {
    string t;
    ContentsPtr c_ptr=walkXpath(xpath,t,false);
    if (c_ptr==NULL) throw XpathException("getXpathValue: xpath not found " + std::string(xpath),XpathException::XPATH_NOT_FOUND);

    if (isAttr(t)) return Contents::Walker::getAttrString(c_ptr,t.substr(1));
    else {
        pair<string,int> e=decomposeElem(t);
        ContentsPtr sub=Contents::Walker::getElem(c_ptr,e.first,e.second);
        if (sub==NULL) throw XpathException("getXpathValue: xpath not found " + std::string(xpath),XpathException::XPATH_NOT_FOUND);
        return Contents::Walker::getSimpleContent(sub);
    }
}

void XmlObject::setXpathValue(const string xpath,const string value) {
    string t;
    ContentsPtr c_ptr=walkXpath(xpath,t,false);
    if (c_ptr==NULL) throw XpathException("setXpathValue: xpath not found " + std::string(xpath),XpathException::XPATH_NOT_FOUND);
    
    XmlObject obj(c_ptr);
    if (isAttr(t)) {
    	Contents::Walker::setAttr(obj,t.substr(1),value);
	return;
    } else {
        pair<string,int> e=decomposeElem(t);

        ContentsPtr sub=Contents::Walker::getElem(XmlObject(c_ptr),e.first,e.second);
        if (sub==NULL) throw XpathException("setXpathValue: xpath not found " + std::string(xpath),XpathException::XPATH_NOT_FOUND);

	XmlObject subObj(sub);
        if (Contents::Walker::hasElements(subObj)) {
            throw XpathException("setXpathValue: invalid xpath (this element has complex content) " + std::string(xpath),
                                 XpathException::INVALID_XPATH_FORMAT);
        }
	
        Contents::Walker::setSimpleContent(subObj,value);
    }
}

void XmlObject::addXpath(const string xpath,const string value) {
    string t;
    ContentsPtr c_ptr = walkXpath(xpath,t,true);
    BOOST_ASSERT(c_ptr!=NULL);
    XmlObject obj(c_ptr);
    if (isAttr(t)) {
    	Contents::Walker::setAttr(obj,t.substr(1),value);
    } else {
        pair<string,int> e=decomposeElem(t);

        ContentsPtr sub=Contents::Walker::cgetElem(obj,e.first,e.second);
	XmlObject subObj(sub);
        Contents::Walker::setSimpleContent(subObj,value);
    }
}

void XmlObject::addCdataXpath(const std::string xpath, const std::string value) {
    std::string x;
    x.append(CDATA_PREFIX);
    x.append(value);
    x.append(CDATA_SUFFIX);
    addXpath(xpath, x);
}

void XmlObject::removeXpath(const std::string xpath) {
    int delim=xpath.find('/');
    if (delim==0) {
        removeXpath(xpath.substr(1));
        return;
    } else if (delim==-1) {
        const string &t=xpath;
        if (isAttr(t)) {
            Contents::Walker::setAttr(*this,t.substr(1),std::string());
        } else {
            pair<string,int> e=decomposeElem(t);

            ContentsPtr sub=Contents::Walker::getElem(*this,e.first,e.second);
            if (sub!=NULL) {
                Contents::Walker::setElem(*this,e.first,ContentsPtr(),e.second);
            } else {
                throw XpathException("removeXpath: xpath not found " + std::string(xpath),
                                     XpathException::XPATH_NOT_FOUND);
            }
        }
        return;
    } else {
        string t=xpath.substr(0,delim);

        pair<string,int> e=decomposeElem(t);
        ContentsPtr  sub=Contents::Walker::getElem(*this,e.first,e.second);
        if (sub==NULL) {
            throw XpathException("removeXpath: xpath not found " + std::string(xpath),
                                 XpathException::XPATH_NOT_FOUND);
        }
	XmlObject subObj(sub);
        subObj.removeXpath(xpath.substr(delim+1));
        if (Contents::Walker::hasEmptyContent(subObj)) {
            Contents::Walker::setElem(*this,e.first,ContentsPtr(),e.second);
        }
    }
}

vector<std::string>* XmlObject::getXpaths(const std::string xpath) {
    vector<std::string>* v=new vector<std::string>();

    string t;
    ContentsPtr c_ptr=walkXpath(xpath,t,false);
    if (c_ptr==NULL) {
        throw XpathException("getXpaths: xpath not found " + std::string(xpath),
                             XpathException::XPATH_NOT_FOUND);
    }
    if (isAttr(t)) {
        v->push_back(xpath);
        return v;
    } else {
        pair<string,int> e=decomposeElem(t);
        BOOST_ASSERT(e.second==0);

        int nr=0;
        vector<pair<QName,ContentsPtr > > els=Contents::Walker::getElems(contents);
        FOREACH(it,els) {
            if (e.first==it->first) {
                ostringstream ss;
                ss<<xpath<<"["<<nr<<"]";
                v->push_back(ss.str());
                nr++;
            }
        }
        return v;
    }
}


void XmlObject::getXpathsLikeMapRec(XpathsMap &m,const std::string prefix) {
    vector<pair<QName,string> > ats=Contents::Walker::getAttrs(contents);
    FOREACH(it,ats) {
        ostringstream ss;
        ss<<prefix<<"@"<<std::string(it->first);
        m.insert(pair<string,string>(ss.str(),it->second));
    }
    map<string,int> counter;

    vector<pair<QName,ContentsPtr > > els=Contents::Walker::getElems(contents);
    FOREACH(it,els) {
        ostringstream ss;
        counter[it->first]++;
        int c=counter[it->first];
        ss<<prefix<<std::string(it->first);
        if (c>1) {
            ss<<"["<<c<<"]";
        }
        string p=ss.str();
        if (it->second!=NULL) {
            if (it->second->hasElements()) {
                //cout<<p<<" i:"<<c<<" has empty content\n";
                const string x=it->second->getSimpleContent();
                m.insert(pair<string,string>(p,x));
            }
            XmlObject(it->second).getXpathsLikeMapRec(m,p+"/");
        } else {
            //m.push_back(p,"");
        }
    }
}

vector<std::string>* XmlObject::getXpathsLike(const std::string xpath) {
    XpathsMap *map=getXpathsLikeMap(xpath);
    vector<std::string>* v=new vector<std::string>();
    FOREACH(it,*map) {
        v->push_back(it->first);
    }
    delete map;
    return v;
}

XpathsMap* XmlObject::getXpathsLikeMap(const std::string xpath) {
    string t;
    ContentsPtr c_ptr=walkXpath(xpath,t,false);
    if (c_ptr==NULL) {
        throw XpathException("getXpathsLikeMap: xpath not found " + std::string(xpath),
                             XpathException::XPATH_NOT_FOUND);
    }
    if (isAttr(t)) {
        throw XpathException("getXpathsLikeMap: invalid xpath (it's an attribute) " + std::string(xpath),
                             XpathException::INVALID_XPATH_FORMAT);
    } else {
        pair<string,int> e=decomposeElem(t);
        ContentsPtr c2_ptr=Contents::Walker::getElem(contents,e.first,e.second);
        if (c2_ptr==NULL) {
            throw XpathException("getXpathsLikeMap: xpath not found " + std::string(xpath),
                                 XpathException::XPATH_NOT_FOUND);
        }
        XpathsMap* map = new XpathsMap();
        XmlObject(c2_ptr).getXpathsLikeMapRec(*map,xpath+"/");
        return map;
    }
}


}
*/

