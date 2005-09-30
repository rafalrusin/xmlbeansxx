/*
 *  Copyright 2004-2005 TouK s.c.
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Created on Jul 12, 2004
 * 
 * TODO To change the template for this generated file go to Window -
 * Preferences - Java - Code Style - Code Templates
 */

package pl.touk.xmlbeansxx;

import java.io.*;
import java.util.*;
import javax.xml.namespace.QName;

import org.apache.xmlbeans.*;
import org.apache.commons.logging.*;

public class ClassGen {
    private static final String CUSTOM_CODE_STR="//-------------Custom Code---------------";
    private static final String CUSTOM_CODE_END_STR="//-------------~Custom Code---------------";
    private static Log LOG = LogFactory.getLog(ClassGen.class);

    public static String outDir;

    /*
      public static final String[] FUNDAMENTAL_TYPES = { "XmlObject",
      "XmlInteger", "XmlDecimal", "XmlDateTime", "XmlBoolean",
      "XmlString", "XmlInt", "XmlDate",
      //and their base class complement
      "XmlAnySimpleType" };
    */
	

    static class Properties {
        public static final int GET_ATTR = 0, SET_ATTR = 1, UNSET_ATTR = 2,
            GET = 10, CGET = 11, SET = 12, UNSET = 13, GET_AT = 20,
            CGET_AT = 21, SET_AT = 22, UNSET_AT = 23, GET_ARRAY = 30,
            SET_ARRAY = 31, UNSET_ARRAY = 32, ADD = 33,
            SIZEOF = 34, ADD_NEW = 36, ISSET_ATTR = 37, ISSET = 38,
            CGET_ARRAY = 39, REMOVE = 40,
            SET_ARRAY_AT = 41, GET_ARRAY_AT = 42,
            DGET_ARRAY = 43, DSET_ARRAY = 44;

        public static final int[] HUMAN = new int[1];

        public static final int[] GENTOR = { GET_ATTR, SET_ATTR, CGET_AT,
                                             GET_AT, SET_AT, ADD, UNSET_ARRAY };
    }

    private static int[] genProperties = Properties.HUMAN;

    /** returs true when given method should be generated */
    private boolean haveToGen(int method) {
        if (genProperties==Properties.HUMAN) return true; 
        else {
            for (int i = 0; i < genProperties.length; i++) {
                if (genProperties[i] == method) {
                    return true;
                }
            }
            return false;
        }
    }

    //---------------------------------------------------------------------

    static class TypeValue {
        String name;

        SchemaType type;
    }

    public static interface PropertyTraversor {
        public void fn(SchemaProperty prop);
    }

    Output out;
    Input in;

    NamespaceLinks nsLinks = new NamespaceLinks();

    public ClassGen(SchemaTypeSystem typeSystem, String name) {
        currentName = name;
        {
            generatingBuiltin = false;
            if (System.getProperty("genbuiltin") != null) {
                generatingBuiltin = true;
            }
            /*
             * //printing all builtin types SchemaTypeSystem
             * t=XmlBeans.getBuiltinTypeSystem(); List all2 = new ArrayList();
             * all2.addAll(Arrays.asList(t.documentTypes()));
             * all2.addAll(Arrays.asList(t.attributeTypes()));
             * all2.addAll(Arrays.asList(t.globalTypes())); for (int i = 0; i <
             * all2.size(); i++) { SchemaType sType = (SchemaType)all2.get(i);
             * System.out.println(className(sType)+"
             * "+sType.getName().getLocalPart());
             * all2.addAll(Arrays.asList(sType.getAnonymousTypes())); }
             */
        }

        calculateAllTypes(typeSystem);
    }

    static class NamespaceLinks {
        private HashMap dict = new HashMap();

        private int nr = 0;

        public String getVarName(String namespace) {
            Object o = dict.get(namespace);
            if (o == null) {
                dict.put(namespace, new String("NS_" + nr));
                nr++;
            }
			
            return nsFromTS(namespace);
        }

        private String nsFromTS(String namespace) {
            return genString(namespace);
        }
		
        /*
        public void printDefinitions(PrintWriter wt) {
            Iterator it = dict.keySet().iterator();
            wt.println("namespace {");
            while (it.hasNext()) {
                String k = (String) it.next();
                String v = (String) dict.get(k);
                wt.println("xmlbeansxx::StoreString " + v + ";");
            }
            wt.println("}");
        }*/
		
        public void printInit(PrintWriter wt) {
            Iterator it = dict.keySet().iterator();
            while (it.hasNext()) {
                String k = (String) it.next();
                String v = (String) dict.get(k);
                wt.println("xmlbeansxx::StoreString::store(" + genString(k) + ");");
            }
        }
    }

    class Includes {
        private Set set = new HashSet();

        private Set cppSet = new HashSet();

        public void add(String name) {
            set.add(name);
        }

        public void remove(String name) {
            set.remove(name);
        }

        public void addCpp(String name) {
            cppSet.add(name);
        }

        public void generate() {
            out.cpp.println("#include <xmlbeansxx/xml-fragment.h>");
            out.cpp.println("#include <xmlbeansxx/XmlArray.h>");
            out.cpp.println("#include <xmlbeansxx/XmlArray.h>");
            out.cpp.println("#include <xmlbeansxx/TextUtils.h>");
            out.cpp.println("#include <xmlbeansxx/XmlObjectBase.h>");
            out.cpp.println("#include <xmlbeansxx/XmlComplexContentImpl.h>");
            out.cpp.println("#include <xmlbeansxx/CXXBooleanHolder.h>");
            out.cpp.println("#include <xmlbeansxx/CXXDecimalHolder.h>");
            out.cpp.println("#include <xmlbeansxx/CXXDoubleHolder.h>");
            out.cpp.println("#include <xmlbeansxx/CXXFloatHolder.h>");
            out.cpp.println("#include <xmlbeansxx/CXXIntegerHolder.h>");
            out.cpp.println("#include <xmlbeansxx/CXXIntHolder.h>");
            out.cpp.println("#include <xmlbeansxx/CXXLongHolder.h>");
            out.cpp.println("#include <xmlbeansxx/CXXStringHolder.h>");
            out.cpp.println("#include <xmlbeansxx/TypeStore.h>");
            out.cpp.println("#include <xmlbeansxx/TypeStoreUser.h>");
            out.cpp.println("#include <xmlbeansxx/XmlParser.h>");
            out.cpp.println("#include <xmlbeansxx/Existence.h>");
            out.cpp.println("#include <xmlbeansxx/XmlBeans.h>");
            out.cpp.println("#include <xmlbeansxx/SchemaTypeLoaderImpl.h>");
            out.cpp.println("#include <xmlbeansxx/SchemaTypeImpl.h>");
			
            if (!generatingBuiltin) {
                out.h.println("#include <xmlbeansxx/XmlTypesGen.h>");
            } else {
                out.h.println("#include <xmlbeansxx/Null.h>");
                out.h.println("#include <xmlbeansxx/Ptr.h>");
            }
            //if (set.isEmpty()) {
            //} else {
            Iterator it = set.iterator();
            while (it.hasNext()) {
                String n = (String) it.next();
                out.h.println("#include \"" + n + ".h\"");
                out.hImpl.println("#include \"" + n + "Impl.h\"");
            }
            //}
            
            out.hImpl.println("#include \"" + currentName + ".h\""); 

            addCpp(currentName);
            it = cppSet.iterator();
            while (it.hasNext()) {
                String n = (String) it.next();
                if (!set.contains(n)) {
                    out.cpp.println("#include \"" + n + ".h\"");
                    out.cpp.println("#include \"" + n + "Impl.h\"");
                }
            }
        }
    }

    public static final String SCHEMA_NAME_VALID_CHARS = "-_.";

    //-----------------------------------------------------------------------------------------------
    //static methods

