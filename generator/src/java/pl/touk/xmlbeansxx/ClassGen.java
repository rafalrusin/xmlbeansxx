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

import org.apache.xmlbeans.*;
import org.apache.commons.logging.*;

public class ClassGen {
    private static final String CUSTOM_CODE_STR="//-------------Custom Code---------------";
    private static final String CUSTOM_CODE_END_STR="//-------------~Custom Code---------------";
    private static Log log = LogFactory.getLog(ClassGen.class);

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
            return "xmlbeansxx::globalTypeSystem()->addNamespace("+genString(namespace)+")";
        }
		
        public void printDefinitions(PrintWriter wt) {
            Iterator it = dict.keySet().iterator();
            wt.println("namespace {");
            while (it.hasNext()) {
                String k = (String) it.next();
                String v = (String) dict.get(k);
                wt.println("  const char *" + v + "=" + genString(k) + ";");
            }
            wt.println("}");
        }
		
        public void printDeclarations(PrintWriter wt) {
            Iterator it = dict.keySet().iterator();
            while (it.hasNext()) {
                String k = (String) it.next();
                String v = (String) dict.get(k);
                wt.println("  "+nsFromTS(k)+";");
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
            out.cpp.println("#include <xmlbeansxx/TextUtils.h>");
            out.h.println("#include <vector>");
            out.h.println("#include <string>");
			
            if (!generatingBuiltin) {
                out.h.println("#include <xmlbeansxx/xmlbeansxx.h>");
            }
            //if (set.isEmpty()) {
            //} else {
            Iterator it = set.iterator();
            while (it.hasNext()) {
                String n = (String) it.next();
                out.h.println("#include \"" + cppHeaderName(n) + "\"");
            }
            //}

            addCpp(currentName);
            it = cppSet.iterator();
            while (it.hasNext()) {
                String n = (String) it.next();
                if (!set.contains(n)) {
                    out.cpp.println("#include \"" + cppHeaderName(n) + "\"");
                }
            }
        }
    }

    public static String cppHeaderName(String name) {
        return name + ".h";
    }

    public static String cppName(String name) {
        return name + ".cpp";
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
                return "mpz_class";

            case SchemaProperty.JAVA_BIG_DECIMAL:
            	return "mpf_class";
            case SchemaProperty.JAVA_BIG_INTEGER:
                return "mpz_class";
            case SchemaProperty.JAVA_STRING:
                return "std::string";
            case SchemaProperty.JAVA_BYTE_ARRAY:
                return "xmlbeansxx::shared_array<unsigned char>";
            case SchemaProperty.JAVA_GDATE:
                return "xmlbeansxx::GDatePtr";
            case SchemaProperty.JAVA_GDURATION:
                //return "org.apache.xmlbeans.GDuration";
            	return "xmlbeansxx::GDurationPtr";
            case SchemaProperty.JAVA_DATE:
                return "xmlbeansxx::DatePtr";
            case SchemaProperty.JAVA_QNAME:
                return "xmlbeansxx::QNamePtr";
            case SchemaProperty.JAVA_LIST:
                return "xmlbeansxx::ListPtr";
            case SchemaProperty.JAVA_CALENDAR:
                return "xmlbeansxx::CalendarPtr";

            case SchemaProperty.JAVA_ENUM:
            	return "std::string";
                //SchemaType sType = sProp.javaBasedOnType().getBaseEnumType();
                //return findJavaType(sType).replace('$', '.') + ".Enum";
                
            case SchemaProperty.JAVA_OBJECT:
            	return "xmlbeansxx::ObjectPtr";
            
            default:
            	return "<NOTHING>";
            }
    }
	
    static String cppTypeForProperty(SchemaProperty sProp) {
        // The type to use is the XML object....
        if (sProp.getJavaTypeCode() == SchemaProperty.XML_OBJECT)
            {
                SchemaType sType = sProp.javaBasedOnType();
                return genPointer(fullClassName(sType));
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
                return "Mpf";
	
            case SchemaProperty.JAVA_BIG_INTEGER:
                return "Mpz";
	
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

    public String constructorFor(String cppType) {
    	if (cppType.equals("unsigned char")) return "0";
    	else return cppType + "()";
    }
		
    
    //~static methods
    //-----------------------------------------------------------------------------------------------

    public String javaPropertyName(SchemaProperty prop) {
        return schemaToJavaPropertyName(prop.getName().getLocalPart());
    }

    /**
     * Zwraca nazwe klasy w kodzie c++ tego typu
     * 
     * @param attr
     */
    public static String className(SchemaType sType) {
        String s = getFullJavaName(sType);
        s = s.substring(s.lastIndexOf('.') + 1);
        return javaToCpp(s);
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

    public static String genThrowingCast(String to, String fromPtr) {
        //return "boost::dynamic_pointer_cast<" + to + ">("+fromPtr+")";
        return "xmlbeansxx::java_cast<" + to + ">(" + fromPtr + ")";
        //return to+"Ptr(("+to+" *)(("+fromPtr+").get()))";
        //return "boost::static_pointer_cast<" + to + ">(" + fromPtr + ")";
    }

    public static String genThrowingArrayCast(String toElementType, String fromSharedArray) {
        return "xmlbeansxx::java_array_cast<" + toElementType + ">("
            + fromSharedArray + ")";
    }

	
    public static String genStaticArrayCast(String to, String fromSharedArray) {
        return "xmlbeansxx::static_array_cast<" + to + ">("
            + fromSharedArray + ")";
        //return fromArrayPtr;
    }

    /** removing this functionality */
    public static String genDefaultValue(SchemaProperty prop, String var) {
        /*if (prop.hasDefault() == SchemaProperty.CONSISTENTLY && prop.hasFixed() != SchemaProperty.CONSISTENTLY) {
          return genDefaultValue2(prop,var);
          } else*/
        return var;
    }

    /** Generates object even for empty default value */
    public static String genDefaultValue2(SchemaProperty prop, String var) {
        return var;
        /*
          String t=prop.getDefaultText();
          if (t == null) t=""; 
          return "defaultValue(" + var + ","
          + genString(t) + ")";*/
    }

    public static String genDefaultArrayValue(SchemaProperty prop,
                                              String array_var) {
        /*if (prop.hasDefault() == SchemaProperty.CONSISTENTLY && prop.hasFixed() != SchemaProperty.CONSISTENTLY) {
          return "defaultArrayValue(" + array_var + ","
          + genString(prop.getDefaultText()) + ")";
          } else*/
        return array_var;
    }

    public String genConvertSimpleType(SchemaType toType, String what) {
        return "xmlbeansxx::convertSimpleType<"
            + fullClassName(toType) + ">(" + what + ")";
    }

    String genDefaultStringPtr(SchemaProperty prop) {
        /*if (prop.hasDefault() == SchemaProperty.CONSISTENTLY && prop.hasFixed() != SchemaProperty.CONSISTENTLY) {
          return "xmlbeansxx::StringPtr(new std::string("
          + genString(prop.getDefaultText()) + "))";
          } else*/
        return "xmlbeansxx::StringPtr()";
    }

    String genDefaultSingletonStringPtr(SchemaProperty prop) {
        if (!prop.extendsJavaArray() && prop.hasDefault() == SchemaProperty.CONSISTENTLY && prop.hasFixed() != SchemaProperty.CONSISTENTLY) {
            return "xmlbeansxx::StringPtr(new std::string("
                + genString(prop.getDefaultText()) + "))";
        } else
            return "xmlbeansxx::StringPtr()";
    }

    static String cutAssigns(String params) {
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
	
    void genMethod(String h_opts,String resultType,String className,String methodName,String params,String body) {
        out.h.println("  " + h_opts + " " + resultType + " " + methodName + "(" + params + ");");
        out.cpp.println(resultType + " " + className + "::" + methodName + "(" + cutAssigns(params) + ") {\n" + body + "\n}" );
    }
	
    public static String genCreateFn(SchemaType st) {
        return fullClassName(st)+"::Factory::newInstanceXmlObject";
    }
	
    public static String genNewXmlObject(SchemaType st) {
        return fullClassName(st)+"::Factory::newInstance()";
    }
	
    public static String genCreateArrayFn(SchemaType st) {
        return fullClassName(st)+"::Factory::newXmlArrayInstance";
    }
	
    public static String genPropName(SchemaProperty elem) {
        return genString(elem.getName().getLocalPart());
    }
	
    public void genRedirect(String fromMethod, String toMethod) {
        //out.h.println("#define " + fromMethod + " " + toMethod);
    }
			
	
    /** Setters / Getters generator */
    class PropGen {
        SchemaProperty prop;
        SchemaType st;
		
        /** xml type without pointer */
        String btype; 
		
        /** xml type with pointer */
        String type;
        /** xml array type with pointer */
        String vtype;
		
        /** xml type with pointer or not xml type */
        String userType;
        /** xml array type with pointer or not xml array type */
        String vuserType;
		
        /** dgetArray type */
        String dvtype;
        String dvbtype;
		
        String x;
        int method;
		
        public static final int M_X=0,M_NORMAL=1,M_STRING=2;
		
        PropGen(SchemaProperty pr) {
            prop=pr;
            st=prop.getType();
            btype=fullClassName(skipNullTypes(prop.getType()));
            type="" + btype + "Ptr ";

            vtype = "xmlbeansxx::shared_array<" + type + " >";
			
            dvbtype = "xmlbeansxx::XmlArray<"
                + btype + ">";
            dvtype="boost::shared_ptr<" + dvbtype + " >";
        }
		
        /** returns true iff property has a simple holder (eg. int) */ 
        public boolean hasHolder() {
            return prop.getJavaTypeCode()!=SchemaProperty.XML_OBJECT;
        }
		
        private String convertToRet(String what, String toVar) {
            String d = userType + " " + toVar + "=" + what + ";";
            if (method==M_X) {
                return d;
            } else if (method==M_NORMAL) {
                if (hasHolder()) {
                    String t=nextTmpID();
					
                    return 
                        type + " " + t + "=" + what + ";\n"
                        + userType + " " + toVar + "=(" + t + "==NULL?" + constructorFor(userType)
                        + ":" + t + "->get" + genJGetName(prop.getJavaTypeCode()) + "Value());\n";
                } else return d;
            } else throw new IllegalStateException();
        }
		
        private String convertToRetArray(String what,String toVar) {
            String d = vtype + " " + toVar + "=" + what + ";";
            if (method==M_X) {
                return d;
            } else if (method==M_NORMAL) {
                if (hasHolder()) {
                    String t1 = nextTmpID();
                    String len = nextTmpID();
                    return 
                        vtype + " " + t1 + "=" + what + ";\n"
                        + "int " + len + "=" + t1 + ".size();\n" 
                        + vuserType + " " + toVar + "(" + len + ");\n" 
                        + "for(int i=0; i<" + len + "; i++) {\n" 
                        + toVar + "[i]="

                        + "(" + t1 + "[i]==NULL?" + constructorFor(userType)
                        + ":" + t1 + "[i]->get" + genJGetName(prop.getJavaTypeCode()) + "Value());\n"
						
                        + "}\n"
                        ;
                } else {
                    return d;
                }
            } else throw new IllegalStateException();
        }
		
        private String convertFromGiven(String what,String toVar) {
            String def = type + " " + toVar + "=" + what + ";\n";
            if (method==M_X) {
                return def;
            } else if (method==M_NORMAL) {
                if (hasHolder()) {
                    return type + " " + toVar + "(new " + btype + "());\n"
                        + toVar + "->set" + genJGetName(prop.getJavaTypeCode()) + "Value(" + what + ");\n";
                } else {
                    return def;
                }
            } else if (method==M_STRING) {
                return type + " " + toVar + "(new " + btype + "(" + what + "));\n";
            } else throw new IllegalStateException();
        }
		
        private String convertFromGivenArray(String what,String toVar) {
            String def = vtype + " " + toVar + "=" + what + ";\n";
            if (method==M_X) {
                return def;
            } else if (method==M_NORMAL) {
                if (hasHolder()) {
                    String len=nextTmpID();
                    String t=nextTmpID();
                    return
                        "int " + len + "=" + what + ".size();\n"
                        + vtype + " " + toVar + "(" + len + ");\n"
                        + "for(int i=0;i<" + len + ";i++) {\n"
                        + type + " " + t + "(new " + btype + "());\n"
                        + t + "->set" + genJGetName(prop.getJavaTypeCode()) + "Value(" + what + "[i]);\n"
                        + toVar + "[i]=" + t + ";\n"
                        + "}";
                } else {
                    return def;
                }
            } else throw new IllegalStateException();
        }

        /** Accessor generation kind */ 
        public void setGenMethod(int method) {
            this.method=method;
            if (method==M_X) {
                x="x";
				
                userType = type;
                vuserType = "xmlbeansxx::shared_array<" + userType + ">"; 
            } else if (method==M_NORMAL) {
                x="";
				
                userType = cppTypeForProperty(prop);
                vuserType = "xmlbeansxx::shared_array<" + userType + ">"; 
            } else if (method==M_STRING) {
                x = "";
				
                userType = "std::string";
                vuserType = "<NOTHING>";
            }
        }
		
        public void genGet() {
            genMethod("", userType, className(currentType), x + "get" + javaPropertyName(prop), "", 
                      type + " r="
                      + genThrowingCast(btype, "contents.getElem("
                                        + genPropName(prop)
                                        + ")") + ";\n"
                      + convertToRet(genDefaultValue(prop, "r"),"r2") + ";\n"
                      + "return r2;" 
                      );
        }

        public void genGetAttr() {
            genMethod("", userType, 
                      className(currentType),
                      x + "get" + javaPropertyName(prop) + "Attr",
                      "",
                      "xmlbeansxx::StringPtr c=contents.getAttr("
                      + genPropName(prop) + ");\n"
                      + "if (c==NULL) {"
                      + convertToRet(genDefaultValue(prop, type + "()"),"r3")
                      + "return r3;\n"
                      + "} else {\n"
                      + type + " v(new " + btype + "());\n"
                      + "v->setSimpleContent(*c);\n"
                      + convertToRet(genDefaultValue(prop, "v"),"r2") + ";\n"
                      + "return r2;\n"
                      + "}"
                      );
        }
		
        public void genCGet() {
            genMethod("", userType, className(currentType), x + "cget" + javaPropertyName(prop),
                      "xmlbeansxx::ObjectCreatorFn createFn=NULL",
                      convertToRet(
                                   genDefaultValue(prop, genThrowingCast(btype,
                                                                         "contents.cgetElemAt("
                                                                         + genPropName(prop)
                                                                         + ",0,createFn,this)"))
                                   ,"r2")+ ";"
                      + "return r2;"
                      );
        }

        public void genGetArray() {
            //get_array
            genMethod("", vuserType, className(currentType), 
                      x + "get" + javaPropertyName(prop) + "Array",
                      "",
                      vtype + " a(" + genThrowingArrayCast(btype, 
                                                           "contents.getElemArray("
                                                           + genPropName(prop)
                                                           + "))" ) 
                      + ";\n"
                      + convertToRetArray("a","b")
                      + "return b;"
                      );
        }

        public void genDGetArray() {
            //dget_array
            genMethod("", dvtype, className(currentType), 
                      "dget" + javaPropertyName(prop) + "Array",
                      "",
                      dvtype
                      + " a(new " + dvbtype + "(contents.getElemArray("
                      + genPropName(prop)
                      + ")));\n"
                      + "return a;"
                      );
        }
		
        public void genGetArrayAt() {
            //getArrayAt
            genMethod("", userType, className(currentType), 
                      x + "get" + javaPropertyName(prop) + "Array",
                      "int index", 
                      convertToRet( genDefaultValue(prop, 
                                                    genThrowingCast(btype,
                                                                    "contents.getElemAt("
                                                                    + genPropName(prop)
                                                                    + ",index)"
                                                                    )
                                                    ),"r2")
                      + "return r2;\n"
                      + ";");
        }
		
        public void genCGetArrayAt() {
            //cgetAt - for gentor xpath
            genMethod("", userType, className(currentType),
                      x + "cget"	+ javaPropertyName(prop) + "Array",
                      "int index,xmlbeansxx::ObjectCreatorFn createFn=NULL",
                      convertToRet(
                                   genDefaultValue(prop, 
                                                   genThrowingCast(btype,
                                                                   "contents.cgetElemAt(" + genPropName(prop) 
                                                                   + ",index,createFn,this)")),"r2") + ";"
                      + "return r2;\n"
                      );
        }

        public void genSet() {
            //set
            genMethod("", "void", className(currentType), 
                      x + "set" + javaPropertyName(prop),
                      userType + " value",
                      convertFromGiven("value","v")
                      + "contents.setElem("	+ genPropName(prop) + ",v);");
        }
	
        public void genSetArray() {
            //set_array
            genMethod("", "void",
                      className(currentType),
                      x + "set" + javaPropertyName(prop)+ "Array",
                      vuserType + " values",
                      convertFromGivenArray("values","v")
                      + "contents.setElemArray("
                      + genPropName(prop)
                      + "," + genStaticArrayCast("xmlbeansxx::XmlObject","v") + ");");
        }
		
        public void genDSetArray() {
            //set_array
            genMethod("", "void",
                      className(currentType),
                      "dset" + javaPropertyName(prop)+ "Array",
                      dvtype + " values",
                      "contents.setElemArray("
                      + genPropName(prop)
                      + ",values->getArray());");
        }
		
        public void genAdd() {
            //append
            genMethod("", "void", 
                      className(currentType),
                      x + "add" + javaPropertyName(prop),
                      userType + " value",
                      convertFromGiven("value","v")
                      + "contents.appendElem("
                      + genPropName(prop) + ",v);"
                      );
        }

        public void genSetArrayAt() {
            //setArray
            genMethod("", "void", className(currentType), 
                      x + "set" + javaPropertyName(prop) + "Array",
                      "int index, " + userType + " value",
                      convertFromGiven("value","v")
                      + "contents.setElemAt("
                      + genPropName(prop)
                      + ",index,v);");
        }
		
		
		
        public void genSetAttr() {
            genMethod("", "void", 
                      className(currentType),
                      x + "set" + javaPropertyName(prop) + "Attr",
                      userType + " value",
                      convertFromGiven("value","v")
                      + "xmlbeansxx::StringPtr c;\n"
                      + "if (v!=NULL) c=xmlbeansxx::StringPtr(new std::string(v->getSimpleContent()));\n"
                      + "contents.setAttr("
                      + genPropName(prop) + ",c);"
                      );
        }
    }

	
    void genAttrCode(SchemaProperty attr) {
        PropGen pg=new PropGen(attr);

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
                genMethod("","bool",
                          className(currentType),"isSet" + javaPropertyName(attr) + "Attr", "", 
                          "return contents.getAttr(" + genPropName(attr) + ")!=NULL;"
                          );
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
					
                    if (!cppTypeForProperty(attr).equals("std::string")) {
                        pg.setGenMethod(PropGen.M_STRING);
                        pg.genSetAttr();
                    }
                }

                if (haveToGen(Properties.UNSET_ATTR)) {
                    genMethod("","void",className(currentType),"unset"
                              + javaPropertyName(attr) + "Attr","",
                              "contents.setAttr(" + genPropName(attr) + ",xmlbeansxx::StringPtr());"
                              );
                }

            } else {
                Utils.check(false);
            }
        }
    }

    void genElemCode(SchemaProperty elem) {
        PropGen pg=new PropGen(elem);
		
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
                    genMethod("","bool",
                              className(currentType),"isSet" + javaPropertyName(elem), "", 
                              "return contents.getElem(" + genPropName(elem) + ")!=NULL;"
                              );
                }

            } else {
                //Multi Elem
				
                if (haveToGen(Properties.SIZEOF)) {
                    genMethod("","int",className(currentType),"sizeOf"+javaPropertyName(elem),"",
                              "return contents.countElems("
                              + genPropName(elem)
                              + ");"
                              );
                }
				
                if (haveToGen(Properties.GET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genGetArray();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genGetArray();
                    }
                }

                if (haveToGen(Properties.DGET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genDGetArray();
                }

                if (haveToGen(Properties.GET_AT)) {
                    genRedirect("get" + javaPropertyName(elem) + "At", "get" + javaPropertyName(elem) + "Array"); 
                }
				
                if (haveToGen(Properties.GET_ARRAY_AT)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genGetArrayAt();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genGetArrayAt();
                    }
                }

                if (haveToGen(Properties.CGET_AT)) {
                    genRedirect("cget" + javaPropertyName(elem) + "At", "cget" + javaPropertyName(elem) + "Array"); 
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
						
                        if (!cppTypeForProperty(elem).equals("std::string")) {
                            pg.setGenMethod(PropGen.M_STRING);
                            pg.genSet();
                        }
                    }
                }

                if (haveToGen(Properties.UNSET)) {
                    out.h.println("  void unset" + javaPropertyName(elem)
                                  + "();");
                    out.cpp.println("void " + className(currentType)
                                    + "::unset" + javaPropertyName(elem) + "() {");
                    out.cpp.println("  contents.removeElems(" + genPropName(elem) + ");");
                    out.cpp.println("}");
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

                if (haveToGen(Properties.DSET_ARRAY)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genDSetArray();
                }

                if (haveToGen(Properties.ADD)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genAdd();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genAdd();

                        if (!cppTypeForProperty(elem).equals("std::string")) {
                            pg.setGenMethod(PropGen.M_STRING);
                            pg.genAdd();
                        }
                    }
                }
				
                if (haveToGen(Properties.ADD_NEW)) {
                    //addNew
                    genMethod("",pg.type,className(currentType),"addNew" + javaPropertyName(elem),"",
                              pg.type + " e=" + genNewXmlObject(pg.st) + ";\n"
                              + "contents.appendElem(" + genPropName(elem) + "," 
                              + "e" + ");\n"
                              + "return e;\n"
                              );
                }

                if (haveToGen(Properties.UNSET_ARRAY)) {
                    //unset_array
                    genMethod("", "void", 
                              className(currentType),
                              "unset" + javaPropertyName(elem)+ "Array",
                              "",
                              "contents.removeElems("
                              + genPropName(elem) + ");"
                              );
                }

                genRedirect("set" + javaPropertyName(elem) + "At", "set" + javaPropertyName(elem) + "Array");
				
                if (haveToGen(Properties.SET_ARRAY_AT)) {
                    pg.setGenMethod(PropGen.M_NORMAL);
                    pg.genSetArrayAt();
					
                    if (pg.hasHolder()) {
                        pg.setGenMethod(PropGen.M_X);
                        pg.genSetArrayAt();

                        if (!cppTypeForProperty(elem).equals("std::string")) {
                            pg.setGenMethod(PropGen.M_STRING);
                            pg.genSetArrayAt();
                        }
                    }
                }
								
                genRedirect("unset" + javaPropertyName(elem) + "At", "remove" + javaPropertyName(elem));
				
                if (haveToGen(Properties.REMOVE)) {
                    genMethod("", "void", className(currentType), "remove" + javaPropertyName(elem),
                              "int index",
                              "contents.removeElemAt("
                              + genPropName(elem)
                              + ",index);");
                }
            }

        }

    }

    //-----------------------------------------------------------------------
    SchemaType currentType = null;

    /*
      void elemCreator(SchemaProperty elem) {
      out.cpp.println("  if (elementName==\"" + genElemName(elem)
      + "\") return xmlbeansxx::XmlObjectPtr(new "
      + fullClassName(elem.getType()) + "());");
      }

      void elemsCreator(SchemaType st) {
      out.h
      .println("  virtual xmlbeansxx::XmlObjectPtr createSubObject(const std::string elementName);");
      out.cpp.println("xmlbeansxx::XmlObjectPtr " + className(st)
      + "::createSubObject(const std::string elementName) {");
      SchemaProperty[] elems = st.getElementProperties();
      for (int i = 0; i < elems.length; i++) {
      elemCreator(elems[i]);
      }
      out.cpp.println("  return xmlbeansxx::XmlObjectPtr(new xmlbeansxx::XmlObject());");
		
      //out.cpp
      //		.println("  throw xmlbeansxx::XmlException(std::string(\"SubElement \")+elementName+\" not supported by "
      //				+ fullClassName(currentType) + "\");");
      out.cpp.println("}");
      }
    */

    public static String genPointer(String name) {
        return name + "Ptr";
    }

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

    private void genInitSchemaType(SchemaType st) {
        out.h.println("  private:");

        out.h.println("  static xmlbeansxx::SchemaType initSchemaType();");
        out.cpp.println("xmlbeansxx::SchemaType "
                        + className(st) + "::initSchemaType() {");
        out.cpp.println("  xmlbeansxx::SchemaType st(typeid("+fullClassName(st)+"));");

        {
            out.cpp.println("  st.createFn = "+genCreateFn(st)+";");
            out.cpp.println("  st.createArrayFn = "+genCreateArrayFn(st)+";");
			
            out.cpp.println("  st.whitespaceRule = "+genWsRule(st.getWhiteSpaceRule())+";");
            out.cpp.println("  st.className = "+genString(fullClassName(st))+";");
			
            out.cpp.println("  st.contentType=" + genContentType(st.getContentType()) + ";");

            if (st.getName() == null) {
                out.cpp.println("  st.xsdTypeName = \"\";");
                out.cpp.println("  st.xsdNamespaceID = -1;");
            } else {
                out.cpp.println("  st.xsdTypeName = "+genString(st.getName().getLocalPart())+";");
                out.cpp.println("  st.xsdNamespaceID = "
                                + nsLinks.getVarName(st.getName().getNamespaceURI())+";");
            }
			
            out.cpp.println("  st.arrayXsdTypeName = \"\";");
            out.cpp.println("  st.arrayXsdNamespaceID = -1;");
			
            {
                // if SchemaType is of xmlbeansxx::XmlObject type, then process
                // it's contents. why?
                String r = null;
                if (fullClassName(st).equals("xmlbeansxx::XmlObject"))
                    r = "false";
                else
                    r = "true";
                out.cpp.println("  st.processContents = "+r+";");
            }
        }

        if (st.isSimpleType()) {
            {
                XmlInteger fd=(XmlInteger)st.getFacet(SchemaType.FACET_FRACTION_DIGITS);
                if (fd!=null) {
                    out.cpp.println("  st.fractionDigits="+fd.getBigIntegerValue().toString()+";");
                }
            }
        } else {
            {
                //String Storage
                SchemaProperty[] properties = st.getProperties();
                for (int i = 0; i < properties.length; i++) {
                    SchemaProperty p = properties[i];
                    out.cpp.println("  xmlbeansxx::StoreString::store("
                                    +genString(p.getName().getLocalPart())+");");
                }
            }
            {
                //SubProperties: Elements with order
                SchemaProperty[] properties = st.getElementProperties();
                for (int i = 0; i < properties.length; i++) {
                    SchemaProperty p = properties[i];
                    out.cpp.println("  st.elements[\"" + p.getName().getLocalPart()
                                    + "\"] = xmlbeansxx::SchemaPropertyPtr(new xmlbeansxx::SchemaProperty("
                                    + (i + 1) + ","+genCreateFn(p.getType())
                                    + "()->getSchemaType(), "
                                    + genDefaultSingletonStringPtr(p) + "));");
                }
            }
            {
                //SubProperties: Attributes with order
                SchemaProperty[] properties = st.getAttributeProperties();
                for (int i = 0; i < properties.length; i++) {
                    SchemaProperty p = properties[i];
                    out.cpp.println("  st.attributes[\"" + p.getName().getLocalPart()
                                    + "\"]=xmlbeansxx::SchemaPropertyPtr(new xmlbeansxx::SchemaProperty("
                                    + (i + 1) + ","+genCreateFn(p.getType())
                                    +"()->getSchemaType(), " + genDefaultSingletonStringPtr(p)
                                    + "));");
                }
            }
        }
        out.cpp.println("  return st;");
        out.cpp.println("}");
    }
	
    private static String genNewObjectCode(SchemaType st) {
        return genPointer(fullClassName(st)) + " obj = "
            + genPointer(fullClassName(st)) + "(new "
            + fullClassName(st) + "());\n "
            //+ " obj->contents.insertDefaults(obj->getSchemaType());\n"
            + " return obj;\n";
    }

    private static String parentConstructor(SchemaType st, String params) {
        if (st.getBaseType() != null) 
            return ":" + fullClassName(st.getBaseType()) + "(" + params + ")";
        else return "";
    }
    
    void printType(SchemaType st) {
        {
            if (classNumber > 80) {
                out.splitCpp();
                classNumber = 0;
            }
            classNumber++;
        }

        st = skipNullTypes(st);
        SchemaType bt = getSuperType(st);

        out.enterNamespace(nsClassName(st));
        currentType = st;
        out.h.println("/** This generated class corresponds to xsd type of QName "
                      + st.getName() + ". */");
        out.h.print("class " + className(st));
        if (bt != null)
            out.h.println(": public " + fullClassName(bt) + " {");
        else
            out.h.println("{");

        genInitSchemaType(st);

        //----------------
        out.h.println("  //Constructors");
        out.h.println("  protected:");
        out.h.println("  " + className(st) + "(const xmlbeansxx::Void &v);");
        out.cpp.println(className(st) + "::" + className(st)
                        + "(const xmlbeansxx::Void &v)"
                        + parentConstructor(st, "v") + " { }");

        out.h.println("  public:");
        out.h.println("  " + className(st) + "();");
        out.cpp.println(className(st) + "::" + className(st) + "()" + parentConstructor(st, "xmlbeansxx::Void()") + " { contents.insertDefaults(" + className(st) + "::type()); }");

        if (st.isSimpleType()) {
            {
                out.h.println("  " + className(st) + "(const std::string &value);");
                out.cpp.println(className(st) + "::" + className(st)
                                + "(const std::string &value) { setSimpleContent(value);}");
            }
			
            {
                out.h.println("  "+className(st)+"(const xmlbeansxx::XmlAnySimpleType &v);");
                out.cpp.println(className(st)+"::"+className(st)+"(const xmlbeansxx::XmlAnySimpleType &v) {");
                out.cpp.println("  setSimpleContent(v.getSimpleContent());");  
                out.cpp.println("}");  
            }
			
			
        }
        //----------------

        out.h.println("  virtual const xmlbeansxx::SchemaType *getSchemaType() const;");
        out.cpp.println("const xmlbeansxx::SchemaType *"
                        + className(st) + "::getSchemaType() const {");
        out.cpp.println("return " + className(st) + "::type();");
        out.cpp.println("}");

        out.h.println("  static const xmlbeansxx::SchemaType *type();");
        out.cpp.println("const xmlbeansxx::SchemaType *"
                        + className(st) + "::type() {");
        out.cpp.println("  static xmlbeansxx::SchemaType schemaType=initSchemaType();");
        out.cpp.println("  return &schemaType; ");
        out.cpp.println("}");

        {
            //Factory
            out.h.println("class Factory {");
            out.h.println("public:");
			
            {
                out.h.println("//Empty Object Creation");
                out.h
                    .println("static "
                             + genPointer(fullClassName(st))
                             + " newInstance();");
                out.cpp.println(genPointer(fullClassName(st)) + " "
                                + className(st) + "::Factory::newInstance() {"
                                + genNewObjectCode(st) + " } " );

				
                out.h.println("static "
                              + genPointer("xmlbeansxx::XmlObject")
                              + " newInstanceXmlObject();");
                out.cpp.println(genPointer("xmlbeansxx::XmlObject") + " "
                                + className(st) + "::Factory::newInstanceXmlObject() { " + genNewObjectCode(st) + " }");
            }
            {
                out.h.println("  //Empty Array Creation");
                out.h.println("  static "
                              + genPointer("xmlbeansxx::XmlObject")
                              + " newXmlArrayInstance();");
                out.cpp.println(genPointer("xmlbeansxx::XmlObject") + " "
                                + className(st) + "::Factory::newXmlArrayInstance() { return "
                                + "boost::shared_ptr<xmlbeansxx::XmlArray<" + className(st) + "> >"
                                + "(new xmlbeansxx::XmlArray<" + className(st) + ">()); }");
            }
	
            //-------------
            out.h.println("//Object Creation using parsing");
            out.h
                .println("static "
                         + genPointer(className(st))
                         + " parse(std::istream &in,xmlbeansxx::XmlOptionsPtr options=xmlbeansxx::XmlOptionsPtr(new xmlbeansxx::XmlOptions()));");
            out.cpp
                .println(genPointer(className(st))
                         + " "
                         + className(st)
                         + "::Factory::parse(std::istream &in,xmlbeansxx::XmlOptionsPtr options) {");
            if (st.isDocumentType()) {
                out.cpp.println(genPointer(className(st)) + " doc(new "
                                + className(st) + ");");
                out.cpp
                    .println("xmlbeansxx::XmlParser p(options); p.parse(in,doc.get()); return doc;");
            } else {
                out.cpp
                    .println(genPointer("xmlbeansxx::definitions::XmlFragmentDocument")
                             + " doc(new xmlbeansxx::definitions::XmlFragmentDocument());");
                out.cpp
                    .println("xmlbeansxx::XmlParser p(options); p.parse(in,doc.get()); return "
                             + genThrowingCast(className(st),
                                               "doc->getXmlFragment()->getElement()")
                             + ";");
                //out.cpp.println(" return
                // "+genThrowingCast(className(st),"xmlbeansxx::XmlObject::parse(in,options)")+";");
                //out.cpp.println(" return java_cast<xmlbeansxx::X
            }
            out.cpp.println("}");
	
            out.h
                .println("static "
                         + genPointer(className(st))
                         + " parse(const std::string &str,xmlbeansxx::XmlOptionsPtr options=xmlbeansxx::XmlOptionsPtr(new xmlbeansxx::XmlOptions()));");
            out.cpp
                .println(genPointer(className(st))
                         + " "
                         + className(st)
                         + "::Factory::parse(const std::string &str,xmlbeansxx::XmlOptionsPtr options) { ");
            out.cpp
                .println("std::istringstream in(str); return parse(in,options);");
            out.cpp.println("}");
			
            //~Factory
            out.h.println("};");
        }
        //-------------

        if (st.isDocumentType()) {
            out.h.println("  //Document methods");
            out.h.println("  virtual int documentElementNamespaceID() const;");
            out.cpp.println("  int "
                            + className(st)
                            + "::documentElementNamespaceID() const { return "
                            + nsLinks.getVarName(st.getDocumentElementName()
                                                 .getNamespaceURI()) + "; }");
            out.h
                .println("  virtual void serialize(std::ostream &out,xmlbeansxx::XmlOptionsPtr opts=xmlbeansxx::XmlOptionsPtr(new xmlbeansxx::XmlOptions())) const;");
            out.cpp
                .println("void "
                         + className(st)
                         + "::serialize(std::ostream &out,xmlbeansxx::XmlOptionsPtr opts) const {");
            out.cpp.println("  contents.serializeDocument(out,opts,this);");
            out.cpp.println("}");
        }


        if (!st.isSimpleType()) {
            //Generate methods for complex content
            SchemaProperty[] properties;
            if (st.getContentType() == SchemaType.SIMPLE_CONTENT) {
                properties = st.getProperties();
            } else {
                properties = st.getDerivedProperties();
            }
            out.h.println("  //Attributes:");
            for (int i = 0; i < properties.length; i++)
                if (properties[i].isAttribute())
                    genAttrCode(properties[i]);
            out.h.println("  //Elements:");
            for (int i = 0; i < properties.length; i++)
                if (!properties[i].isAttribute())
                    genElemCode(properties[i]);
            out.h.println("  //--------//");
            //elemsCreator(st);
        } else {
            /*
              out.h.println("  //Holder:");
              out.h.println("  //" + cppTypeForCode(st.getBuiltinTypeCode()) 
              + " get" + genJGetName(st.getBuiltinTypeCode()) + "Value() const;"); 
              out.h.println("  //void" 
              + " set" + genJGetName(st.getBuiltinTypeCode()) + "Value(" 
              + cppTypeForCode(st.getBuiltinTypeCode()) + " v);");
            */ 
        }
		
        {
            //Custom code insertion
            String s=in.getHGap("class "+fullClassName(st));
            if (s!=null) {
                out.h.println("  "+CUSTOM_CODE_STR);
                out.h.print(s);
                out.h.println("  "+CUSTOM_CODE_END_STR);
            }

            s=in.getCppGap("class "+fullClassName(st));
            if (s!=null) {
                out.cpp.println(CUSTOM_CODE_STR);
                out.cpp.print(s);
                out.cpp.println(CUSTOM_CODE_END_STR);
            }
        }

        out.h.println("};");
        currentType = null;
        out.leaveNamespace();

    }

    /** Prints out xsd dependencies */
    void depend(PrintStream out) {
        final Set files = new HashSet();
        traverseAllTypes(new TypeTraversor() {
                public void fn(SchemaType st) {
                    String n = st.getSourceName();
                    /*
                     * log.info("source:"+n); File f; try { f = new File(new
                     * URI(n)); } catch (URISyntaxException e) {
                     * e.printStackTrace(); throw new RuntimeException(); }
                     * log.info("file:"+f.getName());
                     */
                    files.add(n.substring(n.lastIndexOf('/') + 1));
                }
            });
        Iterator it = files.iterator();
        while (it.hasNext()) {
            out.print(it.next().toString() + ' ');
        }
    }

    void registration(SchemaType st) {
        if (st.getName() != null) {
            //rejestrujemy tylko typy nie anonimowe
            out.cpp.println("    xmlbeansxx::globalTypeSystem()->addType("+genCreateFn(st)+"()->getSchemaType());");
        } else {
            //natomiast dla anonimowych wykonujemy statyczna inicjalizacje schemaType
            out.cpp.println("    " + genCreateFn(st) + "()->getSchemaType();");
        }
    }

    void printDeclarations(SchemaType st) {
        //if (!isInner(st)) {
        String name = className(st);
        out.enterNamespace(nsClassName(st));
        out.h.println("class " + name + ";");
        out.h.println("typedef boost::shared_ptr<"+name+"> "+name+"Ptr;");
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
            log.info("Found cycle in class dependencies !");
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
                    log.error("Invalid value for 'methods' property (" + m
                              + ") ");
                    throw new RuntimeException();
                }
                log.info("Generating methods for: " + m);
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

            log.info("SimpleTypes:" + count[0] + " ComplexTypes:" + count[1]);
        }

        traverseTypes(new TypeTraversor() {
                public void fn(SchemaType t) {
                    calculateIncludes(t);
                }
            });

        includes.generate();
		
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

        out.cpp.println("//-----------------------------------------------------------------");
        out.cpp.println("//namespace links");
        traverseTypesWithDeps(new TypeTraversor() {
                public void fn(SchemaType t) {
                    namespaceLink(t);
                }
            });
        //nsLinks.printDefinitions(out.cpp);
        out.splitCpp();

        out.println("//-----------------------------------------------------------------");
        out.println("//class declarations");
        traverseTypesWithDeps(new TypeTraversor() {
                public void fn(SchemaType t) {
                    printDeclarations(t);
                }
            });
        out.println("//-----------------------------------------------------------------");
        out.println("//class definitions");
        traverseTypes(new TypeTraversor() {
                public void fn(SchemaType t) {
                    printType(t);
                }
            });

        out.println("//-----------------------------------------------------------------");
        out.println("//types registration");
        {
            out.enterNamespace(" ");
            out.cpp.println("class TypesExistence {");
            out.cpp.println("  public:");
            out.cpp.println("  TypesExistence() {");
            nsLinks.printDeclarations(out.cpp);
            traverseTypes(new TypeTraversor() {
                    public void fn(SchemaType t) {
                        registration(t);
                    }
                });
            out.cpp.println("  }");
            out.cpp.println("};");
            out.cpp.println("static TypesExistence te;");
            out.leaveNamespace();
        }
        
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

    public static SchemaTypeSystem loadTS(String name, String[] args) {
        ArrayList xsds = new ArrayList();
        for (int i = 0; i < args.length; i++)
            xsds.add(new File(args[i]));
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
        System.out.println("Arguments: [ file1.xsd file2.xsd ...]");
    }

    public static void main(String[] args) throws Exception {
        if (System.getProperty("genbuiltin") != null) {
            log.info("Generating builtin types");
            //currentName="XmlTypesGen";
            SchemaTypeSystem ts = XmlBeans.getBuiltinTypeSystem();
            ClassGen cg = new ClassGen(ts, "XmlTypesGen");
            cg.printCode();
            log.info("Done");
        } else {
            if (args.length == 0) {
                usage();
            } else {
                String[] xsds = args;
                SchemaTypeSystem ts = loadTS(baseFileName(args[0]), xsds);

                for (int i = 0; i < xsds.length; i++) {
                    String name = baseFileName(xsds[i]);
                    log.info("Processing " + xsds[i]);
                    ClassGen cg = new ClassGen(ts, name); // sets global currentName to name
                    if (System.getProperty("depend") != null) {
                        log.info("generating dependencies");
                        cg.depend(System.out);
                    } else {
                        log.info("generating code");
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
                    log.info("Done");
                }
            }
        }
    }

    private boolean generatingBuiltin; //am I generating builtin types

    private Includes includes;

    private String currentName; //name of file beeing compiled
}





