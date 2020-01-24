//----------------------------------------------------------------------------
// File : xmldesc_utils.cpp
// Project : MacMap
// Purpose : C++ source file : XML creation utils
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2015 Carte Blanche Conseil.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// See the LICENSE.txt file for more information.
//
//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
// 24/02/2005 creation.
//----------------------------------------------------------------------------

#include "xmldesc_utils.h"
#include "bGenericMacMapApp.h"

// ---------------------------------------------------------------------------
// 
// -----------
void add_cdesc(bArray& arr, int indent, const char* name, const char* value){
xmlelt	desc;
	
	desc.indent=indent;
	desc.element=NULL;
	desc.name=strdup(name);
	desc.value=strdup(value);
	
	arr.add(&desc);
}

// ---------------------------------------------------------------------------
// 
// -----------
void add_idesc(bArray& arr, int indent, const char* name, int value){
char	val[256];
	
	snprintf(val,sizeof(val),"%d",value);
	add_cdesc(arr,indent,name,val);
}

// ---------------------------------------------------------------------------
// 
// -----------
void add_ddesc(bArray& arr, int indent, const char* name, double value, int decs){
char	val[256];
	
	snprintf(val,sizeof(val),"%.*f",decs,value);	
//	snprintf(val,sizeof(val),"%g",value); // => pas toujours assez précis
	add_cdesc(arr,indent,name,val);
}

// ---------------------------------------------------------------------------
// 
// -----------
void free_descs(bArray& arr){
long	i,n=arr.count();
xmlelt	desc;
	
	for(i=1;i<=n;i++){
		arr.get(i,&desc);
		if(desc.name){
			free(desc.name);
		}
		if(desc.value){
			free(desc.value);
		}
	}
	arr.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void dump_descs(bArray& arr){
long	i,n=arr.count();
xmlelt	desc;

	for(i=1;i<=n;i++){
		arr.get(i,&desc);
		if(desc.value){
			fprintf(stderr,"%d>%d:%s-%s\n",i,desc.indent,desc.name,desc.value);	
		}
		else{
			fprintf(stderr,"%d>%d:%s\n",i,desc.indent,desc.name);	
		}
	}
}

