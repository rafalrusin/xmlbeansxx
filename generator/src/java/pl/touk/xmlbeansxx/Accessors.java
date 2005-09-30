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

import java.io.IOException;

import org.apache.xmlbeans.SchemaProperty;
import org.apache.xmlbeans.SchemaType;
import org.apache.xmlbeans.*;

/**
 * Provides code generation for accessors. Eg. getBigDecimalValue.
 * @author mind
 *
 */
public class Accessors {
	private SchemaType containerType;
	public StringBuffer result = new StringBuffer(); 
	
	public Accessors(SchemaType containerType) {
		this.containerType = containerType;
	}
	
	private void emitAccessor(int typeCode) {
		emitAccessor(ClassGen.genJGetName(typeCode));
	}
	
	private void emitAccessor(String accessor) {
		result.append(", ");
		result.append("public virtual xmlbeansxx::" + accessor + "Accessor_I");
	}
	
	public void emitSpecializedAccessors() {
		if (containerType.getSimpleVariety() == SchemaType.ATOMIC) {
			if (XmlAnySimpleType.type.equals(containerType)) {
				emitAccessor("String");
			} else if (XmlBase64Binary.type.equals(containerType)) {
				emitAccessor("ByteArray");
			} else if (XmlBoolean.type.equals(containerType)) {
				emitAccessor("Boolean");
			} else if (XmlByte.type.equals(containerType)) {
				emitAccessor("Byte");
			} else if (XmlDate.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
				emitAccessor("Date");
			} else if (XmlDateTime.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
				emitAccessor("Date");
			} else if (XmlDecimal.type.equals(containerType)) {
				emitAccessor("BigDecimal");
			} else if (XmlDouble.type.equals(containerType)) {
				emitAccessor("Double");
			} else if (XmlDuration.type.equals(containerType)) {
				emitAccessor("GDuration");
			} else if (XmlENTITIES.type.equals(containerType)) {
				emitAccessor("List");
			} else if (XmlFloat.type.equals(containerType)) {
				emitAccessor("Float");
			} else if (XmlGDay.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
				emitAccessor("Int");
			} else if (XmlGMonthDay.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
			} else if (XmlGMonth.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
				emitAccessor("Int");
			} else if (XmlGYear.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
				emitAccessor("Int");
			} else if (XmlGYearMonth.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
			} else if (XmlHexBinary.type.equals(containerType)) {
				emitAccessor("ByteArray");
			} else if (XmlIDREFS.type.equals(containerType)) {
				emitAccessor("List");
			} else if (XmlInteger.type.equals(containerType)) {
				emitAccessor("BigInteger");
			} else if (XmlInt.type.equals(containerType)) {
				emitAccessor("Int");
			} else if (XmlLong.type.equals(containerType)) {
				emitAccessor("Long");
			} else if (XmlNMTOKENS.type.equals(containerType)) {
				emitAccessor("List");
			} else if (XmlQName.type.equals(containerType)) {
				emitAccessor("QName");
			} else if (XmlShort.type.equals(containerType)) {
				emitAccessor("Short");
			} else if (XmlTime.type.equals(containerType)) {
				emitAccessor("Calendar");
				emitAccessor("GDate");
			} else if (XmlUnsignedByte.type.equals(containerType)) {
				emitAccessor("Short");
			} else if (XmlUnsignedInt.type.equals(containerType)) {
				emitAccessor("Long");
			} else if (XmlUnsignedShort.type.equals(containerType)) {
				emitAccessor("Int");
			} else {
				//Some non-builtin type
				int btc = containerType.getPrimitiveType().getBuiltinTypeCode();
				if (btc == SchemaType.BTC_DECIMAL) {
					int bits = containerType.getDecimalSize();
					int parentBits = containerType.getBaseType().getDecimalSize();
					if (bits != parentBits || containerType.getBaseType().getFullJavaName() == null)
					{
						if (bits == SchemaType.SIZE_BIG_INTEGER)
						{
							emitAccessor(SchemaProperty.JAVA_BIG_INTEGER);
						}
						else if (bits == SchemaType.SIZE_LONG)
						{
							emitAccessor(SchemaProperty.JAVA_LONG);
						}
						else if (bits == SchemaType.SIZE_INT)
						{
							emitAccessor(SchemaProperty.JAVA_INT);
						}
						else if (bits == SchemaType.SIZE_SHORT)
						{
							emitAccessor(SchemaProperty.JAVA_SHORT);
						}
						else if (bits == SchemaType.SIZE_BYTE)
						{
							emitAccessor(SchemaProperty.JAVA_BYTE);
						}
					}
				}
			}
		}
		/*
		 
		 if (containerType.getSimpleVariety() == SchemaType.UNION)
		 {
		 out.h.println("java.lang.Object getObjectValue();");
		 out.h.println("void setObjectValue(java.lang.Object val);");
		 out.h.println("org.apache.xmlbeans.SchemaType instanceType();");
		 SchemaType ctype = sType.getUnionCommonBaseType();
		 if (ctype != null && ctype.getSimpleVariety() != SchemaType.UNION);
		 out.h.printlnSpecializedAccessors(ctype);
		 }
		 
		 if (sType.getSimpleVariety() == SchemaType.LIST)
		 {
		 out.h.println("java.util.List getListValue();");
		 out.h.println("java.util.List xgetListValue();");
		 out.h.println("void setListValue(java.util.List list);");
		 }*/
		
		Object[] t = {"x", "x", "y", 5};
	}
}
