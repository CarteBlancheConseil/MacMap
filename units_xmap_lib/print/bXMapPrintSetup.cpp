//----------------------------------------------------------------------------
// File : bXMapPrintSetup.cpp
// Project : MacMap
// Purpose : C++ source file : XMap class, printing setup
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

#include "bXMapPrintSetup.h"

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
//
// ------------
static OSStatus DoPageSetupDialog(PMPrintSession ps, PMPageFormat* pf){
OSStatus    status=noErr;
    
    if(*pf==kPMNoPageFormat){
        status=PMCreatePageFormat(pf);
        if((status==noErr)&&(*pf!=kPMNoPageFormat)){
            status=PMSessionDefaultPageFormat(ps,*pf);
        }
    }
    else{
        status=PMSessionValidatePageFormat(ps,*pf,kPMDontWantBoolean);
    }
    if(status==noErr){
        status=NSRunPageLayout(*pf);
        if(status==cNSOKButton){
            NSCopyPageFormat(*pf);
            if(*pf!=kPMNoPageFormat){
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
bXMapPrintSetup	::bXMapPrintSetup(bGenericXMLBaseElement* elt, bGenericMacMapApp* gapp, CFBundleRef bndl)
                :bStdXMap(elt,gapp,bndl){
    setclassname("printsetup");
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bXMapPrintSetup::~bXMapPrintSetup(){
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bGenericXMLBaseElement* bXMapPrintSetup::create(bGenericXMLBaseElement* elt){
    return(new bXMapPrintSetup(elt,_gapp,elt->getbundle()));
}

// ---------------------------------------------------------------------------
//
// ------------
bool bXMapPrintSetup::process(int msg, void* prm){
_bTrace_("bXMapPrintSetup::process",true);
PMPrintSession	ps=NULL;
PMPageFormat	pf=kPMNoPageFormat;
OSStatus		status=PMCreateSession(&ps);
    
    _gapp->printMgr()->get_page_format(&pf,kMapPageFormatName);
    for(;;){
        if(status!=noErr){
            break;
        }
        status=DoPageSetupDialog(ps,&pf);
        if(status!=noErr){
            break;
        }
        break;
    }
    if(pf!=kPMNoPageFormat){
        _gapp->printMgr()->set_page_format(pf,kMapPageFormatName);
        (void)PMRelease(pf);
    }
    if(ps){
        (void)PMRelease(ps);
    }
    return(true);
}

// ---------------------------------------------------------------------------
//
// ------------
bool bXMapPrintSetup::test(void* prm){
    return(ivr_ok(_gapp->printMgr()->get_print_area()));
}

// ---------------------------------------------------------------------------
// 
// ------------
bool bXMapPrintSetup::edit(void* prm){
    return(process(kExtProcessCallFromIntf,prm));
}