/*
  if (!st.isSimpleType()) {
  } else {
  if (haveToGen(Methods.SET_AT)) {
  //setAt
  out.h.println("  void set" + javaPropertyName(elem)
  + "At(int index, " + type + " value);");
  out.cpp.println("void " + className(currentType)
  + "::set" + javaPropertyName(elem)
  + "At(int index, " + type + " value) {");
  out.cpp.println("  contents.setElemAt("
  + genPropName(elem)
  + ",index,value);");
  out.cpp.println("}");
  }

					
  if (haveToGen(Methods.SET_AT) && st.isSimpleType()) {
  //setAt
  out.h.println("  void set" + javaPropertyName(elem)
  + "At(int index, std::string value);");
  out.cpp.println("void " + className(currentType)
  + "::set" + javaPropertyName(elem)
  + "At(int index, std::string value) {");
  out.cpp.println("  contents.setElemAt("
  + genPropName(elem)
  + ",index,"+type+"(new "+btype+"(value)));");
  out.cpp.println("}");
  }
					
  }*/
/*
  if (!st.isSimpleType()) {
  } else {

  if (haveToGen(Methods.SET)) {
  //set
  out.h.println("  void set" + javaPropertyName(elem)
  + "(" + type + " value);");
  out.cpp.println("void " + className(currentType)
  + "::set" + javaPropertyName(elem) + "("
  + type + " value) {");
  out.cpp.println("  contents.setElem("
  + genPropName(elem) + ",value);");
  out.cpp.println("}");
  }
					
  if (haveToGen(Methods.SET) && st.isSimpleType()) {
  out.h.println("  void set" + javaPropertyName(elem)
  + "(std::string value);");
  out.cpp.println("void " + className(currentType)
  + "::set" + javaPropertyName(elem) + "(std::string value) {");
  out.cpp.println("  contents.setElem("
  + genPropName(elem) + ","+type+"(new "+btype+"(value)));");
  out.cpp.println("}");
  }
					
  }*/
/*
  if (haveToGen(Methods.SET_ATTR) && st.isSimpleType()) {
  //setter for std::string
  out.h.println("  void set" + javaPropertyName(attr)
  + "Attr(std::string value);");
  out.cpp.println("void " + className(currentType) + "::set"
  + javaPropertyName(attr) + "Attr(std::string value) {");
  out.cpp.println("  contents.setAttr("
  + genPropName(attr) + ",xmlbeansxx::StringPtr(new std::string("+fullClassName(st)+"(value).getSimpleContent())));");
  out.cpp.println("}");
  }*/
				
