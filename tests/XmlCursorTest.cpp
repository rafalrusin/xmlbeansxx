#include "XmlCursor.h"
#include "XmlCursorTest.h"
#include <string>

#include <xmlbeansxx/logger.h>
#include "XercesParser.h"
#include "XmlObjectDocument.h"
#include <fstream>


CPPUNIT_TEST_SUITE_REGISTRATION( XmlCursorTest );


XMLBEANSXX_LOGGER_PTR_SET(logger,"test.XmlCursorTest");


using namespace xmlbeansxx;
using namespace std;

void XmlCursorTest::xmlCursorTest() {


	{	// next Token test
		std::string cmp = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<e  z='z' xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
			"<a ala='ola'> \n  "
				"<b xmlns:pr='xxxx' elem='1'/>"
				" this is a text "
			"</a>"
			" Dwa "
			" <c/> "
			" Trzy \n " 
		"</e>\n";

		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger,o.toString())
		XmlCursor cursor(o);
		XmlCursor::TokenType t=cursor.currentTokenType();
		XmlCursor::TokenType tokenTab[] = { 
		XmlCursor::INT_STARTDOC,
		/*e*/		XmlCursor::INT_START,
					XmlCursor::INT_ATTR,
					XmlCursor::INT_NAMESPACE,
					XmlCursor::INT_NAMESPACE,
					XmlCursor::INT_NAMESPACE,
			
			/*a*/		XmlCursor::INT_START,
						XmlCursor::INT_ATTR,
						XmlCursor::INT_TEXT,

				/*b*/		XmlCursor::INT_START,
							XmlCursor::INT_ATTR,
							XmlCursor::INT_NAMESPACE,
				/*/b*/		XmlCursor::INT_END,
						
						XmlCursor::INT_TEXT,
			/*/a*/		XmlCursor::INT_END,
				
					XmlCursor::INT_TEXT,
					
			/*c*/		XmlCursor::INT_START,
			/*/c*/		XmlCursor::INT_END,
			
					XmlCursor::INT_TEXT,
		/*/e*/		XmlCursor::INT_END,
		
		/*end of the document*/
		XmlCursor::INT_ENDDOC ,

		XmlCursor::INT_NONE
		};
		
		int tab=0,j=0;
		do {
			for(int i=0;i<tab;i++) cout << " ";
			cout << t  << " pos = " << cursor.pos  << " stack.size = " << cursor.stack.size() << " attrs = " << cursor.countAttrs() << " elems = " << cursor.countElems() << "\n";
			CPPUNIT_ASSERT_EQUAL(tokenTab[j++],t);

			if(t==XmlCursor::INT_START) tab +=4;
			t=cursor.toNextToken();
			if(t==XmlCursor::INT_END) tab -=4;
		
		}while(t != XmlCursor::INT_NONE);

		do{
			for(int i=0;i<tab;i++) cout << " ";
			cout << t  << " pos = " << cursor.pos  << " stack.size = " << cursor.stack.size() << " attrs = " << cursor.countAttrs() << " elems = " << cursor.countElems() << "\n";
			CPPUNIT_ASSERT_EQUAL(tokenTab[j--],t);

			if(t==XmlCursor::INT_END) tab +=4;
			t=cursor.toPrevToken();
			if(t==XmlCursor::INT_START) tab -=4;
		
		}while(t != XmlCursor::INT_NONE);
	
	}
	
	{ //test setName , getName , insertElement , insertNamespace ,insertAttributeWithValue , getAttributeText
		std::string cmp = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<e  z=\"z\" xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
			"<a ala=\"ola\"> \n  "
				"<b xmlns:pr=\"xxx\" elem=\"1\"/>"
				" this is a text "
			"</a>"
			" Dwa "
			" <c attr=\"second\"/> "
			" Trzy \n " 
		"</e>\n";
		std::string cmp2 = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<olek z=\"z\" xmlns=\"http://ala\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
			"<a ala=\"ola\"> \n  "
				"<b elem=\"1\" xmlns:pr=\"xxx\"/>"
				" this is a text "
			"</a>"
			" Dwa "
  			" <c attr0=\"first\" attr=\"second\" xmlns:newNs=\"http://new.namespace.com\"><endns:end xmlns:endns=\"http://wiki\">this is the end</endns:end></c> "
			" Trzy \n " 
		"</olek>\n";

		
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger,o.toString())		
		XmlCursor cursor = o.newCursor();
		cursor.toFirstContentToken();
		cursor.setName(QName("http://ala","olek"));
		CPPUNIT_ASSERT(QName("http://ala","olek")==cursor.getName());
		XmlString end("this is the end");
		cursor.toFirstContentToken();
		cursor.toNextSibling();
		cursor.toFirstContentToken();
		cursor.insertElement(QName("http://wiki","end","endns"),end);
		cursor.toFirstAttribute();
		cursor.insertNamespace("newNs","http://new.namespace.com");
		cursor.insertAttributeWithValue(QName("","attr0"),"first");

		std::string r = o.toString();
		XMLBEANSXX_DEBUG(logger,r);
		CPPUNIT_ASSERT_EQUAL(cmp2,r);
		CPPUNIT_ASSERT_EQUAL(std::string("second"),cursor.getAttributeText(QName("","attr")));
	}
	
	{// beginElement, toEndToken, insertElement , insertAttributeWithValue
		std::string cmp =  "<a> text <b/> text 2 </a>\n";
		std::string cmp2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				   "<a> text <b/> text 2 <new attr=\"first\"><elem> new element </elem></new></a>\n";
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger,o.toString())		
		XmlCursor cursor(o);
		
		cursor.toFirstContentToken();
		cursor.toFirstContentToken();
		cursor.toEndToken();
		cursor.beginElement(QName("","new"));
		cursor.insertAttributeWithValue(QName("","attr"),"first");
		cursor.insertElement(QName("","elem"),XmlString(" new element "));
		std::string r = o.toString();
		XMLBEANSXX_DEBUG(logger,r);
		CPPUNIT_ASSERT_EQUAL(r,cmp2);
		
	}
	
	{ // toChild , toNextSibling , toFirstContentToken, toParent , toPrevSibling, toPrevToken
		std::string cmp =  "<a> text <b/> text 2 <c/> text 3 <b attr=\"second\"/> text4 <b/> <e/> <b attr=\"first\" /> <c/> </a>\n";
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger,o.toString())		
		XmlCursor cursor(o);
		
		cursor.toNextToken();
		cursor.toNextToken();
		cursor.toChild(2);
		cursor.toNextSibling(QName("","b"),3);
		cursor.toFirstContentToken();
		CPPUNIT_ASSERT_EQUAL(std::string("first"),cursor.getAttributeText(QName("","attr")));
		
		cursor.toParent();
		cursor.toPrevSibling();
		CPPUNIT_ASSERT(QName("","e")==cursor.getName());
		
		cursor.toPrevSibling(2);
		cursor.toFirstContentToken(); //INT_END <b>
		CPPUNIT_ASSERT_EQUAL(std::string("second"),cursor.getAttributeText(QName("","attr")));
		
		cout << "\n" << cursor.toPrevToken(); // attr="second"
		cout << "\n" << cursor.toPrevToken(); // start <b>
		cout << "\n" << cursor.toPrevToken(); // text 3
		cout << "\n" << cursor.toPrevToken(); // end	</c>	
		cout << "\n" << cursor.toPrevToken(); // start <c>
		CPPUNIT_ASSERT(QName("","c")==cursor.getName());
	}
	
	{ //  getTextValue, toLastAttribute, toFirstAttribute , toPrevAttribute, toNextAttribute, setTextValue
		std::string cmp =  "<attrTest a='text'  b='text 2' c='text 3' attr2='second'  attr='first' />\n";
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger,o.toString())		
		XmlCursor cursor(o);
		cursor.toFirstContentToken();
		
		cursor.toNextToken();
		CPPUNIT_ASSERT(QName("","a")==cursor.getName());
		CPPUNIT_ASSERT_EQUAL(std::string("text"),cursor.getTextValue());
		cursor.setTextValue("new text");
		cursor.toLastAttribute();
		CPPUNIT_ASSERT(QName("","attr")==cursor.getName());
		CPPUNIT_ASSERT_EQUAL(std::string("first"),cursor.getTextValue());
		cursor.toPrevAttribute();
		CPPUNIT_ASSERT(QName("","attr2")==cursor.getName());
		CPPUNIT_ASSERT_EQUAL(std::string("second"),cursor.getTextValue());
		cursor.toFirstAttribute();
		CPPUNIT_ASSERT(QName("","a")==cursor.getName());
		CPPUNIT_ASSERT_EQUAL(std::string("new text"),cursor.getTextValue());
		cursor.toNextAttribute();
		CPPUNIT_ASSERT(QName("","b")==cursor.getName());
		CPPUNIT_ASSERT_EQUAL(std::string("text 2"),cursor.getTextValue());
	}
	
	{ //  insertChars
		std::string cmp =  	"<Test><a/></Test>\n";
		std::string cmp2 =  	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					"<Test>befor<a/>after</Test>\n";
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger,o.toString())		
		XmlCursor cursor(o);
		cursor.toFirstContentToken();
		cursor.toFirstContentToken();
		cursor.insertChars("befor");
		cursor.toEndToken();
		cursor.insertChars("after");
		
		std::string r = o.toString();
		XMLBEANSXX_DEBUG(logger,r);
		CPPUNIT_ASSERT_EQUAL(cmp2,r);	
	}

	{ //  removeXml
		std::string cmp =  	"<Test empty=\"\" attr=\"rem\"  >befor <b> del </b> 2 <a/>after</Test>\n";
		std::string cmp2 =  	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					"<Test empty=\"\">befor  2 <a/></Test>\n";
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XMLBEANSXX_DEBUG(logger,o.toString())		
		XmlCursor cursor(o);
		cursor.toFirstContentToken();
		cursor.toFirstContentToken();
		cursor.toLastAttribute();
		cursor.removeXml();
		cursor.toChild(QName("","b"));
		cursor.removeXml();
		cursor.toEndToken();
		cursor.toPrevToken();
		XMLBEANSXX_DEBUG(logger,o.toString());
		cursor.removeXml();
		
		std::string r = o.toString();
		XMLBEANSXX_DEBUG(logger,r);
		CPPUNIT_ASSERT_EQUAL(cmp2,r);	
	}
	{	//XmlObject.clone();
		std::string cmp =  	"<Test  passwd=\"secret\"  > <b> del </b> </Test>\n";
		XmlObject o = XmlObjectDocument::Factory::parse(cmp);
		XmlObject c = o.clone();
		XmlCursor cursor(c);
		do {
			if(cursor.getName().toString().find("passwd") != std::string::npos) 
				cursor.setTextValue("****");
		} while(cursor.toNextToken() != XmlCursor::INT_NONE);

		std::string r = o.toString();
		std::string s = c.toString();
		XMLBEANSXX_DEBUG(logger,s)		
		XMLBEANSXX_DEBUG(logger,r);
		
		CPPUNIT_ASSERT(s!=r);	
	
	}
	{	// instertDocument
		std::string str =  "<Test> <b> del </b> </Test>\n";
		std::string cmp =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				   "<Test><Test> <b> del </b> </Test> <b> del </b> </Test>\n";
		XmlObject o = XmlObjectDocument::Factory::parse(str);
		XmlObject o2 = XmlObjectDocument::Factory::parse(str);
		XmlCursor cursor(o);
		cursor.toChild();
		cursor.toNextToken();
		cursor.insertDocument(o2);
		std::string out =o.toString();
		XMLBEANSXX_DEBUG(logger,"insertDocument: " + out);
		CPPUNIT_ASSERT_EQUAL(cmp,out);	
	
	}
	
	

}

