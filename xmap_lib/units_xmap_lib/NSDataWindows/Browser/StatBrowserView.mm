//----------------------------------------------------------------------------
// File : StatBrowserView.mm
// Project : MacMap
// Purpose : Objective C++ source file :  Browser view classes for stat browsers
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
 
//----------------------------------------------------------------------------
// 18/04/2014 creation.
//----------------------------------------------------------------------------

#import "StatBrowserView.h"
#import "BrowserColumn.h"
#import <mox_intf/mm_messages.h>
#import <MacMapSuite/bTrace.h>
#import <MacMapStats/MMsIO.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation MMsPath

// ---------------------------------------------------------------------------
// 
// -----------
-(id)init:(const char*)fpath
	 name:(const char*)fname{
	_fpath=NULL;
	_fname=NULL;
	if(self=[super init]){
		_fpath=strdup(fpath);
		_fname=strdup(fname);
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)dealloc{
	if(_fpath){
		free(_fpath);
	}
	if(_fname){
		free(_fname);
	}
    [super dealloc];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)getPath:(char*)fpath
		  name:(char*)fname{
	fpath[0]=0;
	fname[0]=0;
	if(_fpath){
		strcpy(fpath,_fpath);
	}
	if(_fname){
		strcpy(fname,_fname);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation StatBrowserHeaderView

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[StatBrowserHeaderView initWithFrame]",true);
    self=[super initWithFrame:frameRect];
    if(self){
        _acol=[[NSMutableArray alloc] init];
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
	[_acol release];
	_acol=nil;
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BrowserColumn*) allocator:(bArray*)lins
					  field:(int)fld
					   calc:(void*)clc
					  start:(float)strt
					  width:(float)w{
BrowserColumn*	col=NULL;
int				k;
	
	if(fld){
		_tp->fields()->get_kind(fld,&k);
		if(_tp->fields()->is_hidden(fld)){
			col=[[BrowserColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app]];
		}
		else{
			col=[[BrowserFieldStaticColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app] field:fld type:_tp];
		}
	}
	else if(clc){
		col=[[BrowserCalcColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app] calc:(bGenericCalc*)clc];
	}
	else{
		col=[[BrowserColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app]];
	}
	return(col);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getPanelInfos:(char*)name
		   signature:(char*)sign{
long	sgn=kStatBrowserPanelContentSignature;
	b_message_string("browser",name,_ext->getbundle(),1);
	sprintf(sign,"%.4s",(char*)&sgn);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)read_p{
_bTrace_("[StatBrowserHeaderView read_p]",true);
	if([self loadPath]==nil){
		[super read_p];
		return;
	}
bGenericXMLBaseElement*		root=NULL;
char	directory[PATH_MAX];
char	fname[FILENAME_MAX];
char	aname[FILENAME_MAX];
void*	buffer;
int		sz;
	
	[self getParamsInfos:directory name:fname];
	[[self loadPath] getPath:directory name:aname];
_tm_("analysis read at "+directory+"/"+aname);
	
	if(MMsIOReadParam(directory,aname,&buffer,&sz,fname)==0){
		if(!GetXMLParamFromBuffer([self app],&root,buffer,sz)){
		}
		free(buffer);
	}
	
	if(!root){
		[self initType];
		return;
	}
	
col_p	prm;
	prm.nam[0]=0;
	prm.knd=0;
	prm.fx=0;
	prm.cx=NULL;
	prm.h=[BrowserColumn stdColumnWidth];
	prm.strt=0;
	prm.arr=_cols;
	prm.objs=_lines;
	prm.tp=_tp;
	prm.mgr=[self app]->calcMgr();
	prm.gapp=[self app];
	prm.x=self;
	
	root->dotoall(&prm,0,readProc);
	
	[self app]->classMgr()->ReleaseXMLInstance(root);
	
	[_obj putBounds];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(MMsPath*)loadPath{
	return [[[self window] windowController] loadPath];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(NSMutableArray*)analysisColumns{
	return _acol;
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation StatBrowserView
// ---------------------------------------------------------------------------
//
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[StatBrowserView initWithFrame]",true);
    self=[super initWithFrame:frameRect];
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(MMsMatrix*)matrix{
_bTrace_("[StatBrowserView matrix]",true);
double			x;
MMsMatrix*		data=MMsMatrixAlloc([_hdr lines]->count(),[_hdr selectedColumns]->count());
BrowserColumn*	col;
	
	for(long i=1;i<=[_hdr lines]->count();i++){
		for(long j=1;j<=[_hdr selectedColumns]->count();j++){
			[_hdr selectedColumns]->get(j,&col);
			x=[col value:i-1 
				 inArray:[_hdr lines]];
			MMsSetDouble(data,i,j,x);
		}
	}
	return data;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(MMsMatrix*)onlySelectedMatrix{
double			x;
BrowserColumn*	col;
long			nl=0;
	
	for(long i=0;i<[_hdr lines]->count();i++){
		[_hdr selectedColumns]->get(1,&col);
		if([col lineSelected:i inArray:[_hdr lines]]){
			nl++;
		}
	}
	
MMsMatrix*	data=MMsMatrixAlloc(nl,[_hdr selectedColumns]->count());
	
	nl=0;
	for(long i=1;i<=[_hdr lines]->count();i++){
		[_hdr selectedColumns]->get(1,&col);
		if([col lineSelected:i-1 inArray:[_hdr lines]]){
			nl++;
			for(long j=1;j<=[_hdr selectedColumns]->count();j++){
				[_hdr selectedColumns]->get(j,&col);
				x=[col value:i-1 
					 inArray:[_hdr lines]];
				MMsSetDouble(data,nl,j,x);
			}			
		}
	}
	return data;
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

