//----------------------------------------------------------------------------
// File : bXMapsRasterPublisher.mm
// Project : MacMap
// Purpose : Objective C++ source file : XMap classes, raster exports (JPG, TIF, PNG)
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
// 12/04/2007 creation.
// 10/02/2015 Passage des fenêtres NAV en NS.
//----------------------------------------------------------------------------

#import "bXMapsRasterPublisher.h"
#import <mox_intf/ext_utils.h>
#import <mox_intf/Carb_Utils.h>
#import <mox_intf/mm_messages.h>
#import <mox_intf/endian.h>
#import <mox_intf/bitmap_utils.h>
#import <mox_intf/NSOpenSavePanelWrappers.h>

#import <xmap_lib/bXMapsGISIOUtils.h>

#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/bStdDirectory.h>

#import <Cocoa/Cocoa.h>


#pragma mark ->Cocoa class stuff for NSOpenPanel/NSSavePanel
//----------------------------------------------------------------------------
@interface GISIOExportRasterViewController : NSViewController{
    IBOutlet NSButton*      _opt_btn;
    bXMapsRasterPublisher*  _ext;
}
//----------------------------------------------------------------------------
-(IBAction)doOptions:(id)sender;
//----------------------------------------------------------------------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass;
-(void)putExtern:(bXMapsRasterPublisher*)ext;
//----------------------------------------------------------------------------
@end


// ---------------------------------------------------------------------------
//
// -----------
@implementation GISIOExportRasterViewController
// ---------------------------------------------------------------------------
//
// ------------
-(IBAction)doOptions:(id)sender{
    [[[self view] window] setIsVisible:NO];
    _ext->editOptions();
    [[[self view] window] setIsVisible:YES];
}
// ---------------------------------------------------------------------------
//
// ------------
+(id)allocWithNibNamed:(NSString*)nibNamed
                class:(Class)loadClass{
_bTrace_("[GISIOExportViewController allocWithNibNamed owner class]",true);
    return [[loadClass alloc] initWithNibName:nibNamed
                                       bundle:[NSBundle bundleWithIdentifier:@"com.cbconseil.xmap_lib.framework"]];
}
// ---------------------------------------------------------------------------
//
// ------------
-(void)putExtern:(bXMapsRasterPublisher*)ext{
    _ext=ext;
}
// ---------------------------------------------------------------------------
//
// -----------
@end


#pragma mark ->C wrappers to Objective-C
// ---------------------------------------------------------------------------
//
// ------------
void* GISIORasterExport_getAccessoryView(bXMapsRasterPublisher* ext){
_bTrace_("GISIORasterExport_getAccessoryView",true);
GISIOExportRasterViewController *ctrlr;
NSAutoreleasePool               *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    ctrlr=[GISIOExportRasterViewController allocWithNibNamed:@"GISIOExportRasterView"
                                                       class:[GISIOExportRasterViewController class]];
    [ctrlr putExtern:ext];
NSView*   view=[ctrlr view];
    [localPool release];
    return((void*)view);
}

// ---------------------------------------------------------------------------
//
// ------------
void GISIORasterExport_releaseController(void* ctrlr){
_bTrace_("GISIORasterExport_releaseController",true);
GISIOExportRasterViewController *controller=(GISIOExportRasterViewController*)ctrlr;
NSAutoreleasePool               *localPool;
    
    localPool=[[NSAutoreleasePool alloc] init];
    [controller release];
    [localPool release];
}

#pragma mark ->Options stuff
// ---------------------------------------------------------------------------
// 
// ------------
typedef struct userParam{
	WindowRef			w;
	raster_export_prm	ep;
	bGenericMacMapApp*	gapp;
	int					unit;
}userParam;

enum{
	kXMapUtilsRasterExport				='RexO',
	kXMapUtilsRasterExportNbHID			=11,
	kXMapUtilsRasterExportNbVID			=13,
	kXMapUtilsRasterExportUnitID		=14,
	kXMapUtilsRasterExportUnitCmd		='Unit',
	kXMapUtilsRasterExportMarginID		=15,
	kXMapUtilsRasterExportQualityID		=20,
	kXMapUtilsRasterExportResolutionID	=22,
	kXMapUtilsRasterExportGeoInfoID		=3
};

// ---------------------------------------------------------------------------
// 
// -----------
static double scm2u(bGenericMacMapApp* gapp, double d){
bGenericUnit*	scl=gapp->scaleMgr()->get();
bGenericUnit*	u=gapp->distMgr()->get();
	return(d*(0.01/u->coef())*scl->coef());
}

