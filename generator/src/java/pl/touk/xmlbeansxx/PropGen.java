package pl.touk.xmlbeansxx;

import org.apache.xmlbeans.SchemaProperty;
import org.apache.xmlbeans.SchemaType;

/** Setters / Getters generator */
public class PropGen {
	Output out;
	SchemaType currentType;
    SchemaProperty prop;
    SchemaType st;
	
    /** xml type */
    String type;
    /** xml array type with pointer */
    String vtype;
	
    /** xml type with pointer or not xml type */
    String userType;
    /** xml array type with pointer or not xml array type */
    String vuserType;
	
    String x;
    int method;
	
    public static final int M_X=0,M_NORMAL=1;
	
    PropGen(SchemaType containerType, SchemaProperty pr, Output out) {
		currentType = containerType;
		this.out = out;
        prop=pr;
        st=prop.getType();
        type=ClassGen.fullClassName(ClassGen.skipNullTypes(prop.getType()));

        vtype = "xmlbeansxx::Array<" + type + " >";
    }
	
    static void genMethod(String h_opts,String resultType,String className,String methodName,String params,String body, Output out) {
        out.h.println("virtual " + h_opts + " " + resultType + " " + methodName + "(" + params + ") = 0;");
        out.hImpl.println("virtual " + h_opts + " " + resultType + " " + methodName + "(" + params + ");"); 
        out.cpp.println(resultType + " " + className + "_I::" + methodName + "(" + ClassGen.cutAssigns(params) + ") {\n"
                + "xmlbeansxx::Lock lock(mutex());\n"
                + "check_orphaned();\n"
                + body + "}");
    }

    /** returns true iff property has a simple holder (eg. int) */ 
    public boolean hasHolder() {
        return prop.getJavaTypeCode()!=SchemaProperty.XML_OBJECT;
    }
	
    private String convert(String what) {
        String d = what;
        if (method==M_X) {
            return d;
        } else if (method==M_NORMAL) {
            if (hasHolder()) {
                return 
                    "(" + what + " == xmlbeansxx::Null() ? " + ClassGen.constructorFor(userType)
                    + " : " + ClassGen.genCast("xmlbeansxx::SimpleValue", what) + "->get" + ClassGen.genJGetName(prop.getJavaTypeCode()) + "Value())";
            } else return d;
        } else throw new IllegalStateException();
    }
    
    private String convertAndRet(String what) {
        return "return " + convert(what) + ";\n";
    }
	
    private String convertAndRetArray(String what) {
        String d = "return " + ClassGen.genCast(vuserType, what) + ";\n";
        if (method==M_X) {
            return d;
        } else if (method==M_NORMAL) {
            if (hasHolder()) {
                String len = ClassGen.nextTmpID();
                String toVar = ClassGen.nextTmpID();
                return 
                    "int " + len + " = " + what + ".size();\n" 
                    + vuserType + " " + toVar + "(" + len + ");\n" 
                    + "for(int i = 0; i < " + len + "; i++) {\n" 
                    + toVar + "[i]=" + convert(what + "[i]") + ";\n"
                    + "}\n"
                    + "return " + toVar + ";\n"
                    ;
            } else return d;
        } else throw new IllegalStateException();
    }
	
    private String convertFromGiven(String what,String toVar) {
        String def = type + " " + toVar + "=" + what + ";\n";
        if (method==M_X) {
            return def;
        } else if (method==M_NORMAL) {
            if (hasHolder()) {
                return type + " " + toVar + "(" + ClassGen.genNewXmlObject(st) + ");\n"
                    + toVar + "->set" + ClassGen.genJGetName(prop.getJavaTypeCode()) + "Value(" + what + ");\n";
            } else {
                return def;
            }
        } else throw new IllegalStateException();
    }
	
