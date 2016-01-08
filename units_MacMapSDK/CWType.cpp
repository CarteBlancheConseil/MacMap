//----------------------------------------------------------------------------
// File : CWType.cpp
// Project : MacMap
// Purpose : C++ source file : bGenericType C wrappers
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
// 15/01/2009 creation.
//----------------------------------------------------------------------------

#include "CWType.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/bGenericType.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/MacMapCWrappers.h>
#include <std_ext/bStdXMLBaseElement.h>
#include <MacMapSuite/valconv.h>

// ---------------------------------------------------------------------------
// 
// ------------
CWType	CWType_Create(CWCallback Callback, CWFileName name, CWPath path, int kind, int tkind){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	if(gapp->typesMgr()->add(name,path,NULL,tkind,kind)!=0){
		return(NULL);
	}
CWType	tp=(CWType)gapp->typesMgr()->get(gapp->typesMgr()->count());
	return(tp);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWType	CWType_Open(CWCallback Callback, CWFileName name, CWPath path, int tkind){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
	if(gapp->typesMgr()->add(name,path,tkind)!=0){
		return(NULL);
	}
CWType	tp=(CWType)gapp->typesMgr()->get(gapp->typesMgr()->count());
	return(tp);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWType CWType_Get(CWCallback Callback, int index){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
CWType	tp=(CWType)gapp->typesMgr()->get(index);
	return(tp);
}

// ---------------------------------------------------------------------------
// 
// ------------
CWType CWType_GetCurrent(CWCallback Callback){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
CWType	tp=(CWType)CurType(gapp);
	return(tp);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWType_GetName(CWType typ, CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
	tp->name(data);
}

// ---------------------------------------------------------------------------
// 
// ------------
void CWType_GetPath(CWType typ, CWPath path){
bGenericType*	tp=(bGenericType*)typ;
	tp->location(path);
}

// ---------------------------------------------------------------------------
// 
// ------------
double CWType_GetID(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->tid());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetIndex(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->index());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetKind(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->kind());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetRecordCount(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->nb_rec());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetRecordKilled(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->nb_killed());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetRecordAlive(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->nb_live());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetSelectCount(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->nb_sel());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetContrastCount(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->nb_cont());
}


// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetFieldCount(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetFieldInfo(CWType typ,
						int index,
						int* kind,
						int* len,
						int* decimals,
						CWCharData name){
bGenericType*	tp=(bGenericType*)typ;
	if(tp->fields()->get_kind(index,kind)){
		return(0);
	}
	if(tp->fields()->get_len(index,len)){
		return(0);
	}
	if(tp->fields()->get_decs(index,decimals)){
		return(0);
	}
	if(tp->fields()->get_name(index,name)){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_SetFieldInfo(CWType typ,
						int index,
						int kind,
						int len,
						int decimals,
						CWCharData name){	
bGenericType*	tp=(bGenericType*)typ;
	if(tp->fields()->modify(index,kind,len,decimals,name)!=0){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_AddField(CWType typ,
					int kind,
					int len,
					int decimals,
					CWCharData name){
bGenericType*	tp=(bGenericType*)typ;
	if(tp->fields()->add(kind,len,decimals,name)!=0){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_RemoveField(	CWType typ,
						int index){
bGenericType*	tp=(bGenericType*)typ;
	if(tp->fields()->remove(index)!=0){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_FieldIDWithIndex(CWType typ,
							int index){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->get_id(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_FieldIndexWithID(CWType typ,
							int ID){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->get_index(ID));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_FieldIndexWithName(	CWType typ,
								CWCharData name){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->get_index(name));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetFieldFlags(	CWType typ,
							int index,
							int* delprot,
							int* writeprot,
							int* hidden){
bGenericType*	tp=(bGenericType*)typ;
	*delprot=tp->fields()->is_delprotected(index);
	*writeprot=tp->fields()->is_writeprotected(index);
	*hidden=tp->fields()->is_hidden(index);
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_SetFieldFlags(	CWType typ,
							int index,
							int delprot,
							int writeprot,
							int hidden){
bGenericType*	tp=(bGenericType*)typ;
	if(tp->fields()->set_delprotected(index,delprot)){
		return(0);
	}
	if(tp->fields()->set_writeprotected(index,writeprot)){
		return(0);
	}	if(tp->fields()->set_hidden(index,hidden)){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetFieldConstraintCount(	CWType typ,
									int index){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->count_constraints(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetFieldConstraintKind(	CWType typ,
									int index){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->get_constraints_kind(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetFieldConstraintValue(	CWType typ,
									int fieldindex,
									int constraintindex,
									CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
char			buffer[256];
int				d;
int				ck=tp->fields()->get_constraints_kind(fieldindex);
	tp->fields()->get_decs(fieldindex,&d);
	if(!tp->fields()->get_constraint(fieldindex,constraintindex,buffer)){
		return(0);
	}
	if(ck==_bit){
		strcpy(data,buffer);				
	}
	else{
		xToChar(ck,d,buffer,data);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_SetFieldConstraintValue(	CWType typ,
									int fieldindex,
									int constraintindex,
									CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
char			buffer[256];
int				d;
int				ck=tp->fields()->get_constraints_kind(fieldindex);
	tp->fields()->get_decs(fieldindex,&d);
	if(ck==_bit){
		strcpy(buffer,data);				
	}
	else{
		charToX(ck,d,data,buffer);
	}
	if(!tp->fields()->set_constraint(fieldindex,constraintindex,buffer)){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_AddFieldConstraint(	CWType typ,
								int fieldindex,
								int constraintkind,
								CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
char			buffer[256];
int				d;
	tp->fields()->get_decs(fieldindex,&d);
	if(constraintkind==_bit){
		strcpy(buffer,data);				
	}
	else{
		charToX(constraintkind,d,data,buffer);
	}
	if(!tp->fields()->add_constraint(fieldindex,constraintkind,buffer)){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_RemoveFieldConstraint(	CWType typ,
									int fieldindex,									
									int constraintindex){
bGenericType*	tp=(bGenericType*)typ;
	if(!tp->fields()->rmv_constraint(fieldindex,constraintindex)){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetFieldConstraintIndex(	CWType typ,
									int fieldindex,
									CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
char			buffer[256];
int				d;
int				ck=tp->fields()->get_constraints_kind(fieldindex);
	tp->fields()->get_decs(fieldindex,&d);
	if(ck==_bit){
		strcpy(buffer,data);				
	}
	else{
		charToX(ck,d,data,buffer);
	}
	if(!tp->fields()->get_constraint_index(fieldindex,buffer)){
		return(0);
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_FieldHasDefaultValue(	CWType typ,
									int fieldindex){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->has_default_value(fieldindex));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_FieldGetDefaultValue(	CWType typ,
									int fieldindex,
									CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
char			def[256];
int				kind,d,l,ck;
	tp->fields()->get_kind(fieldindex,&kind);
	tp->fields()->get_len(fieldindex,&l);
	tp->fields()->get_decs(fieldindex,&d);
	ck=tp->fields()->get_constraints_kind(fieldindex);
	if(tp->fields()->get_default_value(fieldindex,def)){
		if(kind==ck){// Pour les champs naturels, on prend le type naturel
			if(!xToChar(kind,d,def,data)){
				return(0);
			}
		}
		else if(ck==_bit){// Pour les champs bit, on prend le type naturel
			if(!xToChar(kind,d,def,data)){
				return(0);
			}
		}
		else{// Pour les champs classe, on prend le nom de la contrainte
			strcpy(data,def);
		}
	}
	else{
		data[0]=0;
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_FieldSetDefaultValue(	CWType typ,
									int fieldindex,
									CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
	
	if(strlen(data)>0){
char	buffer[256];	
int		kind,d,l,ck;
		tp->fields()->get_kind(fieldindex,&kind);
		tp->fields()->get_len(fieldindex,&l);
		tp->fields()->get_decs(fieldindex,&d);
		ck=tp->fields()->get_constraints_kind(fieldindex);
		if(ck==0){
			ck=kind;
		}
		if(kind==ck){// Pour les champs naturels, on prend le type naturel
			if(!charToX(kind,d,data,buffer)){
				return(0);
			}
		}
		else if(ck==_bit){// Pour les champs bit, on prend le type naturel
			if(!charToX(kind,d,data,buffer)){
				return(0);
			}
		}
		else{// Pour les champs classe, on prend le nom de la contrainte
			strcpy(buffer,data);
		}
		if(!tp->fields()->set_default_value(fieldindex,buffer)){
			return(0);
		}
	}
	else{
		if(!tp->fields()->set_default_value(fieldindex,NULL)){
			return(0);
		}
	}
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GeParamCount(CWType typ,
						CWCharData folder){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->count_param(folder));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GeParamName(	CWType typ,
						CWFileName folder,
						int index,
						CWFileName name){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->get_param_name(folder,index,name));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetParam(CWType typ,
					CWFileName folder,
					CWFileName name,
					void** data,
					int* datasize){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->get_param(folder,name,data,datasize));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_SetParam(CWType typ,
					CWFileName folder,
					CWFileName name,
					void* data,
					int datasize){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->fields()->set_param(folder,name,data,datasize));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetXMLParam(	CWCallback Callback,
						CWType typ,
						CWFileName folder,
						CWFileName name,
						CWXMLTree* data){
bGenericMacMapApp*	gapp=(bGenericMacMapApp*)Callback;
bGenericType*		tp=(bGenericType*)typ;
	(*data)=(CWXMLTree)GetXMLParamFromType(gapp,tp,folder,name);
	return((*data)!=NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_SetXMLParam(	CWType typ,
						CWFileName folder,
						CWFileName name,
						CWXMLTree data){
bGenericType*	tp=(bGenericType*)typ;
	return(SetXMLParamToType(tp,folder,name,(bGenericXMLBaseElement*)data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_Iterate(	CWType typ,
					void* data,
					int(*proc)
					(CWObject,void*)){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->iterator()->iterate(data,proc));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetStyleCount(CWType typ){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->count());
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetStyleName(CWType typ, int index, CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
	if((index<1)||(index>tp->styles()->count())){
		return(0);
	}
	tp->styles()->get_name(index,data);
	return(1);
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_SetStyleName(CWType typ, int index, CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->set_name(index,data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetStyleCreator(CWType typ, int index){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->signature(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
CWXMLTree CWType_GetStyleRoot(CWType typ, int index){
bGenericType*	tp=(bGenericType*)typ;
	return((CWXMLTree)tp->styles()->get_root(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetStyleIndexFromName(CWType typ, CWCharData data){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->index((char*)data));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_GetStyleIndexFromRoot(CWType typ, CWXMLTree root){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->index((bStdXMLBaseElement*)root));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_AddStyle(CWType typ, CWXMLTree root){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->add((bStdXMLBaseElement*)root));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_RemoveStyle(CWType typ, int index){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->remove(index));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_CreateStyle(CWType typ, int signature){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->create(signature));
}

// ---------------------------------------------------------------------------
// 
// ------------
int CWType_EditStyle(CWType typ, int index, int signature){
bGenericType*	tp=(bGenericType*)typ;
	return(tp->styles()->edit(index,signature));
}
