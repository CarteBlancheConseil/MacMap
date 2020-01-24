//----------------------------------------------------------------------------
// File : StatStdWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Window controller classes for statistical panels
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
// 02/06/2014 creation.
//----------------------------------------------------------------------------

#import "StatStdWindowController.h"
#import "StatBrowserView.h"
#import <mox_intf/Type_Utils.h>
#import <mox_intf/xmldesc_utils.h>
#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/C_Utils.h>
#import <MacMapStats/MMsIO.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation StatStdPanel

// ---------------------------------------------------------------------------
// 
// -----------
static bool add_oid(bGenericXMLBaseElement *elt, void *prm, int indent){
bArray*		arr=(bArray*)prm;	
char		clssname[_values_length_max_];
int			k;
	
	elt->getclassname(clssname);
	if(!strcmp("int",clssname)){
		elt->getvalue(clssname);
		k=atoi(clssname);
		return(arr->add(&k));
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[StatStdPanel close]",true);
_tm_((void*)self);
StatStdWindowController* mmc=(StatStdWindowController*)[self windowController];
	[mmc cleanupSheets];
	[super close];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
@implementation StatStdWindowController

// ---------------------------------------------------------------------------
// 
// ------------
-(id)init{
_bTrace_("[StatStdWindowController init]",true);
	self=[super init];
_tm_((void*)self);
	if(self){
		init_analysis(_anap,[self analysisType]);
        _lpath=nil;
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[StatStdWindowController dealloc]",true);
_tm_((void*)self);
	if(_lpath){
		[_lpath release];
		_lpath=nil;
	}	
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)checkMacMapEvent{
    if([_brw checkMacMapEvent]==TRUE){
        [self cleanupSheets];
    }
}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doSaveAs:(id)sender{
NSSavePanel*	sav=[NSSavePanel savePanel];
	[sav setAllowedFileTypes:[NSArray arrayWithObjects:[self analysisExt],nil]];
	[sav beginSheetForDirectory:nil 
						   file:nil
				 modalForWindow:[self window]
				  modalDelegate:self 
				 didEndSelector:@selector(saveAsPanelDidEnd:returnCode:contextInfo:)
					contextInfo:nil];
}

// ---------------------------------------------------------------------------
// 
// ------------
//-(IBAction)doCompute:(id)sender{
//}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)saveAsPanelDidEnd:(NSSavePanel *)sheet 
			  returnCode:(int)returnCode 
			 contextInfo:(void *)contextInfo{
_bTrace_("[StatStdWindowController saveAsPanelDidEnd]",true);
    if(NSOKButton==returnCode){
char	fname[FILENAME_MAX];
char	fpath[PATH_MAX];
		
		[[[[sheet filename] stringByReplacingOccurrencesOfString:[sheet directory] withString:@""] 
		  stringByReplacingOccurrencesOfString:@"/" withString:@""] 
		 getCString:fname maxLength:FILENAME_MAX encoding:NSUTF8StringEncoding];		
		[[sheet directory] getCString:fpath maxLength:PATH_MAX encoding:NSUTF8StringEncoding];
		
_tm_(fname);		
_tm_(fpath);
		
// Write datas
long	status=MMsIOWriteAnalysis(fpath,fname,_anap);
		if(status){
_te_("MMsIOWriteAnalysis");
			return;
		}
		
// Write resources
bArray					arr(sizeof(xmlelt));
void*					bf;
int						sz;
bGenericXMLBaseElement*	root;
		
// Type				
		add_cdesc(arr,0,"param","");
		add_cdesc(arr,1,"version","3.2.0");
		add_ddesc(arr,1,"float",[_brw type]->tid(),0);
		root=[_brw app]->classMgr()->ParseXMLDescriptors(&arr);
		free_descs(arr);
		arr.reset();
		if(!root){
_te_("ParseXMLDescriptors");
			return;
		}
		if(!GetBufferFromXMLParam(root,&bf,&sz)){
			[_brw app]->classMgr()->ReleaseXMLInstance(root);
_te_("GetBufferFromXMLParam");
			return;
		}
		[_brw app]->classMgr()->ReleaseXMLInstance(root);
		status=MMsIOWriteParam(fpath,fname,bf,sz,"type.xml");
		free(bf);
		if(status){
_te_("MMsIOWriteParam");
			return;
		}
		
		
// Objects
bGenericGeoElement*	geo;
		add_cdesc(arr,0,"param","");
		add_cdesc(arr,1,"version","3.2.0");
		for(long i=1;i<=[_brw lines]->count();i++){
			[_brw lines]->get(i,&geo);
			add_idesc(arr,1,"int",geo->getID());
		}
		root=[_brw app]->classMgr()->ParseXMLDescriptors(&arr);
		free_descs(arr);
		arr.reset();
		if(!root){
_te_("ParseXMLDescriptors");
			return;
		}
		if(!GetBufferFromXMLParam(root,&bf,&sz)){
			[_brw app]->classMgr()->ReleaseXMLInstance(root);
_te_("GetBufferFromXMLParam");
			return;
		}
		[_brw app]->classMgr()->ReleaseXMLInstance(root);
		status=MMsIOWriteParam(fpath,fname,bf,sz,"objects.xml");
		free(bf);
		if(status){
_te_("MMsIOWriteParam");
			return;
		}
		
// Fields
char			cname[256];
BrowserColumn*	col;
NSEnumerator*	numer=[[(StatBrowserHeaderView*)[_brw header] analysisColumns] objectEnumerator];
		add_cdesc(arr,0,"param","");
		add_cdesc(arr,1,"version","3.2.0");
		while((col=[numer nextObject])){
			[col name:cname];
			add_cdesc(arr,1,"name",cname);
		}
		root=[_brw app]->classMgr()->ParseXMLDescriptors(&arr);
		free_descs(arr);
		arr.reset();
		if(!root){
_te_("ParseXMLDescriptors");
			return;
		}
		if(!GetBufferFromXMLParam(root,&bf,&sz)){
			[_brw app]->classMgr()->ReleaseXMLInstance(root);
_te_("GetBufferFromXMLParam");
			return;
		}
		[_brw app]->classMgr()->ReleaseXMLInstance(root);
		status=MMsIOWriteParam(fpath,fname,bf,sz,"columns.xml");
		free(bf);
		if(status){
_te_("MMsIOWriteParam");
			return;
		}
		
// Specific resources
		if(![self saveToPath:fpath name:fname]){
_te_("[self saveToPath:fpath name:fname] returns NO");
			return;
		}
		
// Panels
NSTabViewItem*	item;
BrowserView*	brv;
		add_cdesc(arr,0,"param","");
		add_cdesc(arr,1,"version","3.2.0");
		numer=[[_tab tabViewItems] objectEnumerator];
		while((item=[numer nextObject])){
			brv=[item view];
			add_cdesc(arr,1,"container","");
			if(![brv savePrefs:arr indent:1 path:fpath name:fname]){
_te_("[brv savePrefs:arr indent:1 path:fpath name:fname] returns NO");
				//return;
			}			
		}
		root=[_brw app]->classMgr()->ParseXMLDescriptors(&arr);
		if(!root){
_te_("ParseXMLDescriptors");
			return;
		}
		free_descs(arr);
		arr.reset();
		if(!GetBufferFromXMLParam(root,&bf,&sz)){
			[_brw app]->classMgr()->ReleaseXMLInstance(root);
_te_("GetBufferFromXMLParam");
			return;
		}
		[_brw app]->classMgr()->ReleaseXMLInstance(root);
		status=MMsIOWriteParam(fpath,fname,bf,sz,"panels.xml");
		free(bf);
		if(status){
_te_("MMsIOWriteParam");
			return;
		}		
    }
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
NSTabViewItem*		item;
MatrixBrowserView*	sheet;
NSEnumerator*		numer;
		
	numer=[[_tab tabViewItems] objectEnumerator];
	[numer nextObject];
	while((item=[numer nextObject])){
		sheet=[item view];
		[sheet updateUI];
	}
	
	[super updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
//-(long)analysisType{
//	return _kAnalysisUndef;
//}

// ---------------------------------------------------------------------------
// 
// -----------
//-(NSString*)analysisExt{
//	return @"";
//}

#pragma mark ---- Utils ----
// ---------------------------------------------------------------------------
// 
// -----------
static bool parseColumn(bGenericXMLBaseElement *elt, 
						void *prm, 
						int indent){
_bTrace_("[StatStdWindowController parseColumn]",false);
StatStdWindowController*	swc=(StatStdWindowController*)prm;
char						clssname[_values_length_max_];
	
	elt->getclassname(clssname);
	if(!strcmp("name",clssname)){
		elt->getvalue(clssname);
		if([swc recoverColumn:clssname]==NO){
_te_("column not found");
_te_(clssname);
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
static bool parsePanel(	bGenericXMLBaseElement *elt, 
							void *prm, 
							int indent){
_bTrace_("[StatStdWindowController parsePanel]",false);
StatStdWindowController*	swc=(StatStdWindowController*)prm;
char						clssname[_values_length_max_];
	
	elt->getclassname(clssname);
	if(!strcmp("container",clssname)){
		if([swc recoverPanel:elt]==NO){
_te_("panel not found");
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)setExt:(bStdNSXMap*)ext
	   atPath:(const char*)path{	
_bTrace_("[StatStdWindowController setExt:atPath]",true);
	
bGenericMacMapApp* gapp=(bGenericMacMapApp*)ext->getapp();

// Parsing path and name
char	fname[FILENAME_MAX];
char	fpath[PATH_MAX];

	strcpy(fpath,path);
	if(fpath[strlen(fpath)-1]=='/'){
		fpath[strlen(fpath)-1]=0;
	}
char*	p=strrchr(fpath,'/');
	if(p){
		strcpy(fname,p+1);
		p[0]=0;
	}
	else{
		strcpy(fname,path);
		strcpy(fpath,"");
	}
	strcat(fname,"/");	
_tm_("path:"+fpath);
_tm_("name:"+fname);
	
long		status;
	
// Read matrix
	status=MMsIOReadAnalysis(fpath,fname,_anap);
	if(status){
_te_("MMsIOReadAnalysis");
		return(NO);
	}
_tm_("analysis kind="+_anap->kind);
	
void*					bf;
int						sz;
bGenericXMLBaseElement*	root;
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
	
// Read type
	status=MMsIOReadParam(fpath,fname,&bf,&sz,"type.xml");
	if(status){
_te_("MMsIOReadParam "+fpath+">"+fname+">"+"type.xml");
		return(NO);
	}
	status=GetXMLParamFromBuffer(gapp,&root,bf,sz)?0:-1;
	free(bf);
	if(status){
_te_("GetXMLParamFromBuffer");
		return(NO);
	}
	elt=gapp->classMgr()->NthElement(root,1,"float");
	if(!elt){
_te_("float not found");
		gapp->classMgr()->ReleaseXMLInstance(root);
		return(NO);
	}
	elt->getvalue(val);
	gapp->classMgr()->ReleaseXMLInstance(root);
double			tid=matof(val);
bGenericType*	tp=gapp->typesMgr()->get(gapp->typesMgr()->index(tid));
	if(!tp){
_te_("type not found "+tid);
fprintf(stderr,"%.0f\n",tid);
		return(NO);
	}

// Read objects
	status=MMsIOReadParam(fpath,fname,&bf,&sz,"objects.xml");
	if(status){
_te_("MMsIOReadParam "+fpath+">"+fname+">"+"objects.xml");
		return(NO);
	}
	status=GetXMLParamFromBuffer(gapp,&root,bf,sz)?0:-1;
	free(bf);
	if(status){
_te_("GetXMLParamFromBuffer");
		return(NO);
	}
bArray	arr(sizeof(int));
	root->dotoall(&arr,0,add_oid);
	gapp->classMgr()->ReleaseXMLInstance(root);
	if(arr.count()!=_anap->data->nl){
_te_("(arr.count()!=_anap->data->nl)");
		return(NO);
	}
bArray	geos(sizeof(bGenericGeoElement*));
	if(!RecoverObjects(tp,arr,geos)){
_te_("RecoverObjects)");
		return(NO);
	}
	arr.reset();
	gapp->selMgr()->flush();
	gapp->selMgr()->add(&geos);
	geos.reset();
	
	_lpath=[[MMsPath alloc ] init:fpath name:fname];

	[super setExt:ext];

// (Browser allocated at setExt, so we can call it now)
_tm_("browser="+(void*)_brw);
	
// Specific load
	if(![self loadFromPath:fpath name:fname app:gapp]){
	}

// Read fields
	status=MMsIOReadParam(fpath,fname,&bf,&sz,"columns.xml");
	if(status){
_te_("MMsIOReadParam "+fpath+">"+fname+">"+"columns.xml");
		[_lpath release];
		_lpath=nil;
		return(NO);
	}
	status=GetXMLParamFromBuffer(gapp,&root,bf,sz)?0:-1;
	free(bf);
	if(status){
_te_("GetXMLParamFromBuffer");
		[_lpath release];
		_lpath=nil;
		return(NO);
	}
	root->dotoall(self,0,parseColumn);
	if([[(StatBrowserHeaderView*)[_brw header] analysisColumns] count]!=_anap->data->nc){
/*_te_("([[(StatBrowserHeaderView*)[_brw header] analysisColumns] count]:"+
	[[(StatBrowserHeaderView*)[_brw header] analysisColumns] count]+
	"!=_anap->data->nc:"+_anap->data->nc);*/
		[_lpath release];
		_lpath=nil;
		return(NO);
	}
	gapp->classMgr()->ReleaseXMLInstance(root);
	
// Read sheets
	status=MMsIOReadParam(fpath,fname,&bf,&sz,"panels.xml");
	if(status){
_te_("MMsIOReadParam "+fpath+">"+fname+">"+"panels.xml");
		[_lpath release];
		_lpath=nil;
		return(NO);
	}
	status=GetXMLParamFromBuffer(gapp,&root,bf,sz)?0:-1;
	free(bf);
	if(status){
_te_("GetXMLParamFromBuffer");
		[_lpath release];
		_lpath=nil;
		return(NO);
	}
	root->dotoall(self,0,parsePanel);
	gapp->classMgr()->ReleaseXMLInstance(root);

	[_lpath release];
	_lpath=nil;
	
	return YES;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)cleanupSheets{
_bTrace_("[StatStdWindowController cleanupSheets]",true);
NSTabViewItem*		item;
MatrixBrowserView*	sheet;
	
	while([_tab numberOfTabViewItems]>1){
		item=[_tab tabViewItemAtIndex:[_tab numberOfTabViewItems]-1];
		sheet=[item view];
		[sheet save];
		[sheet release];
		[_tab removeTabViewItem:item];
	}
	free_analysis(_anap);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)copyColumns{
	if([(StatBrowserHeaderView*)[_brw header] analysisColumns]){
		[[(StatBrowserHeaderView*)[_brw header] analysisColumns] removeAllObjects];
BrowserColumn*	col;
		for(long i=1;i<=[_brw selectedColumns]->count();i++){
			[_brw selectedColumns]->get(i,&col);
			[[(StatBrowserHeaderView*)[_brw header] analysisColumns] addObject:col];
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)recoverColumn:(const char*)name{
BrowserColumn*	col;
NSEnumerator*	numer=[[_brw columns] objectEnumerator];
char			cname[256];

	while((col=[numer nextObject])){
		[col name:cname];
		if(strcmp(name,cname)==0){
			[[(StatBrowserHeaderView*)[_brw header] analysisColumns] addObject:col];
			[col setSelected:YES inArray:[_brw selectedColumns]];
			return YES;
		}
	}
	return NO;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)initBrowser:(NSString*)rsrc
			bundle:(NSBundle*)bndl
			 label:(NSString*)lbl
			matrix:(MMsMatrix*)mmx{
NSViewController*	vcn=nil;
NSTabViewItem*		sheet=nil;
MatrixBrowserView*	mbv=nil;
	
	sheet=[[[NSTabViewItem alloc] initWithIdentifier:nil] autorelease];
	[sheet setLabel:lbl];
	vcn=[[NSViewController alloc] initWithNibName:rsrc bundle:bndl];
	mbv=(MatrixBrowserView*)[vcn view];
	[mbv putExt:_ext];
	[mbv putSource:[_brw header]];
	[mbv putAnalysis:_anap];
	[mbv putMatrix:mmx];
	[mbv putLines:[_brw lines]];
	[sheet setView:mbv];
	[_tab addTabViewItem:sheet];
	[mbv release];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)initGraph:(NSString*)rsrc
		  bundle:(NSBundle*)bndl
		   label:(NSString*)lbl
		  matrix:(MMsMatrix*)mmx{
NSViewController*	vcn=nil;
NSTabViewItem*		sheet=nil;
GraphView*			grv=nil;
	
	sheet=[[[NSTabViewItem alloc] initWithIdentifier:nil] autorelease];
	[sheet setLabel:lbl];
	vcn=[[NSViewController alloc] initWithNibName:rsrc bundle:bndl];
	grv=(GraphView*)[vcn view];
	[grv putExt:_ext];
	[grv putSource:[_brw header]];
	[grv putAnalysis:_anap];
	[grv putMatrix:mmx];
	[self initGraphExtra:grv];
	[grv putLines:[_brw lines]];	
	[sheet setView:grv];
	[_tab addTabViewItem:sheet];
	[grv release];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)initGraphExtra:(GraphView*)grv{
}

// ---------------------------------------------------------------------------
// 
// -----------
-(MMsPath*)loadPath{
	return _lpath;
}


// ---------------------------------------------------------------------------
// 
// ------------
@end