    private static int idNr=0;
    public static String nextTmpID() {
        idNr++;
        return "tmp" + idNr;
    }
	
    public static String baseFileName(String fileName) {
        if (fileName.lastIndexOf('.') != -1) {
            fileName = fileName.substring(0, fileName.lastIndexOf('.'));
        }

        if (fileName.lastIndexOf('/') != -1) {
            fileName = fileName.substring(fileName.lastIndexOf('/') + 1);
        }
        return fileName;
    }

    public static SchemaType getSuperType(SchemaType st) {
        return skipNullTypes(skipNullTypes(st).getBaseType());
    }

    public static String schemaToJavaTypeName(String schemaType) {
        return schemaToJavaPropertyName(schemaType);
    }

    public static String schemaToJavaPropertyName(String schemaName) {
        String n = schemaName;
        StringBuffer r = new StringBuffer();
        boolean upper = true;
        for (int i = 0; i < n.length(); i++) {
            char ch = n.charAt(i);
            if (SCHEMA_NAME_VALID_CHARS.indexOf(ch) != -1) {
                upper = true;
            } else {
                if (upper)
                    ch = Character.toUpperCase(ch);
                r.append(ch);
                upper = false;
            }
        }
        return r.toString();
    }

    static String cppTypeForCode(int javaTypeCode) {
        switch (javaTypeCode)
            {
            case SchemaProperty.JAVA_BOOLEAN:
                return "bool";
            case SchemaProperty.JAVA_FLOAT:
                return "float";
            case SchemaProperty.JAVA_DOUBLE:
                return "double";
            case SchemaProperty.JAVA_BYTE:
                return "unsigned char";
            case SchemaProperty.JAVA_SHORT:
                return "short";
            case SchemaProperty.JAVA_INT:
                return "int";
            case SchemaProperty.JAVA_LONG:
                return "xmlbeansxx::BigInteger";

            case SchemaProperty.JAVA_BIG_DECIMAL:
            	return "xmlbeansxx::BigDecimal";
            case SchemaProperty.JAVA_BIG_INTEGER:
                return "xmlbeansxx::BigInteger";
            case SchemaProperty.JAVA_STRING:
                return "xmlbeansxx::String";
            case SchemaProperty.JAVA_BYTE_ARRAY:
                return "xmlbeansxx::Array<unsigned char>";
            case SchemaProperty.JAVA_GDATE:
                return "xmlbeansxx::GDate";
            case SchemaProperty.JAVA_GDURATION:
                //return "org.apache.xmlbeans.GDuration";
            	return "xmlbeansxx::GDuration";
            case SchemaProperty.JAVA_DATE:
                return "xmlbeansxx::Date";
            case SchemaProperty.JAVA_QNAME:
                return "xmlbeansxx::QName";
            case SchemaProperty.JAVA_LIST:
                return "xmlbeansxx::List";
            case SchemaProperty.JAVA_CALENDAR:
                return "xmlbeansxx::Calendar";

            case SchemaProperty.JAVA_ENUM:
            	return "xmlbeansxx::String";
                //SchemaType sType = sProp.javaBasedOnType().getBaseEnumType();
                //return findJavaType(sType).replace('$', '.') + ".Enum";
                
            case SchemaProperty.JAVA_OBJECT:
            	return "xmlbeansxx::Object";
            
            default:
            	return "<NOTHING>";
            }
    }
	
    static String cppTypeForProperty(SchemaProperty sProp) {
        // The type to use is the XML object....
        if (sProp.getJavaTypeCode() == SchemaProperty.XML_OBJECT)
            {
                SchemaType sType = sProp.javaBasedOnType();
                return fullClassName(sType);
            }

    	return cppTypeForCode(sProp.getJavaTypeCode());
    }
    
    static String genJGetName(int javaType) {
        switch (javaType)
            {
            case SchemaProperty.XML_OBJECT:
                return null;
	
            case SchemaProperty.JAVA_BOOLEAN:
                return "Boolean";
	
            case SchemaProperty.JAVA_FLOAT:
                return "Float";
	
            case SchemaProperty.JAVA_DOUBLE:
                return "Double";
	
            case SchemaProperty.JAVA_BYTE:
                return "Byte";
	
            case SchemaProperty.JAVA_SHORT:
                return "Short";
	
            case SchemaProperty.JAVA_INT:
                return "Int";
	
            case SchemaProperty.JAVA_LONG:
                return "Long";
	
            case SchemaProperty.JAVA_BIG_DECIMAL:
                return "BigDecimal";
	
            case SchemaProperty.JAVA_BIG_INTEGER:
                return "BigInteger";
	
            case SchemaProperty.JAVA_STRING:
                return "String";
	
            case SchemaProperty.JAVA_BYTE_ARRAY:
                return "ByteArray";
	
            case SchemaProperty.JAVA_GDATE:
                return "GDate";
	
            case SchemaProperty.JAVA_GDURATION:
                return "GDuration";
	
            case SchemaProperty.JAVA_CALENDAR:
                return "Calendar";
	
            case SchemaProperty.JAVA_DATE:
                return "Date";

            case SchemaProperty.JAVA_QNAME:
                return "QName";

            case SchemaProperty.JAVA_LIST:
                return "List";

            case SchemaProperty.JAVA_ENUM:
                return "String";

            case SchemaProperty.JAVA_OBJECT:
                return "Object";
            }
        return "<NOTHING>";
    }

    /*
      static String genJGetValue(int javaType) {
      String s=genJGetName(javaType);
      if (s==null) return "";
      else return "->get" + s + "Value()";
      }

      static String genJSetValue(int javaType,String what) {
      String s=genJGetName(javaType);
      if (s==null) return "";
      else return "->set" + s + "Value(" + what + ")";
      }*/

    public static String constructorFor(String cppType) {
    	if (cppType.equals("unsigned char")) return "0";
    	else return cppType + "()";
    }
		
    
    //~static methods
    //-----------------------------------------------------------------------------------------------

    public static String javaPropertyName(SchemaProperty prop) {
        return schemaToJavaPropertyName(prop.getName().getLocalPart());
    }

    public static String className(SchemaType sType) {
        String s = getFullJavaName(sType);
        s = s.substring(s.lastIndexOf('.') + 1);
        return javaToCpp(s);
    }

    public static String classImplName(SchemaType st) {
        return className(st) + "Impl";
    }

    /*
     * String innerClassName(SchemaType sType) { StringTokenizer t = new
     * StringTokenizer(sType.getFullJavaName(), "."); String last = ""; while
     * (t.hasMoreTokens()) { last = t.nextToken(); } return
     * javaToCpp(last.replace('$', '.')); }
     * 
     * boolean isInner(SchemaType t) { String s=t.getFullJavaName(); return
     * (s.indexOf('$')>=0); }
     * 
     * String getOuterClassName(SchemaType st) { String s=st.getFullJavaName();
     * return javaToCpp((s.substring(0,s.lastIndexOf('$'))).replace('$','.')); }
     */

    static String getFullJavaName(SchemaType st) {
        String s=st.getFullJavaName();
        final String q="org.apache.xmlbeans.";
        if (s.length()>=q.length() && s.substring(0,q.length()).equals(q)) {
            return "xmlbeansxx." + s.substring(q.length());
        } else return s;
    }
	
    String nsClassName(SchemaType sType) {
        StringTokenizer t = new StringTokenizer(getFullJavaName(sType), ".");
        StringBuffer b = new StringBuffer();
        String last = "";
        while (t.hasMoreTokens()) {
            b.append(last);
            last = "::" + t.nextToken();
        }
        if (b.length() == 0)
            b.append("::");
        return b.substring(2);
    }

    public static SchemaType skipNullTypes(SchemaType sType) {
        if (sType == null)
            return null;
        while (getFullJavaName(sType) == null)
            sType = sType.getBaseType();
        return sType;
    }

