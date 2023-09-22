//----------------------------------------------------------------------------
// File : BrowserColumn.mm
// Project : MacMap
// Purpose : Objective C++ source file : Browser column classes
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


#import "BrowserColumn.h"
#import <mox_intf/bCoreTextInfo.h>
#import <mox_intf/bEventLog.h>
#import <mox_intf/NSUIUtils.h>
#import <mox_intf/NSContextualMenuWrappers.h>
#import <mox_intf/bGenericCalc.h>

#import <std_ext/bStdUserExt.h>
#import <mox_intf/ext_utils.h>

#import <MacMapSuite/bTrace.h>
#import "BrowserView.h"

// ---------------------------------------------------------------------------
// 
// -----------
#define _XOFFSET_				3.0
#define _YOFFSET_				0.5
#define _RADIUS_				3.0
#define _LINEWIDTH_				0.5
#define _NAMEBACKCOLORNORMAL_	[NSColor lightGrayColor]
#define _NAMEBACKCOLORHILITED_	[NSColor darkGrayColor]
#define _VALUEBACKCOLOR_		[NSColor whiteColor]
#define _STROKECOLOR_			[NSColor blackColor]

#define _DEFAULT_WIDTH_         50
#define _DEFAULT_HEIGHT_        12
#define _DEFAULT_FONTSIZE_      9
#define _DEFAULT_FONT_          @"Geneva"


// ---------------------------------------------------------------------------
// Std column (base class)
// -----------
@implementation BrowserColumn

// ---------------------------------------------------------------------------
// 
// -----------
static bGenericMacMapApp*	_gapp;
static NSMutableDictionary*	_strAttr=NULL;

// ---------------------------------------------------------------------------
// 
// -----------
+(void)frameHeader:(NSRect)rect{
NSRect	box=NSInsetRect(rect,0.5,0.5);
	
	[NSBezierPath setDefaultLineWidth:1];
	[[NSColor whiteColor] set];
	[NSBezierPath strokeLineFromPoint:NSMakePoint(NSMinX(box),NSMinY(box)) toPoint:NSMakePoint(NSMinX(box),NSMaxY(box))];
	[NSBezierPath strokeLineFromPoint:NSMakePoint(NSMinX(box),NSMinY(box)) toPoint:NSMakePoint(NSMaxX(box),NSMinY(box))];
	
	[[NSColor darkGrayColor] set];
	[NSBezierPath strokeLineFromPoint:NSMakePoint(NSMaxX(box),NSMaxY(box)) toPoint:NSMakePoint(NSMaxX(box),NSMinY(box))];
	[NSBezierPath strokeLineFromPoint:NSMakePoint(NSMaxX(box),NSMaxY(box)) toPoint:NSMakePoint(NSMinX(box),NSMaxY(box))];
}

// ---------------------------------------------------------------------------
// 
// -----------
+(void)setHeaderBackground{
	[[NSColor lightGrayColor] set];
}

// ---------------------------------------------------------------------------
// 
// -----------
+(void)setHiliteColor{
	[[NSColor colorWithDeviceRed:0.8 green:0.8 blue:1 alpha:1] set];
}

// ---------------------------------------------------------------------------
// 
// -----------
+(void)setHeaderHiliteColor{
	[[NSColor colorWithDeviceRed:0.64 green:0.64 blue:0.84 alpha:1] set];
}

// ---------------------------------------------------------------------------
// 
// -----------
+(CGFloat)stdLineHeight{
	return _DEFAULT_HEIGHT_;
}

// ---------------------------------------------------------------------------
// 
// -----------
+(CGFloat)stdColumnWidth{
	return _DEFAULT_WIDTH_;
}

// ---------------------------------------------------------------------------
// 
// -----------
+(CGFloat)stdFontSize{
	return _DEFAULT_FONTSIZE_;
}

// ---------------------------------------------------------------------------
// 
// -----------
+(NSString*)stdNSFont{
	return _DEFAULT_FONT_;
}