// ---------------------------------------------------------------------------
// 
// -----------
static double u2scm(bGenericMacMapApp* gapp, double d){
bGenericUnit*	scl=gapp->scaleMgr()->get();
bGenericUnit*	u=gapp->distMgr()->get();
	return(d/(0.01/u->coef())/scl->coef());
}

// ---------------------------------------------------------------------------
// 
// -----------
static pascal OSStatus options_evt_hdlr(EventHandlerCallRef hdlr, EventRef evt, void *up){
OSStatus			result=eventNotHandledErr;
HICommand			cmd;
userParam*			prm=(userParam*)up;
ControlRef			c;
ControlID			cid={kXMapUtilsRasterExport,0};
char				str[256];
int					ibf;

	if(GetEventClass(evt)==kEventClassCommand){
		GetEventParameter(	evt,
							kEventParamDirectObject,
							typeHICommand,
							NULL,
							sizeof(HICommand),
							NULL,
							&cmd);
		switch(cmd.commandID){
			case kHICommandOK:
				cid.id=kXMapUtilsRasterExportNbHID;
				GetControlByID(prm->w,&cid,&c);
				GetTextControlValue(c,str,sizeof(str)-1);
				ibf=atoi(str);
				if(ibf>0){
					prm->ep.nh=ibf;
				}
				cid.id=kXMapUtilsRasterExportNbVID;
				GetControlByID(prm->w,&cid,&c);
				GetTextControlValue(c,str,sizeof(str)-1);
				ibf=atoi(str);
				if(ibf>0){
					prm->ep.nv=ibf;
				}
				
				cid.id=kXMapUtilsRasterExportUnitID;
				GetControlByID(prm->w,&cid,&c);
				ibf=GetControl32BitValue(c);
				
				cid.id=kXMapUtilsRasterExportMarginID;
				GetControlByID(prm->w,&cid,&c);
				GetTextControlValue(c,str,sizeof(str)-1);
				prm->ep.mrg=matof(str);
				if(ibf==2){
					prm->ep.mrg=scm2u(prm->gapp,prm->ep.mrg);
				}
				
				cid.id=kXMapUtilsRasterExportQualityID;
				GetControlByID(prm->w,&cid,&c);
				switch(GetControl32BitValue(c)){
					case 1:
						prm->ep.q=codecMinQuality;
						break;
					case 2:
						prm->ep.q=codecLowQuality;
						break;
					case 3:
						prm->ep.q=codecNormalQuality;
						break;
					case 4:
						prm->ep.q=codecHighQuality;
						break;
					case 5:
						prm->ep.q=codecMaxQuality;
						break;
					case 6:
						prm->ep.q=codecLosslessQuality;
						break;
				}
				
				cid.id=kXMapUtilsRasterExportResolutionID;
				GetControlByID(prm->w,&cid,&c);
				GetTextControlValue(c,str,sizeof(str)-1);
				ibf=atoi(str);
				if(ibf>0){
					prm->ep.r=ibf;
				}

				cid.id=kXMapUtilsRasterExportGeoInfoID;
				if(GetControlByID(prm->w,&cid,&c)==noErr){
					prm->ep.gr=GetControl32BitValue(c);
				}
				
				QuitAppModalLoopForWindow(prm->w);
				result=noErr;
				break;
				
			case kHICommandCancel:
				QuitAppModalLoopForWindow(prm->w);
				result=noErr;
				break;
				
			case kXMapUtilsRasterExportUnitCmd:
				cid.id=kXMapUtilsRasterExportUnitID;
				GetControlByID(prm->w,&cid,&c);
				ibf=GetControl32BitValue(c);
				
				cid.id=kXMapUtilsRasterExportMarginID;
				GetControlByID(prm->w,&cid,&c);
				GetTextControlValue(c,str,sizeof(str)-1);
				prm->ep.mrg=matof(str);

				if((ibf==2)&&(prm->unit!=2)){
					prm->ep.mrg=u2scm(prm->gapp,prm->ep.mrg);
				}
				else if((ibf==1)&&(prm->unit!=1)){
					prm->ep.mrg=scm2u(prm->gapp,prm->ep.mrg);
				}
				prm->unit=ibf;
				{
					bString	str("");
					str+prm->ep.mrg;
					SetTextControlValue(c,str.string());
				}
				result=noErr;
				break;
				
			default:
				break;
				
		}
	}
	return(result);
}