    public static String javaToCpp(String x) {
        x = x.replace('$', '_');
        StringTokenizer t = new StringTokenizer(x, ".");
        StringBuffer b = new StringBuffer();
        while (t.hasMoreTokens()) {
            b.append("::" + t.nextToken());
        }
        return b.substring(2);
    }

    /**
     * Returns full C++ path of namespace keywords and a class name of given
     * type
     */
    public static String fullClassName(SchemaType type) {
        return javaToCpp(getFullJavaName(type));
    }

    public static String fullClassImplName(SchemaType st) {
        return fullClassName(st) + "Impl";
    }

    public static String genString(String s) {
        return "\"" + s + "\"";
    }

    /*
      public static boolean isFundamentalType(SchemaType st) {
      //return st.isBuiltinType();
      if (st.isBuiltinType()) {
      if (st.getName() == null)
      return false;
      String cl = className(st);
      for (int i = 0; i < FUNDAMENTAL_TYPES.length; i++) {
      if (cl.equals(FUNDAMENTAL_TYPES[i])) {
      return true;
      }
      }
      }
      return false;
      }*/

    /** returns first fundamental schema type on inheritance path to root */
    /*
      SchemaType fundamentalType(SchemaType st) {
      log.debug("fundamental from " + fullClassName(st));
      while (!isFundamentalType(st)) {
      st = st.getBaseType();
      }
      log.debug("fundamental to " + fullClassName(st));
      return st;
      }*/

    public static String genCast(String to, String from) {
        //return "boost::dynamic_pointer_cast<" + to + ">("+fromPtr+")";
        return "xmlbeansxx::cast<" + to + " >(" + from + ")";
        //return to+"Ptr(("+to+" *)(("+fromPtr+").get()))";
        //return "boost::static_pointer_cast<" + to + ">(" + fromPtr + ")";
    }

    public static String cutAssigns(String params) {
        StringTokenizer tok=new StringTokenizer(params,",");
        String r="";
        boolean first=true;
        while (tok.hasMoreTokens()) {
            StringTokenizer tok2=new StringTokenizer(tok.nextToken(),"=");
            if (!first) r+=",";
            if (tok2.hasMoreTokens()) r+=tok2.nextToken();
            first=false;
        }
        return r;
    }
    
    public static String genCreateRawXmlObject(SchemaType st) {
        return fullClassImplName(st) + "::newInstanceRawXmlObject";
    }
	
    public static String genNewXmlObject(SchemaType st) {
        return fullClassName(st)+"::Factory::newInstance()";
    }
	
    public static String genPropName(SchemaProperty elem) {
        return genQNamePropertyVar(elem);
    }
	