// ---------------------------------------------------------------------------
// 
// -----------
+(NSMutableDictionary*)stringAttributes{
	return _strAttr;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp{
	_gapp=gapp;
	
	if(_strAttr==NULL){
		_strAttr = [NSMutableDictionary dictionary];
		[_strAttr setObject:[NSFont fontWithName:_DEFAULT_FONT_ size:_DEFAULT_FONTSIZE_] forKey:NSFontAttributeName];
		[_strAttr setObject:[NSColor blackColor] forKey:NSForegroundColorAttributeName];
		[_strAttr retain];
	}
	
    if(self=[super init]){
		_sel=false;
		_frame=frame;
		_frame.size.height=100;
		_ord=1;
		_srt=kSortNum;
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)dealloc{
//_bTrace_("[BrowserColumn dealloc]",true);
    [super dealloc];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setFrame:(NSRect)frame{
	_frame=frame;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(NSRect)frame{
	return _frame;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setSelected:(BOOL)flg 
		   inArray:(bArray*)cells{
	if(flg!=_sel){
BrowserColumn*	x=self;
		if(flg){
			cells->add(&x);
		}
		else{
			for(long i=cells->count();i>0;i--){
				cells->get(i,&x);
				if(x==self){
					cells->rmv(i);
				}
			}
		}
	}
	_sel=flg;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)selected{
	return _sel;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setLineSelected:(long)lin 
			   inArray:(bArray*)lines 
				status:(BOOL)flg{
bGenericGeoElement*	geo;
	if(lines->get(lin+1,&geo)){
		geo->setselect(flg,true);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)lineSelected:(long)lin
			inArray:(bArray*)lines{
bGenericGeoElement*	geo;
	if(lines->get(lin+1,&geo)){
		return geo->selected();
	}
	return false;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)name:(char*)buffer 
	 forLine:(long)lin
	 inArray:(bArray*)arr{
bGenericGeoElement* geo;
	buffer[0]=0;
	if(!arr->get(lin+1,&geo)){
		return;
	}
	geo->getName(buffer);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)value:(char*)buffer 
	 forLine:(long)lin
	   inArray:(bArray*)arr{
	buffer[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)value:(long)lin
	 inArray:(bArray*)arr{
	return 0;
}

// ---------------------------------------------------------------------------
//
// -----------
-(void*)ref:(long)lin
       inArray:(bArray*)arr{
bGenericGeoElement* geo;
    if(!arr->get(lin+1,&geo)){
        return NULL;
    }
    return geo;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)name:(char*)buffer{
	buffer[0]=0;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)kind{
	return _char;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)colKind{
	return 0;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)len{
	return 256;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)decs{
	return 0;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)sortKind{
	return _srt;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setSortKind:(long)sort{
	_srt=sort;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)sortOrder{
	return _ord;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setSortOrder:(long)order{
	_ord=order;
}

// ---------------------------------------------------------------------------
// Dessin du top header
// -----------
-(void)drawHeader:(NSRect)rect offset:(CGFloat)off{
NSRect	box=_frame;
	box.size.height=rect.size.height;
	box=NSOffsetRect(box,-off,0);
	if(_sel){
		[BrowserColumn setHeaderHiliteColor];
		NSRectFill(box);
	}		
	[BrowserColumn frameHeader:box];
	
char	str[256];
	[self name:str];
	if(strlen(str)>0){
		[NSGraphicsContext saveGraphicsState];
		NSRectClip(box);
NSString* nstr=[NSString stringWithCString:str encoding:NSMacOSRomanStringEncoding];
		[nstr drawAtPoint:NSMakePoint(box.origin.x+2,box.origin.y+1) withAttributes:_strAttr];
		[NSGraphicsContext restoreGraphicsState];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)drawRight:(NSRect)rect{
	[NSBezierPath setDefaultLineWidth:0.5];
	[[NSColor darkGrayColor] set];
NSPoint	pta=NSMakePoint(NSMaxX(_frame),NSMinY(rect));
NSPoint	ptb=NSMakePoint(NSMaxX(_frame),NSMaxY(rect));
	[NSBezierPath strokeLineFromPoint:pta toPoint:ptb];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)drawLine:(long)lin 
		inArray:(bArray*)arr
		   rect:(NSRect)frame{
char	str[1024];
	[self value:str forLine:lin inArray:arr];
    if(strlen(str)==0 && [self lineSelected:lin inArray:arr]==false && _sel==false ){// => rien à dessiner
        return;
    }

NSRect    box=_frame;
    box.size.height=_DEFAULT_HEIGHT_;
    box.origin.y=_frame.origin.y+lin*_DEFAULT_HEIGHT_;
	[NSGraphicsContext saveGraphicsState];
	NSRectClip(box);
	if(([self lineSelected:lin inArray:arr])||_sel){
		[BrowserColumn setHiliteColor];
		NSRectFill(box);
	}
	if(strlen(str)>0){
NSString* nstr=[NSString stringWithCString:str encoding:NSMacOSRomanStringEncoding];	
		[nstr drawAtPoint:NSMakePoint(box.origin.x+2,box.origin.y+1) withAttributes:_strAttr];
	}
	[NSGraphicsContext restoreGraphicsState];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)mouse:(NSView*)view 
		line:(long)lin 
	 inArray:(bArray*)arr{
	return NO;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)rightMouse:(NSView*)view
			 line:(long)lin 
		  inArray:(bArray*)arr{
	return NO;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)abort:(BOOL)closing;
{
	return NO;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end


// ---------------------------------------------------------------------------
// Field column
// -----------
@implementation BrowserFieldColumn

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp
			field:(long)fld
			 type:(bGenericType*)typ{	
    if(self=[super initWithRect:frame lines:lins application:gapp]){
		_field=fld;
		_gtp=typ;
		switch([self kind]){
			case _bool:
			case _int:
			case _double:
				_srt=kSortNum;
				break;
			default:
				_srt=kSortAlpha;
				break;	
		}		
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)value:(char*)buffer 
	 forLine:(long)lin
	 inArray:(bArray*)arr{
bGenericGeoElement* geo;
	buffer[0]=0;
	if(!arr->get(lin+1,&geo)){
		return;
	}
	geo->getValue(_field,buffer);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)value:(long)lin
	   inArray:(bArray*)arr{
bGenericGeoElement* geo;
	if(!arr->get(lin+1,&geo)){
		return 0;
	}
double	r;
	geo->getValue(_field,&r);
	return r;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)name:(char*)buffer{
	_gtp->fields()->get_name(_field,buffer);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)kind{
int	k;
    
    if(_gtp->fields()->count_constraints(_field)>0){
        k=_gtp->fields()->get_constraints_kind(_field);
    }
    else{
        _gtp->fields()->get_kind(_field,&k);
    }

//	if((_field==kOBJ_SubType_)||(_field==kOBJ_Color_)){
//		return _char;
//	}
//	_gtp->fields()->get_kind(_field,&k);
	return k;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)len{
int	l;
	_gtp->fields()->get_len(_field,&l);
	return l;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)decs{
int	d;
	_gtp->fields()->get_decs(_field,&d);
	return d;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)colKind{
	return 1;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)rightMouse:(NSView*)view		
             line:(long)lin 
		  inArray:(bArray*)arr{
bGenericGeoElement* geo;
	if(!arr->get(lin+1,&geo)){
		return NO;
	}
long			n=_gapp->calcMgr()->count();
menuitem_desc	desc[n];
		
	for(long i=1;i<=_gapp->calcMgr()->count();i++){
		_gapp->calcMgr()->ext_name(i,desc[i-1].name);
		desc[i-1].checked=false;
        desc[i-1].disabled=false;
        desc[i-1].index=0;
	}

NSPoint nsp=NSMakePoint(0,0);
    nsp=[view convertPoint:nsp toView:[[view window] contentView]];
    nsp.x*=2.0;//?
    nsp.x+=_frame.origin.x;
    nsp.y=_frame.origin.y+[BrowserFieldColumn stdLineHeight]*lin;
CGPoint location=NSPointToCGPoint(NSViewConvertPointTo(view,nsp));
long	which=popUpContextMenuWithCGPointAndNSWindow(location,
													 [view window],
													 desc,
													 n,
                                                     0.5);
	
	if(which>0){
bGenericCalc*	clc=(bGenericCalc*)(void*)_gapp->calcMgr()->get(which);
		if(!clc){
			return NO;
		}
bStdDBValue*	val=clc->solve(geo);
		if(!clc){
			return NO;
		}
char	cval[1024]="";
int		ival,k;
double	dval;
bool	b=false;
		
		if(FieldInView(_gapp,_gtp,_field)){
			geo->inval(_field);
		}
		
CFBundleRef	bndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.xmapbrowser"));
bEventLog	log(_gapp,
				bndl,
				kBrowserMessageID,
				GetSignatureFromBundle(bndl));

		_gtp->fields()->get_kind(_field,&k);
		switch(k){
			case _bool:
			case _int:
				val->get(&ival);
				b=geo->setValue(_field,ival);
				break;
			case _double:
			case _date:
			case _time:
				val->get(&dval);
				b=geo->setValue(_field,dval);
				break;
			case _char:
				val->get(cval);
				b=geo->setValue(_field,cval);
				break;
		}
		log.close();
		if(!b){
			NSBeep();
		}
		delete val;
		if(FieldInView(_gapp,_gtp,_field)){
			geo->inval(_field);
		}
		[view setNeedsDisplay:YES];
	}
	return YES;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

// ---------------------------------------------------------------------------
// Static text field column
// -----------
@implementation BrowserFieldStaticColumn

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)rightMouse:(NSView*)view
			 line:(long)lin 
		  inArray:(bArray*)arr{
	return NO;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

//----------------------------------------------------------------------------
// Check Field column classe
//----------------------------------------------------------------------------
@implementation BrowserFieldCheckColumn

//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)mouse:(NSView*)view 
		line:(long)lin 
	 inArray:(bArray*)arr{
bGenericGeoElement* geo;
	if(!arr->get(lin+1,&geo)){
		return NO;
	}
int	val;
	geo->getValue(_field,&val);
	val=!val;
	
	if(FieldInView(_gapp,_gtp,_field)){
		geo->inval(_field);
	}	
CFBundleRef	bndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.xmapbrowser"));
bEventLog	log(_gapp,
				bndl,
				kBrowserMessageID,
				GetSignatureFromBundle(bndl));	
	geo->setValue(_field,&val);
	log.close();
	if(FieldInView(_gapp,_gtp,_field)){
		geo->inval(_field);
	}
	[view setNeedsDisplay:YES];

	return NO;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// Popup menu Field column classe
//----------------------------------------------------------------------------
@implementation BrowserFieldPopupColumn

//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)mouse:(NSView*)view 
		line:(long)lin 
	 inArray:(bArray*)arr{
_bTrace_("[BrowserFieldPopupColumn mouse:line:inArray]",true);
bGenericGeoElement* geo;
	if(!arr->get(lin+1,&geo)){
		return NO;
	}
long			i,idx,n=_gtp->fields()->count_constraints(_field);
menuitem_desc	desc[n];
int				k,d,ck,fk;
char			cval[256];
int				ival;
double			dval;
	
	_gtp->fields()->get_kind(_field,&k);
	_gtp->fields()->get_decs(_field,&d);
	ck=_gtp->fields()->get_constraints_kind(_field);
	fk=k;
	if((ck!=k)&&(ck!=_bit)){
		k=_char;
	}
	else if((ck!=k)&&(ck==_bit)){
		k=_bit;
	}
	
	switch(k){
		case _bool:
		case _int:
			geo->getValue(_field,&ival);
			idx=_gtp->fields()->get_constraint_index(_field,&ival);
			for(i=1;i<=n;i++){
				_gtp->fields()->get_constraint(_field,i,&ival);
				sprintf(desc[i-1].name," %d",ival);	
				desc[i-1].checked=(i==idx);
                desc[i-1].disabled=false;
                desc[i-1].index=0;
			}
			break;
		case _double:
			geo->getValue(_field,&dval);
			idx=_gtp->fields()->get_constraint_index(_field,&dval);
			for(i=1;i<=n;i++){
				_gtp->fields()->get_constraint(_field,i,&dval);
				sprintf(desc[i-1].name," %.*f",d,dval);
				desc[i-1].checked=(i==idx);
                desc[i-1].disabled=false;
                desc[i-1].index=0;
			}
			break;
		case _char:
			geo->getValue(_field,cval);
			idx=_gtp->fields()->get_constraint_index(_field,cval);
			for(i=1;i<=n;i++){
				_gtp->fields()->get_constraint(_field,i,desc[i-1].name);
				desc[i-1].checked=(i==idx);
                desc[i-1].disabled=false;
                desc[i-1].index=0;
			}
			break;
		case _bit:
			if(fk==_char){
				geo->getValue(_field,cval);
				if(strlen(cval)<n){
					for(int j=strlen(cval);j<n;j++){
						strcat(cval,"0");
					}				
				}
				for(i=1;i<=n;i++){
					_gtp->fields()->get_constraint(_field,i,desc[i-1].name);
					desc[i-1].checked=(cval[i-1]!='0');
                    desc[i-1].disabled=false;
                    desc[i-1].index=0;
				}
			}
			else if(fk==_int){
				geo->getValue(_field,&ival);
				for(i=1;i<=n;i++){
					_gtp->fields()->get_constraint(_field,i,desc[i-1].name);
					desc[i-1].checked=btst(ival,i-1);
                    desc[i-1].disabled=false;
                    desc[i-1].index=0;
				}
			}
			break;
	}

NSPoint nsp=NSMakePoint(0,0);
    nsp=[view convertPoint:nsp toView:[[view window] contentView]];
    nsp.x*=2.0;//?
    nsp.x+=_frame.origin.x;
	nsp.y=_frame.origin.y+[BrowserFieldColumn stdLineHeight]*lin;
CGPoint location=NSPointToCGPoint(NSViewConvertPointTo(view,nsp));
long    which=popUpContextMenuWithCGPointAndNSWindow(location,
                                                     [view window],
                                                     desc,
                                                     n,
                                                     0.5);
	
	if(which>0){
bool b=false;
		if(FieldInView(_gapp,_gtp,_field)){
			geo->inval(_field);
		}
		
CFBundleRef	bndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.xmapbrowser"));
bEventLog	log(_gapp,
				bndl,
				kBrowserMessageID,
				GetSignatureFromBundle(bndl));	
		
		if(ck!=fk){
			if(ck==_bit){
				if(fk==_int){
					geo->getValue(_field,&ival);
					if(btst(ival,which-1)){
						bclr(ival,which-1);
					}
					else{
						bset(ival,which-1);
					}
					b=geo->setValue(_field,ival);
				}
				else if(fk==_char){
					geo->getValue(_field,cval);
					if(strlen(cval)<n){
						for(int j=strlen(cval);j<n;j++){
							strcat(cval,"0");
						}
					}
					if(cval[which-1]=='0'){
						cval[which-1]='1';
					}
					else{
						cval[which-1]='0';
					}
					b=geo->setValue(_field,cval);				
				}				
			}
			else{
				b=geo->setValue(_field,(int)which);
			}
		}
		else switch(ck){
			case _bool:
			case _int:
				_gtp->fields()->get_constraint(_field,which,&ival);
				b=geo->setValue(_field,ival);
				break;
			case _double:
			case _date:
			case _time:
				_gtp->fields()->get_constraint(_field,which,&dval);
				b=geo->setValue(_field,dval);
				break;
			case _char:
				_gtp->fields()->get_constraint(_field,which,cval);
				b=geo->setValue(_field,cval);
				break;
		}
		if(!b){
			NSBeep();
		}
		else{
			[view setNeedsDisplayInRect:_frame];
		}
		log.close();
		if(FieldInView(_gapp,_gtp,_field)){
			geo->inval(_field);
		}
		
		[view setNeedsDisplay:YES];
	}	

	return NO;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
// EditText Field column classe
//----------------------------------------------------------------------------
@implementation BrowserFieldEditTextColumn

//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp
			field:(long)fld
			 type:(bGenericType*)typ{	
    if(self=[super initWithRect:frame 
						  lines:lins 
					application:gapp 
						  field:fld 
						   type:typ]){
		_fld=NULL;
		_geo=NULL;
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)mouse:(NSView*)view 
		line:(long)lin 
	 inArray:(bArray*)arr{
	if(!arr->get(lin+1,&_geo)){
		return NO;
	}
	if(_fld==NULL){
NSRect	frame=_frame;
		frame.origin.y=_frame.origin.y+[BrowserFieldColumn stdLineHeight]*lin;
		frame.size.height=[BrowserFieldColumn stdLineHeight];
		
		frame=NSInsetRect(frame,-4.0,-4.0);
		if(frame.size.width>300){
			frame.size.width=300;
		}
		_fld=[[NSTextField alloc] initWithFrame:frame];
		[view addSubview:_fld];
		[[view window] makeKeyAndOrderFront:nil];
		[[view window] makeFirstResponder:_fld];
		[_fld setTarget:self];
		[_fld setAction:@selector(assign:)];
		
		
char	str[1024]="";
		_geo->getValue(_field,str);
		[_fld setCharValue:str];
	}
	
	return YES;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(IBAction)assign:(id)sender{
	if(FieldInView(_gapp,_gtp,_field)){
		_geo->inval(_field);
	}		
char			str[1024]="";
NSString*		nsstr=[_fld stringValue];
	[nsstr getCString:str maxLength:sizeof(str)-1 encoding:NSMacOSRomanStringEncoding];
CFBundleRef	bndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.xmapbrowser"));
bEventLog	log(_gapp,
				bndl,
				kBrowserMessageID,
				GetSignatureFromBundle(bndl));
	_geo->setValue(_field,str);
	log.close();
	[[_fld superview] endEdit];
	[_fld removeFromSuperview];
	[_fld release];
	_fld=NULL;
	if(FieldInView(_gapp,_gtp,_field)){
		_geo->inval(_field);
	}
	_geo=NULL;		
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)abort:(BOOL)closing{
	if(_fld!=NULL){
		if(!closing){
			[self assign:_fld];
		}
		[_fld removeFromSuperview];
		[_fld release];
		_fld=NULL;
		_geo=NULL;
		return YES;
	}	
	return NO;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

@end



// ---------------------------------------------------------------------------
// Calc column
// -----------
@implementation BrowserCalcColumn

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp
			calc:(bGenericCalc*)clc{	
    if(self=[super initWithRect:frame lines:lins application:gapp]){
		_calc=clc;
		switch([self kind]){
			case _bool:
			case _int:
			case _double:
				_srt=kSortNum;
				break;
			default:
				_srt=kSortAlpha;
				break;	
		}				
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)value:(char*)buffer 
	 forLine:(long)lin
	 inArray:(bArray*)arr{
bGenericGeoElement* geo;
	buffer[0]=0;
	if(!arr->get(lin+1,&geo)){
		return;
	}
bArray			bk(*_gapp->selMgr()->elements());
int				st=_gapp->selMgr()->getState();
bStdDBValue*	val=_calc->solve(geo);
	if(val){
		val->get(buffer);
		delete val;
	}
	else{
		strcpy(buffer,"");
	}
	if(_gapp->selMgr()->getState()!=st){
		_gapp->selMgr()->flush();
		_gapp->selMgr()->add(&bk);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)value:(long)lin
	   inArray:(bArray*)arr{
bGenericGeoElement* geo;
	if(!arr->get(lin+1,&geo)){
		return 0;
	}
bArray			bk(*_gapp->selMgr()->elements());
int				st=_gapp->selMgr()->getState();
bStdDBValue*	val=_calc->solve(geo);
double			r=0;
	if(val){
		val->get(&r);
		delete val;
	}
	if(_gapp->selMgr()->getState()!=st){
		_gapp->selMgr()->flush();
		_gapp->selMgr()->add(&bk);
	}
	return r;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)name:(char*)buffer{
user_ext_data p;
	_calc->process(kExtProcessCallGetData,&p);
	if(p.name==NULL){
		GetName(_calc,buffer);
	}
	else{
		strncpy(buffer,p.name,256);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)kind{
	return _calc->kind();
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)len{
	switch(_calc->kind()){
		case _bool:
		case _int:
			return _kIntLen;
			break;
			
		case _double:
			return _kDoubleLen;
			break;
			
		case _date:
		case _time:
			return _kDateTimeLen;
			break;
	}
	return 1024;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)decs{
	if(_calc->kind()==_double){
		return 6;
	}
	return 0;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end


// ---------------------------------------------------------------------------
// Matrix column
// -----------
@implementation BrowserMatrixColumn

// ---------------------------------------------------------------------------
// 
// -----------
-(id)initWithRect:(NSRect)frame
			lines:(bArray*)lins
	  application:(bGenericMacMapApp*)gapp
		   matrix:(MMsMatrix*)mmx
			index:(long)idx
			 name:(char*)nam{
	if(self=[super initWithRect:frame lines:lins application:gapp]){
		_mmx=mmx;
		_idx=idx;
		_srt=kSortNum;
		_name=strdup(nam);
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)dealloc{
//_bTrace_("[BrowserMatrixColumn dealloc]",true);
	if(_name){
		free(_name);
		_name=NULL;
	}
    [super dealloc];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)value:(char*)buffer 
	 forLine:(long)lin
	 inArray:(bArray*)arr{
	buffer[0]=0;
	if(lin>=_mmx->nl){
//		sprintf(buffer,"!!! for %ld;%ld",lin+1,_idx);
        buffer[0]=0;
		return;
	}
	sprintf(buffer,"%*f",(int)[self decs],MMsGetDouble(_mmx,lin+1,_idx));
}

// ---------------------------------------------------------------------------
// 
// -----------
-(double)value:(long)lin
	   inArray:(bArray*)arr{
	if(lin>=_mmx->nl){
		return -1234567890;
		return nan("");
	}
	return MMsGetDouble(_mmx,lin+1,_idx);
}

// ---------------------------------------------------------------------------
//
// -----------
-(void*)ref:(long)lin
    inArray:(bArray*)arr{
    return NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)name:(char*)buffer{
//	sprintf(buffer,"%s",_name);
    strcpy(buffer,_name);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)kind{
	return _double;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)len{
	return _kDoubleLen;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)decs{
	return 6;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(long)colKind{
	return 2;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

