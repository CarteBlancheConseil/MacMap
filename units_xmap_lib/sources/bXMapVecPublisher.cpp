//----------------------------------------------------------------------------
// File : bXMapVecPublisher.cpp
// Project : MacMap
// Purpose : C++ source file : XMap classes, graphic vector exports (PDF, KML)
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
// 21/03/2006 creation.
// 22/02/2007 ajout de choose_location pour passer le path en paramètre
//            au PDFGraphicContext.
// 10/02/2015 passage des fenêtres NAV en NS.
// 07/05/2015 ajout de l'accès au format d'export pour le KML
// 14/04/2017 layermgr clone.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#include "bXMapVecPublisher.h"
#include "bXMapsGISIOUtils.h"
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>

#include <mox_intf/NSOpenSavePanelWrappers.h>

#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapVecPublisher	::bXMapVecPublisher(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bStdXMap(elt,gapp,bndl){
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapVecPublisher::~bXMapVecPublisher(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapVecPublisher::process(int msg, void* prm){
	switch(msg){
		case kExtProcessCallFromIntf:
		case kExtProcessCallWithParams:
			dump((char*)prm);
			break;
		case kExtProcessCallWithXMLTree:{
char					path[PATH_MAX];
bGenericXMLBaseElement*	elt;
				elt=getelement(1);
				if(!elt){
					return(false);
				}
				elt->getvalue(path);
				dump(path);
			}break;
		default:
			break;
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapVecPublisher::edit(void* prm){
char	path[PATH_MAX];
	if(choose_location(path)!=noErr){
		return(true);
	}
	return(process(kExtProcessCallFromIntf,path));
}

// ---------------------------------------------------------------------------
// 
// ------------
OSStatus bXMapVecPublisher::choose_location(char* path){
_bTrace_("bXMapVecPublisher::edit",true);
char    ext[64];
    
    if(_gc==kPDFGraphicContext){
        strcpy(ext,"pdf");
    }
    else if(_gc==kKMLGraphicContext){
        strcpy(ext,"kml");
    }
    else if(_gc==kSVGGraphicContext){
        strcpy(ext,"svg");
    }
    else{
        strcpy(ext,"pdf");
    }
    
void   *ctrlr=NULL,*view=NULL;
    
    if(_gc==kKMLGraphicContext){
        ctrlr=GISIOExport_getAccessoryViewController();
        view=GISIOExport_getAccessoryViewFromController(ctrlr);
    }
    
    if(NSPutFilePath(ext,
                     cNSMacOSRomanStringEncoding,
                     view,
                     path)==cNSCancelButton){
        if(ctrlr){
            GISIOExport_releaseController(ctrlr);
        }
        return -1;
    }
    if(ctrlr){
        GISIOExport_releaseController(ctrlr);
    }
    
_tm_(path);
    return noErr;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bXMapVecPublisher::dump(char* path){
_bTrace_("bXMapVecPublisher::dump",false);
ivx_rect			vxr=*(_gapp->printMgr()->get_print_area());
	if(!ivr_ok(&vxr)){
_te_("bad area ("+vxr.left+";"+vxr.top+";"+vxr.right+";"+vxr.bottom+")");
		return;
	}
CGRect		r;

	r=ivr2cgr(_gapp,vxr);

VectorData	dt;
	strcpy(dt.path,path);
	MacRoman2UTF8(dt.path,PATH_MAX*3);
	dt.bounds.origin.x=dt.bounds.origin.y=0;
	dt.bounds.size.width=r.size.width;
	dt.bounds.size.height=r.size.height;
	
ivx_rect			back;
<<<<<<< HEAD
bGenericLayersMgr*  mgr=_gapp->layersMgr()->clone();
    
    _gapp->mapIntf()->screenBounds(&back);
    _gapp->locConverter()->reset(&vxr);
    mgr->SwitchContext(_gc,&dt);
    mgr->DrawLayers(NULL,&vxr);
    mgr->SwitchContext(kCGGraphicContext,NULL);
    _gapp->locConverter()->reset(&back);
    _gapp->layersMgr()->cloneDelete(mgr);
=======
    
	_gapp->mapIntf()->screenBounds(&back);
	_gapp->locConverter()->reset(&vxr);
    _gapp->layersMgr()->SwitchContext(_gc,&dt);
	_gapp->layersMgr()->DrawLayers(NULL,&vxr);
	_gapp->layersMgr()->SwitchContext(kCGGraphicContext,NULL);
    _gapp->locConverter()->reset(&back);
>>>>>>> origin/master
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapPDFPublisher	::bXMapPDFPublisher(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapVecPublisher(elt,gapp,bndl){
	setclassname("pdfpublisher");
	_gc=kPDFGraphicContext;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPDFPublisher::~bXMapPDFPublisher(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPDFPublisher::create(bGenericXMLBaseElement* elt){
	return(new bXMapPDFPublisher(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bXMapKMLPublisher	::bXMapKMLPublisher(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl) 
					:bXMapVecPublisher(elt,gapp,bndl){
	setclassname("kmlpublisher");
	_gc=kKMLGraphicContext;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapKMLPublisher::~bXMapKMLPublisher(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapKMLPublisher::create(bGenericXMLBaseElement* elt){
	return(new bXMapKMLPublisher(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapKMLPublisher::test(void* prm){
	return(_gapp->document()->srid()!=-1);
}

