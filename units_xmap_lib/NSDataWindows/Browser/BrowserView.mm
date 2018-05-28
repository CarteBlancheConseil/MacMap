//----------------------------------------------------------------------------
// File : BrowserView.mm
// Project : MacMap
// Purpose : Objective C++ source file : Base browser view classes
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

#import "BrowserWindowController.h"
#import "BrowserColumn.h"
#import "bBrowserSortValue.h"

#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/valconv.h>

#import <MacMapStats/MMsIO.h>

#import <mox_intf/Carb_Utils.h>
#import <mox_intf/ext_utils.h>
#import <mox_intf/xmldesc_utils.h>
#import <mox_intf/NSUIUtils.h>
#import <mox_intf/NSContextualMenuWrappers.h>
#import <mox_intf/Type_Utils.h>
#import <mox_intf/mm_messages.h>
#import <mox_intf/bEventLog.h>

#import <std_ext/bStdNSXMapCocoaStuff.h>

// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserHeaderView

// ---------------------------------------------------------------------------
// 
// ------------
bool readProc(bGenericXMLBaseElement* elt,
			  void* prm,
			  int indent){
col_p*			p=(col_p*)prm;
char			name[_names_length_max_];
char			value[_values_length_max_];
BrowserColumn*	col;
float			w;
	
	elt->getclassname(name);
	if(!strcmp(name,"name")){
		elt->getvalue(p->nam);
	}
	else if(!strcmp(name,"int")){
		elt->getvalue(value);
		p->knd=atoi(value);
	}
	else if(!strcmp(name,"float")){
		switch(p->knd){
			case 1:
				p->fx=p->tp->fields()->get_index(p->nam);
				if((p->fx)&&(p->tp->fields()->is_hidden(p->fx))){
					p->fx=0;
				}
				p->cx=NULL;
				break;
			case 2:
				p->cx=(bGenericCalc*)(void*)p->mgr->find(p->nam);
				p->fx=0;
				break;
			default:
				p->fx=p->tp->fields()->get_index(p->nam);
				if(p->fx){
					if(p->tp->fields()->is_hidden(p->fx)){
						p->fx=0;
					}
				}
				else{
					p->cx=(bGenericCalc*)(void*)p->mgr->find(p->nam);
				}
				break;
		}
		
		elt->getvalue(value);
		w=atof(value);
		
		col=[p->x allocator:p->objs field:p->fx calc:p->cx start:p->strt width:w];
		
		p->fx=0;
		p->cx=NULL;
				
		if(col){
			[p->arr addObject:col];
			[col release];
		}				  
		
		p->strt+=w;
	}
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BrowserColumn*) allocator:(bArray*)lins
                      field:(int)fld
                       calc:(bGenericCalc*)clc
                      start:(float)strt
                      width:(float)w{
BrowserColumn*	col=NULL;
int					k;
	
	if(fld){
		_tp->fields()->get_kind(fld,&k);
		if(_tp->fields()->is_hidden(fld)){
			col=[[BrowserColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app]];
		}
		else if(_tp->fields()->is_writeprotected(fld)){
			col=[[BrowserFieldStaticColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app] field:fld type:_tp];
		}
		else if(k==_bool){
			col=[[BrowserFieldCheckColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app] field:fld type:_tp];
		}
		else if(_tp->fields()->count_constraints(fld)>0){
			col=[[BrowserFieldPopupColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app] field:fld type:_tp];
		}
		else{
			col=[[BrowserFieldEditTextColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app] field:fld type:_tp];
		}
	}
	else if(clc){
		col=[[BrowserCalcColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app] calc:clc];
	}
	else{
		col=[[BrowserColumn alloc] initWithRect:NSMakeRect(strt,0,w,[BrowserColumn stdLineHeight]) lines:lins application:[self app]];
	}
	return(col);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[BrowserHeaderView initWithFrame]",true);
	if((self=[super initWithFrame:frameRect])!=nil){
		_cols=[[NSMutableArray alloc] init];
		_tp=NULL;
		_lines=NULL;
		_csels=new bArray(sizeof(BrowserColumn*));
		_cur=-1;
		_o=NSZeroPoint;
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[BrowserHeaderView dealloc]",true);
	[_cols release];
	delete _csels;
	if(_lines){
		delete _lines;
	}
	_cols=nil;
	_lines=NULL;
	_csels=NULL;
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)isFlipped{
	return YES;
}

// ---------------------------------------------------------------------------
// Dessin du column header OK
// ------------
-(void)drawRect:(NSRect)rect{
	[BrowserColumn setHeaderBackground];
	NSRectFill(rect);

BrowserColumn*	col;
NSEnumerator*		numer;
	numer=[_cols objectEnumerator];
	while((col=[numer nextObject])){
		if(NSMaxX([col frame])-_o.x<NSMinX([self bounds])){
			continue;
		}
		if(NSMinX([col frame])-_o.x>NSMaxX([self bounds])){
			break;
		}
		[col drawHeader:[self frame] offset:_o.x];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putExt:(bStdNSXMap*)ext{
	_ext=ext;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putLines:(bArray*)lines{
_bTrace_("[BrowserHeaderView putLines]",true);
bGenericGeoElement* geo;	
	_lines=lines;
	if(!_lines->get(1,&geo)){
		return;
	}
	_tp=[self app]->typesMgr()->get(geo->getType());
	[self read_p];
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
	return _tp;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(NSMutableArray*)columns{
	return _cols;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bArray*)selectedColumns{
	return _csels;
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
-(void)flushSelectedLines{
	[self app]->selMgr()->flush();
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getParamsInfos:(char*)directory 
				 name:(char*)prmName{
	GetSignatureString(_ext,directory);
	sprintf(prmName,"browsercolumns.xml");
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)getPanelInfos:(char*)name
		  signature:(char*)sign{
long	sgn=kBrowserPanelContentSignature;
	b_message_string("browser",name,_ext->getbundle(),1);
	sprintf(sign,"%.4s",(char*)&sgn);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)read_p{
_bTrace_("[BrowserHeaderView read_p]",true);
bGenericXMLBaseElement*		root=NULL;
char	directory[PATH_MAX];
char	fname[FILENAME_MAX];
	
	[self getParamsInfos:directory name:fname];
	root=GetXMLParamFromType([self app],_tp,directory,fname);
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
// ------------
-(void)write_p{
_bTrace_("[BrowserHeaderView write_p]",true);
bGenericXMLBaseElement*	root=[self make_p];
	if(!root){
		return;
	}
char	directory[PATH_MAX];
char	fname[FILENAME_MAX];
	
	[self getParamsInfos:directory name:fname];
	if(!SetXMLParamToType(_tp,directory,fname,root)){
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
_bTrace_("[BrowserHeaderView savePrefs]",true);
char	stra[PATH_MAX];
char	strb[FILENAME_MAX];
	
	[self getPanelInfos:stra signature:strb];
	add_cdesc(arr,indt+1,"sign",strb);
	add_cdesc(arr,indt+1,"name",stra);

bGenericXMLBaseElement*	root=[self make_p];
	if(!root){
_tw_("root==NULL");
		return YES;
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
_bTrace_("[BrowserHeaderView make_p]",true);
bArray			arr(sizeof(xmlelt));
BrowserColumn*	col;
char			str[256];
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");

NSEnumerator*	numer=[_cols objectEnumerator];
	while((col=[numer nextObject])){		
		[col name:str];
		add_cdesc(arr,1,"name",str);
		add_idesc(arr,1,"int",[col colKind]);
		add_ddesc(arr,1,"float",[col frame].size.width,2);
	}
	
bGenericXMLBaseElement*	root=[self app]->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	return(root);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)initType{
_bTrace_("[BrowserHeaderView initType]",true);
bGenericCalc*		clc;
float				strtx=0,strty=0;
float				w1=0;
BrowserColumn*	col;
	
	strtx+=w1;
	strty+=[BrowserColumn stdLineHeight];
	
int		l,k;
wbase	bse=_tp->fields();
float	w;

// Fields	
	for(long i=kOBJ_Name_;i<=_tp->fields()->count();i++){
		if(bse->is_hidden(i)){
			continue;
		}
		bse->get_len(i,&l);
		w=((float)l)*([BrowserColumn stdFontSize]*3.0/4.0);
		if(w>100){
			w=100;
		}
		else if(w<[BrowserColumn stdLineHeight]){
			w=[BrowserColumn stdLineHeight];
		}
		col=[self allocator:_lines field:i calc:NULL start:strtx width:w];
		if(col){
			[_cols addObject:col];
			[col release];
		}				  		
		strtx+=w;
	}

// Calcs	
	for(long i=1;i<=[self app]->calcMgr()->count();i++){
		clc=(bGenericCalc*)(void*)[self app]->calcMgr()->get(i);
		col=[self allocator:_lines field:0 calc:clc start:strtx width:[BrowserColumn stdColumnWidth]];
		if(col){
			[_cols addObject:col];
			[col release];
		}				  		
		strtx+=[BrowserColumn stdColumnWidth];
	}
	
	l=[_cols count]+1;
	k=128-l;
	if(k<=0){
_tm_("dépassement");
		[_obj putBounds];
		return;
	}
	
	for(long i=l;i<=k;i++){
		col=[self allocator:_lines field:0 calc:NULL start:strtx width:[BrowserColumn stdColumnWidth]];
		if(col){
			[_cols addObject:col];
			[col release];
		}				  		
		strtx+=[BrowserColumn stdColumnWidth];
	}
	[_obj putBounds];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)resetType{
	_csels->reset(); 
	[_cols removeAllObjects];
	[self initType];
	[self write_p];
	[_obj putBounds];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)setOrigin:(NSPoint)o{
	_o=o;
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(long)findColumn:(NSPoint)location
			limit:(BOOL&)flg{
BrowserColumn*	col;
	for(long i=0;i<[_cols count];i++){
		col=[_cols objectAtIndex:i];
		if(	(location.x+_o.x<=NSMaxX([col frame]))	&&
			(location.x+_o.x>NSMinX([col frame]))	){
			flg=(location.x+_o.x>NSMaxX([col frame])-3);
			return i+1;
		}
	}
	return 0;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{
BrowserColumn*	col;
NSUInteger		mod=[event modifierFlags];
BOOL			lim=NO;
long			x=[self findColumn:[self convertPoint:[event locationInWindow] fromView:nil] limit:lim];
		
	if((x>[_cols count])||(x==0)){
		return;
	}
	x--;
	if(lim){
		_cur=x;
	}
	
	if(mod&NSCommandKeyMask){
		col=[_cols objectAtIndex:x];
		if([col selected]){
			[col setSelected:NO inArray:_csels];
		}
		else{
			[col setSelected:YES inArray:_csels];
		}
	}
	else if(mod&NSShiftKeyMask){
long		first=-1,last=-1;
		
		for(long i=0;i<[_cols count];i++){
			col=[_cols objectAtIndex:i];
			if([col selected]){
				first=i;
				break;
			}
		}
		if(first!=-1){
			for(long i=[_cols count]-1;i>-1;i--){
				col=[_cols objectAtIndex:i];
				if([col selected]){
					last=i;
					break;
				}
			}
		}
		else{
			first=x;
		}
		if(last==-1){
			last=x;
		}
		if(x<=first){
			last=first-1;
			first=x;
		}
		else if(x>=last){
			first=last+1;
			last=x;
		}
		else{
			first=x;
			last=x;
		}		
		for(long i=first;i<=last;i++){
			col=[_cols objectAtIndex:i];
			if(![col selected]){
				[col setSelected:YES inArray:_csels];
			}
		}
	}
	else{
NSEnumerator*	numer=[_cols objectEnumerator];
		while((col=[numer nextObject])){		
			[col setSelected:NO inArray:_csels];
		}
		[self app]->selMgr()->flush();		
		col=[_cols objectAtIndex:x];
		[col setSelected:YES inArray:_csels];
	}
	[[[self window] windowController] updateUI];
}

// ---------------------------------------------------------------------------
// Change column
// ------------
-(void)rightMouseDown:(NSEvent*)event{
BOOL	lim=NO;
long	idx=[self findColumn:[self convertPoint:[event locationInWindow] fromView:nil] limit:lim];
	if((idx>[_cols count])||(idx==0)){
		return;
	}
	idx--;
BrowserColumn*	col=[_cols objectAtIndex:idx];
long				x,cur=0,n=(_tp->fields()->count()-kOBJ_Name_+1)+([self app]->calcMgr()->count())+3;
menuitem_desc*		desc=new menuitem_desc[n];		
char				str[256];
	
	[col name:str];

	x=0;
// Build Popup menu
	b_message_string(kBrowserMsgEmptyCol,desc[x].name,_ext->getbundle(),0);
	desc[x].checked=false;
    desc[x].disabled=false;
    desc[x].index=0;
	strcpy(desc[++x].name,"-");
	desc[x].checked=false;
    desc[x].disabled=false;
    desc[x].index=0;
	for(long i=kOBJ_Name_;i<=_tp->fields()->count();i++){
		_tp->fields()->get_name(i,desc[++x].name);
		desc[x].checked=false;
		desc[x].disabled=_tp->fields()->is_hidden(i);
        desc[x].index=0;
		if((cur==0)&&(!strcmp(desc[x].name,str))){
			cur=x;
		}
	}
	strcpy(desc[++x].name,"-");
	desc[x].checked=false;
	desc[x].disabled=false;
    desc[x].index=0;
	for(long i=1;i<=[self app]->calcMgr()->count();i++){
		[self app]->calcMgr()->ext_name(i,desc[++x].name);
		desc[x].checked=false;
		desc[x].disabled=false;
        desc[x].index=0;
		if((cur==0)&&(!strcmp(desc[x].name,str))){
			cur=x;
		}
	}
	desc[cur].checked=true;

// Pop the menu
NSPoint o=[col frame].origin;
	o.x-=_o.x;
CGPoint	location=NSPointToCGPoint([self convertPointToBase:o]);
long	cmd=popUpContextMenuWithCGPointAndNSWindow(location,[self window],desc,n);
	delete[] desc;
	if(cmd==0){
		return;
	}
// Replace column
BrowserColumn*	newcol=NULL;
NSRect				frame=[col frame];
	for(;;){
		if(cmd==1){
			newcol=[self allocator:_lines 
							 field:0
							  calc:NULL
							 start:frame.origin.x
							 width:frame.size.width];
			break;
		}

		cmd-=2;
		if(cmd<=(_tp->fields()->count()-kOBJ_Name_+1)){
			newcol=[self allocator:_lines
							 field:cmd+kOBJ_Name_-1
							  calc:NULL
							 start:frame.origin.x
							 width:frame.size.width];
			break;
		}
	
		cmd-=_tp->fields()->count();
		cmd+=kOBJ_Name_;
		cmd-=2;
		
		newcol=[self allocator:_lines
						 field:0
						  calc:(bGenericCalc*)(void*)[self app]->calcMgr()->get(cmd)
						 start:frame.origin.x
						 width:frame.size.width];
		break;
	}
	if(newcol){
BOOL	sel=[col selected];
		if(sel){
			[col setSelected:NO inArray:_csels];
		}
		[newcol setFrame:frame];
		[_cols replaceObjectAtIndex:idx withObject:newcol];
		[newcol release];
		if(sel){
			col=[_cols objectAtIndex:idx];
			[col setSelected:YES inArray:_csels];
		}
		[self setNeedsDisplay:YES];
		[_obj setNeedsDisplay:YES];
	}				  		
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseUp:(NSEvent*)event{
	_cur=-1;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDragged:(NSEvent*)event{
	if(_cur>-1){
CGFloat				dx=[event deltaX];
BrowserColumn*	col=[_cols objectAtIndex:_cur];
NSRect				frame=[col frame];
		frame.size.width+=dx;
		if(frame.size.width>[BrowserColumn stdLineHeight]){
			[col setFrame:frame];
			for(long i=_cur+1;i<[_cols count];i++){
				col=[_cols objectAtIndex:i];
				frame=[col frame];
				frame.origin.x+=dx;
				[col setFrame:frame];
			}
			[_obj putBounds];
			[self setNeedsDisplay:YES];
			[_obj setNeedsDisplay:YES];
		}
	}
}

// ---------------------------------------------------------------------------
// OK
// ------------
-(void)scrollWheel:(NSEvent *)evt{
	[super scrollWheel:evt];
	[_obj scrollWheel:evt];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserLeftView

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)isFlipped{
	return YES;
}

// ---------------------------------------------------------------------------
// Dessin du row header OK
// ------------
-(void)drawRect:(NSRect)rect{
	[BrowserColumn setHeaderBackground];
	NSRectFill(rect);
long		first=ceil(_o.y/[BrowserColumn stdLineHeight]);
long		last=first+floor([self frame].size.height/[BrowserColumn stdLineHeight]);
NSString*	nstr;
NSRect		box=[self bounds];
	box.size.height=[BrowserColumn stdLineHeight];
	if(first>0){
		first--;
	}
CGFloat	residu=first*[BrowserColumn stdLineHeight]-_o.y;
	box.origin.y+=residu;
BrowserColumn*	col=[[_hdr columns] objectAtIndex:0];
	for(long i=first;i<=last;i++){
		if([col lineSelected:i inArray:[_hdr lines]]){
			[BrowserColumn setHeaderHiliteColor];
			NSRectFill(box);
		}
		[BrowserColumn frameHeader:box];
		nstr=[NSString stringWithFormat:@"%ld",i+1];
		[nstr drawAtPoint:NSMakePoint(box.origin.x+2,box.origin.y+1) withAttributes:[BrowserColumn stringAttributes]];
		box.origin.y+=[BrowserColumn stdLineHeight];
	}
}

// ---------------------------------------------------------------------------
// OK
// ------------
-(void)setOrigin:(NSPoint)o{
	_o=o;
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// OK
// ------------
-(long)findRow:(NSPoint)location{
long	x=(location.y+_o.y)/[BrowserColumn stdLineHeight];
	return x;
}

// ---------------------------------------------------------------------------
// OK
// ------------
-(void)mouseDown:(NSEvent*)event{	
BrowserColumn*	col=[[_hdr columns] objectAtIndex:0];
NSUInteger		mod=[event modifierFlags];
long			x=[self findRow:[self convertPoint:[event locationInWindow] fromView:nil]];
	if((x>[_hdr lines]->count())||(x<0)){
		return;
	}
	if(mod&NSCommandKeyMask){
		[col setLineSelected:x
					 inArray:[_hdr lines]
					  status:![col lineSelected:x
										inArray:[_hdr lines]]];
	}
	else if(mod&NSShiftKeyMask){
long		first=-1,last=-1;
		
		for(long i=0;i<[_hdr lines]->count();i++){
			if([col lineSelected:i inArray:[_hdr lines]]){
				first=i;
				break;
			}
		}
		if(first!=-1){
			for(long i=[_hdr lines]->count()-1;i>-1;i--){
				if([col lineSelected:i inArray:[_hdr lines]]){
					last=i;
					break;
				}
			}
		}
		else{
			first=x;
		}
		if(last==-1){
			last=x;
		}
		if(x<=first){
			last=first;
			first=x;
		}
		else if(x>=last){
			first=last;
			last=x;
		}
		else{
			first=x;
			last=x;
		}		
		for(long i=first;i<=last;i++){
			[col setLineSelected:i inArray:[_hdr lines] status:YES];
		}
	}
    else if(mod&NSControlKeyMask){
// Nothing => Object drag to map
    }
	else{
NSEnumerator*	numer=[[_hdr columns] objectEnumerator];
		while((col=[numer nextObject])){		
			[col setSelected:NO inArray:[_hdr selectedColumns]];
		}
		[_hdr flushSelectedLines];		
		col=[[_hdr columns] objectAtIndex:0];
		[col setLineSelected:x
					 inArray:[_hdr lines]
					  status:YES];
	}
	if([event clickCount]>1){
		SendCommandToApplication('fich');
	}	
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseUp:(NSEvent*)event{
NSUInteger	mod=[event modifierFlags];
    if(mod&NSControlKeyMask){
NSPoint     p=[event locationInWindow];
i2dvertex   vx;
NSWindow*   wd=[event window];
    
        if(!NSPointInRect(p,[[wd initialFirstResponder] bounds])){
            p=[wd convertBaseToScreen:p];
            wd=(NSWindow*)[_hdr app]->mapIntf()->ref();
            p=[wd convertScreenToBase:p];
            if(NSPointInRect(p,[[wd initialFirstResponder] bounds])){
CGPoint         cgp=NSPointToCGPoint(p);
                [_hdr app]->locConverter()->convert(&vx,&cgp);
BrowserColumn*      col=[[_hdr columns] objectAtIndex:0];
bGenericGeoElement* geo=(bGenericGeoElement*)[col ref:0 inArray:[_hdr lines]];
                if(!geo){
                    return;
                }

bEventLog       log([_hdr app],
                    [_hdr ext]);
ivertices*      vxs;
long            dh,dv;
                for(long i=1;i<=[_hdr lines]->count();i++){
                    if([col lineSelected:i-1 inArray:[_hdr lines]]){
bGenericGeoElement* geo=(bGenericGeoElement*)[col ref:i-1 inArray:[_hdr lines]];
                        if(geo){
                            geo->inval(kOBJ_Vertices_);
                            geo->getVertices(&vxs);
                            dh=vx.h-vxs->vx.vx2[0].h;
                            dv=vx.v-vxs->vx.vx2[0].v;
                            ivs_move(vxs,dh,dv);
                            geo->setVertices(vxs);
                        }
                    }
                }
                log.close();
            }
            else{
                NSBeep();
            }
        }
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)scrollWheel:(NSEvent *)evt{
	[super scrollWheel:evt];
	[_obj scrollWheel:evt];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserNameLeftView

// ---------------------------------------------------------------------------
// Dessin du row header OK
// ------------
-(void)drawRect:(NSRect)rect{
	[BrowserColumn setHeaderBackground];
	NSRectFill(rect);
long		first=ceil(_o.y/[BrowserColumn stdLineHeight]);
long		last=first+floor([self frame].size.height/[BrowserColumn stdLineHeight]);
NSString*	nstr;
char		str[256];
NSRect		box=[self bounds];
	box.size.height=[BrowserColumn stdLineHeight];
	if(first>0){
		first--;
	}
CGFloat	residu=first*[BrowserColumn stdLineHeight]-_o.y;
	box.origin.y+=residu;
BrowserColumn*	col=[[_hdr columns] objectAtIndex:0];
	for(long i=first;i<=last;i++){
		if([col lineSelected:i inArray:[_hdr lines]]){
			[BrowserColumn setHeaderHiliteColor];
			NSRectFill(box);
		}
		[BrowserColumn frameHeader:box];
		
		[col name:str forLine:i inArray:[_hdr lines]];
		nstr=[NSString stringWithFormat:@"%s",str];
		
		[nstr drawAtPoint:NSMakePoint(box.origin.x+2,box.origin.y+1) withAttributes:[BrowserColumn stringAttributes]];
		box.origin.y+=[BrowserColumn stdLineHeight];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserObjView

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[BrowserObjView initWithFrame]",true);
	if((self=[super initWithFrame:frameRect])!=nil){
		_cidx=-1;
	}
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[BrowserObjView dealloc]",true);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)acceptsFirstResponder{
	return(YES);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawRect:(NSRect)rect{
	[[NSColor whiteColor] set];

NSScrollView*	scrollView=[self enclosingScrollView];	
NSRect			drawRect=[scrollView documentVisibleRect];
	
	[NSGraphicsContext saveGraphicsState];
	NSRectClip(drawRect);
	
	NSRectFill(drawRect);
	
	[_hdr setOrigin:drawRect.origin];
	[_lft setOrigin:drawRect.origin];

long	first=ceil(drawRect.origin.y/[BrowserColumn stdLineHeight]);
long	last=first+floor(drawRect.size.height/[BrowserColumn stdLineHeight]);

	if(first>0){
		first--;
	}
	
	[NSBezierPath setDefaultLineWidth:0.5];
	[[NSColor darkGrayColor] set];

NSRect	lineRect=drawRect;
	lineRect.origin.y=round(lineRect.origin.y/[BrowserColumn stdLineHeight])*[BrowserColumn stdLineHeight];
	lineRect.size.height=[BrowserColumn stdLineHeight];

// Dessin des valeurs
BrowserColumn*	col;
NSEnumerator*		numer;
	numer=[[_hdr columns] objectEnumerator];
	while((col=[numer nextObject])){
		if(NSMaxX([col frame])<NSMinX(drawRect)){
			continue;
		}
		if(NSMinX([col frame])>NSMaxX(drawRect)){
			break;
		}
		for(long i=first;i<=last;i++){
			[col drawLine:i inArray:[_hdr lines] rect:drawRect];
		}
		[col drawRight:rect];
	}
	
// Dessin de la ligne horizontale
	for(long i=first;i<=last;i++){
NSPoint	pta=NSMakePoint(NSMinX(lineRect),NSMinY(lineRect));
NSPoint	ptb=NSMakePoint(NSMaxX(lineRect),NSMinY(lineRect));
		[NSBezierPath strokeLineFromPoint:pta toPoint:ptb];
		lineRect.origin.y+=[BrowserColumn stdLineHeight];
	}
	
	[NSGraphicsContext restoreGraphicsState];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)isFlipped{
    return YES;
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
	if(_cidx!=-1){
		return;		
	}
	[super scrollWheel:evt];
	[_hdr setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{
	if(_cidx!=-1){
		[self abort:NO];
		[self endEdit];
		return;		
	}
NSPoint				location=[self convertPoint:[event locationInWindow] fromView:nil];
BrowserColumn*	col=NULL;
	
	for(long i=0;i<[[_hdr columns] count];i++){
		col=[[_hdr columns] objectAtIndex:i];
		if(	(location.x<=NSMaxX([col frame]))	&&
			(location.x>NSMinX([col frame]))	){
			_cidx=i;
			break;
		}
	}
	if(col==NULL){
		[self endEdit];
		return;
	}
long	ridx=location.y/[BrowserColumn stdLineHeight];
	if(![col mouse:self line:ridx inArray:[_hdr lines]]){
		[self endEdit];
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)rightMouseDown:(NSEvent*)event{
NSPoint				location=[self convertPoint:[event locationInWindow] fromView:nil];
BrowserColumn*	col=NULL;
	
	for(long i=0;i<[[_hdr columns] count];i++){
		col=[[_hdr columns] objectAtIndex:i];
		if((location.x<=NSMaxX([col frame]))&&
		   (location.x>NSMinX([col frame]))	){
			break;
		}
	}
	if(col==NULL){
		return;
	}
long ridx=location.y/[BrowserColumn stdLineHeight];
	if(![col rightMouse:self line:ridx inArray:[_hdr lines]]){
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)abort:(BOOL)closing{
	if(_cidx!=-1){
BrowserColumn*	col=[[_hdr columns] objectAtIndex:_cidx];
		[self endEdit];
		return [col abort:closing];
	}
	[self endEdit];
	return NO;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)endEdit{
	_cidx=-1;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)putBounds{
_bTrace_("[BrowserObjView putBounds]",true);	
NSScrollView*		scrollView=[self enclosingScrollView];	
BrowserColumn*	col=[[_hdr columns] lastObject];
NSRect				bnds=NSZeroRect;
	
	bnds.size.width=NSMaxX([col frame]);
	bnds.size.height=[_hdr lines]->count()*12;
	
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
// -----------
-(void)updateField:(long)fieldId{
//long			idx=_tp->fields()->get_index(fieldId);
//	if(idx){
//BrowserColumn*	Column;
//NSRect			frame;
//bool			fok=false;
//		for(long i=0;i<[_Columns count];i++){
//			Column=[_Columns objectAtIndex:i];
//			if([Column fieldIndex]==idx){
//				frame=[Column frame];
//				[_Columns removeObjectAtIndex:i];
//				makeColumn(_gapp,_tp,_Columns,idx,CGPointZero,true,true,kColumnNoKind);
//				fok=true;
//				break;
//			}
//		}
//		if(fok){
//			Column=[_Columns lastObject];
//			[Column setFrame:frame];
//			[self write_p];
//			[self setNeedsDisplay:YES];
//		}
//	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)removeField:(long)fieldId{
//long	idx=_tp->fields()->get_index(fieldId);
//	if(idx){
//BrowserColumn*	Column;
//		for(long i=0;i<[_Columns count];i++){
//			Column=[_Columns objectAtIndex:i];
//			if([Column fieldIndex]==idx){
//				[_Columns removeObjectAtIndex:i];
//				break;
//			}
//		}		
//		[self write_p];
//		[self setNeedsDisplay:YES];
//	}	
}

// ---------------------------------------------------------------------------
// 
// -----------
//-(void)addField:(long)fieldId{
//long	idx=_tp->fields()->get_index(fieldId);
//	if(idx){
//		makeColumn(_gapp,_tp,_Columns,idx,CGPointZero,true,true,kColumnNoKind);
//		[self write_p];
//		[self setNeedsDisplay:YES];
//	}
//}

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserCornerView

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)isFlipped{
	return YES;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)drawRect:(NSRect)rect{
	[BrowserColumn setHeaderBackground];
	NSRectFill([self bounds]);
	[BrowserColumn frameHeader:[self bounds]];
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

// ---------------------------------------------------------------------------
// 
// ------------
@implementation BrowserView
// ---------------------------------------------------------------------------
// 
// ------------
-(void)putExt:(bStdNSXMap*)ext{
	_sav=nil;
	[_hdr putExt:ext];
	[_mnu setAutoenablesItems:NO];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)putLines:(bArray*)lines{
	_bTrace_("[BrowserView putLines]",true);
	[_hdr putLines:lines];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericMacMapApp*)app{
	return [_hdr app];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bGenericType*)type{
	return [_hdr type];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(NSMutableArray*)columns{
	return [_hdr columns];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bArray*)selectedColumns{
	return [_hdr selectedColumns];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(bArray*)lines{
	return [_hdr lines];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BrowserHeaderView*)header{
	return _hdr;
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
	[_hdr write_p];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)savePrefs:(bArray&)arr
		  indent:(long)indt
			path:(const char*)fpath
			name:(const char*)fname{
	return [_hdr savePrefs:arr indent:indt path:fpath name:fname];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)abort{
	return [_obj abort:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)checkMacMapEvent{
_bTrace_("[BrowserController checkMacMapEvent]",true);
bArray*				arr=[_hdr app]->eventMgr()->events();
bGenericEvent*		evt;
int					i,j,k;
BOOL				needinval=FALSE;
BOOL                needreset=FALSE;
    
	for(i=1;i<=arr->count();i++){
		arr->get(i,&evt);
		if(evt->action()==kEventActionCreate){
			continue;
		}		
		switch(evt->kind()){
			case kEventKindGeoElement:{
bGenericGeoElement	*o,*mo;
				if(evt->action()==kEventActionModify){
bArray				tmp(*[_hdr lines]);
					tmp.sort(ObjRefComp);
					for(j=1;j<=evt->elements()->count();j++){
						evt->elements()->get(j,&mo);
						o=mo->get_ref();
						if(tmp.search(&o,ObjRefComp)){
							needinval=TRUE;
							break;
						}
					}
				}
				else if(evt->action()==kEventActionDestroy){
bArray				tmp(*(evt->elements()));
					tmp.sort(ObjRefComp);
					for(j=[_hdr lines]->count();j>0;j--){
						[_hdr lines]->get(j,&o);
						if((k=tmp.search(&o,ObjRefComp))){
							needinval=TRUE;
                            needreset=TRUE;
							[_hdr lines]->rmv(j);
						}
					}
					if([_hdr lines]->count()==0){
						CCWDXMap_Close(self);
						return FALSE;
					}
					if(needinval){
						[_obj putBounds];
					}
				}
			}
				break;
			case kEventKindDataStruct:{
field_info		fi;
				if(evt->action()==kEventActionModify){
					for(j=1;j<=evt->elements()->count();j++){
						evt->elements()->get(j,&fi);
						if(fi.tp!=[_hdr type]){
							continue;
						}
						needinval=true;
// trouver le moyen de retrouver le champ
// pour traiter le cas du changement de type
					}
				}
				else if(evt->action()==kEventActionDestroy){
					for(j=1;j<=evt->elements()->count();j++){
						evt->elements()->get(j,&fi);
						if(fi.tp!=[_hdr type]){
							continue;
						}
						needinval=true;
// trouver le moyen de retrouver le champ
// le supprimer
// gérer les ids
					}
				}
			}
				break;
			case kEventKindTypeElement:{
bGenericType*	tp;
				if(evt->action()==kEventActionDestroy){
					for(j=1;j<=evt->elements()->count();j++){
						evt->elements()->get(j,&tp);
						if([_hdr type]==tp){
							CCWDXMap_Close(self);
                            return FALSE;
						}
					}
				}
			}
				break;
			case kEventKindUserExt:{
//bGenericExt*		ext;
// trouver le moyen de retrouver la calc
				if(evt->action()==kEventActionModify){
					
				}
				else if(evt->action()==kEventActionDestroy){
					
				}
			}
				break;
		}
	}
	if(needinval){
		[[[self window] windowController] updateUI];
	}
    return needreset;
}

#pragma mark ---- Actions ----
// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doInsertColumn:(id)sender{
BrowserColumn*	col;
long				i;
	
	for(i=0;i<[[_hdr columns] count];i++){
		col=[[_hdr columns] objectAtIndex:i];
		if([col selected]){
			break;
		}
	}
	if(i==[[_hdr columns] count]){
		i=0;
		col=[[_hdr columns] objectAtIndex:0];
	}
NSRect	frame=[col frame];
	col=[_hdr allocator:[_hdr lines]
				  field:0
				   calc:NULL
				  start:frame.origin.x
				  width:[BrowserColumn stdColumnWidth]];
	frame.size.width=[BrowserColumn stdColumnWidth];
	[col setFrame:frame];
	[[_hdr columns] insertObject:col atIndex:i];
	[col release];
	
	i++;
	for(;i<[[_hdr columns] count];i++){
		col=[[_hdr columns] objectAtIndex:i];
		frame=NSOffsetRect([col frame],[BrowserColumn stdColumnWidth],0);
		[col setFrame:frame];
	}
	
	[_obj putBounds];
	[_hdr setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doRemoveColumn:(id)sender{
BrowserColumn*	col;
NSEnumerator*		numer=[[_hdr columns] reverseObjectEnumerator];
	while((col=[numer nextObject])){
		if([col selected]){
			[col setSelected:NO inArray:[_hdr selectedColumns]];
			[[_hdr columns] removeObject:col];
		}
	}
	
	if([[_hdr columns] count]==0){
		[_hdr resetType];
	}
	
CGFloat	start=0;
NSRect	frame;
	
	numer=[[_hdr columns] objectEnumerator];
	while((col=[numer nextObject])){
		frame=[col frame];
		frame.origin.x=start;
		[col setFrame:frame];
		start+=frame.size.width;
	}
	
	[_obj putBounds];
	[_hdr setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doSelectColumns:(id)sender{
BrowserColumn*	col;
NSEnumerator*		numer=[[_hdr columns] objectEnumerator];
	if([[NSApp currentEvent] modifierFlags]&NSCommandKeyMask){
		while((col=[numer nextObject])){
			[col setSelected:![col selected] inArray:[_hdr selectedColumns]];
		}
	}
	else{
		while((col=[numer nextObject])){
			[col setSelected:YES inArray:[_hdr selectedColumns]];
		}
	}
	[_obj setNeedsDisplay:YES];
	[_hdr setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doResetColumns:(id)sender{
	[_hdr resetType];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doInsertObjects:(id)sender{
long k=SelectionIsMonoType([_hdr app]);
	if(!k){
		return;
	}
	if(k!=[_hdr type]->index()){
		return;
	}
bArray				tmp(*[_hdr lines]);
bGenericGeoElement*	geo;
	
	tmp.sort(ObjRefComp);
	for(long i=1;i<=[_hdr app]->selMgr()->count();i++){
		[_hdr app]->selMgr()->elements()->get(i,&geo);
		k=tmp.search(&geo,ObjRefComp);
		if(!k){
			[_hdr lines]->add(&geo);
		}
	}
	[_obj putBounds];
	[[[self window] windowController] updateUI];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doRemoveObjects:(id)sender{
bGenericGeoElement*	geo;
	for(long i=[_hdr lines]->count();i>0;i--){
		[_hdr lines]->get(i,&geo);
		if(geo->selected()){
			[_hdr lines]->rmv(i);
		}
	}
	[_obj putBounds];
	[[[self window] windowController] updateUI];
	if([_hdr lines]->count()==0){
		CCWDXMap_Close(self);// No more objects in the browser, so we have to close it
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doSelectObjects:(id)sender{
NSUInteger			mod=[[NSApp currentEvent] modifierFlags];
bGenericGeoElement*	geo;
	
	if(mod&NSCommandKeyMask){
		for(long i=1;i<=[_hdr lines]->count();i++){
			[_hdr lines]->get(i,&geo);
			geo->setselect(!geo->selected(),true);
		}
	}
	else if(mod&NSShiftKeyMask){
		bArray	tmp(sizeof(bGenericGeoElement*));
		for(long i=1;i<=[_hdr lines]->count();i++){
			[_hdr lines]->get(i,&geo);
			if(!geo->selected()){
				tmp.add(&geo);
			}
		}
		[_hdr app]->selMgr()->add(&tmp);
	}
	else{
		[_hdr flushSelectedLines];
		[_hdr app]->selMgr()->add([_hdr lines]);
	}
}


#pragma mark ---- Sort ----
// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doStartSort:(id)sender{	
BrowserColumn*	col;
char			str[256];
	NSPopupButtonRemoveAllItems(_col);
	for(long i=1;i<=[_hdr selectedColumns]->count();i++){
		[_hdr selectedColumns]->get(i,&col);
		[col name:str];
		NSPopupButtonAddMenuItemValue(_col,str);
	}
    [_hdr selectedColumns]->get(1,&col);
    [_knd selectItemAtIndex:[col sortKind]-1];
    [_ord selectItemAtIndex:([col sortOrder]==-1?1:0)];
	[NSApp beginSheet:_srt
       modalForWindow:[self window]
        modalDelegate:self
       didEndSelector:@selector(sortPanelDidEnd:returnCode:contextInfo:)
          contextInfo:nil];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doChooseColumn:(id)sender{	
BrowserColumn*	col;
	[_hdr selectedColumns]->get([_col indexOfSelectedItem]+1,&col);
	[_knd selectItemAtIndex:[col sortKind]-1];
	[_ord selectItemAtIndex:([col sortOrder]==-1?1:0)];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doChooseSortKind:(id)sender{
BrowserColumn*	col;
	[_hdr selectedColumns]->get([_col indexOfSelectedItem]+1,&col);
	[col setSortKind:[_knd indexOfSelectedItem]+1];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doChooseSortOrder:(id)sender{
BrowserColumn*	col;
	[_hdr selectedColumns]->get([_col indexOfSelectedItem]+1,&col);
	[col setSortOrder:([_ord indexOfSelectedItem]==0?1:-1)];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doSortOk:(id)sender{
	[NSApp endSheet:_srt returnCode:NSOKButton];	
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)doSortCancel:(id)sender{
	[NSApp endSheet:_srt returnCode:NSCancelButton];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)	sortPanelDidEnd:(NSSavePanel *)sheet 
			 returnCode:(int)returnCode 
			contextInfo:(void *)contextInfo{
    [_srt orderOut:self];
	if(NSOKButton==returnCode){
bArray				lsel(sizeof(bBrowserStackValue*));
bBrowserStackValue*	stk;
bBrowserSortValue*	val;
bGenericGeoElement*	geo;
BrowserColumn*		col;
bool				all=false;
long				i,j;
double				vn;
char				vc[1024],msg[__MESSAGE_STRING_LENGTH_MAX__];
		
// Data load
		{	
			b_message_string(kBrowserMsgDataLoad,msg,[_hdr ext]->getbundle(),0);
bProgressWait wt("",msg,true,[_hdr lines]->count()*2L);
			
// Only selected lines
			for(i=1;i<=[_hdr lines]->count();i++){
				if(!wt.set_progress(i)){
					break;
				}
				[_hdr lines]->get(i,&geo);
				if(!geo->selected()){
					continue;
				}
				stk=new bBrowserStackValue([_hdr selectedColumns]->count());
				stk->put_ref(geo);
				for(j=1;j<=[_hdr selectedColumns]->count();j++){
					[_hdr selectedColumns]->get(j,&col);
					switch([col sortKind]){
						case kSortNum:
							vn=[col value:i-1 inArray:[_hdr lines]];
							val=new bBrowserDoubleSortValue([col sortOrder],vn);
							break;
						case kSortAlpha:
							[col value:vc forLine:i-1 inArray:[_hdr lines]];
							val=new bBrowserCharSortValue([col sortOrder],vc);
							break;
						case kSortAlphaNum:
							[col value:vc forLine:i-1 inArray:[_hdr lines]];
							val=new bBrowserCharSortValue([col sortOrder],vc);// Alpha num par la suite
							break;
						default:
							[col value:vc forLine:i-1 inArray:[_hdr lines]];
							val=new bBrowserCharSortValue([col sortOrder],vc);
							break;
					}
					stk->put(j-1,val);
				}
				lsel.add(&stk);
			}
			
// All if no line selected
			if(lsel.count()==0){
				all=true;
				for(i=1;i<=[_hdr lines]->count();i++){
					if(!wt.set_progress(i+[_hdr lines]->count())){
						break;
					}
					[_hdr lines]->get(i,&geo);
					stk=new bBrowserStackValue([_hdr selectedColumns]->count());
					stk->put_ref(geo);
					for(j=1;j<=[_hdr selectedColumns]->count();j++){
						[_hdr selectedColumns]->get(j,&col);
						switch([col sortKind]){
							case kSortNum:
								vn=[col value:i-1 inArray:[_hdr lines]];
								val=new bBrowserDoubleSortValue([col sortOrder],vn);
								break;
							case kSortAlpha:
								[col value:vc forLine:i-1 inArray:[_hdr lines]];
								val=new bBrowserCharSortValue([col sortOrder],vc);
								break;
							case kSortAlphaNum:
								[col value:vc forLine:i-1 inArray:[_hdr lines]];
								val=new bBrowserCharSortValue([col sortOrder],vc);// Alpha num par la suite
								break;
							default:
								[col value:vc forLine:i-1 inArray:[_hdr lines]];
								val=new bBrowserCharSortValue([col sortOrder],vc);
								break;
						}
						stk->put(j-1,val);
					}
					lsel.add(&stk);
				}
				
			}
		}
		
// Sort		
		{	
			b_message_string(kBrowserMsgSortPrgr,msg,[_hdr ext]->getbundle(),0);
bBarberWait wt("",msg,false);
			bBrowserStackValue::put_wt(&wt);
			lsel.sort(bBrowserStackValue::compare);
		}	
		
// Update lines index
		if(!all){
			int			ck=0;
			for(i=1;i<=[_hdr lines]->count();i++){
				[_hdr lines]->get(i,&geo);
				if(!geo->selected()){
					continue;
				}
				ck++;
				lsel.get(ck,&stk);
				geo=(bGenericGeoElement*)stk->get_ref();
				[_hdr lines]->put(i,&geo);
			}
		}
		else{
			for(i=1;i<=[_hdr lines]->count();i++){
				lsel.get(i,&stk);
				geo=(bGenericGeoElement*)stk->get_ref();
				[_hdr lines]->put(i,&geo);
			}
		}
		
		// Free
		for(i=1;i<=lsel.count();i++){
			lsel.get(i,&stk);
			delete stk;
		}	
		
		[self updateUI];
	}
}

#pragma mark ---- Export ----
// ---------------------------------------------------------------------------
// Export
// ------------
-(IBAction)doExport:(id)sender{
	_sav=[NSSavePanel savePanel];
	[_sav setAccessoryView:_fmt];
	[_sav setAllowedFileTypes:[NSArray arrayWithObjects:@"txt",nil]];
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
-(IBAction)doPutFileType:(id)sender{
	if(_sav){
		switch([_fmt indexOfSelectedItem]){
			case 1:
				[_sav setAllowedFileTypes:[NSArray arrayWithObjects:@"csv",nil]];
				break;
			case 2:
				[_sav setAllowedFileTypes:[NSArray arrayWithObjects:@"dbf",nil]];
				break;
			case 3:
				[_sav setAllowedFileTypes:[NSArray arrayWithObjects:@"sql",nil]];
				break;
			default:
				[_sav setAllowedFileTypes:[NSArray arrayWithObjects:@"txt",nil]];
				break;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)	savePanelDidEnd:(NSSavePanel *)sheet 
			 returnCode:(int)returnCode 
			contextInfo:(void *)contextInfo{
_bTrace_("[BrowserWindowController savePanelDidEnd]",true);
    if(NSOKButton==returnCode){
		unsigned long fkind;
		switch([_fmt indexOfSelectedItem]){
			case 1:
				fkind=kTableCSVText;
				break;
			case 2:
				fkind=kTableDBF;
				break;
			case 3:
				fkind=kTableSQL;
				break;
			default:
				fkind=kTableTabText;
				break;
		}
		
char	fname[FILENAME_MAX];
char	fpath[PATH_MAX];
		
		[[[[sheet filename] stringByReplacingOccurrencesOfString:[sheet directory] withString:@""] 
		  stringByReplacingOccurrencesOfString:@"/" withString:@""] 
		 getCString:fname maxLength:FILENAME_MAX encoding:NSUTF8StringEncoding];
		
		[[sheet directory] getCString:fpath maxLength:PATH_MAX encoding:NSUTF8StringEncoding];
		if(strlen(fpath)>0){
			strcat(fpath,"/");
		}
_tm_(fname);		
_tm_(fpath);	
_tm_([_fmt indexOfSelectedItem]);

int		status=0;
wtable	tbl=wtbl_alloc(fkind,fpath,fname,true,1,-1,&status);
		if(tbl){
			
BrowserColumn*	col;
NSEnumerator*	numer=[[_hdr columns] objectEnumerator];
long			maxSz=0;
char			cname[256];
			
			while((col=[numer nextObject])){
				if([col selected]){
					if([col len]>maxSz){
						maxSz=[col len];
					}
					[col name:cname];
					if(strlen(cname)==0){
						sprintf(cname,"C%d",[[_hdr columns] indexOfObject:col]+1);
					}
					if(tbl->AddField(cname,[col kind],[col len],[col decs])!=0){
_te_("tbl->AddField failed for "+cname+" at index "+([[_hdr columns] indexOfObject:col]+1));
						_sav=nil;
						delete tbl;
						return;
					}					
				}
			}

bool	onlySel=NO;
			
			col=[[_hdr columns] objectAtIndex:0];
			for(long i=1;i<=[_hdr lines]->count();i++){
				if([col lineSelected:i-1 inArray:[_hdr lines]]){
					onlySel=YES;
					break;
				}
			}
			
void*	buffer=(char*)malloc(maxSz+1);
long	ci,ri;
char	msg[__MESSAGE_STRING_LENGTH_MAX__];
			message_string(kMsgProgress,msg,0);
bProgressWait wt("",msg,true,[_hdr lines]->count());
			
			ri=1;
			for(long i=1;i<=[_hdr lines]->count();i++){
				if(!wt.set_progress(i)){
					break;
				}
				ci=0;
				numer=[[_hdr columns] objectEnumerator];
				while((col=[numer nextObject])){
					if(![col selected]){
						continue;
					}
					ci++;
					if(onlySel&&(ci==1)&&(![col lineSelected:i-1 inArray:[_hdr lines]])){
						break;
					}
					[col value:(char*)buffer forLine:i-1 inArray:[_hdr lines]];
					if(charToX([col kind],[col decs],(char*)buffer,buffer)){
						tbl->WriteVal(ri,ci,buffer);
					}
				}
				ri++;
			}
			delete tbl;
			free(buffer);
			
		}
		_sav=nil;
    }
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
		NSPopupButtonMenuItemDisable(_mnu,2);
		NSPopupButtonMenuItemDisable(_mnu,10);
		NSPopupButtonMenuItemDisable(_mnu,12);
	}
	else{
		NSPopupButtonMenuItemEnable(_mnu,2);
		NSPopupButtonMenuItemEnable(_mnu,10);		
		NSPopupButtonMenuItemEnable(_mnu,12);		
	}
	
	if([_hdr app]->selMgr()->count()==0){
		NSPopupButtonMenuItemDisable(_mnu,6);
		NSPopupButtonMenuItemDisable(_mnu,7);
	}
	else{
		NSPopupButtonMenuItemEnable(_mnu,6);
		NSPopupButtonMenuItemEnable(_mnu,7);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
@end

