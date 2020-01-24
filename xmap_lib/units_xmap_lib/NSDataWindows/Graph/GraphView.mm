//----------------------------------------------------------------------------
// File : GraphView.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base graph view classes
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
// 27/05/2014 creation.
//----------------------------------------------------------------------------

#import "GraphView.h"
#import "StatBrowserView.h"
#import "BrowserColumn.h"
#import "StatRegression.h"
#import "MatrixItemRef.h"
#import "StatStdWindowController.h"
#import "bBrowserSortValue.h"

#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/C_Utils.h>

#import <MacMapStats/MMsIO.h>
#import <MacMapStats/MMsDiscretization.h>

#import <mox_intf/xmldesc_utils.h>
#import <mox_intf/NSUIUtils.h>
#import <mox_intf/Type_Utils.h>
#import <mox_intf/mm_messages.h>
#import <mox_intf/bEventLog.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation GraphObjView

// ---------------------------------------------------------------------------
// 
// ------------
+(void)setHiliteFill{
	[[NSColor colorWithDeviceRed:0.7 green:0.7 blue:0.9 alpha:0.5] setFill];
}

// ---------------------------------------------------------------------------
// 
// ------------
+(void)setHiliteStroke{
	[NSBezierPath setDefaultLineWidth:_DEFAULTSTROKEWIDTH_];
	[_DEFAULTSTROKECOLOR_ setStroke];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[GraphObjView initWithFrame]",true);
	if((self=[super initWithFrame:frameRect])!=nil){
		_rx=1;
		_ry=1;
		_a1=1;
		_onDrag=NO;
		_grpf=0;
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)acceptsFirstResponder{
	return(YES);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)isFlipped{
    return NO;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)viewDidEndLiveResize{
	[self putBounds];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)scrollWheel:(NSEvent *)evt{
	[super scrollWheel:evt];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{	
	_start=[self convertPoint:[event locationInWindow] fromView:nil];
	_end=_start;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseUp:(NSEvent*)event{
	_onDrag=NO;
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDragged:(NSEvent*)event{
	_end=[self convertPoint:[event locationInWindow] fromView:nil];
	_onDrag=YES;
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putExt:(bStdNSXMap*)ext{
	_ext=ext;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)putBounds{
NSRect	frm=[self objRect];
	
	_ry=frm.size.height/(_vbnds.ymax-_vbnds.ymin);
	_rx=frm.size.width/(_vbnds.xmax-_vbnds.xmin);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putAnalysis:(mmx_analysis*)ana{
	_ana=ana;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putMatrix:(MMsMatrix*)mmx{
	_mmx=mmx;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putAxis:(long)x 
		 yAxis:(long)y{
	_a1=x;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putHCAClass:(hca_clss**)clss
			 count:(long)n{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putRoot:(bGenericXMLBaseElement*)root{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putLines:(bArray*)lines{
bGenericGeoElement*	geo;
	_lines=lines;
	_lines->get(1,&geo);
	_typ=[self app]->typesMgr()->get(geo->getType());
	[self read_p];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putSource:(BrowserHeaderView*)shd{
	_shd=shd;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putGroupField:(long)fld{
	_grpf=fld;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bStdNSXMap*)ext{
	return _ext;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericMacMapApp*)app{
	return (bGenericMacMapApp*)_ext->getapp();
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericType*)type{
	return _typ;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bArray*)lines{
	return _lines;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(long)groupField{
	return _grpf;
}

// ---------------------------------------------------------------------------
// 
// ------------
//-(void)getParamsInfos:(char*)directory 
//				 name:(char*)prmName{
//}

// ---------------------------------------------------------------------------
// 
// ------------
//-(void)getPanelInfos:(char*)name
//		  signature:(char*)sign{
//}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSRect)objRect{
NSScrollView*	scrollView=[self enclosingScrollView];		
NSRect			frm=[[scrollView documentView] frame];
	return frm;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)read_p{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)write_p{
_bTrace_("[GraphObjView write_p]",true);
bGenericXMLBaseElement*	root=[self make_p];
	if(!root){
		return;
	}
char	directory[PATH_MAX];
char	fname[FILENAME_MAX];

	[self getParamsInfos:directory name:fname];
	if(!SetXMLParamToType(_typ,directory,fname,root)){
	}
	[self app]->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)savePrefs:(bArray&)arr
		  indent:(long)indt
			path:(const char*)fpath
			name:(const char*)fname{
_bTrace_("[GraphObjView savePrefs]",true);
char	stra[PATH_MAX];
char	strb[FILENAME_MAX];
	
	[self getPanelInfos:stra signature:strb];
	add_cdesc(arr,indt+1,"sign",strb);
	add_cdesc(arr,indt+1,"name",stra);
	
bGenericXMLBaseElement*	root=[self make_p];
	if(!root){
_te_("root==NULL");
		return NO;
	}
	[self getParamsInfos:stra name:strb];
	
void*	buffer;
int		sz;
	if(GetBufferFromXMLParam(root,&buffer,&sz)==true){
		if(MMsIOWriteParam(fpath,fname,buffer,sz,strb)==0){
		}
		free(buffer);
	}
	[self app]->classMgr()->ReleaseXMLInstance(root);
	return YES;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericXMLBaseElement*)make_p{
_bTrace_("[GraphObjView make_p]",true);
	return NULL;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(double)zoomXFactor{
	return 2.0;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(double)zoomYFactor{
	return 2.0;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)zoomIn{
NSScrollView*	scrollView=[self enclosingScrollView];		
NSRect			frm=[[scrollView documentView] frame];
	frm.size.width*=[self zoomXFactor];
	frm.size.height*=[self zoomYFactor];
	[[scrollView documentView] setFrame:frm];
	[self putBounds];
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)zoomOut{
NSScrollView*	scrollView=[self enclosingScrollView];		
NSRect			frm=[[scrollView documentView] frame];
	frm.size.width/=[self zoomXFactor];
	frm.size.height/=[self zoomYFactor];
	[[scrollView documentView] setFrame:frm];
	[self putBounds];
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawRect:(NSRect)rect{
	[self putBounds];
	[[NSColor whiteColor] set];
	
NSScrollView*	scrollView=[self enclosingScrollView];	
NSRect			drawRect=[scrollView documentVisibleRect];
	
	[NSGraphicsContext saveGraphicsState];
	NSRectFill(drawRect);
	
	[self drawExtra:drawRect];
	[self drawFields:drawRect];
	[self drawObjects:drawRect];
	[self drawSelect:drawRect];
	
	if(_onDrag){
		[self drawPath:drawRect];
	}
	
	[NSGraphicsContext restoreGraphicsState];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawExtra:(NSRect)rect{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawFields:(NSRect)rect{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawObjects:(NSRect)rect{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawSelect:(NSRect)rect{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawPath:(NSRect)rect{
	[GraphObjView setHiliteStroke];
	
NSRect	nsr;
	nsr.origin.x=(_start.x<_end.x)?_start.x:_end.x;
	nsr.origin.y=(_start.y<_end.y)?_start.y:_end.y;
	nsr.size.width=fabs(_end.x-_start.x);
	nsr.size.height=fabs(_end.y-_start.y);
	
	[NSBezierPath strokeRect:nsr];
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end



// ---------------------------------------------------------------------------
// 
// ------------
@implementation HistogramGraphObjView

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct objref{
	double				value;
	int					index;
	bGenericGeoElement*	o;
}objref;

// ---------------------------------------------------------------------------
// 
// ------------
static int ref_comp(const void* a,
					const void* b){
objref*	ra=(objref*)a;
objref*	rb=(objref*)b;
	if(ra->value<rb->value){
		return(-1);
	}
	if(ra->value>rb->value){
		return(1);
	}
	return(0);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[HistogramGraphObjView initWithFrame]",true);
	if((self=[super initWithFrame:frameRect])!=nil){
	}
	_objects=new bArray(sizeof(objref));
	_hr=_hg=_hb=0;
	_lr=_lg=_lb=0.5;
	_sr=1;
	_sg=_sb=0;
	_ha=_la=_sa=1;
	_mid=0;
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[HistogramGraphObjView dealloc]",true);
	if(_objects){
		delete _objects;
		_objects=nil;
	}	
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{	
	[super mouseDown:event];
	
	if([event clickCount]>1){
NSRect	nsr=[self objRect];
		if(_start.y<nsr.origin.y){
mmx_uni*	uni=(mmx_uni*)_ana;
			if(!uni->clss){
				uni->clss=MMsMatrixAlloc(2,2);
				MMsSetDouble(uni->clss,1,kDiscretizationIndexColumn_,1);
				MMsSetDouble(uni->clss,1,kDiscretizationLimitColumn_,MMsGetDouble(uni->indic,_kMinIndex,1));
				MMsSetDouble(uni->clss,2,kDiscretizationIndexColumn_,_objects->count());
				MMsSetDouble(uni->clss,2,kDiscretizationLimitColumn_,MMsGetDouble(uni->indic,_kMaxIndex,1));
			}
long		n=ceil((_start.x-nsr.origin.x)/_rx);
objref		ref,sup;

			if(!_objects->get(n,&ref)){
				return;
			}
			if(!_objects->get(n+1,&sup)){
				return;
			}
			for(long i=2;i<uni->clss->nl;i++){
				if(n+1==MMsGetDouble(uni->clss,i,kDiscretizationIndexColumn_)){
					uni->clss=MMsMatrixRemoveLine(uni->clss,i);
					[self putClass];
					[self updateUI];
					return;
				}
			}
double		e=(ref.value+sup.value)/2.0;
			for(long i=2;i<=uni->clss->nl;i++){
				if(	(e>MMsGetDouble(uni->clss,i-1,kDiscretizationLimitColumn_))	&&
					(e<MMsGetDouble(uni->clss,i,kDiscretizationLimitColumn_))	){
					uni->clss=MMsMatrixInsertLine(uni->clss,i);
					MMsSetDouble(uni->clss,i,kDiscretizationIndexColumn_,n+1);
					MMsSetDouble(uni->clss,i,kDiscretizationLimitColumn_,e);
					[self putClass];
					[self updateUI];
					return;
				}
			}
			
		}
	}	
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseUp:(NSEvent*)event{
	[super mouseUp:event];
NSRect	frm=[self objRect];
	if((_start.y<frm.origin.y)&&!_onDrag){
		return;
	}
	
NSPoint	a,b;
	a.x=(_start.x<_end.x)?_start.x:_end.x;
	a.y=(_start.y<_end.y)?_start.y:_end.y;
	b.x=(_start.x<_end.x)?_end.x:_start.x;
	b.y=(_start.y<_end.y)?_end.y:_start.y;
	
	a.x-=frm.origin.x;
	a.y-=frm.origin.y;
	b.x-=frm.origin.x;
	b.y-=frm.origin.y;	
	
NSUInteger		mod=[event modifierFlags];		
long			start,end;
objref			ref;
bArray			geos(sizeof(bGenericGeoElement*));	
	
	if(NSEqualPoints(_start,_end)){ // Point select
		start=ceil(a.x/_rx);
		if(mod&NSShiftKeyMask){
long		prv=start,nxt=start;
			while(true){
				prv--;
				if(_objects->get(prv,&ref)){
					if(ref.o->selected()){
						nxt=prv;
						break;
					}
				}
				else{
					prv=0;
				}
				nxt++;
				if(_objects->get(nxt,&ref)){
					if(ref.o->selected()){
						prv=nxt;
						break;
					}
				}
				else{
					nxt=0;
				}
				if((prv==0)&&(nxt==0)){
					break;
				}
			}
			if((prv>0)&&(prv==nxt)){
				if(start>nxt){
					for(long i=start;i>prv;i--){
						if(_objects->get(i,&ref)){
							geos.add(&ref.o);
						}
					}
				}
				else{
					for(long i=start;i<nxt;i++){
						if(_objects->get(i,&ref)){
							geos.add(&ref.o);
						}
					}					
				}
			}
			else{
				if(_objects->get(start,&ref)){
					geos.add(&ref.o);
				}
			}
		}
		else{
			if(_objects->get(start,&ref)){
				geos.add(&ref.o);
			}
		}
	}
	else{	// Drag select
		start=ceil(a.x/_rx);
		end=ceil(b.x/_rx);
		for(long i=start;i<=end;i++){
			if(_objects->get(i,&ref)){
				geos.add(&(ref.o));
			}
		}
	}
	
	
bGenericGeoElement	*geo;
	if(mod&NSCommandKeyMask){
		for(long i=1;i<=geos.count();i++){
			geos.get(i,&geo);
			geo->setselect(!geo->selected(),true);
		}
	}
	else if(mod&NSShiftKeyMask){
bArray	tmp(sizeof(bGenericGeoElement*));
		for(long i=1;i<=geos.count();i++){
			geos.get(i,&geo);
			if(!geo->selected()){
				tmp.add(&geo);
			}
		}
		[self app]->selMgr()->add(&tmp);
	}
	else{
		[self app]->selMgr()->flush();
		[self app]->selMgr()->add(&geos);
	}
	
	if([event clickCount]>1){
		SendCommandToApplication('fich');
	}	
}
// ---------------------------------------------------------------------------
// 
// ------------
-(void)putLines:(bArray*)lines{
_bTrace_("[HistogramGraphObjView putLines]",true);
	[super putLines:lines];
	
NSMutableArray*	cols=[((StatBrowserHeaderView*)_shd) analysisColumns];
BrowserColumn*	col=[cols objectAtIndex:0];
objref			ref;
	
	for(int i=1;i<=_lines->count();i++){
		ref.value=[col value:i-1 inArray:_lines];
		_lines->get(i,&ref.o);
		ref.index=i;
		_objects->add(&ref);
	}
	_objects->sort(ref_comp);
	
	if(!_saved){
		_vbnds.ymin=MMsGetDouble(_ana->indic,_kMinIndex,_a1);
		_vbnds.ymax=MMsGetDouble(_ana->indic,_kMaxIndex,_a1);
		_mid=MMsGetDouble(_ana->indic,_kAverageIndex,_a1);
	}
	_vbnds.xmin=0;
	_vbnds.xmax=_lines->count();
	
	[self putBounds];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getParamsInfos:(char*)directory 
				 name:(char*)prmName{
	GetSignatureString(_ext,directory);
	sprintf(prmName,"histogram.xml");
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getPanelInfos:(char*)name
		   signature:(char*)sign{
long	sgn=kStatGraphHistogramPanelContentSignature;
	b_message_string("histogram",name,_ext->getbundle(),1);
	sprintf(sign,"%.4s",(char*)&sgn);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSRect)objRect{
NSRect	frm=[super objRect];
	frm.origin.x+=_LEFTSPACE_;
	frm.origin.y+=_BOTTOMSPACE_;
	frm.size.width-=_LEFTSPACE_;
	frm.size.height-=_BOTTOMSPACE_;
	frm.size.width-=_MARGIN_;
	frm.size.height-=_MARGIN_;
	return frm;
}

// ---------------------------------------------------------------------------
// 
// ------------
//-(BOOL)savePrefs:(bArray&)arr
//		  indent:(long)indt
//			path:(const char*)fpath
//			name:(const char*)fname{
//_bTrace_("[HistogramGraphObjView savePrefs]",true);	
//	[super savePrefs:arr indent:indt path:fpath name:fname];
//	add_idesc(arr,indt+1,"int",_a1);
//	add_idesc(arr,indt+1,"int",_a2);
//	return YES;
//}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)read_p{
_bTrace_("[HistogramGraphObjView read_p]",true);
bGenericXMLBaseElement*		root=NULL;
char	directory[PATH_MAX];
char	fname[FILENAME_MAX];
char	aname[FILENAME_MAX];
	
	_saved=false;
	
	[self getParamsInfos:directory name:fname];
	
	if([((StatBrowserHeaderView*)_shd) loadPath]==nil){
_tm_("std pref read");
		root=GetXMLParamFromType([self app],_typ,directory,fname);
	}
	else{
void*	buffer;
int		sz;
		[[((StatBrowserHeaderView*)_shd) loadPath] getPath:directory name:aname];
_tm_("analysis read at "+directory+"/"+aname);
		if(MMsIOReadParam(directory,aname,&buffer,&sz,fname)==0){
			if(!GetXMLParamFromBuffer([self app],&root,buffer,sz)){
			}
			free(buffer);
		}
		_saved=true;
	}
	if(!root){
_te_("no root");
		return;
	}
	
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
	
	elt=[self app]->classMgr()->NthElement(root,1,"float");
	if(elt){
		elt->getvalue(val);
		_hr=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,2,"float");
	if(elt){
		elt->getvalue(val);
		_hg=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,3,"float");
	if(elt){
		elt->getvalue(val);
		_hb=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,4,"float");
	if(elt){
		elt->getvalue(val);
		_ha=matof(val);
	}
	
	elt=[self app]->classMgr()->NthElement(root,5,"float");
	if(elt){
		elt->getvalue(val);
		_lr=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,6,"float");
	if(elt){
		elt->getvalue(val);
		_lg=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,7,"float");
	if(elt){
		elt->getvalue(val);
		_lb=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,8,"float");
	if(elt){
		elt->getvalue(val);
		_la=matof(val);
	}

	elt=[self app]->classMgr()->NthElement(root,9,"float");
	if(elt){
		elt->getvalue(val);
		_sr=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,10,"float");
	if(elt){
		elt->getvalue(val);
		_sg=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,11,"float");
	if(elt){
		elt->getvalue(val);
		_sb=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,12,"float");
	if(elt){
		elt->getvalue(val);
		_sa=matof(val);
	}
	
	if(_saved){
		elt=[self app]->classMgr()->NthElement(root,13,"float");
		if(elt){
			elt->getvalue(val);
			_mid=matof(val);
		}
		elt=[self app]->classMgr()->NthElement(root,14,"float");
		if(elt){
			elt->getvalue(val);
			_vbnds.ymin=matof(val);
		}
		elt=[self app]->classMgr()->NthElement(root,15,"float");
		if(elt){
			elt->getvalue(val);
			_vbnds.ymax=matof(val);
		}		
	}
	
	elt=[self app]->classMgr()->NthElement(root,1,"int");
	if(elt){
		elt->getvalue(val);
		_grpf=_typ->fields()->get_index(atoi(val));
	}
	
	[self app]->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericXMLBaseElement*)make_p{
_bTrace_("[HistogramGraphObjView make_p]",true);
bArray	arr(sizeof(xmlelt));
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	
	add_ddesc(arr,1,"float",_hr,2);
	add_ddesc(arr,1,"float",_hg,2);
	add_ddesc(arr,1,"float",_hb,2);
	add_ddesc(arr,1,"float",_ha,2);
	
	add_ddesc(arr,1,"float",_lr,2);
	add_ddesc(arr,1,"float",_lg,2);
	add_ddesc(arr,1,"float",_lb,2);
	add_ddesc(arr,1,"float",_la,2);
	
	add_ddesc(arr,1,"float",_sr,2);
	add_ddesc(arr,1,"float",_sg,2);
	add_ddesc(arr,1,"float",_sb,2);
	add_ddesc(arr,1,"float",_sa,2);
	
	add_ddesc(arr,1,"float",_mid,15);
	add_ddesc(arr,1,"float",_vbnds.ymin,15);
	add_ddesc(arr,1,"float",_vbnds.ymax,15);

	add_idesc(arr,1,"int",_typ->fields()->get_id(_grpf));

bGenericXMLBaseElement*	root=[self app]->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(double)zoomYFactor{
	return 1.0;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawExtra:(NSRect)rect{
NSRect			nsr=[self objRect];
NSPoint			a,b;
NSString*		nstr;
NSSize			sz;
	
	[GraphObjView setHiliteStroke];
		
	a=NSMakePoint(NSMinX(nsr),NSMinY(nsr));
	b=NSMakePoint(NSMaxX(nsr),NSMinY(nsr));
	[NSBezierPath strokeLineFromPoint:a toPoint:b];
	
	a=NSMakePoint(NSMinX(nsr),NSMinY(nsr));
	b=NSMakePoint(NSMinX(nsr),NSMaxY(nsr));
	[NSBezierPath strokeLineFromPoint:a toPoint:b];
	
// Ordinate values
	nstr=[NSString stringWithFormat:@"%f",_vbnds.ymin];
	sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];
	[nstr drawAtPoint:NSMakePoint(NSMinX(nsr)-sz.width-_MARGIN_,NSMinY(nsr)) 
	   withAttributes:[BrowserColumn stringAttributes]];

	nstr=[NSString stringWithFormat:@"%f",_mid];
	sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];
	[nstr drawAtPoint:NSMakePoint(NSMinX(nsr)-sz.width-_MARGIN_,NSMinY(nsr)+_mid*_ry) 
	   withAttributes:[BrowserColumn stringAttributes]];
	
	nstr=[NSString stringWithFormat:@"%f",_vbnds.ymax];
	sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];
	[nstr drawAtPoint:NSMakePoint(NSMinX(nsr)-sz.width-_MARGIN_,NSMaxY(nsr)-sz.height) 
	   withAttributes:[BrowserColumn stringAttributes]];
	
NSMutableArray*	cols=[((StatBrowserHeaderView*)_shd) analysisColumns];
BrowserColumn*	col;
char			str[256];
	
// Ordinate field name
	col=[cols objectAtIndex:0];
	[col name:str];
	nstr=[NSString stringWithCString:str encoding:NSMacOSRomanStringEncoding];
	sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];
	[nstr drawAtPoint:NSMakePoint(NSMinX(nsr)+_MARGIN_,NSMaxY(nsr)-sz.height) 
	   withAttributes:[BrowserColumn stringAttributes]];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawObjects:(NSRect)rect{
NSRect		nsr=[self objRect];
double		e;
NSRect		hbox,lbox;
NSPoint		a,b;
double		h=_mid*_ry;
mmx_uni*	uni=(mmx_uni*)_ana;

// Drawing objects	
	hbox=NSMakeRect(nsr.origin.x,nsr.origin.y+h,_rx,nsr.size.height-h);
	lbox=NSMakeRect(nsr.origin.x,nsr.origin.y,_rx,h);

	for(long i=1;i<=_ana->data->nl;i++){
		e=MMsGetDouble(_ana->data,i,_a1)-_vbnds.ymin;
		lbox.size.height=(e>_mid)?h:(e*_ry);
		[[NSColor colorWithDeviceRed:_lr green:_lg blue:_lb alpha:_la] set];
		NSRectFill(lbox);
		if(e>_mid){
			hbox.size.height=(e*_ry)-h;
			[[NSColor colorWithDeviceRed:_hr green:_hg blue:_hb alpha:_ha] set];
			NSRectFill(hbox);
		}
		hbox.origin.x+=_rx;
		lbox.origin.x+=_rx;
	}
		
// Drawing separators
	if(uni->clss){
NSColor*		clr=[NSColor colorWithDeviceRed:_sr green:_sg blue:_sb alpha:_sa];
NSString*		nstr;
NSMutableArray*	cols=[((StatBrowserHeaderView*)_shd) analysisColumns];
BrowserColumn*	col;
NSSize			sz;
NSRect			tbox;
double			end,last=nsr.origin.x;
		
		col=[cols objectAtIndex:0];
		
		[GraphObjView setHiliteStroke];
		[clr setStroke];

NSMutableDictionary*	strAttr=[NSMutableDictionary dictionary];
		[strAttr setObject:[NSFont fontWithName:[BrowserColumn stdNSFont] size:[BrowserColumn stdFontSize]] forKey:NSFontAttributeName];
		[strAttr setObject:clr forKey:NSForegroundColorAttributeName];
		
		for(int i=2;i<uni->clss->nl;i++){
			e=MMsGetDouble(uni->clss,i,kDiscretizationIndexColumn_)-1.0;
			a=NSMakePoint(nsr.origin.x+e*_rx,NSMinY(nsr)-_BOTTOMSPACE_+_MARGIN_+2.0);
			b=NSMakePoint(nsr.origin.x+e*_rx,NSMaxY(nsr));
			[NSBezierPath strokeLineFromPoint:a toPoint:b];
			[NSBezierPath strokeRect:NSMakeRect(a.x-2.0,a.y-4.0,4.0,4.0)];
			
			nstr=[NSString stringWithFormat:@"%.*f",[col decs]+1,MMsGetDouble(uni->clss,i,kDiscretizationLimitColumn_)];
			sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];

			tbox.origin.y=nsr.origin.y-sz.height-_MARGIN_;
			tbox.size.height=sz.height;

			tbox.origin.x=(nsr.origin.x+e*_rx)-sz.width;
			
			end=tbox.origin.x+sz.width;
			if(tbox.origin.x<last){
				tbox.origin.x=last;
			}
			tbox.size.width=end-tbox.origin.x;
			last=end;
			
			[NSGraphicsContext saveGraphicsState];
			NSRectClip(tbox);

			[nstr drawAtPoint:NSMakePoint(NSMinX(tbox),NSMinY(tbox)) 
			   withAttributes:strAttr];

			[NSGraphicsContext restoreGraphicsState];
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawSelect:(NSRect)rect{
NSRect			nsr=[self objRect];
NSRect			hbox;
NSMutableArray*	cols=[((StatBrowserHeaderView*)_shd) analysisColumns];
BrowserColumn*	col;
objref			ref;
NSBezierPath*	path;
	
	col=[cols objectAtIndex:0];
	hbox=NSMakeRect(nsr.origin.x,nsr.origin.y,_rx,nsr.size.height);
	[GraphObjView setHiliteFill];
	for(int i=1;i<=_objects->count();i++){
		_objects->get(i,&ref);
		if([col lineSelected:ref.index-1 inArray:_lines]){
			path=[NSBezierPath bezierPathWithRect:hbox];
			[path fill];
		}
		hbox.origin.x+=_rx;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)putClass{
mmx_uni*			uni=(mmx_uni*)_ana;
bGenericGeoElement*	geo;
objref				ref;
int					k,ki=2,n=1;	
char				msg[__MESSAGE_STRING_LENGTH_MAX__];
	
	if(_grpf){
		GetName([self ext],msg);
bEventLog	log([self app],
				_ext->getbundle(),
				msg,
				GetSignature([self ext]));
		if(!uni->clss){
			for(int i=1;i<=_objects->count();i++){
				_objects->get(i,&ref);
				_lines->get(ref.index,&geo);
				geo->setValue(_grpf,(int)0);
			}
			log.close();
			return;
		}
		k=MMsGetDouble(uni->clss,ki,kDiscretizationIndexColumn_);
		for(int i=1;i<=_objects->count();i++){
			if(i>=k){
				n++;
				ki++;
				if(ki<=uni->clss->nl){
					k=MMsGetDouble(uni->clss,ki,kDiscretizationIndexColumn_);
				}
			}
			_objects->get(i,&ref);
			_lines->get(ref.index,&geo);
			geo->setValue(_grpf,n);
		}
		log.close();
		if(FieldInView([self app],_typ,_grpf)){
			[self app]->mapIntf()->inval();
		}
	}
}


#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doStartPresentation:(id)sender{
	[_phc setColor:[NSColor colorWithDeviceRed:_hr
										 green:_hg
										  blue:_hb
										 alpha:_ha]];
	[_plc setColor:[NSColor colorWithDeviceRed:_lr
										 green:_lg
										  blue:_lb
										 alpha:_la]];
	[_psc setColor:[NSColor colorWithDeviceRed:_sr
										 green:_sg
										  blue:_sb
										 alpha:_sa]];
	[_pmd setDoubleValue:_mid];
	[_pmn setDoubleValue:_vbnds.ymin];
	[_pmx setDoubleValue:_vbnds.ymax];
	
	[NSApp beginSheet:_pwd
       modalForWindow:[self window]
        modalDelegate:self
       didEndSelector:@selector(presentationPanelDidEnd:returnCode:contextInfo:)
          contextInfo:nil];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doPresentationOk:(id)sender{
	if(	([_pmn doubleValue]<[_pmd doubleValue])	&&
		([_pmd doubleValue]<[_pmx doubleValue])	){
		[NSApp endSheet:_pwd returnCode:NSOKButton];
	}
	else{
		NSBeep();
	}
}

// ---------------------------------------------------------------------------
// 
// ------------

-(IBAction)doPresentationCancel:(id)sender{
	[NSApp endSheet:_pwd returnCode:NSCancelButton];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)presentationPanelDidEnd:(NSSavePanel *)sheet 
					returnCode:(int)returnCode 
				   contextInfo:(void *)contextInfo{
	[_pwd orderOut:self];
	if(NSOKButton==returnCode){
NSColor		*clr,*cclr;
		
		_mid=[_pmd doubleValue];
		_vbnds.ymin=[_pmn doubleValue];
		_vbnds.ymax=[_pmx doubleValue];
		
		clr=[_phc color];
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceRGBColorSpace"];
		_hr=[cclr redComponent];
		_hg=[cclr greenComponent];
		_hb=[cclr blueComponent];
		_ha=[cclr alphaComponent];

		clr=[_plc color];
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceRGBColorSpace"];
		_lr=[cclr redComponent];
		_lg=[cclr greenComponent];
		_lb=[cclr blueComponent];
		_la=[cclr alphaComponent];

		clr=[_psc color];
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceRGBColorSpace"];
		_sr=[cclr redComponent];
		_sg=[cclr greenComponent];
		_sb=[cclr blueComponent];
		_sa=[cclr alphaComponent];
		
		[self updateUI];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doStartDiscretization:(id)sender{
	[_dnc setIntValue:d_estimate_class_count(_mmx)];
	[NSApp beginSheet:_dwd
       modalForWindow:[self window]
        modalDelegate:self
       didEndSelector:@selector(discretizationPanelDidEnd:returnCode:contextInfo:)
          contextInfo:nil];
}

// ---------------------------------------------------------------------------
// 
// ------------

-(IBAction)doDiscretizationOk:(id)sender{
	[NSApp endSheet:_dwd returnCode:NSOKButton];
}

// ---------------------------------------------------------------------------
// 
// ------------

-(IBAction)doDiscretizationCancel:(id)sender{
	[NSApp endSheet:_dwd returnCode:NSCancelButton];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)discretizationPanelDidEnd:(NSSavePanel *)sheet 
					  returnCode:(int)returnCode 
					 contextInfo:(void *)contextInfo{
	[_dwd orderOut:self];
	if(NSOKButton==returnCode){
mmx_uni*	uni=(mmx_uni*)_ana;
		if(uni->clss){
			MMsMatrixFree(uni->clss);
			uni->clss=NULL;
		}
		switch([_dmt indexOfSelectedItem]){
			case 0:
				uni->clss=d_equal_range(uni->data,1,[_dnc intValue]);
				break;
			case 1:
				uni->clss=d_equal_manning(uni->data,1,[_dnc intValue]);
				break;
			case 2:
				uni->clss=d_standardized(uni->data,1,[_dnc intValue]);
				break;
			case 3:
				uni->clss=d_arithmetic_progression(uni->data,1,[_dnc intValue]);
				break;
			case 4:
				uni->clss=d_geometric_progression(uni->data,1,[_dnc intValue]);
				break;
			case 5:
				uni->clss=d_quantile(uni->data,1,[_dnc intValue]);
				break;
			case 6:
				uni->clss=d_natural_breaks(uni->data,1,[_dnc intValue]);
				break;
			case 7:
				uni->clss=d_bertin_average(uni->data,1,[_dnc intValue]);
				break;
			case 8:
				uni->clss=d_bertin_median(uni->data,1,[_dnc intValue]);
				break;
		}
		[self putClass];
		[self updateUI];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doKillDiscretization:(id)sender{
mmx_uni*	uni=(mmx_uni*)_ana;
	if(uni->clss){
		MMsMatrixFree(uni->clss);
		uni->clss=NULL;
	}
	[self putClass];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end



// ---------------------------------------------------------------------------
// 
// ------------
@implementation RepDiagramGraphObjView

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{	
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseUp:(NSEvent*)event{
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDragged:(NSEvent*)event{
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)putBounds{
mmx_uni*	uni=(mmx_uni*)_ana;
	
	_vbnds.xmin=0;
	_vbnds.ymin=0;
	if(uni->clss){
		_vbnds.xmax=uni->clss->nl-1;
double	x,a,b;
		_vbnds.ymax=0;
		for(int i=2;i<=uni->clss->nl;i++){
			a=MMsGetDouble(uni->clss,i,kDiscretizationIndexColumn_);
			b=MMsGetDouble(uni->clss,i-1,kDiscretizationIndexColumn_);
			if(i==uni->clss->nl){
				b+=1;
			}
			x=a-b;
			if((x>_vbnds.ymax)){
				_vbnds.ymax=x;
			}
		}
	}
	else{
		_vbnds.xmax=1;
		_vbnds.ymax=1;
	}
	[super putBounds];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getParamsInfos:(char*)directory 
				 name:(char*)prmName{
	GetSignatureString(_ext,directory);
	sprintf(prmName,"distributiondiagram.xml");
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getPanelInfos:(char*)name
		   signature:(char*)sign{
long	sgn=kStatGraphDistribDiagramPanelContentSignature;
	b_message_string("distributiondiagram",name,_ext->getbundle(),1);
	sprintf(sign,"%.4s",(char*)&sgn);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSRect)objRect{
NSRect	frm=[super objRect];
	frm.origin.x+=_LEFTSPACE_;
	frm.origin.y+=_BOTTOMSPACE_;
	frm.size.width-=_LEFTSPACE_;
	frm.size.height-=_BOTTOMSPACE_;
	frm.size.width-=_MARGIN_;
	frm.size.height-=_MARGIN_;
	return frm;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)read_p{
_bTrace_("[RepDiagramGraphObjView read_p]",true);
bGenericXMLBaseElement*		root=NULL;
char	directory[PATH_MAX];
char	fname[FILENAME_MAX];
char	aname[FILENAME_MAX];
	
	[self getParamsInfos:directory name:fname];
	
	if([((StatBrowserHeaderView*)_shd) loadPath]==nil){
_tm_("std pref read");
		root=GetXMLParamFromType([self app],_typ,directory,fname);
	}
	else{
void*	buffer;
int		sz;
		[[((StatBrowserHeaderView*)_shd) loadPath] getPath:directory name:aname];
_tm_("analysis read at "+directory+"/"+aname);
		if(MMsIOReadParam(directory,aname,&buffer,&sz,fname)==0){
			if(!GetXMLParamFromBuffer([self app],&root,buffer,sz)){
			}
			free(buffer);
		}
	}
	if(!root){
_te_("no root");
		return;
	}
	
bGenericXMLBaseElement*		elt;
char						val[_values_length_max_];
	
	elt=[self app]->classMgr()->NthElement(root,1,"float");
	if(elt){
		elt->getvalue(val);
		_r=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,2,"float");
	if(elt){
		elt->getvalue(val);
		_g=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,3,"float");
	if(elt){
		elt->getvalue(val);
		_b=matof(val);
	}
	elt=[self app]->classMgr()->NthElement(root,4,"float");
	if(elt){
		elt->getvalue(val);
		_a=matof(val);
	}
	
	[self app]->classMgr()->ReleaseXMLInstance(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericXMLBaseElement*)make_p{
_bTrace_("[RepDiagramGraphObjView make_p]",true);
bArray	arr(sizeof(xmlelt));
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	
	add_ddesc(arr,1,"float",_r,2);
	add_ddesc(arr,1,"float",_g,2);
	add_ddesc(arr,1,"float",_b,2);
	add_ddesc(arr,1,"float",_a,2);
		
bGenericXMLBaseElement*	root=[self app]->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawExtra:(NSRect)rect{
NSRect			nsr=[self objRect];
NSPoint			a,b;
NSString*		nstr;
NSSize			sz;
	
	[GraphObjView setHiliteStroke];
	
	a=NSMakePoint(NSMinX(nsr),NSMinY(nsr));
	b=NSMakePoint(NSMaxX(nsr),NSMinY(nsr));
	[NSBezierPath strokeLineFromPoint:a toPoint:b];
	
	a=NSMakePoint(NSMinX(nsr),NSMinY(nsr));
	b=NSMakePoint(NSMinX(nsr),NSMaxY(nsr));
	[NSBezierPath strokeLineFromPoint:a toPoint:b];
	
// Ordinate values
	nstr=[NSString stringWithFormat:@"%f",_vbnds.ymin];
	sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];
	[nstr drawAtPoint:NSMakePoint(NSMinX(nsr)-sz.width-_MARGIN_,NSMinY(nsr)) 
	   withAttributes:[BrowserColumn stringAttributes]];
		
	nstr=[NSString stringWithFormat:@"%f",_vbnds.ymax];
	sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];
	[nstr drawAtPoint:NSMakePoint(NSMinX(nsr)-sz.width-_MARGIN_,NSMaxY(nsr)-sz.height) 
	   withAttributes:[BrowserColumn stringAttributes]];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawObjects:(NSRect)rect{
mmx_uni*	uni=(mmx_uni*)_ana;
	if(!uni->clss){
		return;
	}
	
double		e;
NSRect		nsr=[self objRect],tbox,box=nsr;
NSSize		sz;
NSString*	nstr;
	
	box.size.width=_rx;

	[[NSColor colorWithDeviceRed:_r green:_g blue:_b alpha:_a] set];
	for(long i=2;i<=uni->clss->nl;i++){
		e=MMsGetDouble(uni->clss,i,kDiscretizationIndexColumn_)-MMsGetDouble(uni->clss,i-1,kDiscretizationIndexColumn_);
		if(i==uni->clss->nl){
			e+=1;
		}
		box.size.height=e*_ry;
		NSRectFill(box);
		
		nstr=[NSString stringWithFormat:@"%.0f",e];
		sz=[nstr sizeWithAttributes:[BrowserColumn stringAttributes]];
		
		tbox=box;
		tbox.origin.y-=sz.height;
		tbox.size.height=sz.height;
		[NSGraphicsContext saveGraphicsState];
		NSRectClip(tbox);
		[nstr drawAtPoint:NSMakePoint(NSMidX(tbox)-sz.width/2.0,NSMinY(tbox)) 
		   withAttributes:[BrowserColumn stringAttributes]];
		[NSGraphicsContext restoreGraphicsState];
		
		box.origin.x+=_rx;
	}

}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doStartPresentation:(id)sender{
	[_pcl setColor:[NSColor colorWithDeviceRed:_r
										 green:_g
										  blue:_b
										 alpha:_a]];	
	[NSApp beginSheet:_pwd
       modalForWindow:[self window]
        modalDelegate:self
       didEndSelector:@selector(presentationPanelDidEnd:returnCode:contextInfo:)
          contextInfo:nil];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doPresentationOk:(id)sender{
	[NSApp endSheet:_pwd returnCode:NSOKButton];
}

// ---------------------------------------------------------------------------
// 
// ------------

-(IBAction)doPresentationCancel:(id)sender{
	[NSApp endSheet:_pwd returnCode:NSCancelButton];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)presentationPanelDidEnd:(NSSavePanel *)sheet 
					returnCode:(int)returnCode 
				   contextInfo:(void *)contextInfo{
	[_pwd orderOut:self];
	if(NSOKButton==returnCode){
NSColor		*clr,*cclr;
				
		clr=[_pcl color];
		cclr=[clr colorUsingColorSpaceName:@"NSDeviceRGBColorSpace"];
		_r=[cclr redComponent];
		_g=[cclr greenComponent];
		_b=[cclr blueComponent];
		_a=[cclr alphaComponent];
				
		[self updateUI];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end



// ---------------------------------------------------------------------------
// 
// ------------
@implementation GraphView

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
	if((self=[super initWithFrame:frameRect])!=nil){
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putExt:(bStdNSXMap*)ext{
	_sav=nil;
	[_obj putExt:ext];
	[_mnu setAutoenablesItems:NO];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putAnalysis:(mmx_analysis*)ana{
	[_obj putAnalysis:ana];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putMatrix:(MMsMatrix*)mmx{
	[_obj putMatrix:mmx];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putSource:(BrowserHeaderView*)shd{
	[_obj putSource:shd];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putAxis:(long)x 
		 yAxis:(long)y{
	[_obj putAxis:x yAxis:y];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putRoot:(bGenericXMLBaseElement*)root{
	[_obj putRoot:root];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putLines:(bArray*)lines{
	[_obj putLines:lines];
bGenericGeoElement*	geo;
bGenericType*		typ;
	
	lines->get(1,&geo);
	typ=[_obj app]->typesMgr()->get(geo->getType());
	
	if([_mnu numberOfItems]<4){
		return;
	}
	
NSMenuItem*	subm=[_mnu itemAtIndex:4];
NSMenuItem*	item;
NSString*	nsstr;
char		str[256];
	
	for(long i=kOBJ_Dir_+1;i<=typ->fields()->count();i++){
		typ->fields()->get_name(i,str);
		nsstr=[NSString stringWithCString:str encoding:NSMacOSRomanStringEncoding];
		item=[[subm submenu] addItemWithTitle:nsstr action:@selector(doChooseGroupField:) keyEquivalent:@""];
		[item setTarget:self];
		[item setTag:i];
		[item setState:([_obj groupField]==i)];
	}
	if([_obj groupField]>0){
		item=[[subm submenu] itemAtIndex:0];
		[item setState:NSOffState];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putHCAClass:(hca_clss**)clss
			 count:(long)n{
	[_obj putHCAClass:clss count:n];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericMacMapApp*)app{
	return [_obj app];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericType*)type{
	return [_obj type];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putBounds{
	[_obj putBounds];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)save{
	[_obj write_p];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)savePrefs:(bArray&)arr
		  indent:(long)indt
			path:(const char*)fpath
			name:(const char*)fname{
	return [_obj savePrefs:arr indent:indt path:fpath name:fname];
}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doPublish:(id)sender{
	_sav=[NSSavePanel savePanel];
	[_sav setAllowedFileTypes:[NSArray arrayWithObjects:@"pdf",nil]];
	[_sav beginSheetForDirectory:nil 
							file:nil
				  modalForWindow:[self window]
				   modalDelegate:self 
				  didEndSelector:@selector(savePanelDidEnd:returnCode:contextInfo:)
					 contextInfo:nil];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doSelectAll:(id)sender{
NSUInteger			mod=[[NSApp currentEvent] modifierFlags];
bGenericGeoElement*	geo;
	
	if(mod&NSCommandKeyMask){
		for(long i=1;i<=[_obj lines]->count();i++){
			[_obj lines]->get(i,&geo);
			geo->setselect(!geo->selected(),true);
		}
	}
	else if(mod&NSShiftKeyMask){
bArray	tmp(sizeof(bGenericGeoElement*));
		for(long i=1;i<=[_obj lines]->count();i++){
			[_obj lines]->get(i,&geo);
			if(!geo->selected()){
				tmp.add(&geo);
			}
		}
		[_obj app]->selMgr()->add(&tmp);
	}
	else{
		[_obj app]->selMgr()->flush();
		[_obj app]->selMgr()->add([_obj lines]);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doChooseGroupField:(id)sender{
_bTrace_("[GraphView doChooseGroupField]",true);
NSMenuItem*		item=(NSMenuItem*)sender;
NSEnumerator*	numer=[[[item menu] itemArray] objectEnumerator];
	while(item=[numer nextObject]){
		if(![item isSeparatorItem]){
			[item setState:NSOffState];
		}
	}
	item=(NSMenuItem*)sender;
	[item setState:NSOnState];
	[_obj putGroupField:[item tag]];
_tm_("Group field: "+[item tag]);
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doZoomIn:(id)sender{
	[_obj zoomIn];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doZoomOut:(id)sender{
	[_obj zoomOut];
}

#pragma mark ---- Export ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)	savePanelDidEnd:(NSSavePanel *)sheet 
			 returnCode:(int)returnCode 
			contextInfo:(void *)contextInfo{
    if(NSOKButton==returnCode){		
		_sav=nil;
NSRect	r=[_obj bounds];
NSData*	data=[_obj dataWithPDFInsideRect:r];
		[data writeToFile:[sheet filename] atomically:YES];
    }
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
	[_obj updateUI];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end