    private String convertFromGivenArray(String what,String toVar) {
        String def = vtype + " " + toVar + "=" + what + ";\n";
        if (method==M_X) {
            return def;
        } else if (method==M_NORMAL) {
            if (hasHolder()) {
                String len=ClassGen.nextTmpID();
                String t=ClassGen.nextTmpID();
                return
                    "int " + len + "=" + what + ".size();\n"
                    + vtype + " " + toVar + "(" + len + ");\n"
                    + "for(int i=0;i<" + len + ";i++) {\n"
                    + type + " " + t + "(" + ClassGen.genNewXmlObject(st) + ");\n"
                    + t + "->set" + ClassGen.genJGetName(prop.getJavaTypeCode()) + "Value(" + what + "[i]);\n"
                    + toVar + "[i]=" + t + ";\n"
                    + "}\n";
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
            vuserType = "xmlbeansxx::Array<" + userType + ">"; 
        } else if (method==M_NORMAL) {
            x="";
			
            userType = ClassGen.cppTypeForProperty(prop);
            vuserType = "xmlbeansxx::Array<" + userType + ">"; 
        }
    }
	
    public void genGet() {
        genMethod("", 
                  userType, 
				  ClassGen.classImplName(currentType), 
                  x + "get" + ClassGen.javaPropertyName(prop), 
                  "", 
                  type + " target="
                  + ClassGen.genCast(type, "get_store()->find_element("
                                    + ClassGen.genPropName(prop)
                                    + ", 0)") + ";\n"
                  + convertAndRet("target")
                  , out);
    }

    public void genGetAttr() {
        genMethod("", 
                  userType, 
                  ClassGen.classImplName(currentType), 
                  x + "get" + ClassGen.javaPropertyName(prop) + "Attr", 
                  "", 
                  type + " target="
                  + ClassGen.genCast(type, "get_store()->find_attribute("
                                    + ClassGen.genPropName(prop)
                                    + ")") + ";\n"
                  + convertAndRet("target"),
                  out);
    }

    private String genCGetBase(String index) {
        return type + " target = " + ClassGen.genCast(type, "cget_helper(" + ClassGen.genPropName(prop) + ", " 
                + index + ", type != xmlbeansxx::Null() ? type : " + ClassGen.fullClassName(prop.getType()) + "::type)") + ";\n"
                  + convertAndRet("target");
    }

    public void genCGet() {
        genMethod("", userType, ClassGen.classImplName(currentType), x + "cget" + ClassGen.javaPropertyName(prop),
                  "const xmlbeansxx::SchemaType &type = xmlbeansxx::Null()",
                  genCGetBase("0"),
                  out);
    }

    public void genGetArray() {
        //get_array
		genMethod("", vuserType, ClassGen.classImplName(currentType), 
                  x + "get" + ClassGen.javaPropertyName(prop) + "Array",
                  "",
                  "xmlbeansxx::Array<xmlbeansxx::TypeStoreUser> targetList;\n"
                  + "get_store()->find_all_elements(" + ClassGen.genPropName(prop) + ", targetList);\n"
                  + convertAndRetArray("targetList")
                  , out);
    }

    /*
    public void genDGetArray() {
        //dget_array
        genMethod("", dvtype, classImplName(currentType), 
                  "dget" + javaPropertyName(prop) + "Array",
                  "",
                  "xmlbeansxx::Array<xmlbeansxx::TypeStoreUser> targetList;\n"
                  + "get_store()->find_all_elements(" + genPropName(prop) + ", targetList);\n"
                  + "xmlbeansxx::XmlArray xmlArray(xmlbeansxx::XmlArray::Factory::newInstance());\n"
                  + "xmlArray->setEArray(targetList);\n"
                  + "return xmlArray;\n"
                  );
    }*/
	
    public void genGetArrayAt() {
        //getArrayAt
		genMethod("", userType, ClassGen.classImplName(currentType), 
                  x + "get" + ClassGen.javaPropertyName(prop) + "Array",
                  "int index", 
                  type + " target="
                  + ClassGen.genCast(type, "get_store()->find_element("
                                    + ClassGen.genPropName(prop)
                                    + ", index)") + ";\n"
                  + convertAndRet("target")
                  , out);
    }
	
    public void genCGetArrayAt() {
        //cgetAt - for gentor xpath
		genMethod("", userType, ClassGen.classImplName(currentType),
                  x + "cget"	+ ClassGen.javaPropertyName(prop) + "Array",
                  "int index, "
                  + "const xmlbeansxx::SchemaType &type = xmlbeansxx::Null()",
                  genCGetBase("index")
                  , out);
    }

    public void genSet() {
        //set
		genMethod("", "void", ClassGen.classImplName(currentType), 
                  x + "set" + ClassGen.javaPropertyName(prop),
                  "const " + userType + " &value",
                  convertFromGiven("value", "value2")
                  + "get_store()->set_element(" + ClassGen.genPropName(prop) + ", 0, " + ClassGen.genCast("xmlbeansxx::TypeStoreUser", "value2") + ");\n"
                  , out);
    }

    public void genSetArray() {
        //set_array
		genMethod("", "void",
				  ClassGen.classImplName(currentType),
                  x + "set" + ClassGen.javaPropertyName(prop)+ "Array",
                  "const " + vuserType + " &values",
                  convertFromGivenArray("values","values2")
                  + "arraySetterHelper(" + ClassGen.genPropName(prop) + ", " + ClassGen.genCast("xmlbeansxx::Array<xmlbeansxx::XmlObject>", "values2") + ");\n"
                  , out);
    }
	
    /*
    public void genDSetArray() {
        //set_array
        genMethod("", "void",
                  classImplName(currentType),
                  "dset" + javaPropertyName(prop)+ "Array",
                  "const " + dvtype + " &values",
                  "arraySetterHelper(" + genPropName(prop) + ", values->getEArray());\n"
                  );
    }*/
	
    public void genAdd() {
        //append
		genMethod("", "void", 
				  ClassGen.classImplName(currentType),
                  x + "add" + ClassGen.javaPropertyName(prop),
                  "const " + userType + " &value",
                  convertFromGiven("value","value2")
                  + "get_store()->add_element(" + ClassGen.genPropName(prop) + ", " + ClassGen.genCast("xmlbeansxx::TypeStoreUser", "value2") + ");\n"
                  , out);
    }

    public void genSetArrayAt() {
        //setArray
		genMethod("", "void", ClassGen.classImplName(currentType), 
                  x + "set" + ClassGen.javaPropertyName(prop) + "Array",
                  "int index, const " + userType + " &value",
                  convertFromGiven("value", "value2")
                  + "get_store()->set_element(" + ClassGen.genPropName(prop)+ ", index, " + ClassGen.genCast("xmlbeansxx::TypeStoreUser", "value2") + ");\n",
                  out);
    }
	
    public void genSetAttr() {
		genMethod("", "void", ClassGen.classImplName(currentType), 
                  x + "set" + ClassGen.javaPropertyName(prop) + "Attr",
                  "const " + userType + " &value",
                  convertFromGiven("value","value2")
                  + "get_store()->remove_attribute(" + ClassGen.genPropName(prop) + ");\n"
                  + "if (value2 != xmlbeansxx::Null()) get_store()->add_attribute(" + ClassGen.genPropName(prop) + ", " + ClassGen.genCast("xmlbeansxx::TypeStoreUser", "value2") + ");\n",
                  out);
    }
    
    public void genSizeOf() {
		genMethod("","int",ClassGen.classImplName(currentType),"sizeOf"+ClassGen.javaPropertyName(prop),"",
                  "return get_store()->count_elements(" + ClassGen.genPropName(prop) + ");\n"
                  , out);
    }

    public void genIsSet() {
		genMethod("","bool",
				  ClassGen.classImplName(currentType),"isSet" + ClassGen.javaPropertyName(prop), "", 
                  "return get_store()->find_element(" + ClassGen.genPropName(prop) + ", 0) != xmlbeansxx::Null();\n"
                  , out);
    }

    public void genUnsetAttr() {
        genMethod("","void",ClassGen.classImplName(currentType),"unset"
                  + ClassGen.javaPropertyName(prop) + "Attr","",
                  "get_store()->remove_attribute(" + ClassGen.genPropName(prop) + ");\n"
                  , out);
    }

    public void genIsSetAttr() {
        genMethod("","bool",
                  ClassGen.classImplName(currentType),"isSet" + ClassGen.javaPropertyName(prop) + "Attr", "", 
                  "return get_store()->find_attribute(" + ClassGen.genPropName(prop) + ") != xmlbeansxx::Null();\n"
                  , out);
    }
    public void genUnset() {
        genMethod("", "void", ClassGen.classImplName(currentType), "unset" + ClassGen.javaPropertyName(prop), "",
                  "get_store()->remove_all_elements(" + ClassGen.genPropName(prop) + ");\n"
                  , out);
    }

    public void genAddNew() {
        genMethod("",userType,ClassGen.classImplName(currentType),"addNew" + ClassGen.javaPropertyName(prop),"",
                  type + " e = " + ClassGen.genNewXmlObject(st) + ";\n"
                  + "get_store()->add_element(" + ClassGen.genPropName(prop) + ", " + ClassGen.genCast("xmlbeansxx::TypeStoreUser", "e") + ");\n"
                  + "return e;\n"
                  , out);
    }
    
    public void genUnsetArray() {
        genMethod("", "void", 
                  ClassGen.classImplName(currentType),
                  "unset" + ClassGen.javaPropertyName(prop)+ "Array",
                  "",
                  "get_store()->remove_all_elements(" + ClassGen.genPropName(prop) + ");"
                  , out);
    }

    public void genRemove() {
        genMethod("", "void", ClassGen.classImplName(currentType), "remove" + ClassGen.javaPropertyName(prop),
                  "int index",
                  "get_store()->remove_element("
                  + ClassGen.genPropName(prop)
                  + ", index);"
                  , out);
    }
}
