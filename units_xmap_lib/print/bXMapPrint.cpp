//----------------------------------------------------------------------------
// File : bXMapPrint.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, printing
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
// 22/11/2006 creation.
// 26/07/2016 cocoa intf.
//----------------------------------------------------------------------------

#include "bXMapPrint.h"

#include <mox_intf/NSPrintWrappers.h>
#include <mox_intf/NSOpenSavePanelWrappers.h>

#include <mox_intf/bEventLog.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Type_Utils.h>
#include <mox_intf/bEventLog.h>
#include <mox_intf/mm_messages.h>
#include <mox_intf/bStdWait.h>

#include <MacMapSuite/vx_manip.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
OSStatus	DoPrintDialog(	PMPrintSession* ps,
                          PMPageFormat pf,
                          PMPrintSettings* pt,
                          UInt32 n){
_bTrace_("bXMapPrint::DoPrintDialog",true);
OSStatus	status=noErr;
    
    if(*pt==kPMNoPrintSettings){
        status=PMCreatePrintSettings(pt);
        if((status==noErr)&&(*pt!=kPMNoPrintSettings)){
            status=PMSessionDefaultPrintSettings(*ps,*pt);
        }
    }
    else{
        status=PMSessionValidatePrintSettings(*ps,*pt,kPMDontWantBoolean);
    }
    if(status==noErr){
        status=PMSetPageRange(*pt,1,n);
    }
    if(status==noErr){
        status=NSRunPrintPanel(*pt);
        if(status==cNSOKButton){
            status=noErr;
            NSCopyPrintSettings(*pt);
            *ps=NSGetPrintSession();
            if(*pt!=kPMNoPrintSettings){
                status=noErr;
            }
            else{
                status=kPMCancel;
            }
        }
        else{
            status=kPMCancel;
        }
    }
    return(status);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPrint	::bXMapPrint(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl)
            :bStdXMap(elt,gapp,bndl){
    setclassname("print");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPrint::~bXMapPrint(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPrint::create(bGenericXMLBaseElement* elt){
    return(new bXMapPrint(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
//
// ------------
bool bXMapPrint::process(int msg, void* prm){
_bTrace_("bXMapPrint::process",true);
PMPageFormat	pf=kPMNoPageFormat;
PMPrintSettings	pt=kPMNoPrintSettings;
PMPrintSession	ps=NULL;
OSStatus		status=_gapp->printMgr()->get_page_format(&pf,kMapPageFormatName);
PMRect			pr;
ivx_rect		vr=*(_gapp->printMgr()->get_print_area());
CGPoint			tl,br;
i2dvertex		vx;
UInt32			nh,nv;
    
    vx.h=vr.left;
    vx.v=vr.bottom;
    Convert(&tl,&vx);
    vx.h=vr.right;
    vx.v=vr.top;
    Convert(&br,&vx);
    
    if(status){
_te_("status");
        return(true);
    }
    for(;;){
        status=PMGetAdjustedPageRect(pf,&pr);
        if(status){
_te_("PMGetAdjustedPageRect");
            break;
        }
        nh=ceil((br.x-tl.x)/(pr.right-pr.left));
        nv=ceil((br.y-tl.y)/(pr.bottom-pr.top));
        
        ps=NSGetPrintSession();
        status=DoPrintDialog(&ps,pf,&pt,nh*nv);
        if(status){
_te_("DoPrintDialog");
            break;
        }
        
        status=print_loop(ps,pf,pt,nh,nv);
        if(status){
_te_("print_loop");
            break;
        }
        break;
    }
    if(pf!=kPMNoPageFormat){
        (void)PMRelease(pf);
    }
    if(pt!=kPMNoPrintSettings){
        (void)PMRelease(pt);
    }
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bXMapPrint::test(void* prm){
    return(ivr_ok(_gapp->printMgr()->get_print_area()));
}

// ---------------------------------------------------------------------------
//
// ------------
bool bXMapPrint::edit(void* prm){
    return(process(kExtProcessCallFromIntf,prm));
}

// ---------------------------------------------------------------------------
//
// ------------
OSStatus bXMapPrint::print_loop(PMPrintSession ps,
                                PMPageFormat pf,
                                PMPrintSettings pt,
                                UInt32 nh,
                                UInt32 nv){
_bTrace_("bXMapPrint::print_loop",true);
OSStatus		status=noErr,tempErr;
UInt32			n,i,j,f,l,c;
PMRect			pr,ppr;
int				mh,mv;
CGContextRef	ctx;
ivx_rect		back,cur;
ivx_rect		area=*(_gapp->printMgr()->get_print_area());
CGPoint			cg;
CGRect			clip,rclip;
i2dvertex		vx;
bool			badb,badr;
    
// Sauvegarder la zone de dessin de la fenêtre
    _gapp->mapIntf()->screenBounds(&back);
    
    status=PMGetAdjustedPageRect(pf,&pr);
    if(status!=noErr){
_te_("PMGetAdjustedPageRect : "+status);
        return(status);
    }
    status=PMGetAdjustedPaperRect(pf,&ppr);
    if(status!=noErr){
_te_("PMGetAdjustedPaperRect : "+status);
        return(status);
    }
    
// Détermination du clip
    clip.origin.x=-ppr.left;
    clip.origin.y=-ppr.top;
    clip.size.width=pr.right-pr.left;
    clip.size.height=pr.bottom-pr.top;
    
// Détermination de la taille (géographique) des prints
    cg.x=pr.left;
    cg.y=pr.bottom;
    Convert(&vx,&cg);
    cur.top=vx.v;
    cur.left=vx.h;
    
    cg.x=pr.right;
    cg.y=pr.top;
    Convert(&vx,&cg);
    cur.bottom=vx.v;
    cur.right=vx.h;
    
    mh=cur.right-cur.left;
    mv=cur.bottom-cur.top;
    
// Vérification du nombre de pages
    status=PMGetFirstPage(pt,&f);
    if(status!=noErr){
_te_("PMGetFirstPage : "+status);
        return(status);
    }
    status=PMGetLastPage(pt,&l);
    if(status!=noErr){
_te_("PMGetFirstPage : "+status);
        return(status);
    }
    n=nh*nv;
    if(n<l){
        l=n;
    }
    status=PMSetFirstPage(pt,f,false);
    if(status!=noErr){
_te_("PMSetFirstPage : "+status);
        return(status);
    }
    status=PMSetLastPage(pt,l,false);
    if(status!=noErr){
_te_("PMSetLastPage : "+status);
        return(status);
    }
    
// Début du dessin
    status=PMSessionBeginCGDocumentNoDialog(ps,pt,pf);
    if(status!=noErr){
_te_("PMSessionBeginCGDocument : "+status);
        return(status);
    }
// Changer le contexte graphique de MacMap
    _gapp->layersMgr()->SwitchContext(kPrintGraphicContext,NULL);
    
// Boucle de dessin
    cur=area;
    for(i=1;i<=nv;i++){
        cur.bottom=cur.top+mv;
        if(cur.bottom>area.bottom){
            cur.bottom=area.bottom;
            badb=true;
        }
        else{
            badb=false;
        }
        cur.left=area.left;
        for(j=1;j<=nh;j++){
            cur.right=cur.left+mh;
            if(cur.right>area.right){
                cur.right=area.right;
                badr=true;
            }
            else{
                badr=false;
            }
            c=((i-1)*nh)+j;
            if((c<f)||(c>l)){
                cur.left=cur.right;
                continue;
            }
            status=PMSessionBeginPageNoDialog(ps,pf,NULL);
            if(status!=noErr){
_te_("PMSessionBeginPage : "+status);
                break;
            }
            status=PMSessionGetCGGraphicsContext(ps,&ctx);
            if(status!=noErr){
_te_("PMSessionGetCGGraphicsContext : "+status);
                break;
            }
            _gapp->locConverter()->reset(&cur);
// Recalcul du clip si nécessaire
            rclip=clip;
            if(badr||badb){
                vx.h=cur.right;
                vx.v=cur.top;
                Convert(&cg,&vx);
                if(badr){
                    rclip.size.width=cg.x-rclip.origin.x;
                }
                if(badb){
                    rclip.size.height=cg.y;
                    rclip.origin.y=clip.size.height-cg.y+clip.origin.y;
                }
            }
// Clip pour ne pas dessiner dans les marges
            CGContextClipToRect(ctx,rclip);
// Compensation des marges
            CGContextTranslateCTM(ctx,rclip.origin.x,rclip.origin.y);
            _gapp->printMgr()->set_print_context(ctx);
            _gapp->layersMgr()->DrawLayers(NULL,&cur);
            tempErr=PMSessionEndPageNoDialog(ps);
            if(status!=noErr){
                status=tempErr;
_te_("PMSessionEndPage : "+status);
            }
            cur.left=cur.right;
            if(	(status!=noErr)				||
               (PMSessionError(ps)!=noErr)	){
                break;
            }
        }
        cur.top=cur.bottom;
        if(	(status!=noErr)				||
           (PMSessionError(ps)!=noErr)	){
            break;
        }
    }
// Restaurer le contexte graphique de MacMap
    _gapp->layersMgr()->SwitchContext(kCGGraphicContext,NULL);
    tempErr=PMSessionEndDocumentNoDialog(ps);
    if(status==noErr){
        status=tempErr;
_te_("PMSessionEndDocument : "+status);
    }
    
    tempErr=PMSessionError(ps);
    if(status==noErr){
        status=tempErr;
_te_("PMSessionError : "+status);
    }
    
// Restaurer la zone de dessin de la fenêtre
    _gapp->locConverter()->reset(&back);
    
    if((status!=noErr)&&(status!=kPMCancel)){
_te_(status);
    }
    return(status);
}
