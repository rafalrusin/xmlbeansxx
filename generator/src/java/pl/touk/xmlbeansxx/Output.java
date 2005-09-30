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

package pl.touk.xmlbeansxx;

import java.io.*;
import java.util.*;

public class Output {
    public PrintWriter h, cpp, hImpl;

    private String dest(String fileName) {
        return ClassGen.outDir + "/" + fileName;
    }

    public Output(String name) throws IOException {
        String hname = name + ".h", cppname = name + ".cpp", hImplName = name + "Impl.h";

        h = new PrintWriter(new BufferedOutputStream(
                new FileOutputStream(dest(hname))));
        hImpl = new PrintWriter(new BufferedOutputStream(
                new FileOutputStream(dest(hImplName))));
        cpp = new PrintWriter(new BufferedOutputStream(new FileOutputStream(
                dest(cppname))));
        h.println("#ifndef _" + onlyAlNum(name.toUpperCase()) + "_H_INCLUDED_");
        h.println("#define _" + onlyAlNum(name.toUpperCase()) + "_H_INCLUDED_");
        
        hImpl.println("#ifndef _" + onlyAlNum(name.toUpperCase()) + "_H_IMPL_INCLUDED_");
        hImpl.println("#define _" + onlyAlNum(name.toUpperCase()) + "_H_IMPL_INCLUDED_");
    }

    public void close() {
        exitNamespace();
        h.println("#endif");
        h.close();
        hImpl.println("#endif");
        hImpl.close();
        cpp.close();
    }

    int nsDepth = -2;
    
    public void splitCpp() {
    	String cns = currentNamespace;
    	enterNamespace("");
    	cpp.println("//--split--");
    	enterNamespace(cns);
    }

    private String currentNamespace = "";

    private void print(String what) {
        h.print(what);
        hImpl.print(what);
        cpp.print(what);
    }

    private void println(String what) {
        h.println(what);
        hImpl.println(what);
        cpp.println(what);
    }

    private void println() {
        h.println();
        hImpl.println();
        cpp.println();
    }

    public void enterNamespace(String namespace) {
        if (!currentNamespace.equals(namespace)) {
            if (!currentNamespace.equals("")) exitNamespace();
            
            StringTokenizer t = new StringTokenizer(namespace, "::");
            nsDepth = 0;
            while (t.hasMoreTokens()) {
                String s = "namespace " + t.nextToken() + " {";
                print(s);
                nsDepth++;
            }
            println();
            
            currentNamespace = namespace;
        }
    }

    private void exitNamespace() {
        if (nsDepth==-2) return;
        for (int i = 0; i < nsDepth; i++) {
            print("}");
        }
        nsDepth = -2;
        println("//--namespace");
    }

    public void leaveNamespace() {
    }

    private String onlyAlNum(String s) {
    	StringBuffer b=new StringBuffer();
    	for(int i=0;i<s.length();i++) {
    		if (!Character.isLetterOrDigit(s.charAt(i))) {
    			b.append("_");
    		} else {
    			b.append(s.charAt(i));
    		}
    	}
    	return b.toString();
    }
}
