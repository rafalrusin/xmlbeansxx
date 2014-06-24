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

public class Graph {
	static class Node {
	    int n_outer=0;
	    ArrayList inner=new ArrayList();
	}

    public HashMap nodes=new HashMap();


    void addNode(Object key) {
        if (((Node)nodes.get(key))==null) {
            nodes.put(key,new Node());
            //System.out.println("New node "+key.toString());
        }
    }

    /**
     * Pobiera 2 klucze, tworzy krawedz z k1 do k2.
     * Wezly musza istniec w grafie.
     */
    void addBranch(Object k1,Object k2) {
        ((Node)nodes.get(k1)).n_outer++;
        ((Node)nodes.get(k2)).inner.add(k1);
        //System.out.println(k1.toString()+" -> "+k2.toString());
    }

    ArrayList topologicalSort() {
        int ile=nodes.size();
        ArrayList ret=new ArrayList();
        Stack zero=new Stack();
        Iterator it=nodes.keySet().iterator();
        while (it.hasNext()) {
            Object u=it.next();
            if (((Node)nodes.get(u)).n_outer==0) {
                zero.push(u);
            }
        }

        while (!zero.isEmpty()) {
            Object u=zero.pop();
            ret.add(u);
            Iterator it2=((Node)nodes.get(u)).inner.iterator();
            while (it2.hasNext()) {
                Object o=it2.next();
                Node x=(Node)nodes.get(o);
                x.n_outer--;
                if (x.n_outer==0) zero.push(o);
            }
        }
        if (ile!=ret.size()) {
            return null;
        }
        return ret;
    }
}
