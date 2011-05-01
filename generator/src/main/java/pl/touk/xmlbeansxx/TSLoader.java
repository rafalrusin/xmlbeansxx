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

import org.apache.xmlbeans.impl.schema.SchemaTypeSystemCompiler;
import org.apache.xmlbeans.impl.schema.PathResourceLoader;
import org.apache.xmlbeans.ResourceLoader;
import org.apache.xmlbeans.impl.schema.StscState;
import org.apache.xmlbeans.impl.schema.SchemaTypeLoaderImpl;
import org.apache.xmlbeans.impl.common.XmlErrorPrinter;
import org.apache.xmlbeans.impl.common.XmlErrorWatcher;
import org.apache.xmlbeans.XmlError;
import org.apache.xmlbeans.impl.values.XmlListImpl;
import org.apache.xmlbeans.SchemaTypeSystem;
import org.apache.xmlbeans.SchemaTypeLoader;
import org.apache.xmlbeans.XmlBeans;
import org.apache.xmlbeans.XmlOptions;
import org.apache.xmlbeans.XmlObject;
import org.apache.xmlbeans.XmlException;
import org.apache.xmlbeans.SimpleValue;
//import org.apache.xml.xmlbeans.x2004.x02.xbean.config.ConfigDocument;

import java.io.File;
import java.util.*;
import java.net.URI;

//import org.w3.x2001.xmlSchema.SchemaDocument;

import org.apache.xmlbeans.impl.xb.xsdschema.SchemaDocument.Schema;
import org.apache.xmlbeans.impl.xb.xsdschema.SchemaDocument;
import org.apache.xmlbeans.BindingConfig;

public class TSLoader {
    public static SchemaTypeSystem loadTypeSystem(String name, File[] xsdFiles, File baseDir) {
	    //XmlErrorWatcher errorListener = new XmlErrorWatcher();
	    //XmlErrorWatcher errorListener = new XmlErrorWatcher(new XmlErrorPrinter(true,null));
	    XmlErrorWatcher errorListener = new XmlErrorWatcher(new MyXmlErrorPrinter());
	    //XmlErrorWatcher errorListener = new XmlErrorWatcher(Collections.EMPTY_SET);
        SchemaTypeSystem system = loadTypeSystem2(name, xsdFiles, null,
        		System.getProperty("download") != null,
        		System.getProperty("noUpa") != null,
        		System.getProperty("noPvr") != null,
        		Collections.EMPTY_SET, baseDir, new HashMap(), errorListener);
        if (errorListener.hasError())
            return null;
        return system;
	}


    private static SchemaTypeSystem loadTypeSystem2(
        String name, File[] xsdFiles,
        ResourceLoader cpResourceLoader,
        boolean download, boolean noUpa, boolean noPvr, Set mdefNamespaces,
        File baseDir, Map sourcesToCopyMap, Collection outerErrorListener)
    {
        XmlErrorWatcher errorListener = new XmlErrorWatcher(outerErrorListener);

        // For parsing XSD and WSDL files, we should use the SchemaDocument
        // classloader rather than the thread context classloader.  This is
        // because in some situations (such as when being invoked by ant
        // underneath the ide) the context classloader is potentially weird
        // (because of the design of ant).

        SchemaTypeLoader loader = XmlBeans.typeLoaderForClassLoader(SchemaDocument.class.getClassLoader());

        // step 1, parse all the XSD files.
        ArrayList scontentlist = new ArrayList();
        if (xsdFiles != null)
        {
            for (int i = 0; i < xsdFiles.length; i++)
            {
                try
                {
                    XmlOptions options = new XmlOptions();
                    options.setLoadLineNumbers();
                    options.setLoadMessageDigest();
                    options.setLoadSubstituteNamespaces(MAP_COMPATIBILITY_CONFIG_URIS);

                    XmlObject schemadoc = loader.parse(xsdFiles[i], null, options);
                    if (!(schemadoc instanceof SchemaDocument))
                    {
                        StscState.addError(errorListener, "Document " + xsdFiles[i] + " is not a schema file", XmlError.SEVERITY_ERROR, schemadoc);
                    }
                    else
                    {
                        StscState.addInfo(errorListener, "Loading schema file " + xsdFiles[i]);
                        XmlOptions opts = new XmlOptions().setErrorListener(errorListener);
                        if (schemadoc.validate(opts))
                            scontentlist.add(((SchemaDocument)schemadoc).getSchema());
                    }
                }
                catch (XmlException e)
                {
                    errorListener.add(e.getError());
                }
                catch (Exception e)
                {
//                    StscState.addError(errorListener, "Cannot load file " + xsdFiles[i] + ": " + e, XmlErrorContext.CANNOT_LOAD_XSD_FILE, xsdFiles[i]);
                }
            }
        }

        SchemaDocument.Schema[] sdocs = (SchemaDocument.Schema[])scontentlist.toArray(new SchemaDocument.Schema[scontentlist.size()]);

        // now the config files.
        ArrayList cdoclist = new ArrayList();
//        BindingConfig cdocs = (ConfigDocument.Config[])cdoclist.toArray(new ConfigDocument.Config[cdoclist.size()]);

        SchemaTypeLoader linkTo = SchemaTypeLoaderImpl.build(null, cpResourceLoader, null);

        URI baseURI = null;
        if (baseDir != null)
            baseURI = baseDir.toURI();

        XmlOptions opts = new XmlOptions();
        if (download)
            opts.setCompileDownloadUrls();
        if (noUpa)
            opts.setCompileNoUpaRule();
        if (noPvr)
            opts.setCompileNoPvrRule();
        if (mdefNamespaces != null)
            opts.setCompileMdefNamespaces(mdefNamespaces);
        opts.setCompileNoValidation(); // already validated here

        // now pass it to the main compile function
        SchemaTypeSystemCompiler.Parameters params = new SchemaTypeSystemCompiler.Parameters();
        params.setName(name);
        params.setSchemas(sdocs);
//        params.setConfigs(cdocs);
        params.setLinkTo(linkTo);
        params.setOptions(opts);
        params.setErrorListener(errorListener);
        params.setJavaize(true);
        params.setBaseURI(baseURI);
        params.setSourcesToCopyMap(sourcesToCopyMap);
        return SchemaTypeSystemCompiler.compile(params);
    }

    private static final String CONFIG_URI = "http://xml.apache.org/xmlbeans/2004/02/xbean/config";
    private static final String COMPATIBILITY_CONFIG_URI = "http://www.bea.com/2002/09/xbean/config";
    private static final Map MAP_COMPATIBILITY_CONFIG_URIS;
    static
    {
        MAP_COMPATIBILITY_CONFIG_URIS = new HashMap();
        MAP_COMPATIBILITY_CONFIG_URIS.put(COMPATIBILITY_CONFIG_URI, CONFIG_URI);
    }
}