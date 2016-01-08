//----------------------------------------------------------------------------
// File : bXMapStdPicker.h
// Project : MacMap
// Purpose : Header file : XMap base class for pickers
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
// 30/05/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapStdPicker__
#define __bXMapStdPicker__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>

//----------------------------------------------------------------------------

enum{	
	kXMapStdPickerSignature		='Pick',
	kXMapStdPickerListID		=1,
	kXMapStdPickerViewID		=2,
	kXMapStdPickerImportID		=3,
	kXMapStdPickerImportCmd		='Impo',
	kXMapStdPickerListProperty	='Name'
};

//----------------------------------------------------------------------------

class bXMapStdPicker : public bStdXMapWithModalIntf{
public:
    bXMapStdPicker							(	bGenericXMLBaseElement* elt, 
                                                bGenericMacMapApp* gapp, 
                                                CFBundleRef bndl);
    virtual ~bXMapStdPicker					(	);
    virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
    
    virtual void open						(	int* flags);
    virtual bool edit						(	void* prm);
    virtual bool process					(	int msg, 
                                                void* prm);
    virtual bool test						(	void* prm);
    
    virtual bool wd_event					(	EventRef evt, 
                                                WindowRef wd);
    virtual void wd_init					(	WindowRef wd);
    
protected:															
    virtual void choose_action				(	DataBrowserItemID item);
    virtual void draw						(	CGContextRef ctx);
//    virtual void import						(	NavObjectFilterUPP filter);
    virtual void import						(	const char** pnsft,
                                                int nnsft);

    picker_prm	_prm;
    char		_sub[FILENAME_MAX];
    
private:
    
    virtual void browser_fill				(	);
    static OSStatus list_handler			(	ControlRef browser, 
                                                DataBrowserItemID itemID, 
                                                DataBrowserPropertyID property, 
                                                DataBrowserItemDataRef itemData, 
                                                Boolean changeValue);
    static void list_notifier				(	ControlRef browser, 
                                                DataBrowserItemID item, 
                                                DataBrowserItemNotification msg);
    static OSStatus draw_proc				(	EventHandlerCallRef hdlr,
                                                EventRef evt,
                                                void* udt);
};

//----------------------------------------------------------------------------

#endif
