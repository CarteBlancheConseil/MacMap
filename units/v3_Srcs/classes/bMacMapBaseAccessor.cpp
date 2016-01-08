//----------------------------------------------------------------------------
// File : bMacMapBaseAccessor.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap base accessor class (encapsulation of bStdBaseAccessor for event management)
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
// 13/01/2004 creation.
//----------------------------------------------------------------------------

#include "bMacMapBaseAccessor.h"
#include "bFieldEvent.h"
#include "bMacMapApp.h"
#include <MacMapSuite/wbase.h>
#include <mox_intf/endian.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapBaseAccessor	::bMacMapBaseAccessor(void* tp)
					:bStdBaseAccessor(){
	_bse=NULL;
	_tp=tp;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bMacMapBaseAccessor::~bMacMapBaseAccessor(){
	if(_bse){
		delete _bse;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::read(int o, int f, void* val){
	return(_bse->read(o,f,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::write(int o, int f, void* val){
	return(_bse->write(o,f,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::count(){
	return(_bse->count());
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::add(int kind, int len, int decs, const char *name){
int	status=_bse->add(kind,len,decs,name);
	if(status){
		return(status);
	}
field_info	fi;
	
	fi.fidx=_bse->count();
	fi.fid=_bse->get_id(fi.fidx);
	_bse->get_kind(fi.fidx,&fi.kind);
	_bse->get_len(fi.fidx,&fi.len);
	_bse->get_decs(fi.fidx,&fi.decs);
	_bse->get_name(fi.fidx,fi.name);
	fi.tp=(bGenericType*)_tp;
	eventmgr->create(&fi);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::modify(int f, int kind, int len, int decs, const char* name){
field_info	fi;
	fi.fidx=f;
	fi.fid=_bse->get_id(fi.fidx);
	_bse->get_kind(fi.fidx,&fi.kind);
	_bse->get_len(fi.fidx,&fi.len);
	_bse->get_decs(fi.fidx,&fi.decs);
	_bse->get_name(fi.fidx,fi.name);
	fi.tp=(bGenericType*)_tp;
int	status=_bse->modify(f,kind,len,decs,name);
	if(status){
		return(status);
	}
	eventmgr->modify(&fi);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::remove(int f){
field_info	fi;
	fi.fidx=f;
	fi.fid=_bse->get_id(fi.fidx);
	_bse->get_kind(fi.fidx,&fi.kind);
	_bse->get_len(fi.fidx,&fi.len);
	_bse->get_decs(fi.fidx,&fi.decs);
	_bse->get_name(fi.fidx,fi.name);
	fi.tp=(bGenericType*)_tp;
int	status=_bse->remove(f);
	if(status){
		return(status);
	}
	eventmgr->kill(&fi);
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_kind(int	f,	int	*k){
	return(_bse->get_kind(f,k));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_len(int	f,	int	*l){
	return(_bse->get_len(f,l));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_decs(int f,	int	*d){
	return(_bse->get_decs(f,d));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_name(int f,	char* name){
	return(_bse->get_name(f,name));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_size(int f, int* sz){
	return(_bse->get_size(f,sz));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::get_id(int f){
	return(_bse->get_id(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::get_id(const char* name){
	return(_bse->get_id(name));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::get_index(int fid){
	return(_bse->get_index(fid));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::get_index(const char* name){
	return(_bse->get_index(name));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::count_constraints(int f){
	return(_bse->count_constraints(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapBaseAccessor::get_constraints_kind(int f){
	return(_bse->get_constraints_kind(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_constraint(int f, int n, void* val){
	return(_bse->get_constraint(f,n,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::set_constraint(int f, int n, void* val){
	return(_bse->set_constraint(f,n,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::add_constraint(int f, int kind, void* val){
	return(_bse->add_constraint(f,kind,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::rmv_constraint(int f, int n){
	return(_bse->rmv_constraint(f,n));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapBaseAccessor::get_constraint_index(int f, void* val){
	return(_bse->get_constraint_index(f,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::set_constraint_index(int f, int from, int to){
	return(_bse->set_constraint_index(f,from,to));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::has_default_value(int f){
	return(_bse->has_default_value(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_default_value(int f, void* val){
	return(_bse->get_default_value(f,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::set_default_value(int f, void* val){
	return(_bse->set_default_value(f,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::is_delprotected(int f){
	return(_bse->is_delprotected(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::set_delprotected(int f, bool b){
	return(_bse->set_delprotected(f,b));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::is_writeprotected(int f){
	return(_bse->is_writeprotected(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::set_writeprotected(int f, bool b){
	return(_bse->set_writeprotected(f,b));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::is_hidden(int f){
	return(_bse->is_hidden(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::set_hidden(int f, bool b){
	return(_bse->set_hidden(f,b));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::is_dyn(int f){
	return(_bse->is_dyn(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::is_dyn_ref(int f){
	return(_bse->is_dyn_ref(f));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapBaseAccessor::count_records(){
	return(_bse->count_records());
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::kill_record(int r){
	return(_bse->kill_record(r));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::unkill_record(int r){
	return(_bse->unkill_record(r));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::killed_record(int r){
	return(_bse->killed_record(r));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::h_read(int	o,	int	f,	void	*val){
	return(_bse->h_read(o,f,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
int	bMacMapBaseAccessor::h_write(int	o,	int	f,	void	*val){	
	return(_bse->h_write(o,f,val));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapBaseAccessor::count_param(const char* sub){
	return(_bse->count_param(sub));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_param_name(const char* sub, int idx, char* name){	
	return(_bse->get_param_name(sub,idx,name));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::get_param(const char* sub, const char* name, void** p, int* sz){	
	return(_bse->get_param(sub,name,p,sz));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMacMapBaseAccessor::set_param(const char* sub, const char* name, void* p, int sz){	
	return(_bse->set_param(sub,name,p,sz));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapBaseAccessor::add_dyn(const char* path, const char* name, int bse_f, int tbl_f){
	return(_bse->add_dyn(path,name,bse_f,tbl_f));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapBaseAccessor::signature(){
	return(_bse->signature());
}

// ---------------------------------------------------------------------------
// 
// -----------
int bMacMapBaseAccessor::version(){
	return(_bse->version());
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapBaseAccessor::open(const char* hpath, int wanted_srid, double wanted_reso){
int	status;
	_bse=wbse_openws(hpath,wanted_srid,wanted_reso,&status);
	if(!_bse){
		status=-10;
	}
	return(status);
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapBaseAccessor::create(const char* hpath,  
								int kind,
								int srid,
								double reso,
								double u2m,
								const char* name,
								const char* data,
								int hkind){
int	status;
#ifdef __LITTLE_ENDIAN__
	LBSwapWord(&kind,sizeof(UInt32));
#endif
	_bse=wbse_create(	kind,
						hpath,
						name,
						data,
						hkind,
						reso,
						u2m,
						srid,
						&status);
	if(!_bse){
		status=-10;
	}
	return(status);
}

