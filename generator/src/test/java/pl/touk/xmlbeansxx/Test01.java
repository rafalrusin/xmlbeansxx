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

import java.util.*;

public class Test01 {
    public static void main(String[] args) {
        Graph g=new Graph();
        g.addNode("a");
        g.addNode("b");
        g.addNode("c");
        g.addNode("d");
        g.addBranch("a","c");
        g.addBranch("c","b");
        g.addBranch("b","a");
        g.addBranch("d","a");
        g.addBranch("d","b");
        g.addBranch("d","c");
        ArrayList l=g.topologicalSort();
        Iterator it=l.iterator();
        while (it.hasNext()) {
            System.out.println(it.next());
        }
    }
}
