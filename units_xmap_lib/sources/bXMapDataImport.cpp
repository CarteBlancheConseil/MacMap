//----------------------------------------------------------------------------
// File : bXMapDataImport.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, fills objects data from DBF, text, csv, ... file
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
// Problèmes des champs avec contrainte classe à régler
// Doit-on prendre l'intitulé ou l'index ?
// Ex avec les couleurs et la gestion des traduction des intitulés
//----------------------------------------------------------------------------
// 25/10/2007 creation.
// 11/02/2015 NAV to NS transformation.
//----------------------------------------------------------------------------

#include "bXMapDataImport.h"

#include <mox_intf/bEventLog.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/mm_errors.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bStdWait.h>
#include <mox_intf/bStdAlert.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

#include <MacMapSuite/MacMapSuiteCPP.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct val_rec{
	int		fk;
	void*	fdat;
	int		tk;
	void*	tdat;
}val_rec;

typedef struct it_prm{
	int		n;
	int		msk;
	bArray*	ft;
	int		fx;
	int		sz;
}it_prm;

typedef struct g_rec{
	bGenericGeoElement*	o;
	void*				val;
}g_rec;

// ---------------------------------------------------------------------------
// INT CHECK
// -----------
static int comp_int(const void* a, const void* b){
g_rec* ga=(g_rec*)a;
g_rec* gb=(g_rec*)b;

	if((*((int*)ga->val))>(*((int*)gb->val))){
		return(1);
	}
	if((*((int*)ga->val))<(*((int*)gb->val))){
		return(-1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
static int comp_double(const void* a, const void* b){
g_rec* ga=(g_rec*)a;
g_rec* gb=(g_rec*)b;
double	r=(*((double*)ga->val))-(*((double*)gb->val));
	if(r<0){
		return(-1);
	}
	if(r>0){
		return(1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// -----------
static int comp_str(const void* a, const void* b){
g_rec* ga=(g_rec*)a;
g_rec* gb=(g_rec*)b;
	return(strcmp((char*)ga->val,(char*)gb->val));
}

// ---------------------------------------------------------------------------
// 
// ------------
static int count(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
it_prm*				p=(it_prm*)prm;

	if(geo->masked()){
		if(p->msk){
			p->n++;
		}
	}
	else{
		p->n++;
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
static int fill(void *o, void *prm){
bGenericGeoElement*	geo=(bGenericGeoElement*)o;
it_prm*				p=(it_prm*)prm;
g_rec				g;

	if(geo->masked()){
		if(p->msk){
			p->n++;
			g.o=geo;
			g.val=malloc(p->sz);
			if(g.val==NULL){
fprintf(stderr,"malloc failed in fill\n");
				return(-1);
			}
			if(!geo->getValue(p->fx,g.val)){
			}
			p->ft->put(p->n,&g);
		}
	}
	else{
		p->n++;
		g.o=geo;
		g.val=malloc(p->sz);
		if(!geo->getValue(p->fx,g.val)){
			if(g.val==NULL){
fprintf(stderr,"malloc failed in fill\n");
				return(-1);
			}
		}
		p->ft->put(p->n,&g);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
void dump_line(int x, bStdTable* tbl, bStdFile* report, char* msg, int tpk, double res){
	if(report==NULL){
		return;
	}
	
	report->write("#");
	report->write(msg);
	report->write("\n");

char	buffer[1024];
void*	val=buffer;
char	value[1024];
char*	geom;

int		k;
int		d;
int		i;
	
	for(i=1;i<=tbl->CountFields();i++){
		tbl->FieldSign(i,&k);
		tbl->FieldDecimals(i,&d);
		tbl->ReadVal(x,i,val);
		if(k==_ivxs2){
			geom=ivs2text(tpk,res,*((ivertices**)val));
			report->write(geom);
			free(geom);
		}	
		else{
			if(!xToChar(k,d,val,value)){
				sprintf(value,"????");
			}
			report->write(value);
		}
		if(i<tbl->CountFields()){
			report->write("\t");
		}
		else{
			report->write("\n");
		}
	}
}

#pragma mark -
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapDataImport	::bXMapDataImport(bGenericXMLBaseElement* elt, 
								bGenericMacMapApp* gapp, 
								CFBundleRef bndl) 
				:bStdXMapWithModalIntf(elt,gapp,bndl)
				,_cols(sizeof(datajoin_act)){
	setclassname("dataimport");
	_report=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapDataImport::~bXMapDataImport(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapDataImport::create(bGenericXMLBaseElement* elt){
	return(new bXMapDataImport(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapDataImport::open(int* flags){
	bStdXMapWithModalIntf::open(flags);
	_tpi=1;
	_use_msk=false;
	_unique=false;
	_report=NULL;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataImport::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			join();
			break;
		case kExtProcessCallWithParams:
			join((datajoin_prm*)prm,true);
			break;
		case kExtProcessCallWithXMLTree:
			parse_tree();
			break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataImport::edit(void* prm){
_bTrace_("bXMapDataImport::edit",true);
int             status;
char            name[FILENAME_MAX];
char            ext[FILENAME_MAX];
char            path[SHRT_MAX];
char            msg[__MESSAGE_STRING_LENGTH_MAX__];
const char*		nsft[4]={"TXT","CSV","DBF","MMT"};
const char**	pnsft=nsft;
    
    if(NSGetFilePath(pnsft,
                     4,
                     cNSUTF8StringEncoding,
                     false,
                     NULL,
                     path)==cNSCancelButton){
_te_("user canceled");
        return(true);
    }
    splitPath(path,path,name);
char	*bf=strrchr(name,'.');
    if(bf){
        bf++;
        strcpy(ext,bf);
    }
int	sign=ext2sign(ext);
_tm_(path);
_tm_(name);
_tm_(ext);
_tm_(sign);

	if(sign==0){
_te_("unsupported format");
b_error_string(error_num(0,kXMapDataImportUnsupportedFormatErr),msg,getbundle());
bAlertStop(msg,"");
		return(true);
	}
	
double		reso=_gapp->document()->dist_resolution();
d2dvertex	origin=_gapp->document()->get_origin();
int			asrid=_gapp->document()->srid();
int			tsrid=_gapp->document()->srid();

// Récupération du SRID source par grille d'import
bGenericExt*	fexp=_gapp->xmapMgr()->find('IGrd');
	if(fexp){
import_grid		prm={0,NULL,NULL,tsrid};
		fexp->process(kExtProcessCallGetData,&prm);
		tsrid=prm.srid;
	}

	status=0;
	_tbl=wtbl_falloc(sign,
                     path,
                     name,
                     false,
                     &reso,
                     &origin.x,
                     &origin.y,
                     &tsrid,
                     &asrid,
                     &status);
	if(!_tbl){
_te_("table not allocated");
b_error_string(error_num(0,kXMapDataImportOpenErr),msg,getbundle());
bAlertStop(msg,"");
		return(true);
	}
	if(status<0){
_te_("wtbl_falloc failed with "+status);
b_error_string(error_num(0,kXMapDataImportOpenErr),msg,getbundle());
bAlertStop(msg,"");
		wtbl_free(_tbl);
		_tbl=NULL;
		return(true);
	}
	if((strlen(path)+strlen(name)+5)<FILENAME_MAX){
		sprintf(ext,"%s%s.burp",path,name);
	}
	else{
		sprintf(ext,"/%s.burp",name);
	}
	_report=new bStdFile(ext,"w");
	return(bStdXMapWithModalIntf::edit(prm));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapDataImport::wd_event(EventRef evt, WindowRef wd){
bool				b=true;
HICommand			cmd;
UInt32				clss;	
	
	clss=GetEventClass(evt);
	if(clss==kEventClassCommand){
		GetEventParameter(evt,kEventParamDirectObject,typeHICommand,NULL,sizeof(HICommand),NULL,&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
			case kHICommandCancel:
				DataBrowserClose(get_control(kXMapDataImportSignature,kXMapDataImportFieldsID));
				_use_msk=GetControl32BitValue(get_control(kXMapDataImportSignature,kXMapDataImportUseMaskedID));
				_unique=GetControl32BitValue(get_control(kXMapDataImportSignature,kXMapDataImportUniqueID));
				break;
				
			case kXMapDataImportTypesCmd:
				type_action();
				break;
	
			case kXMapDataImportActionCmd:
				act_action();
				break;
				
			case kXMapDataImportFieldsCmd:
				field_action();
				break;
				
			default:
				b=false;
				break;
		}
	}
	return(b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapDataImport::wd_init(WindowRef wd){
	_cur=0;

	populate_act();
	
ControlRef	c;

	c=get_control(kXMapDataImportSignature,kXMapDataImportTypesID);
	ClearPopupControl(c,1);
	PopulatePopupControlWithType(_gapp,c,0,_tpi);

	c=get_control(kXMapDataImportSignature,kXMapDataImportUseMaskedID);
	SetControl32BitValue(c,_use_msk);
	
	c=get_control(kXMapDataImportSignature,kXMapDataImportUniqueID);
	SetControl32BitValue(c,_unique);

	c=get_control(kXMapDataImportSignature,kXMapDataImportListID);
	DataBrowserInit(c,col_handler,col_notifier,(long)this);
	AddDataBrowserItems(c,
						kDataBrowserNoItem,
						_tbl->CountFields(),
						NULL,
						kXMapDataImportColumnsProperty);
						
	c=get_control(kXMapDataImportSignature,kXMapDataImportFieldsID);
	ClearPopupControl(c,3);
bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
	PopulatePopupControlWithFields(tp,c,kOBJ_Vertices_,1);
	if(_tbl->signature()!=kTableMMText){
		DisablePopupControlMenuItem(c,3);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bXMapDataImport::col_handler(ControlRef browser, 
									DataBrowserItemID itemID, 
									DataBrowserPropertyID property, 
									DataBrowserItemDataRef itemData, 
									Boolean changeValue){

CFStringRef			name;
char				cname[256];
bXMapDataImport*		x=(bXMapDataImport*)GetControlReference(browser);
	
	switch(property){
		case kXMapDataImportColumnsProperty:
			if(!changeValue){
				x->_tbl->FieldName(itemID,cname);
				name=CFStringCreateWithCString(kCFAllocatorDefault,cname,CFStringGetSystemEncoding());
				SetDataBrowserItemDataText(itemData,name);
				CFRelease(name);
			}
			break;
		case kDataBrowserItemIsSelectableProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,true)){
			}
			break;
		case kDataBrowserItemIsContainerProperty:
			if(SetDataBrowserItemDataBooleanValue(itemData,false)){
			}
			break;
		default:
			break;			
	}
	return(noErr);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::col_notifier(	ControlRef browser, 
									DataBrowserItemID item, 
									DataBrowserItemNotification msg){
bXMapDataImport*	x=(bXMapDataImport*)GetControlReference(browser);
    switch(msg){  
        case kDataBrowserItemAdded:
			break;
        case kDataBrowserItemRemoved:
            break;
        case kDataBrowserContainerOpened:
            break;
        case kDataBrowserContainerClosing:
            break;
        case kDataBrowserContainerClosed:
			break;
        case kDataBrowserItemSelected:
			x->col_action(item);
            break;
        case kDataBrowserItemDeselected:
			if(DataBrowserCountSelected(browser)==0){
				x->col_action(0);
			}
			break;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::col_action(DataBrowserItemID item){
ControlRef	c=get_control(kXMapDataImportSignature,kXMapDataImportActionID);
	_cur=item;
	if(!test_ignore(_cur)){
		DisablePopupControlMenuItem(c,kDataJoinActionIgnore);
	}
	else{
		EnablePopupControlMenuItem(c,kDataJoinActionIgnore);
	}
	if(!test_search(_cur)){
		DisablePopupControlMenuItem(c,kDataJoinActionSearch);
	}
	else{
		EnablePopupControlMenuItem(c,kDataJoinActionSearch);
	}
	if(!test_fill(_cur)){
		DisablePopupControlMenuItem(c,kDataJoinActionFill);
	}
	else{
		EnablePopupControlMenuItem(c,kDataJoinActionFill);
	}
	if(!test_create(_cur)){
		DisablePopupControlMenuItem(c,kDataJoinActionCreate);
	}
	else{
		EnablePopupControlMenuItem(c,kDataJoinActionCreate);
	}
	if(_cur){
datajoin_act	action;
		_cols.get(_cur,&action);
		SetControl32BitValue(c,action.k);
		SetControlAbility(c,true);
		c=get_control(kXMapDataImportSignature,kXMapDataImportFieldsID);
		if(action.fidx>0){
			SetControl32BitValue(c,(action.fidx-kOBJ_Vertices_)+3);
		}
		else{
			SetControl32BitValue(c,1);
		}
		if(	(action.k==kDataJoinActionIgnore)	||
			(action.k==kDataJoinActionCreate)	){
			SetControlAbility(c,false);
		}
		else{
			SetControlAbility(c,true);
		}
	}
	else{
		SetControlAbility(c,false);
		c=get_control(kXMapDataImportSignature,kXMapDataImportFieldsID);
		SetControlAbility(c,false);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::type_action(){
ControlRef	c;

	c=get_control(kXMapDataImportSignature,kXMapDataImportTypesID);
	_tpi=GetControl32BitValue(c);
bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
	c=get_control(kXMapDataImportSignature,kXMapDataImportFieldsID);
	ClearPopupControl(c,3);
	PopulatePopupControlWithFields(tp,c,kOBJ_Vertices_,1);
	if(_tbl->signature()!=kTableMMText){
		DisablePopupControlMenuItem(c,3);
	}
	populate_act();
	col_action(_cur);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::act_action(){
	if(!_cur){
		return;
	}
ControlRef		c;
	c=get_control(kXMapDataImportSignature,kXMapDataImportActionID);
datajoin_act	action;
	_cols.get(_cur,&action);
	action.k=GetControl32BitValue(c);
	_cols.put(_cur,&action);
	
	c=get_control(kXMapDataImportSignature,kXMapDataImportFieldsID);
	if(	(action.k==kDataJoinActionIgnore)	||
		(action.k==kDataJoinActionCreate)	){
		SetControlAbility(c,false);
	}
	else{
		SetControlAbility(c,true);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::field_action(){
	if(!_cur){
		return;
	}
ControlRef		c;
	c=get_control(kXMapDataImportSignature,kXMapDataImportFieldsID);
datajoin_act	action;
	_cols.get(_cur,&action);
	action.fidx=(GetControl32BitValue(c)-3)+kOBJ_Vertices_;
	_cols.put(_cur,&action);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::populate_act(){
	_cols.reset();

bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
wbase			bse=tp->fields();
char			name[256];
datajoin_act	action;
bool			srch=false;
int				k;

	for(int i=1;i<=_tbl->CountFields();i++){
		_tbl->FieldName(i,name);
		_tbl->FieldSign(i,&k);
		action.fidx=bse->get_index(name);
		if(action.fidx){
			if(srch){
				action.k=kDataJoinActionFill;
			}
			else if((k==_char)	||
					(k==_int)	||
					(k==_bool)	||
					(k==_double)||
					(k==_date)	||
					(k==_time)	){
				action.k=kDataJoinActionSearch;
				srch=true;
			}
			else{
				action.k=kDataJoinActionFill;
			}
		}
		else{
			if(	(k==_char)	||
				(k==_int)	||
				(k==_bool)	||
				(k==_double)||
				(k==_date)	||
				(k==_time)	){
				action.k=kDataJoinActionCreate;
			}
			else{
				action.k=kDataJoinActionIgnore;
			}
		}
		_cols.add(&action);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataImport::test_ignore(int f){
	if(!f){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataImport::test_search(int f){
	if(!f){
		return(false);
	}
datajoin_act	action;
bool			srch=false;
int				i;
	for(i=1;i<=_cols.count();i++){
		_cols.get(i,&action);
		if(action.k==kDataJoinActionSearch){
			srch=true;
			break;
		}
	}
	if(!srch){
		_tbl->FieldSign(f,&i);
		if( (i!=_char)	&&
			(i!=_int)	&&
			(i!=_bool)	&&
			(i!=_double)&&
			(i!=_date)	&&
			(i!=_time)	){
			return(false);
		}
	}
	return((srch&&(f==i))||(!srch));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataImport::test_fill(int f){
	if(!f){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bXMapDataImport::test_create(int f){
	if(!f){
		return(false);
	}
int				k;
	_tbl->FieldSign(f,&k);
	if( (k!=_char)	&&
		(k!=_int)	&&
		(k!=_bool)	&&
		(k!=_double)&&
		(k!=_date)	&&
		(k!=_time)	){
		return(false);
	}

datajoin_act	action;
char			name[256];
	_cols.get(f,&action);
	_tbl->FieldName(f,name);
bGenericType*	tp=_gapp->typesMgr()->get(_tpi);
wbase			bse=tp->fields();
	return(bse->get_index(name)==0);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::parse_tree(){
_bTrace_("bXMapDataImport::parse_tree",true);
datajoin_prm			prm;
datajoin_act			dtact;
bGenericXMLBaseElement*	elt;
bGenericType*			tp;
int						sign,status=0;
char					name[FILENAME_MAX];
char					path[PATH_MAX];
char					value[_values_length_max_];
char*					ext;

// Ouverture de la table
	elt=getelement(1);
	if(!elt){
		return;
	}
	elt->getvalue(path);
	elt=getelement(2);
	if(!elt){
		return;
	}
	elt->getvalue(name);
	ext=strrchr(name,'.');
	if(!ext){
		return;
	}
	ext++;
	sign=ext2sign(ext);
	
double		reso=_gapp->document()->dist_resolution();
d2dvertex	origin=_gapp->document()->get_origin();
int			asrid=_gapp->document()->srid();
int			tsrid=_gapp->document()->srid();
	
// Récupération du SRID source par grille d'import
bGenericExt*	fexp=_gapp->xmapMgr()->find('IGrd');
	if(fexp){
import_grid		prm={0,NULL,NULL,tsrid};
		fexp->process(kExtProcessCallGetData,&prm);
		tsrid=prm.srid;
	}
	
	status=0;
	_tbl=wtbl_falloc(	sign,
						path,
						name,
						false, 
						&reso,
						&origin.x,
						&origin.y,
						&tsrid,
						&asrid,
						&status);					
	if(!_tbl){
_te_("table not allocated");
b_error_string(error_num(0,kXMapDataImportOpenErr),value,getbundle());
bAlertStop(value,"",true);
		return;
	}
	if(status<0){
_te_("wtbl_falloc failed with "+status);
b_error_string(error_num(0,kXMapDataImportOpenErr),value,getbundle());
bAlertStop(value,"",true);
		wtbl_free(_tbl);
		_tbl=NULL;
		return;
	}
	
//	Type
	elt=getelement(3);
	if(!elt){
		wtbl_free(_tbl);
		_tbl=NULL;
		return;
	}
	elt->getvalue(value);
	_tpi=_gapp->typesMgr()->index(value);
	if(_tpi==0){
		wtbl_free(_tbl);
		_tbl=NULL;
		return;
	}
	tp=_gapp->typesMgr()->get(_tpi);
	
//	Masqués
	elt=getelement(4);
	if(!elt){
		wtbl_free(_tbl);
		_tbl=NULL;
		return;
	}
	elt->getvalue(value);
	_use_msk=atoi(value);
	
//	Unique
	elt=getelement(5);
	if(!elt){
		wtbl_free(_tbl);
		_tbl=NULL;
		return;
	}
	elt->getvalue(value);
	_unique=atoi(value);
	
// Colonne/Action->Champ
	for(int i=1;i<=(countelements()-5);i+=2){
		elt=getelement((i-1)+6);
		if(!elt){
			break;
		}
		elt->getvalue(value);
_tm_(i+"a:"+value);
		dtact.k=atoi(value);
		if(	(dtact.k<kDataJoinActionIgnore)	||
			(dtact.k>kDataJoinActionCreate)	){
			break;
		}
		elt=getelement((i-1)+7);
		if(!elt){
			break;
		}
		elt->getvalue(value);
_tm_(i+"b:"+value);
		dtact.fidx=tp->fields()->get_index(value);		
		_cols.add(&dtact);
	}
	
	prm.cols=&_cols;
	prm.tpi=_tpi;
	prm.tbl=_tbl;
	prm.use_msk=_use_msk;
	prm.unique=_unique;
	join(&prm,true);
	wtbl_free(_tbl);
	_tbl=NULL;
	_cols.reset();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::join(){
bEventLog	log(_gapp,
				getbundle(),
				kXMapDataImportMessageID,
				GetSignature(this));
datajoin_prm	prm;
	prm.cols=&_cols;
	prm.tpi=_tpi;
	prm.tbl=_tbl;
	prm.use_msk=_use_msk;
	prm.unique=_unique;
	join(&prm,false);
	wtbl_free(_tbl);
	_tbl=NULL;
	_cols.reset();
	log.close();
}

// ---------------------------------------------------------------------------
// 
// -----------
void bXMapDataImport::join(datajoin_prm* prm, bool mode){
_bTrace_("bXMapDataImport::join(datajoin_prm*,bool)",false);

char	msg[__ERROR_STRING_LENGTH_MAX__];
char	exp[__ERROR_STRING_LENGTH_MAX__];
char	str[__ERROR_STRING_LENGTH_MAX__];

// Ajout des champs
	if(add_fields(prm)){
		error_string(error_num(_bDataBaseErr_,_bDataBaseAddFieldErr_),msg);
		b_message_string(kXMapDataImportReadContinueID,exp,getbundle(),0);
bAlertWarningYes	alrt(msg,exp,mode);
		if(!alrt.result()){
			return;
		}
	}

bGenericType*	tp=_gapp->typesMgr()->get(prm->tpi);
wbase			bse=tp->fields();
datajoin_act	action;
bArray			vals(sizeof(val_rec),prm->cols->count());
val_rec			val;
int				i,sz,srchx=0,flcnt=0;
bool			err=false,warn=false;
int				tpk;
double			reso;

	tpk=tp->kind();
	reso=GetPrecisionCoef(_gapp);
	
// Allocation buffers et recherche de la clef 
	for(i=1;i<=prm->cols->count();i++){
		prm->cols->get(i,&action);
		if(action.fidx>0){
			prm->tbl->FieldSign(i,&val.fk);
			bse->get_kind(action.fidx,&val.tk);
			prm->tbl->FieldSize(i,&sz);
			val.fdat=malloc(sz);
			bse->get_size(action.fidx,&sz);
			val.tdat=malloc(sz);			
		}
		else{
			val.fk=0;
			val.fdat=NULL;
			val.tk=0;
			val.tdat=NULL;
		}
		vals.put(i,&val);
		switch(action.k){
			case kDataJoinActionSearch:
				srchx=i;
				break;
			case kDataJoinActionFill:
			case kDataJoinActionCreate:
				flcnt++;
				break;
		}
	}

// Pas de remplissage/Clef non définie
	if((flcnt==0)||(srchx==0)){
		for(i=1;i<=vals.count();i++){
			vals.get(i,&val);
			if(val.fdat){
				free(val.fdat);
			}
			if(val.tdat){
				free(val.tdat);
			}
		}
		if(flcnt==0){
			b_error_string(error_num(0,kXMapDataImportNoDataToFillErr),msg,getbundle());
		}
		else{
			b_error_string(error_num(0,kXMapDataImportKeyNotDefinedErr),msg,getbundle());
		}
bAlertStop	alrt(msg,"",mode);
		return;
	}
	
datajoin_act	ref_act;
	prm->cols->get(srchx,&ref_act);

// Choix de la fonction de comparaison
int	k;
	bse->get_kind(ref_act.fidx,&k);
	switch(k){
		case _char:
			_comp=comp_str;
_tm_("char comp");
			break;
		case _int:
		case _bool:
_tm_("int comp");
			_comp=comp_int;
			break;	
		case _double:
		case _date:
		case _time:
_tm_("double comp");
			_comp=comp_double;
			break;	
	}

// Indexation des objets
it_prm	it;
	it.msk=prm->use_msk;
	it.n=0;
	tp->iterator()->iterate(&it,count);
bArray	l(sizeof(g_rec),it.n);
	it.ft=&l;
	it.n=0;
	it.fx=ref_act.fidx;
	bse->get_size(ref_act.fidx,&it.sz);	
	tp->iterator()->iterate(&it,fill);
	l.sort(_comp);	

// Boucle principale
int			j,d;
g_rec		ga,gref;
val_rec		ref_val;
ivertices*	vxs;

//DEBUG
/*FILE	*fp=fopen("/list.txt","w");
for(i=1;i<l.count();i++){
	l.get(i,&gref);
	fprintf(fp,"%d\n",*((int*)(gref.val)));
}
fclose(fp);*/
//DEBUG

	bse->get_decs(ref_act.fidx,&d);
	vals.get(srchx,&ref_val);
	gref.val=ref_val.tdat;
		
int		tk,first,last;
bool	fill;

char				ttl[__MESSAGE_STRING_LENGTH_MAX__];
	GetName(this,ttl);
	b_message_string(kXMapDataImportMessageID,msg,getbundle(),0);
bProgressWait wt(ttl,msg,true,true,_tbl->CountRecords());

_tm_("-- DataImport REPORT BEGIN --");
	for(i=1;i<=prm->tbl->CountRecords();i++){
		if(!wt.set_progress(i)){
			break;
		}
		if(prm->tbl->ReadVal(i,srchx,ref_val.fdat)){
_te_("_tbl->ReadVal() at "+i+"->"+srchx);
		error_string(error_num(_bDataBaseErr_,_bDataBaseReadErr_),msg);
bAlertStop	alrt(msg,"",mode);
			break;
		}
		switch(ref_val.fk){
			case _char:
				fill=charToX(ref_val.tk,d,(char*)ref_val.fdat,ref_val.tdat);
				break;
			case _int:
			case _bool:
				fill=intToX(ref_val.tk,d,*((int*)ref_val.fdat),ref_val.tdat);
//_tm_(*((int*)ref_val.fdat));
				break;	
			case _double:
				fill=doubleToX(ref_val.tk,d,*((double*)ref_val.fdat),ref_val.tdat);
				break;	
			case _date:
				fill=dateToX(ref_val.tk,d,*((double*)ref_val.fdat),ref_val.tdat);
				break;	
			case _time:
				fill=timeToX(ref_val.tk,d,*((double*)ref_val.fdat),ref_val.tdat);
				break;	
			default:
				fill=false;
				break;
		}
		if(!fill){
			continue;
		}
// Recherche de la plage
		tk=l.search(&gref,_comp);
		if(!tk){
			warn=true;
			b_message_string(kXMapDataImportLineNumberID,msg,getbundle(),0);
			b_error_string(error_num(0,kXMapDataImportNoObjectErr),exp,getbundle());
			sprintf(str,"%s %d : %s",msg,i,exp);
			dump_line(i,prm->tbl,_report,str,tpk,reso);
			//fprintf(stderr,str);
			continue;
		}
		first=last=tk;
		l.get(tk,&ga);
		for(;first>0;first--){
			l.get(first,&ga);
			if(_comp(&gref,&ga)){
				first++;
				break;
			}
		}
		if(first<1){
			first=1;
		}
		for(;last<=l.count();last++){
			l.get(last,&ga);
			if(_comp(&gref,&ga)){
				last--;
				break;
			}
		}
		if(last>l.count()){
			last=l.count();
		}
		
		if(first!=last){
			if(prm->unique){
				warn=true;
				b_message_string(kXMapDataImportLineNumberID,msg,getbundle(),0);
				b_error_string(error_num(0,kXMapDataImportNotUniqueErr),exp,getbundle());
				sprintf(str,"%s %d : %s",msg,i,exp);
				dump_line(i,prm->tbl,_report,str,tpk,reso);
				//fprintf(stderr,str);
				continue;
			}
		}
			
// Lecture/Ecriture
		for(j=1;j<=prm->tbl->CountFields();j++){
			prm->cols->get(j,&action);
			if(	(action.k!=kDataJoinActionFill)		&&
				(action.k!=kDataJoinActionCreate)	){
				continue;
			}
			vals.get(j,&val);
			
			if(prm->tbl->ReadVal(i,j,val.fdat)){
_te_("_tbl->ReadVal() at "+i+"->"+srchx);
				error_string(error_num(_bDataBaseErr_,_bDataBaseReadErr_),msg);
bAlertStop	alrt(msg,"",mode);
				err=true;
				break;
			}
			
			if(val.fk==_ivxs2){
				if(prm->tbl->ReadVal(i,j,&vxs)){
_te_("_tbl->ReadVal() at "+i+"->"+j);
					error_string(error_num(_bDataBaseErr_,_bDataBaseReadErr_),msg);
bAlertStop	alrt(msg,"",mode);
					err=true;
					break;
				}			
			}
			else{
				if(prm->tbl->ReadVal(i,j,val.fdat)){
_te_("_tbl->ReadVal() at "+i+"->"+j);
					error_string(error_num(_bDataBaseErr_,_bDataBaseReadErr_),msg);
bAlertStop	alrt(msg,"",mode);
					err=true;
					break;
				}				
				switch(val.fk){
					case _char:
						fill=charToX(val.tk,d,(char*)val.fdat,val.tdat);
						break;
					case _int:
					case _bool:
						fill=intToX(val.tk,d,*((int*)val.fdat),val.tdat);
						break;	
					case _double:
						fill=doubleToX(val.tk,d,*((double*)val.fdat),val.tdat);
						break;	
					case _date:
						fill=dateToX(val.tk,d,*((double*)val.fdat),val.tdat);
						break;	
					case _time:
						fill=timeToX(val.tk,d,*((double*)val.fdat),val.tdat);
						break;
					default:
						fill=false;
						break;
				}
			}
			if(!fill){
				continue;
			}
			for(tk=first;tk<=last;tk++){
				l.get(tk,&ga);
				if((val.fk==_ivxs2)&&(val.tk==_ivxs2)){
					ga.o->setVertices(vxs);
				}
				else if(!ga.o->setValue(action.fidx,val.tdat)){
_te_("ga.o->setValue() at "+action.fidx);
					error_string(error_num(_bDataBaseErr_,_bDataBaseReadErr_),msg);
bAlertStop	alrt(msg,"",mode);
					err=true;
					break;
				}
			}
			if(err){
				break;
			}
		}
		if(err){
			break;
		}
	}
_tm_("-- DataImport REPORT END --");
	if(_report){
		delete _report;
		_report=NULL;
	}
	
// Libération
	for(i=1;i<=l.count();i++){
		l.get(i,&ga);
		if(ga.val){
			free(ga.val);
		}
	}
	for(i=1;i<=vals.count();i++){
		vals.get(i,&val);
		if(val.fdat){
			free(val.fdat);
		}
		if(val.tdat){
			free(val.tdat);
		}
	}

	if(warn){
		b_message_string(kXMapDataImportErrorsOccursID,msg,getbundle(),0);
		b_message_string(kXMapDataImportReadLogID,exp,getbundle(),0);
bAlertStop	alrt(msg,exp,mode);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bXMapDataImport::add_fields(datajoin_prm* prm){
bGenericType*	tp=_gapp->typesMgr()->get(prm->tpi);
wbase			bse=tp->fields();
datajoin_act	action;
int				k,l,d;
char			name[256];
int				err=0;

	for(int i=1;i<=prm->cols->count();i++){
		prm->cols->get(i,&action);
		if(action.k!=kDataJoinActionCreate){
			continue;
		}
		prm->tbl->FieldName(i,name);
		prm->tbl->FieldSign(i,&k);
		if(	(k!=_char)	&&
			(k!=_int)	&&
			(k!=_bool)	&&
			(k!=_double)&&
			(k!=_time)	&&
			(k!=_date)	){
			continue;
		}
		prm->tbl->FieldLength(i,&l);
		prm->tbl->FieldDecimals(i,&d);
		if(bse->add(k,l,d,name)){
			err--;
			continue;
		}
		action.fidx=bse->get_index(name);
		prm->cols->put(i,&action);
	}
	return(err);
}