// ---------------------------------------------------------------------------
// 
// -----------
static void get_options(userParam* prm){
CFBundleRef			bndl=CFBundleGetBundleWithIdentifier(CFSTR("com.cbconseil.xmap_lib.framework"));
OSStatus			status;
IBNibRef			nib=NULL;		
const EventTypeSpec	kWindowEvents[]={{kEventClassCommand,kEventCommandProcess}};
ControlRef			c;
ControlID			cid={kXMapUtilsRasterExport,0};
char				msg[256];
bString				str("");

	prm->w=NULL;
	prm->unit=1;
	
	for(;;){
		status=CreateNibReferenceWithCFBundle(bndl,CFSTR("xmap_utils"),&nib);
		if(status){
			break;
		}
		
		status=CreateWindowFromNib(	nib,
									CFSTR("RasterExport"),
									&prm->w);
		if(status){
			break;
		}
		InstallWindowEventHandler(	prm->w,
									options_evt_hdlr,
									GetEventTypeCount(kWindowEvents),
									kWindowEvents,
									prm,
									NULL);
		
		SetPortWindowPort(prm->w);
		
		cid.id=kXMapUtilsRasterExportNbHID;
		GetControlByID(prm->w,&cid,&c);
		SetTextControlFilter(c,uint_filter);
		str+prm->ep.nh;
		SetTextControlValue(c,str.string());
		str.reset();
		
		cid.id=kXMapUtilsRasterExportNbVID;
		GetControlByID(prm->w,&cid,&c);
		SetTextControlFilter(c,uint_filter);
		str+prm->ep.nv;
		SetTextControlValue(c,str.string());
		str.reset();

		cid.id=kXMapUtilsRasterExportUnitID;
		GetControlByID(prm->w,&cid,&c);
		SetControl32BitValue(c,prm->unit);
bGenericUnit*	u=prm->gapp->distMgr()->get();
		u->long_name(msg);
		SetPopupControlMenuItemValue(c,1,msg);

		cid.id=kXMapUtilsRasterExportMarginID;
		GetControlByID(prm->w,&cid,&c);
		SetTextControlFilter(c,ufloat_filter);
		str+prm->ep.mrg;
		SetTextControlValue(c,str.string());
		str.reset();
		
		cid.id=kXMapUtilsRasterExportQualityID;
		GetControlByID(prm->w,&cid,&c);
		switch(prm->ep.q){
			case codecMinQuality:
				SetControl32BitValue(c,1);
				break;
			case codecLowQuality:
				SetControl32BitValue(c,2);
				break;
			case codecNormalQuality:
				SetControl32BitValue(c,3);
				break;
			case codecHighQuality:
				SetControl32BitValue(c,4);
				break;
			case codecMaxQuality:
				SetControl32BitValue(c,5);
				break;
			case codecLosslessQuality:
				SetControl32BitValue(c,6);
				break;
		}
		
		cid.id=kXMapUtilsRasterExportResolutionID;
		GetControlByID(prm->w,&cid,&c);
		SetTextControlFilter(c,uint_filter);
		str+prm->ep.r;
		SetTextControlValue(c,str.string());
		str.reset();
		
		cid.id=kXMapUtilsRasterExportGeoInfoID;
		if(GetControlByID(prm->w,&cid,&c)==noErr){
			SetControl32BitValue(c,prm->ep.gr);
		}
				
		ShowWindow(prm->w);
		
		status=RunAppModalLoopForWindow(prm->w);
		if(status){
			break;
		}
		break;
	}
	if(nib){
		DisposeNibReference(nib);
	}
	if(prm->w){
		DisposeWindow(prm->w);
	}
}