    void genAttrCode(SchemaType containerType, SchemaProperty attr) {
        PropGen pg=new PropGen(containerType, attr, out);

        //getters
        if (!attr.extendsJavaArray()) {
            if (haveToGen(Properties.GET_ATTR)) {
                pg.setGenMethod(PropGen.M_NORMAL);
                pg.genGetAttr();
				
                if (pg.hasHolder()) {
                    pg.setGenMethod(PropGen.M_X);
                    pg.genGetAttr();
                }
            }

            if (haveToGen(Properties.ISSET_ATTR)) {
                pg.genIsSetAttr();
            }
			
        } else {
            Utils.check(false);
        }
        //--

        //setters
        if (!attr.isReadOnly()) {
            if (!attr.extendsJavaArray()) {
                if (haveToGen(Properties.SET_ATTR)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genSetAttr();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genSetAttr();
                    }
					
                    /*
                    if (!cppTypeForProperty(attr).equals("xmlbeansxx::String")) {
                        pg.setGenMethod(PropGen.M_STRING);
                        pg.genSetAttr();
                    }*/
                }

                if (haveToGen(Properties.UNSET_ATTR)) {
                    pg.genUnsetAttr();
                }

            } else {
                Utils.check(false);
            }
        }
    }

    void genElemCode(SchemaType containerType, SchemaProperty elem) {
        PropGen pg=new PropGen(containerType, elem, out);
		
        {
            //getters
            if (!elem.extendsJavaArray()) {
                //Single Elem
				
                if (haveToGen(Properties.GET)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genGet();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genGet();
                    }
                }

                if (haveToGen(Properties.CGET)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genCGet();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genCGet();
                    }
                }
				
                if (haveToGen(Properties.ISSET)) {
                    pg.genIsSet();
                }

            } else {
                //Multi Elem
				
                if (haveToGen(Properties.SIZEOF)) {
                    pg.genSizeOf();
                }
				
                if (haveToGen(Properties.GET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genGetArray();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genGetArray();
                    }
                }

                /*
                if (haveToGen(Properties.DGET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genDGetArray();
                }*/

                if (haveToGen(Properties.GET_ARRAY_AT)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genGetArrayAt();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genGetArrayAt();
                    }
                }

                if (haveToGen(Properties.CGET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genCGetArrayAt();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genCGetArrayAt();
                    }
                }
            }
        }

        if (!elem.isReadOnly()) {
            if (!elem.extendsJavaArray()) {
                //Single Elem
				
                if (haveToGen(Properties.SET)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genSet();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genSet();
						
                        /*
                        if (!cppTypeForProperty(elem).equals("xmlbeansxx::String")) {
                            pg.setGenMethod(PropGen.M_STRING);
                            pg.genSet();
                        }*/
                    }
                }

                if (haveToGen(Properties.UNSET)) {
                    pg.genUnset();
                }

            } else {
                //Multi Elem
				
                if (haveToGen(Properties.SET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genSetArray();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genSetArray();
                    }
                }

                /*
                if (haveToGen(Properties.DSET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genDSetArray();
                }*/

                if (haveToGen(Properties.ADD)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genAdd();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genAdd();

                        /*
                        if (!cppTypeForProperty(elem).equals("xmlbeansxx::String")) {
                            pg.setGenMethod(PropGen.M_STRING);
                            pg.genAdd();
                        }*/
                    }
                }
				
                if (haveToGen(Properties.ADD_NEW)) {
                    pg.genAddNew();
                }

                if (haveToGen(Properties.UNSET_ARRAY)) {
                    pg.genUnsetArray();
                }

                if (haveToGen(Properties.SET_ARRAY_AT)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genSetArrayAt();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genSetArrayAt();

                        /*
                        if (!cppTypeForProperty(elem).equals("xmlbeansxx::String")) {
                            pg.setGenMethod(PropGen.M_STRING);
                            pg.genSetArrayAt();
                        }*/
                    }
                }
								
                if (haveToGen(Properties.REMOVE)) {
                    pg.genRemove();
                }
            }

        }

    }


    static void traverseProperties(SchemaType st, PropertyTraversor traversor) {
        if (!st.isSimpleType()) {
            //Traverse properties for complex content
            SchemaProperty[] properties;
            if (st.getContentType() == SchemaType.SIMPLE_CONTENT) {
                properties = st.getProperties();
            } else {
                properties = st.getDerivedProperties();
            }
            for (int i = 0; i < properties.length; i++)
                traversor.fn(properties[i]); 
        }
    }

    //-----------------------------------------------------------------------
    SchemaType currentType = null;

    /*
    public static String genPtr(String name) {
        return name + "Ptr";
    }*/

    private int classNumber = 0;

    public static String genWsRule(int ws_rule) {
        String p = "xmlbeansxx::SchemaType::";
		
        if (ws_rule==SchemaType.WS_COLLAPSE) { return p + "WS_COLLAPSE"; }
        else if (ws_rule==SchemaType.WS_PRESERVE) { return p + "WS_PRESERVE"; }
        else if (ws_rule==SchemaType.WS_REPLACE) { return p + "WS_REPLACE"; }
        else if (ws_rule==SchemaType.WS_UNSPECIFIED) { return p + "WS_UNSPECIFIED"; }
        else throw new IllegalStateException();
    }
	
    public static String genContentType(int contentType) {
        String p = "xmlbeansxx::SchemaType::";
		
        if (contentType==SchemaType.EMPTY_CONTENT) return p + "EMPTY_CONTENT";
        else if (contentType==SchemaType.SIMPLE_CONTENT) return p + "SIMPLE_CONTENT";
        else if (contentType==SchemaType.ELEMENT_CONTENT) return p + "ELEMENT_CONTENT";
        else if (contentType==SchemaType.MIXED_CONTENT) return p + "MIXED_CONTENT";
        else if (contentType==SchemaType.NOT_COMPLEX_TYPE) return p + "NOT_COMPLEX_TYPE";
        else throw new IllegalStateException();
    }

    private String genQName(QName name) {
        return "xmlbeansxx::QName(" + nsLinks.getVarName(name.getNamespaceURI()) + ", " + genString(name.getLocalPart()) + ")";
    }

    private static String genQNamePropertyVar(SchemaProperty prop) {
        return "_" + javaPropertyName(prop).toUpperCase() + (prop.isAttribute() ? "_ATTR" : "");
    }

    private static String genNVCType(int nvc) {
        if (nvc == SchemaProperty.NEVER) return "xmlbeansxx::SchemaProperty::NEVER";
        else if (nvc == SchemaProperty.VARIABLE) return "xmlbeansxx::SchemaProperty::VARIABLE";
        else if (nvc == SchemaProperty.CONSISTENTLY) return "xmlbeansxx::SchemaProperty::CONSISTENTLY";
        else throw new IllegalStateException();
    }
    
    private String genNewSchemaProperty(int i, SchemaProperty prop) {
        return fullClassName(prop.getType()) + "::staticInit();\n"
            + "{\n"
            + "xmlbeansxx::SchemaPropertyImpl _prop(xmlbeansxx::SchemaPropertyImpl::New());\n"
            + "_prop->order = " + (i + 1) + ";\n"
            + "_prop->schemaType = " + fullClassName(prop.getType()) + "::type;\n"
            + "_prop->_hasDefault = " + genNVCType(prop.hasDefault()) + ";\n"
            + "_prop->defaultText = " + genString(prop.getDefaultText() == null ? "" : prop.getDefaultText()) + ";\n"
            + "st->" + (prop.isAttribute() ? "attributes" : "elements") + "[" + genQName(prop.getName()) + "] = _prop;\n" 
            + "};\n";
    }

    private void genInitSchemaType(SchemaType st) {
        out.hImpl.println("public:");

        out.hImpl.println("static xmlbeansxx::SchemaType initSchemaType();");
        out.cpp.println("xmlbeansxx::SchemaType "
                        + classImplName(st) + "_I::initSchemaType() {");
        out.cpp.println("xmlbeansxx::SchemaTypeImpl st(xmlbeansxx::SchemaTypeImpl::New(typeid("+fullClassName(st)+")));");

        {
            out.cpp.println("st->_isDocumentType = " + (st.isDocumentType()?"true":"false") + ";");
            out.cpp.println("st->createRawXmlObject = " + genCreateRawXmlObject(st)+";");
			
            out.cpp.println("st->whitespaceRule = "+genWsRule(st.getWhiteSpaceRule())+";");
            out.cpp.println("st->className = "+genString(fullClassName(st))+";");
			
            out.cpp.println("st->contentType=" + genContentType(st.getContentType()) + ";");

            if (st.getName() == null) {
                out.cpp.println("st->name = xmlbeansxx::QName();");
            } else {
                out.cpp.println("st->name = xmlbeansxx::QName(" + nsLinks.getVarName(st.getName().getNamespaceURI()) + ", "+genString(st.getName().getLocalPart())+");");
            }
			
            {
                // if SchemaType is of xmlbeansxx::XmlObject type, then process
                // it's getContents()-> why?
                String r = null;
                if (fullClassName(st).equals("xmlbeansxx::XmlObject"))
                    r = "false";
                else
                    r = "true";
                out.cpp.println("st->processContents = "+r+";");
            }
        }

        if (st.isSimpleType()) {
            {
                XmlInteger fd=(XmlInteger)st.getFacet(SchemaType.FACET_FRACTION_DIGITS);
                if (fd!=null) {
                    out.cpp.println("st->fractionDigits="+fd.getBigIntegerValue().toString()+";");
                }
            }
        } else {
            {
                //String Storage
                SchemaProperty[] properties = st.getProperties();
                for (int i = 0; i < properties.length; i++) {
                    SchemaProperty p = properties[i];
                    out.cpp.println("xmlbeansxx::StoreString::store("
                                    +genString(p.getName().getLocalPart())+");");
                }
            }
            {
                //SubProperties: Elements with order
                SchemaProperty[] properties = st.getElementProperties();
                for (int i = 0; i < properties.length; i++) {
                    SchemaProperty p = properties[i];
                    out.cpp.print(genNewSchemaProperty(i, p));
                }
            }
            {
                //SubProperties: Attributes with order
                SchemaProperty[] properties = st.getAttributeProperties();
                for (int i = 0; i < properties.length; i++) {
                    SchemaProperty p = properties[i];
                    out.cpp.print(genNewSchemaProperty(i, p));
                }
            }
        }
        out.cpp.println("return st;");
        out.cpp.println("}");
    }
	
    private static String genNewObjectCode(SchemaType st) {
        return "return " + genCast(fullClassName(st), "xmlbeansxx::XmlBeans::getContextTypeLoader()->newInstance(" + fullClassName(st) + "::type, xmlbeansxx::XmlOptions::New())") + ";\n";
        /*
        return fullClassName(st) + " obj("
            + "new " + fullClassImplName(st) + "());\n"
            + " return obj;\n";*/
    }

    private static String parentConstructor(SchemaType st, String params) {
        if (st.getBaseType() != null) 
            return ":" + fullClassName(st.getBaseType()) + "(" + params + ")";
        else return "";
    }

    /*
    private static String getContentClassName(SchemaType st) {
        if (st.isSimpleType()) {
            if (st.getJavaTypeCode()
            return "xmlbeansxx::CXXStringHolder";
        } else {
            return "xmlbeansxx::XmlComplexContentImpl";
        }
    }*/
    
    private static String javaImplToCXX(String javaClass) {
        String bc = javaClass;
        if (bc.equals("org.apache.xmlbeans.impl.values.XmlComplexContentImpl")) {
            return "xmlbeansxx::XmlComplexContentImpl";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.XmlAnySimpleTypeImpl")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaBooleanHolderEx")) {
            return "xmlbeansxx::CXXBooleanHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaBase64HolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaHexBinaryHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaUriHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaQNameHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaNotationHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaFloatHolderEx")) {
            return "xmlbeansxx::CXXFloatHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaDoubleHolderEx")) {
            return "xmlbeansxx::CXXDoubleHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaDecimalHolderEx")) {
            return "xmlbeansxx::CXXDecimalHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaIntegerHolderEx")) {
            return "xmlbeansxx::CXXIntegerHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaLongHolderEx")) {
            return "xmlbeansxx::CXXLongHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaIntHolderEx")) {
            return "xmlbeansxx::CXXIntHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaStringEnumerationHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaStringHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaGDateHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.JavaGDurationHolderEx")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.XmlListImpl")) {
            return "xmlbeansxx::CXXStringHolder";
        } else if (bc.equals("org.apache.xmlbeans.impl.values.XmlUnionImpl")) {
            return "xmlbeansxx::CXXStringHolder";
        } else {
            LOG.error(bc);
            throw new IllegalStateException();
        }
    }
    
    private void printType(SchemaType _st) {
        /*
        {
            if (classNumber > 80) {
                out.splitCpp();
                classNumber = 0;
            }
            classNumber++;
        }*/

        final SchemaType st = skipNullTypes(_st);
        final SchemaType bt = getSuperType(st);

		Accessors accessors = new Accessors(st);
		accessors.emitSpecializedAccessors();
		
        currentType = st;
        out.h.println("/** This generated class corresponds to xsd type of QName "
                      + st.getName() + ". */");
        out.h.print("class " + className(st) + "_I");
        if (bt != null) {
            out.h.println(": public virtual " + fullClassName(bt) + "_I" + accessors.result.toString() + " {");
        } else {
            //out.h.println(": public boost::enable_shared_from_this<" + className(st) + "> {");
            out.h.println(": public virtual XmlTokenSource_I" + accessors.result.toString() + " {");
        }

        {
            //Impl
            out.hImpl.println("class " + classImplName(st) + "_I: public virtual " + className(st) + "_I, public virtual " + getBaseCXXClass(st) + "_I {");
        }
        {
            //Declare QNames of properties
            out.hImpl.println("private:");
            traverseProperties(st, new PropertyTraversor() {
                public void fn(SchemaProperty prop) {
                    out.hImpl.println("static xmlbeansxx::QName " + genQNamePropertyVar(prop) + ";");
                }
            });
        }
        {
            out.hImpl.println("public:");
            out.hImpl.println("virtual xmlbeansxx::SchemaType get_schema_type();");
            out.cpp.println("xmlbeansxx::SchemaType " + fullClassImplName(st) + "_I::get_schema_type() {");
            out.cpp.println("return " + className(st) + "::type;");
            out.cpp.println("}");

            /*
            if (st.getWhiteSpaceRule() == SchemaType.WS_COLLAPSE) {
                out.hImpl.println("virtual void setStringValue(const xmlbeansxx::String &value);");
                out.cpp.println("void " + fullClassImplName(st) + "_I::setStringValue(const xmlbeansxx::String &value) {");
                out.cpp.println("xmlbeansxx::XmlObjectBase::setStringValue(xmlbeansxx::TextUtils::collapse(value));");
                out.cpp.println("}");
            }*/
        }


        //----------------
        out.h.println("public:");
        out.h.println("typedef void factory_tag;");

        
        //Generating properties
        traverseProperties(st, new PropertyTraversor() {
            public void fn(SchemaProperty prop) {
                if (prop.isAttribute()) genAttrCode(st, prop);
                else genElemCode(st, prop);
            }
        });
		
        {
            //Custom code insertion
            String s=in.getHGap("class "+fullClassName(st));
            if (s!=null) {
                out.h.println("  "+CUSTOM_CODE_STR);
                out.h.print(s);
                out.h.println("  "+CUSTOM_CODE_END_STR);
            }

            s=in.getHGap("class " + fullClassImplName(st) + "_I");
            if (s!=null) {
                out.hImpl.println("  "+CUSTOM_CODE_STR);
                out.hImpl.print(s);
                out.hImpl.println("  "+CUSTOM_CODE_END_STR);
            }

            s=in.getCppGap("class " + fullClassImplName(st));
            if (s!=null) {
                out.cpp.println(CUSTOM_CODE_STR);
                out.cpp.print(s);
                out.cpp.println(CUSTOM_CODE_END_STR);
            }
        }

        {
            out.hImpl.println("class MyExistence_I: public xmlbeansxx::Existence_I {");
            out.hImpl.println("public:");
            //static_init
            out.hImpl.println("MyExistence_I();");
            out.cpp.println(fullClassImplName(st) + "_I::MyExistence_I::MyExistence_I() {");
            out.cpp.println(fullClassName(st) + "::type = " + fullClassImplName(st) + "_I::initSchemaType();");
            if (st.getName() != null) {
                //we register olny non anonymous types
                out.cpp.println(genCast("xmlbeansxx::SchemaTypeLoaderImpl", "xmlbeansxx::XmlBeans::getContextTypeLoader()") + "->addType(" + fullClassName(st) + "::type);");
            } else {
                //but for anonymous we invoke static initialization of schemaType 
            }

            {
                //Setting QNames of properties
                traverseProperties(st, new PropertyTraversor() {
                    public void fn(SchemaProperty prop) {
                        out.cpp.println(genQNamePropertyVar(prop) + " = " + genQName(prop.getName()) + ";");
                    }
                });
            }
            out.cpp.println("}");

            //static destruct
            out.hImpl.println("~MyExistence_I();");
            out.cpp.println(fullClassImplName(st) + "_I::MyExistence_I::~MyExistence_I() {");
            out.cpp.println(genCast("xmlbeansxx::SchemaTypeLoaderImpl", "xmlbeansxx::XmlBeans::getContextTypeLoader()") + "->removeType(" + fullClassName(st) + "::type);");
            out.cpp.println(fullClassName(st) + "::type = xmlbeansxx::Null();");
            {
                //Setting QNames of properties
                traverseProperties(st, new PropertyTraversor() {
                    public void fn(SchemaProperty prop) {
                        out.cpp.println(genQNamePropertyVar(prop) + " = xmlbeansxx::QName();");
                    }
                });
            }
            out.cpp.println("}");
            
            out.hImpl.println("};");
        }
        genInitSchemaType(st);
        {
            //Init QNames of properties
            traverseProperties(st, new PropertyTraversor() {
                public void fn(SchemaProperty prop) {
                    out.cpp.println("xmlbeansxx::QName " + fullClassImplName(st) + "_I::" + genQNamePropertyVar(prop) 
                        + " = " + fullClassImplName(st) + "_I::" + genQNamePropertyVar(prop) + ";");
                }
            });
        }
        {
            //~Impl
            out.hImpl.println("};");
        }
        
        out.h.println("};");

        out.h.println("BEGIN_CLASS(" + className(st) + ", " + className(st) + "_I)");
        out.hImpl.println("BEGIN_CLASS(" + classImplName(st) + ", " + classImplName(st) + "_I)");
        {
                out.hImpl.println("static "
                              + "xmlbeansxx::XmlObject"
                              + " newInstanceRawXmlObject();");
                out.cpp.println("xmlbeansxx::XmlObject" + " "
                                + classImplName(st) + "::newInstanceRawXmlObject() {\n return " + fullClassImplName(st) + "::New();\n}");
        }
        /*
        out.h.println("class " + className(st) + ": public boost::shared_ptr<" + className(st) + "_I> {");
        out.h.println("public:");
        out.h.println("typedef " + className(st) + "_I value_type;");
        out.h.println(className(st) + "() {}");
        out.h.println(className(st) + "(const boost::shared_ptr<" + className(st) + "_I> &ptr): boost::shared_ptr<" + className(st) + "_I>(ptr) {}");
        */
        {
            out.h.println("static xmlbeansxx::SchemaType type;");
            out.cpp.println("xmlbeansxx::SchemaType " + className(st) + "::type = " + className(st) + "::type;");
            out.h.println("static xmlbeansxx::Existence staticInit();");
            out.cpp.println("xmlbeansxx::Existence " + className(st) + "::staticInit() {");
            out.cpp.println("static xmlbeansxx::Existence ptr(new " + fullClassImplName(st) + "_I::MyExistence_I());");
            out.cpp.println("return ptr;");
            out.cpp.println("}");
        }
        
        {
            //Factory
            out.h.println("class Factory {");
            out.h.println("public:");
			
            {
                out.h.println("//Empty Object Creation");
                out.h
                    .println("static "
                             + fullClassName(st)
                             + " newInstance();");
                out.cpp.println(fullClassName(st) + " "
                                + className(st) + "::Factory::newInstance() {\n"
                                + genNewObjectCode(st) + "\n}" );

				
                /*
                out.h.println("static "
                              + "xmlbeansxx::XmlObject"
                              + " newInstanceXmlObject();");
                out.cpp.println("xmlbeansxx::XmlObject" + " "
                                + className(st) + "::Factory::newInstanceXmlObject() {\n" + genNewObjectCode(st) + "\n}");
                

                out.h.println("static "
                              + "xmlbeansxx::XmlObject"
                              + " newInstanceRawXmlObject();");
                out.cpp.println("xmlbeansxx::XmlObject" + " "
                                + className(st) + "::Factory::newInstanceRawXmlObject() {\n return xmlbeansxx::XmlObject(new " + fullClassImplName(st) + "_I());\n}");
                */
            }
            
            /*
            if (st.isSimpleType()) {
                {
                    out.h.println("static " + genPtr(fullClassName(st)) + " newInstance(const xmlbeansxx::String &value);");
                    out.cpp.println(genPtr(fullClassName(st)) + " " + className(st) + "::Factory::newInstance(const xmlbeansxx::String &value) {");
                    out.cpp.println(genPtr(fullClassName(st)) + " n(newInstance());");
                    out.cpp.println(genCast("xmlbeansxx::SimpleValuePtr","n") + "->setStringValue(value);");
                    out.cpp.println("return n;");
                    out.cpp.println("}");
                            
                    out.h.println("static " + genPtr(fullClassName(st)) + " newInstance(const xmlbeansxx::XmlAnySimpleTypePtr &value);");
                    out.cpp.println(genPtr(fullClassName(st)) + " " + className(st) + "::Factory::newInstance(const xmlbeansxx::XmlAnySimpleTypePtr &value) {");
                    out.cpp.println(genPtr(fullClassName(st)) + " n(newInstance());");
                    out.cpp.println("n->setSimpleContent(value->getSimpleContent());");
                    out.cpp.println(genCast("xmlbeansxx::SimpleValuePtr","n") + "->setStringValue(" + genCast("xmlbeansxx::SimpleValuePtr", "value") + "->getStringValue());");
                    out.cpp.println("return n;");
                    out.cpp.println("}");
                }
            }*/
        
            //-------------
            out.h.println("//Object Creation using parsing");
            out.h.println("static "
                         + className(st)
                         + " parse(std::istream &in, const xmlbeansxx::XmlOptions &options = xmlbeansxx::XmlOptions::New());");
            out.cpp.println(className(st)
                         + " "
                         + className(st)
                         + "::Factory::parse(std::istream &in, const xmlbeansxx::XmlOptions &options) {");
            if (st.isDocumentType()) {
                out.cpp.println(className(st) + " doc(" + genNewXmlObject(st) + ");");
                out.cpp.println("xmlbeansxx::XmlParser p(xmlbeansxx::XmlParser::create(options)); p->parse(in, doc); return doc;");
            } else {
                out.cpp.println("xmlbeansxx::definitions::XmlFragmentDocument"
                             + " doc(xmlbeansxx::definitions::XmlFragmentDocument::Factory::newInstance());");
                out.cpp.println("xmlbeansxx::XmlParser p(xmlbeansxx::XmlParser::create(options)); p->parse(in, doc); return "
                             + genCast(className(st),
                                               "doc->getXmlFragment()->getElement()")
                             + ";");
            }
            out.cpp.println("}");
	
            out.h.println("static "
                         + className(st)
                         + " parse(const xmlbeansxx::String &str, const xmlbeansxx::XmlOptions &options = xmlbeansxx::XmlOptions::New());");
            out.cpp.println(className(st)
                         + " "
                         + className(st)
                         + "::Factory::parse(const xmlbeansxx::String &str, const xmlbeansxx::XmlOptions &options) { ");
            out.cpp.println("std::istringstream in(str); return parse(in,options);");
            out.cpp.println("}");
            
            {
                //Custom code insertion
                String s=in.getHGap("class "+fullClassName(st)+"::Factory");
                if (s!=null) {
                    out.h.println("  "+CUSTOM_CODE_STR);
                    out.h.print(s);
                    out.h.println("  "+CUSTOM_CODE_END_STR);
                }
    
                s=in.getCppGap("class "+fullClassName(st)+"::Factory");
                if (s!=null) {
                    out.cpp.println(CUSTOM_CODE_STR);
                    out.cpp.print(s);
                    out.cpp.println(CUSTOM_CODE_END_STR);
                }
            }
			
            //~Factory
            out.h.println("};");
        }
        out.h.println("END_CLASS()");
        out.hImpl.println("END_CLASS()");
        currentType = null;

    }

    /** Prints out xsd dependencies */
    void depend(PrintStream out) {
        final Set files = new HashSet();
        traverseAllTypes(new TypeTraversor() {
                public void fn(SchemaType st) {
                    String n = st.getSourceName();
                    /*
                     * LOG.info("source:"+n); File f; try { f = new File(new
                     * URI(n)); } catch (URISyntaxException e) {
                     * e.printStackTrace(); throw new RuntimeException(); }
                     * LOG.info("file:"+f.getName());
                     */
                    files.add(n.substring(n.lastIndexOf('/') + 1));
                }
            });
        Iterator it = files.iterator();
        while (it.hasNext()) {
            out.print(it.next().toString() + ' ');
        }
    }

    void printDeclarations(SchemaType st) {
        //if (!isInner(st)) {
        String name = className(st);
        out.enterNamespace(nsClassName(st));
        out.h.println("class " + name + "_I;");
        out.h.println("class " + name + ";");
        //out.h.println("typedef boost::shared_ptr<" + name + "_I> " + name + ";");
        /*
          out.h.println("DECLARE_PTR(" + name + "," + name + "Ptr,const" + name
          + "Ptr)");
          out.cpp.println("DECLARE_PTR_CODE(" + name + "," + name + "Ptr,const"
          + name + "Ptr)");
        */
        /*
         * String p=name+"Ptr"; out.h.println("struct "+p+": boost::shared_ptr
         * <"+name+"> {"); out.h.println(" "+p+"();");
         * out.cpp.println(p+"::"+p+"() {}");
         * 
         * out.h.println(" "+p+"(const "+p+"& b);");
         * out.cpp.println(p+"::"+p+"(const "+p+"& b): boost::shared_ptr
         * <"+name+">(b) {}");
         * 
         * out.h.println(" "+p+"("+name+" *b);");
         * out.cpp.println(p+"::"+p+"("+name+" *b): boost::shared_ptr
         * <"+name+">(b) {}"); out.h.println("};");
         */
        out.leaveNamespace();
        //}
    }

    ArrayList allTypes = null;

    void calculateAllTypes(SchemaTypeSystem typeSystem) {
        HashMap ts = new HashMap();

        {
            /*
             * Standard treewalk that visits every SchemaType in
             * the SchemaTypeSystem once, including nested definitions.
             */
            List all2 = new ArrayList();
            all2.addAll(Arrays.asList(typeSystem.documentTypes()));
            all2.addAll(Arrays.asList(typeSystem.attributeTypes()));
            all2.addAll(Arrays.asList(typeSystem.globalTypes()));
            for (int i = 0; i < all2.size(); i++) {
                SchemaType sType = (SchemaType) all2.get(i);
                //System.out.prinlnt("Visiting " + sType.toString());
                all2.addAll(Arrays.asList(sType.getAnonymousTypes()));
            }

            for (int i = 0; i < all2.size(); i++) {
                SchemaType t = (SchemaType) all2.get(i);
                if (getFullJavaName(t) != null) {
                    ts.put(fullClassName(t), t);
                }
            }
        }

        // the Graph class allows to print all the types in correct order
        Iterator it;
        Graph g = new Graph();
        it = ts.keySet().iterator();
        while (it.hasNext()) {
            g.addNode(it.next());
        }

        it = ts.keySet().iterator();
        while (it.hasNext()) {
            String s = (String) it.next();
            SchemaType t = (SchemaType) ts.get(s);

            /*
             * //zaleznosc 1 if (isInner(t)) {
             * g.addBranch(s,getOuterClassName(t)); }
             */

            //zaleznosc od klasy bazowej
            SchemaType bt = getSuperType(t);
            if (bt == null)
                continue;
            String bn = fullClassName(bt);
            if (ts.get(bn) != null) {
                g.addBranch(s, bn);
            }

            /*
             * SchemaProperty[] properties=t.getProperties(); for(int i=0;i
             * <properties.length;i++) { SchemaProperty p=properties[i];
             * SchemaType t2=skipNullTypes(p.getType()); if (isInner(t2)) {
             * g.addBranch(s,getOuterClassName(t2)); } }
             */
        }
        ArrayList kolejnosc = g.topologicalSort();
        if (kolejnosc == null) {
            LOG.info("Found cycle in class dependencies !");
            Utils.check(false);
        }

        allTypes = new ArrayList();
        it = kolejnosc.iterator();
        while (it.hasNext()) {
            allTypes.add(ts.get(it.next()));
        }
    }

    private boolean isInGivenSchema(SchemaType st) {
        if (generatingBuiltin)
            return true; 
        else
            return baseFileName(st.getSourceName()).equals(currentName);
    }

    /**
     * Traversing types from given schema file only.
     */
    public void traverseTypes(TypeTraversor tt) {
        for (int i = 0; i < allTypes.size(); i++) {
            SchemaType st = (SchemaType) allTypes.get(i);
            if (isInGivenSchema(st)) {
                tt.fn(st);
            }
        }
    }

    /**
     * Traversing all types (including these from included schemas)
     */
    public void traverseAllTypes(TypeTraversor tt) {
        for (int i = 0; i < allTypes.size(); i++) {
            tt.fn((SchemaType) allTypes.get(i));
        }
    }

    /**
     * Traversing all types from given schema file and its dependency types
     * (property types and base class). Excludes builtin types.
     */
    void traverseTypesWithDeps(TypeTraversor tt) {
        Set were = new HashSet();

        for (int i = 0; i < allTypes.size(); i++) {
            SchemaType st = (SchemaType) allTypes.get(i);
            if (isInGivenSchema(st)) {
                List l = typeDeps(st);
                l.add(st);
                Iterator it = l.iterator();
                while (it.hasNext()) {
                    SchemaType t = (SchemaType) it.next();
                    if (!t.isBuiltinType() || generatingBuiltin) {
                        String n = fullClassName(t);
                        if (!were.contains(n)) {
                            were.add(n);
                            tt.fn(t);
                        }
                    }
                }
            }
        }
    }

    private void namespaceLink(SchemaType t) {
        if (t.getName() != null) {
            nsLinks.getVarName(t.getName().getNamespaceURI());
        }
        if (t.getDocumentElementName() != null) {
            nsLinks.getVarName(t.getDocumentElementName().getNamespaceURI());
        }
    }

    private List typeDeps(SchemaType st) {
        List l = new ArrayList();

        SchemaProperty[] props;
        props = st.getProperties();
        for (int i = 0; i < props.length; i++) {
            l.add(props[i].getType());
        }

        SchemaType superType=getSuperType(st);
        if (superType!=null) l.add(superType);
        return l;
    }

    private void calculateIncludes(SchemaType st) {
        {
            List l = new ArrayList();
            l.add(getSuperType(st));

            //calculate header file names
            Iterator it = l.iterator();
            while (it.hasNext()) {
                SchemaType t = (SchemaType) it.next();

                if (t != null && !t.isBuiltinType()) {
                    String n = baseFileName(t.getSourceName());
                    if (!n.equals(currentName)) {
                        includes.add(n);
                    }
                }
            }
        }

        {
            List l = typeDeps(st);
            //calculate cpp file names
            Iterator it = l.iterator();
            while (it.hasNext()) {
                SchemaType t = (SchemaType) it.next();

                if (t != null && !t.isBuiltinType()) {
                    String n = baseFileName(t.getSourceName());
                    if (!n.equals(currentName)) {
                        includes.addCpp(n);
                    }
                }
            }
        }
    }

    /**
     * generates code of type system given in constructor
     */
    public void printCode() throws Exception {
        {
            String m = System.getProperty("methods");
            if (m != null) {
                if (m.equals("gentor")) {
                    genProperties = Properties.GENTOR;
                } else if (m.equals("human")) {
                    genProperties = Properties.HUMAN;
                } else {
                    LOG.error("Invalid value for 'methods' property (" + m
                              + ") ");
                    throw new RuntimeException();
                }
                LOG.info("Generating methods for: " + m);
            }
        }
		
        in = new Input(currentName);
        out = new Output(currentName);
        includes = new Includes();

        {
            // it's only for informative purposes

            final int[] count = new int[2];
            count[0] = 0; // simple types
            count[1] = 0; // other

            traverseTypes(new TypeTraversor() {
                    public void fn(SchemaType t) {
                        t = skipNullTypes(t);
                        if (t.isSimpleType()) {
                            count[0]++;
                        } else {
                            count[1]++;
                        }
                    }
                });

            LOG.info("SimpleTypes:" + count[0] + " ComplexTypes:" + count[1]);
        }

        traverseTypes(new TypeTraversor() {
                public void fn(SchemaType t) {
                    calculateIncludes(t);
                }
            });

        includes.generate();
        //nsLinks.printDefinitions(out.cpp);
		
        {
            //print preamble from .xh and .xcpp
            String s = in.getHGap("preamble");
            if (s != null) {
                out.h.println(CUSTOM_CODE_STR);
                out.h.print(s);
                out.h.println(CUSTOM_CODE_END_STR);
            }

            s = in.getCppGap("preamble");
            if (s != null) {
                out.cpp.println(CUSTOM_CODE_STR);
                out.cpp.print(s);
                out.cpp.println(CUSTOM_CODE_END_STR);
            }
        }

        traverseTypesWithDeps(new TypeTraversor() {
                public void fn(SchemaType t) {
                    namespaceLink(t);
                }
            });
        out.splitCpp();

        traverseTypesWithDeps(new TypeTraversor() {
                public void fn(SchemaType t) {
                    printDeclarations(t);
                }
            });
        
        {
            //Static Init / Destruct
            
            out.enterNamespace(" ");
            out.cpp.println("class MyExistence_I: public xmlbeansxx::Existence_I {");
            out.cpp.println("std::vector<xmlbeansxx::Existence> dependencies;");
            out.cpp.println("public:");
            out.cpp.println("MyExistence_I() {");
            out.cpp.println("dependencies.push_back(xmlbeansxx::XmlBeans::staticInit());");
            nsLinks.printInit(out.cpp);
            traverseTypes(new TypeTraversor() {
                    public void fn(SchemaType st) {
                        out.cpp.println("dependencies.push_back(" + fullClassName(st) + "::staticInit());");
                    }
                });
            out.cpp.println("}");
            /*
            out.cpp.println("~MyExistence() {");
            traverseTypes(new TypeTraversor() {
                    public void fn(SchemaType st) {
                    }
                });
            out.cpp.println("}");
            */
            out.cpp.println("};");
            out.cpp.println("xmlbeansxx::Existence staticInit() {");
            out.cpp.println("static xmlbeansxx::Existence ptr(new MyExistence_I());");
            out.cpp.println("return ptr;");
            out.cpp.println("}");
            out.cpp.println("xmlbeansxx::Existence existenceVar = staticInit();");
            out.leaveNamespace();
        }

        traverseTypes(new TypeTraversor() {
                public void fn(SchemaType t) {
                    out.enterNamespace(nsClassName(t));
                    printType(t);
                    out.leaveNamespace();
                }
            });

        
        out.enterNamespace("");

        {
            //print ending from .xh and .xcpp
            String s = in.getHGap("ending");
            if (s != null) {
                out.h.println(CUSTOM_CODE_STR);
                out.h.print(s);
                out.h.println(CUSTOM_CODE_END_STR);
            }

            s = in.getCppGap("ending");
            if (s != null) {
                out.cpp.println(CUSTOM_CODE_STR);
                out.cpp.print(s);
                out.cpp.println(CUSTOM_CODE_END_STR);
            }
        }
        out.close();
    }

    public static SchemaTypeSystem loadTS(String name, List<String> xsdFileNames) {
        ArrayList xsds = new ArrayList();
        for (String xsd : xsdFileNames)
            xsds.add(new File(xsd));
        //xsds.add(new File("b.xsd"));
        File[] xsdstab = new File[xsds.size()];
        for (int i = 0; i < xsds.size(); i++)
            xsdstab[i] = (File) xsds.get(i);
        return TSLoader.loadTypeSystem(name, xsdstab, new File("."));
    }

    /*
     * public static SchemaTypeSystem mergeTypeSystems(SchemaTypeSystem
     * a,SchemaTypeSystem b) { SchemaTypeLoader[] list=new SchemaTypeLoader[2];
     * list[0]=a; list[1]=b; SchemaTypeLoader
     * union=XmlBeans.typeLoaderUnion(list); XmlBeans.c }
     */

    public static void usage() {
        System.out.println("Arguments: [-Dout-dir=<directory>] [ file1.xsd file2.xsd ...]");
    }

    public static String paramValue(String paramName, String param) {
        if (paramName.length() > param.length()) {
            return null;
        }
        if (param.substring(0, paramName.length()).equals(paramName)) {
            return param.substring(paramName.length());
        }
        return null;
    }

    public static void main(String[] args) throws Exception {
        outDir = System.getProperty("out-dir");
        if (outDir == null) outDir = ".";
                
        if (System.getProperty("genbuiltin") != null) {
            LOG.info("Generating builtin types");
            //currentName="XmlTypesGen";
            SchemaTypeSystem ts = XmlBeans.getBuiltinTypeSystem();
            ClassGen cg = new ClassGen(ts, "XmlTypesGen");
            cg.printCode();
            LOG.info("Done");
        } else {
            if (args.length == 0) {
                usage();
            } else {
                List<String> xsds = new ArrayList<String>();
                for (int i = 0; i < args.length; i++) {
                    xsds.add(args[i]);
                }
                
                SchemaTypeSystem ts = loadTS(baseFileName(args[0]), xsds);

                for (String xsd : xsds) {
                    String name = baseFileName(xsd);
                    LOG.info("Processing " + xsd);
                    ClassGen cg = new ClassGen(ts, name); // sets global currentName to name
                    if (System.getProperty("depend") != null) {
                        LOG.info("generating dependencies");
                        cg.depend(System.out);
                    } else {
                        LOG.info("generating code");
                        cg.printCode();
                        {
                            // write names of xsd file dependencies to given file
                            String inc_name = System.getProperty("includes");
                            if (inc_name != null) {
                                PrintStream out = new PrintStream(new FileOutputStream(inc_name));
                                cg.depend(out);
                                out.close();
                            }
                        }
                    }
                    LOG.info("Done");
                }
            }
        }
    }

    private boolean generatingBuiltin; //am I generating builtin types

    private Includes includes;

    private String currentName; //name of file beeing compiled
    
    
    //Taken from org.apache.xmlbeans.impl.schema.SchemaTypeCodePrinter
    static String getAtomicRestrictionType(SchemaType sType) {
        SchemaType pType = sType.getPrimitiveType();
        switch (pType.getBuiltinTypeCode())
        {
            case SchemaType.BTC_ANY_SIMPLE:
                return "org.apache.xmlbeans.impl.values.XmlAnySimpleTypeImpl";
            case SchemaType.BTC_BOOLEAN:
                return "org.apache.xmlbeans.impl.values.JavaBooleanHolderEx";
            case SchemaType.BTC_BASE_64_BINARY:
                return "org.apache.xmlbeans.impl.values.JavaBase64HolderEx";
            case SchemaType.BTC_HEX_BINARY:
                return "org.apache.xmlbeans.impl.values.JavaHexBinaryHolderEx";
            case SchemaType.BTC_ANY_URI:
                return "org.apache.xmlbeans.impl.values.JavaUriHolderEx";
            case SchemaType.BTC_QNAME:
                return "org.apache.xmlbeans.impl.values.JavaQNameHolderEx";
            case SchemaType.BTC_NOTATION:
                return "org.apache.xmlbeans.impl.values.JavaNotationHolderEx";
            case SchemaType.BTC_FLOAT:
                return "org.apache.xmlbeans.impl.values.JavaFloatHolderEx";
            case SchemaType.BTC_DOUBLE:
                return "org.apache.xmlbeans.impl.values.JavaDoubleHolderEx";
            case SchemaType.BTC_DECIMAL:
                switch (sType.getDecimalSize())
                {
                    default:
                        assert(false);
                    case SchemaType.SIZE_BIG_DECIMAL:
                        return "org.apache.xmlbeans.impl.values.JavaDecimalHolderEx";
                    case SchemaType.SIZE_BIG_INTEGER:
                        return "org.apache.xmlbeans.impl.values.JavaIntegerHolderEx";
                    case SchemaType.SIZE_LONG:
                        return "org.apache.xmlbeans.impl.values.JavaLongHolderEx";
                    case SchemaType.SIZE_INT:
                    case SchemaType.SIZE_SHORT:
                    case SchemaType.SIZE_BYTE:
                        return "org.apache.xmlbeans.impl.values.JavaIntHolderEx";
                }
            case SchemaType.BTC_STRING:
                if (sType.hasStringEnumValues())
                    return "org.apache.xmlbeans.impl.values.JavaStringEnumerationHolderEx";
                else
                    return "org.apache.xmlbeans.impl.values.JavaStringHolderEx";

            case SchemaType.BTC_DATE_TIME:
            case SchemaType.BTC_TIME:
            case SchemaType.BTC_DATE:
            case SchemaType.BTC_G_YEAR_MONTH:
            case SchemaType.BTC_G_YEAR:
            case SchemaType.BTC_G_MONTH_DAY:
            case SchemaType.BTC_G_DAY:
            case SchemaType.BTC_G_MONTH:
                return "org.apache.xmlbeans.impl.values.JavaGDateHolderEx";

            case SchemaType.BTC_DURATION:
                return "org.apache.xmlbeans.impl.values.JavaGDurationHolderEx";
            default:
                assert(false) : "unrecognized primitive type";
                return null;
        }
    }
    
    private static SchemaType findBaseType(SchemaType sType) {
        while (sType.getFullJavaName() == null)
            sType = sType.getBaseType();
            return sType;
    }   
   
    private static String getBaseCXXClass(SchemaType sType) {
        if (!sType.isBuiltinType()) sType = sType.getBaseType();
        SchemaType baseType = findBaseType(sType);

        switch (sType.getSimpleVariety())
        {
            case SchemaType.NOT_SIMPLE:
                // non-simple-content: inherit from base type impl
                if (!XmlObject.type.equals(baseType))
                    return fullClassImplName(baseType);
                else 
                    return javaImplToCXX("org.apache.xmlbeans.impl.values.XmlComplexContentImpl");

            case SchemaType.ATOMIC:
                return javaImplToCXX(getAtomicRestrictionType(sType));

            case SchemaType.LIST:
                return javaImplToCXX("org.apache.xmlbeans.impl.values.XmlListImpl");

            case SchemaType.UNION:
                return javaImplToCXX("org.apache.xmlbeans.impl.values.XmlUnionImpl");

            default:
                LOG.error(sType.getSimpleVariety());
                throw new IllegalStateException();
        }
    }
}

				
