/*
 *  Copyright 2004-2008 TouK sp. z o.o. s.k.a.
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

import java.util.AbstractCollection;
import java.util.Iterator;
import java.util.Collections;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import org.apache.xmlbeans.XmlError;

public class MyXmlErrorPrinter extends AbstractCollection {
	private static Log log=LogFactory.getLog(MyXmlErrorPrinter.class);

	public int size() {
		return 0;
	}
	
	public boolean add(Object o) {
        if (o instanceof XmlError)
        {
            XmlError err = (XmlError)o;
            if (err.getSeverity() == XmlError.SEVERITY_ERROR) {
            	log.error(err.toString());
            	throw new RuntimeException("ERROR!");
            } else if (err.getSeverity() == XmlError.SEVERITY_WARNING) {
            	log.warn(err.toString());
            } else {
            	log.info(err.toString());
            }
        }
        return false;
	}

	public Iterator iterator() {
		return Collections.EMPTY_LIST.iterator();
	}
}