#pragma mark ->STD C++
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapsRasterPublisher	::bXMapsRasterPublisher(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
						:bStdXMap(elt,gapp,bndl){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapsRasterPublisher::~bXMapsRasterPublisher(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapsRasterPublisher::open(int* flags){
	bStdXMap::open(flags);
	_prm.nh=1;
	_prm.nv=1;
	_prm.q=codecNormalQuality;
	_prm.r=72;
	_prm.gr=1;
	_prm.mrg=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapsRasterPublisher::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
			raster_export(&_prm);
			break;
		case kExtProcessCallWithParams:
			return(raster_export((raster_export_prm*)prm));
			break;
		case kExtProcessCallWithXMLTree:
			return(raster_export());
			break;

		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapsRasterPublisher::edit(void* prm){
_bTrace_("bXMapsRasterPublisher::edit",true);
char    ext[64];
    switch(_prm.codec){
        case kQTFileTypeJPEG:
            strcpy(ext,"jpg");
            break;
        case kQTFileTypeTIFF:
            strcpy(ext,"tif");
            break;
        case kQTFileTypePNG:
            strcpy(ext,"png");
            break;
        default:
            strcpy(ext,"jpg");
            break;
    }

void* ctrlr=GISIORasterExport_getAccessoryView(this);
    if(NSPutFilePath(ext,
                     cNSUTF8StringEncoding,
                     ctrlr,
                    _prm.path)==cNSCancelButton){
        GISIORasterExport_releaseController(ctrlr);
        return true;
    }
    GISIORasterExport_releaseController(ctrlr);
    splitPath(_prm.path,_prm.path,_prm.name);
_tm_(_prm.path);
_tm_(_prm.name);
    return(process(kExtProcessCallFromIntf,NULL));
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapsRasterPublisher::editOptions(/*bool needCodec*/){
_bTrace_("bXMapsRasterPublisher::editOptions(bool)",true);
userParam	up;
    
    up.w=NULL;
    up.ep=_prm;
    up.gapp=_gapp;
    get_options(&up);
    _prm=up.ep;
}

// ---------------------------------------------------------------------------
//
// ------------
void bXMapsRasterPublisher::putCodec(UInt32 codec){
_bTrace_("bXMapsRasterPublisher::putCodec(UInt32)",true);
    _prm.codec=codec;
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapsRasterPublisher::raster_export(){
raster_export_prm		p;
char					val[_values_length_max_];
bGenericXMLBaseElement*	elt;

	elt=getelement(1);
	if(!elt){
		return(false);
	}
	elt->getvalue(p.path);

	elt=getelement(2);
	if(!elt){
		return(false);
	}
	elt->getvalue(p.name);

	elt=getelement(3);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	p.nh=atoi(val);
	if(p.nh<1){
		return(false);
	}

	elt=getelement(4);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	p.nv=atoi(val);
	if(p.nv<1){
		return(false);
	}

	elt=getelement(5);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	p.r=atoi(val);
	switch(p.r){
		case 1:
			p.q=codecMinQuality;
			break;
		case 2:
			p.q=codecLowQuality;
			break;
		case 3:
			p.q=codecNormalQuality;
			break;
		case 4:
			p.q=codecHighQuality;
			break;
		case 5:
			p.q=codecMaxQuality;
			break;
		case 6:
			p.q=codecLosslessQuality;
			break;
		default:
			return(false);
			break;
	}

	elt=getelement(6);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	p.r=atoi(val);
	if(p.r<72){
		return(false);
	}

	elt=getelement(7);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	p.gr=atoi(val);

	elt=getelement(8);
	if(!elt){
		return(false);
	}
	elt->getvalue(val);
	p.mrg=atof(val);
	if(p.mrg<0){
		p.mrg=0;
	}
	
	return(raster_export(&p));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapsRasterPublisher::raster_export(raster_export_prm* ep){
_bTrace_("bXMapsRasterPublisher::raster_export",true);
ivx_rect				area=*(_gapp->printMgr()->get_print_area());

	if(!ivr_ok(&area)){
_te_("bad area "+area.left+";"+area.top+";"+area.right+";"+area.bottom);
		return(true);
	}

ivx_rect            vxr,back;
i2dvertex           vx;
CGRect              r;
long                i,j;
double              h,v;
char                fnam[FILENAME_MAX],ext[FILENAME_MAX],bse[FILENAME_MAX];
char*               c;
bStdUTF8Directory	dr(ep->path);
BitmapData          dt;
int                 q;

	if(dr.status()){
_te_("bad directory "+ep->path+"->"+dr.status());
		return(true);
	}

	switch(ep->q){
		case codecMinQuality:
			q=1;
			break;
		case codecLowQuality:
			q=2;
			break;
		case codecNormalQuality:
			q=3;
			break;
		case codecHighQuality:
			q=4;
			break;
		case codecMaxQuality:
			q=5;
			break;
		case codecLosslessQuality:
			q=6;
			break;
		default:
			q=3;
			break;
	}
	
	strcpy(fnam,ep->name);
	strcpy(bse,ep->name);
	c=strchr(bse,'.');
	if(c){
		strcpy(ext,c);
		c[0]=0;
	}
	else{
		strcpy(ext,"");
	}

	h=(double)(area.right-area.left)/(double)ep->nh;
	v=(double)(area.bottom-area.top)/(double)ep->nv;
	
bArray*					arr;
	SaveCurrentMargins(_gapp,&arr);
	SetMarginToCurview(_gapp,ep->mrg);
		
	for(j=0;j<ep->nv;j++){
		vxr.top=area.top+round((double)j*v);
		vxr.bottom=area.top+round((double)(j+1)*v);
		
		for(i=0;i<ep->nh;i++){
// Init zone d'export
			vxr.left=area.left+round((double)i*h);
			vxr.right=area.left+round((double)(i+1)*h);
		
			if((ep->nv>1)||(ep->nh>1)){
				sprintf(fnam,"%s%ld%s",bse,(j*ep->nv)+i+1,ext);
			}
			r=ivr2cgr(_gapp,vxr);
			
// Bitmap Infos
			dt.width=r.size.width;//(r.right-r.left);
			dt.height=r.size.height;//(r.bottom-r.top);
			dt.bitsPerComponent=8;
			dt.colorspace=CGColorSpaceCreateDeviceRGB();
			dt.bytesPerRow=(dt.width*4);
			dt.data=malloc(dt.bytesPerRow*(dt.height));
			if(dt.data==NULL){
_te_("dt.data==NULL");
				continue;
			}
			dt.bitmapInfo=kCGImageAlphaPremultipliedLast;
			dt.outbm=NULL;
			memset(dt.data,0,dt.bytesPerRow*dt.height);
			
// Restauration contexte de dessin
			_gapp->mapIntf()->screenBounds(&back);
			_gapp->locConverter()->reset(&vxr);
			_gapp->layersMgr()->SwitchContext(kBitMapGraphicContext,&dt);
			_gapp->layersMgr()->DrawLayers(NULL,&vxr);
			
_tm_("dt.outbm="+(void*)dt.outbm);
			if(dt.outbm==NULL){
_te_("dt.outbm==NULL");
				free(dt.data);
				dt.data=NULL;
				_gapp->layersMgr()->SwitchContext(kCGGraphicContext,NULL);
				_gapp->locConverter()->reset(&back);
				continue;
			}
// Export Image
			(void)CGBitmapContextExport(dt.outbm,"",fnam,_prm.codec,ep->r,q);
// GéoRéférencement TFW
			if(ep->gr){
				c=strchr(fnam,'.');
				if(c){
					c[0]=0;
				}
				strcat(fnam,".tfw");
				
d2dvertex		dtl,dbr;
				vx.h=vxr.left;
				vx.v=vxr.top;				
				Convert(&dtl,&vx);
				vx.h=vxr.right;
				vx.v=vxr.bottom;				
				Convert(&dbr,&vx);
				
bStdFile		f(fnam,"w");
double			d[4]={0,0,0,0};
				
				d[0]=((dbr.x-dtl.x)/(double)r.size.width/*(r.right-r.left)*/)-d[1];
				d[3]=d[2]-((dtl.y-dbr.y)/(double)r.size.height/*(r.bottom-r.top)*/);
				sprintf(fnam,"%f\n%f\n%f\n%f\n%f\n%f\n",d[0],d[1],d[2],d[3],dtl.x,dtl.y);
				f.write(fnam);
			}
			
			CGContextRelease(dt.outbm);
			dt.outbm=NULL;
			free(dt.data);
			dt.data=NULL;
		
// Restauration contexte de dessin
			_gapp->layersMgr()->SwitchContext(kCGGraphicContext,NULL);
			_gapp->locConverter()->reset(&back);
		}
	}
	
	RestoreCurrentMargins(_gapp,arr);
	delete arr;

	return(true);
}

#pragma mark ->JPEG
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapJPGPublisher	::bXMapJPGPublisher(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapsRasterPublisher(elt,gapp,bndl){
	_prm.codec=kQTFileTypeJPEG;
	setclassname("jpegpublisher");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapJPGPublisher::~bXMapJPGPublisher(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapJPGPublisher::create(bGenericXMLBaseElement* elt){
	return(new bXMapJPGPublisher(elt,_gapp,elt->getbundle()));
}

#pragma mark ->TIFF
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapTIFFPublisher	::bXMapTIFFPublisher(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapsRasterPublisher(elt,gapp,bndl){
	_prm.codec=kQTFileTypeTIFF;
	setclassname("tiffpublisher");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapTIFFPublisher::~bXMapTIFFPublisher(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapTIFFPublisher::create(bGenericXMLBaseElement* elt){
	return(new bXMapJPGPublisher(elt,_gapp,elt->getbundle()));
}

#pragma mark ->PNG
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPNGPublisher	::bXMapPNGPublisher(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapsRasterPublisher(elt,gapp,bndl){
	_prm.codec=kQTFileTypePNG;
	setclassname("pngpublisher");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPNGPublisher::~bXMapPNGPublisher(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPNGPublisher::create(bGenericXMLBaseElement* elt){
	return(new bXMapJPGPublisher(elt,_gapp,elt->getbundle()));
}

