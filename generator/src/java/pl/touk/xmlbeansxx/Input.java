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
import java.util.*;
import java.io.*;

/**
 * @author Rafal Rusin
 */
public class Input {
	private Map h_gaps,cpp_gaps;
	public Input(String name) {
		h_gaps=readGaps(name+".xh");
		cpp_gaps=readGaps(name+".xcpp");
	}
	
	private Map readGaps(String fileName) {
		Map gaps=new HashMap();
		try {
			BufferedReader rd=new BufferedReader(new FileReader(fileName));
			String l,gapName=null;
			StringBuffer b=new StringBuffer();
			while ((l=rd.readLine())!=null) {
				if (l.length()>=2 && l.substring(0,2).equals("%%")) {
					if (gapName!=null) {
						String s=(String)gaps.get(gapName);
						if (s==null) s="";
						gaps.put(gapName,s+b.toString());
						b=new StringBuffer();
					}

					StringTokenizer tok=new StringTokenizer(l.substring(2));
					String t;
					gapName="";
					String delim="";
					while (tok.hasMoreTokens()) {
						t=tok.nextToken();
						gapName=gapName+delim+t;
						delim=" ";
					}
				} else {
					b.append(l);
					b.append("\n");
				}
			}
			rd.close();
		} catch (IOException e) {
			//file not found - ok
		}
		return gaps;
	}
	
	public String getHGap(String name) {
		return (String)h_gaps.get(name);
	}

	public String getCppGap(String name) {
		return (String)cpp_gaps.get(name);
	}
}
