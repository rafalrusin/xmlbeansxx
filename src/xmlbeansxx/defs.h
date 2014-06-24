/*
    Copyright 2004-2008 TouK sp. z o.o. s.k.a.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License. */


#ifndef _DEFS_H__
#define _DEFS_H__

//Additional language definitions
#define XMLBEANSXX_FOREACH(type,it,c) for(type it=(c).begin(); it!=(c).end(); ++it)
#define XMLBEANSXX_FOREACH_BACKWARD(type,it,c) for(type it=(c).rbegin(); it!=(c).rend(); ++it)
//#define XMLBEANSXX_FOR(i,n) for(int i=0;i<(n);++i)
//#define VAL(i,v) __typeof(v) i=(v)

#endif
