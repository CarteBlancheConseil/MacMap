//----------------------------------------------------------------------------
// File : MatrixBrowserView.mm
// Project : MacMap
// Purpose : Objective C++ source file : Browser view classes for browsers using matrix data
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
// 18/04/2014 creation.
//----------------------------------------------------------------------------

#import "MatrixBrowserView.h"
#import "BrowserColumn.h"
#import "bXMapStatFieldElement.h"

#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/valconv.h>

#import <mox_intf/NSUIUtils.h>
#import <mox_intf/mm_messages.h>
#import <mox_intf/bStdWait.h>
#import <mox_intf/bEventLog.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation MatrixBrowserHeaderView

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[MatrixBrowserHeaderView initWithFrame]",true);
	if((self=[super initWithFrame:frameRect])!=nil){
		_shd=NULL;
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[MatrixBrowserHeaderView dealloc]",true);
	if(_lines){
		_lines=NULL;// Original objects array, do not free it, still in use in std browser !
	}
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BrowserColumn*) allocator:(bArray*)lins
					  field:(int)fld
					   calc:(bGenericCalc*)cx
					  start:(float)strt
					  width:(float)w
					  label:(char*)str{	
	return [[BrowserMatrixColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight])
											   lines:lins application:[self app]
											  matrix:_mmx
											   index:fld
												name:str];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putSource:(BrowserHeaderView*)shd{
_bTrace_("[MatrixBrowserHeaderView putSource]",true);
	_shd=shd;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BrowserHeaderView*)sourceHeader{
	return _shd;
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
-(MMsMatrix*)matrix{
	return _mmx;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)name:(char*)buffer 
	atIndex:(long)idx{
	sprintf(buffer,"col_%ld",idx);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)read_p{
BrowserColumn*	col;
double			strtx=0;
double			w=[BrowserColumn stdColumnWidth]*2.0;
char			str[256];
	
	for(long i=1;i<=_mmx->nc;i++){
		[self name:str atIndex:i-1];
		col=[self allocator:_lines field:i calc:NULL start:strtx width:w label:str];
		strtx+=w;
		[_cols addObject:col];
		[col release];
	}
	[((MatrixBrowserObjView*)_obj) putBounds];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)write_p{
_bTrace_("[MatrixBrowserHeaderView write_p]",true);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericXMLBaseElement*)make_p{
_bTrace_("[MatrixBrowserHeaderView make_p]",true);
	return(NULL);
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
@implementation MatrixBrowserNonGeoHeaderView

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[MatrixBrowserNonGeoHeaderView initWithFrame]",true);
	if((self=[super initWithFrame:frameRect])!=nil){
		_lines=new bArray(sizeof(bXMapStatFieldElement*));
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[MatrixBrowserNonGeoHeaderView dealloc]",true);
	if(_lines){
bXMapStatFieldElement*	fld;
		for(long i=1;i<=_lines->count();i++){
			_lines->get(i,&fld);
			delete fld;
		}
		_lines->reset();// Supprimé dans le super
	}
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)flushSelectedLines{
bXMapStatFieldElement*	fld;
	for(long i=1;i<=_lines->count();i++){
		_lines->get(i,&fld);
		fld->setselect(false);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
@implementation MatrixBrowserIndicatorsHeaderView
// ---------------------------------------------------------------------------
// 
// ------------
-(void)name:(char*)buffer 
	atIndex:(long)idx{
BrowserColumn* col;
	if([_shd selectedColumns]->get(idx+1,&col)){
char	str[256];
		[col name:str];
		sprintf(buffer,"%s",str);
		return;
	}
	sprintf(buffer,"????_%ld",idx);
}
	 
// ---------------------------------------------------------------------------
// 
// ------------
-(void)putLines:(bArray*)lines{
_bTrace_("[MatrixBrowserIndicatorsHeaderView putLines]",true);
bXMapStatFieldElement*	fld;
char					name[256]="";

	for(long i=1;i<=_mmx->nl;i++){
		fld=new bXMapStatFieldElement();
		switch(i){
			case _kMinIndex:
				b_message_string("min",name,_ext->getbundle(),0);
				break;
			case _kMaxIndex:
				b_message_string("max",name,_ext->getbundle(),0);
				break;
			case _kExtendIndex:
				b_message_string("extend",name,_ext->getbundle(),0);
				break;
			case _kSumIndex:
				b_message_string("sum",name,_ext->getbundle(),0);
				break;
			case _kAverageIndex:
				b_message_string("average",name,_ext->getbundle(),0);
				break;
			case _kMedianIndex:
				b_message_string("median",name,_ext->getbundle(),0);
				break;
			case _kStdDeviationIndex:
				b_message_string("stddeviation",name,_ext->getbundle(),0);
				break;
			case _kVarianceIndex:
				b_message_string("variance",name,_ext->getbundle(),0);
				break;
			case _kcVariationIndex:
				b_message_string("cvariation",name,_ext->getbundle(),0);
				break;
			case _kcSkewnessIndex:
				b_message_string("cskewness",name,_ext->getbundle(),0);
				break;
			case _kcPearsonIndex:
				b_message_string("cpearson",name,_ext->getbundle(),0);
				break;
			case _kcFisherIndex:
				b_message_string("cfischer",name,_ext->getbundle(),0);
				break;
			case _kcCorrelationIndex:
				b_message_string("ccorrelation",name,_ext->getbundle(),0);
				break;
			case _kCovarianceIndex:
				b_message_string("covariance",name,_ext->getbundle(),0);
				break;
			default:
				sprintf(name,"????");	
				break;
		}
		fld->setName(name);
		_lines->add(&fld);
	}
	[self read_p];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getPanelInfos:(char*)name
		  signature:(char*)sign{
long	sgn=kStatBrowserIndicatorPanelContentSignature;
	b_message_string("indicators",name,_ext->getbundle(),1);
	sprintf(sign,"%.4s",(char*)&sgn);
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
@implementation MatrixBrowserNameNonGeoLeftView

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{
	[super mouseDown:event];
	[self setNeedsDisplay:YES];
	[_obj setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
@implementation MatrixBrowserObjView

// ---------------------------------------------------------------------------
// 
// -----------
-(void)putBounds{
_bTrace_("[MatrixBrowserObjView putBounds]",true);	
NSScrollView*	scrollView=[self enclosingScrollView];	
BrowserColumn*	col=[[_hdr columns] lastObject];
NSRect			bnds=NSZeroRect;
MMsMatrix*		mmx=[((MatrixBrowserHeaderView*)_hdr) matrix];
	
	bnds.size.width=NSMaxX([col frame]);
	bnds.size.height=mmx->nl*12;

bool	vscroll=bnds.size.width>[scrollView bounds].size.width;
bool	hscroll=bnds.size.height>[scrollView bounds].size.height;
		
	bnds=NSUnionRect(bnds,[scrollView bounds]);

	if(!vscroll){
		bnds.size.width-=17;
	}
	if(!hscroll){
		bnds.size.height-=17;
		bnds.origin.y+=17;
	}
	
	[[scrollView documentView] setFrame:bnds];
	
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
@implementation MatrixBrowserView

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putSource:(BrowserHeaderView*)shd{
	[((MatrixBrowserHeaderView*)_hdr) putSource:shd];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putAnalysis:(mmx_analysis*)ana{
	[((MatrixBrowserHeaderView*)_hdr) putAnalysis:ana];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putMatrix:(MMsMatrix*)mmx{
	[((MatrixBrowserHeaderView*)_hdr) putMatrix:mmx];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)abort{
	return NO;
}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doBackDataFill:(id)sender{
_bTrace_("[MatrixBrowserView doBackDataFill]",true);
char	prefix[256]="";
char	name[256];
char	bf[__MESSAGE_STRING_LENGTH_MAX__];
	
	b_message_string("getaname",bf,[_hdr ext]->getbundle(),0);
	if(!GetAName(prefix,bf,true)){
		return;
	}
BrowserColumn*		col;
long				i=0,fidx;
double				dbf;
bGenericGeoElement*	geo;
	
	message_string(kMsgProgress,bf,0);
bProgressWait wt("",bf,true,[_hdr lines]->count()*([[_hdr columns] count]-1));
	GetName([_hdr ext],bf);
bEventLog	log([_hdr app],
				[_hdr ext]->getbundle(),
				bf,
				GetSignatureFromBundle([_hdr ext]->getbundle()));
	
NSEnumerator*	numer=[[_hdr columns] objectEnumerator];
	while((col=[numer nextObject])){
		i++;
		if(![col selected]){
			continue;
		}
		[col name:name];
		sprintf(bf,"%s%s",prefix,name);
		fidx=[_hdr type]->fields()->get_index(bf);
		if(fidx==0){
			[_hdr type]->fields()->add(_double,_kDoubleLen,6,bf);
			fidx=[_hdr type]->fields()->get_index(bf);
		}
		if(fidx==0){
			break;
		}
		for(long j=1;j<=[((MatrixBrowserHeaderView*)_hdr) matrix]->nl;j++){
			if(((j%20)==0)&&(!wt.set_progress((i-2)*[_hdr lines]->count()+j))){
				fidx=0;
				break;
			}
			dbf=MMsGetDouble([((MatrixBrowserHeaderView*)_hdr) matrix],j,i-1);
			[_hdr lines]->get(j,&geo);
			geo->setValue(fidx,dbf);
		}
		if(fidx==0){
			break;
		}
	}
	log.close();
	
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
	[_crn setNeedsDisplay:YES];
	[_obj setNeedsDisplay:YES];
	[_hdr setNeedsDisplay:YES];
	[_lft setNeedsDisplay:YES];
	
	if([_hdr selectedColumns]->count()==0){
		NSPopupButtonMenuItemDisable(_mnu,3);
		NSPopupButtonMenuItemDisable(_mnu,4);
	}
	else{	
		NSPopupButtonMenuItemEnable(_mnu,3);		
		NSPopupButtonMenuItemEnable(_mnu,4);		
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end



// ---------------------------------------------------------------------------
// 
// ------------
@implementation MatrixNonGeoBrowserView

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
// Pour les non-geo
// ------------
-(IBAction)doSelectObjects:(id)sender{
_bTrace_("[MatrixNonGeoBrowserView doSelectObjects]",true);
NSUInteger				mod=[[NSApp currentEvent] modifierFlags];
bXMapStatFieldElement*	fld;
	
	if(mod&NSCommandKeyMask){
		for(long i=1;i<=[_hdr lines]->count();i++){
			[_hdr lines]->get(i,&fld);
			fld->setselect(!fld->selected());
		}
	}
	else if(mod&NSShiftKeyMask){
		for(long i=1;i<=[_hdr lines]->count();i++){
			[_hdr lines]->get(i,&fld);
			fld->setselect(true);
		}
	}
	else{
		[_hdr flushSelectedLines];
		for(long i=1;i<=[_hdr lines]->count();i++){
			[_hdr lines]->get(i,&fld);
			fld->setselect(true);
		}
	}
	
	[_lft setNeedsDisplay:YES];
	[_obj setNeedsDisplay:YES];

}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
	[_crn setNeedsDisplay:YES];
	[_obj setNeedsDisplay:YES];
	[_hdr setNeedsDisplay:YES];
	[_lft setNeedsDisplay:YES];
	
	if([_hdr selectedColumns]->count()==0){
		NSPopupButtonMenuItemDisable(_mnu,3);
	}
	else{	
		NSPopupButtonMenuItemEnable(_mnu,3);		
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

