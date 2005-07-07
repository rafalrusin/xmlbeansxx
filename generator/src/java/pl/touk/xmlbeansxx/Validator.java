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
import org.apache.xmlbeans.*;

public class Validator {

    public static void usage() {
        System.out.println("Arguments: file.xml [file1.xsd file2.xsd ... ]");
    }
    public static void main(String[] args) {
        try {
            if (args.length==0) {
                usage();
            } else {
                String name=args[0];
                String[] xsds=new String[args.length-1];
                for(int i=0;i<args.length-1;i++) xsds[i]=args[i+1];
                go(name,xsds);
            }
        } catch (Exception e) {
            //System.out.println("Exception:"+e.toString());
            e.printStackTrace();
        }
    }
    public static void go(String xml,String[] xsds) throws Exception {
        String[] xmls={xml};
		
        SchemaTypeSystem ts=ClassGen.loadTS("",xsds);
        SchemaTypeLoader tl;
        /*
          {
          XmlObject[] array = new XmlObject[xsds.length];
          for(int i=0;i<array.length;i++) array[i] = XmlObject.Factory.parse(new File(xsds[i]));
			
          typeSystem = XmlBeans.compileXsd(array, XmlBeans
          .getBuiltinTypeSystem(), null);
			
			
          List allSeenTypes = new ArrayList();
          allSeenTypes.addAll(Arrays.asList(typeSystem.documentTypes()));
          allSeenTypes.addAll(Arrays.asList(typeSystem.attributeTypes()));
          allSeenTypes.addAll(Arrays.asList(typeSystem.globalTypes()));
          for (int i = 0; i < allSeenTypes.size(); i++) {
          SchemaType sType = (SchemaType) allSeenTypes.get(i);
          System.out.println("Visiting <"+sType.getClass().getName()+"> " + sType.toString());
          }
          allSeenTypes.clear();
          System.out.println("anonymous:");
          for (int i = 0; i < allSeenTypes.size(); i++) {
          SchemaType sType = (SchemaType) allSeenTypes.get(i);
          System.out.println("Visiting <"+sType.getClass().getName()+"> " + sType.toString());
          allSeenTypes.addAll(Arrays.asList(sType.getAnonymousTypes()));
          }
			
          }
		
        */
        tl = XmlBeans.typeLoaderUnion(
            new SchemaTypeLoader[] {ts, XmlBeans.getBuiltinTypeSystem()}
            );
		
        {
            for(int i=0;i<xmls.length;i++) {
                XmlObject mydoc = tl.parse(new File(xmls[i]), null, null);
				
                // Create an XmlOptions instance and set the error listener.
                XmlOptions validateOptions = new XmlOptions();
                ArrayList errorList = new ArrayList();
                validateOptions.setErrorListener(errorList);
				
                // Validate the XML.
                boolean isValid = mydoc.validate(validateOptions);
                if (isValid) { System.out.println(xmls[i]+" is valid"); }
                else { System.out.println(xmls[i]+" is not valid"); }
				
                // If the XML isn't valid, loop through the listener's contents,
                // printing contained messages.
                if (!isValid) {
                        for (int j = 0; i < errorList.size(); i++)
                            {
                                XmlError error = (XmlError)errorList.get(j);
						
                                System.out.println("\n");
                                System.out.println("Message: " + error.getMessage() + "\n");
                                System.out.println("Location of invalid XML: " + 
                                                   error.getCursorLocation().xmlText() + "\n");
                            }
                    }			
            }
        }
    }
}
