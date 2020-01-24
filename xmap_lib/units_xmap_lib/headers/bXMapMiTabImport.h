//----------------------------------------------------------------------------
// File : bXMapMiTabImport.h
// Project : MacMap
// Purpose : Header file : XMap class, mif-mid-tab import
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
// 23/06/2008 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapMiTabImport__
#define __bXMapMiTabImport__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

#define	kXMapMiTabImportFieldCreateMessageID	"FieldCreate"
#define	kXMapMiTabImportObjectCreateMessageID	"ObjCreate"
#define	kXMapMiTabImporOpenMiTabErrorID			"OpenMiTabErr"
#define	kXMapMiTabImporOpenDBFErrorID			"OpenDBFErr"
#define	kXMapMiTabImportInvalidGridErrorID		"GridErr"
#define	kXMapMiTabImportFieldCreateErrorID		"FieldCreateErr"
#define	kXMapMiTabImportObjectCreateErrorID		"ObjCreateErr"
#define	kXMapMiTabImportExplCreateErrorID		"ExplErr"
#define	kXMapMiTabImportIGridNotFoundErrorID	"IGridNotFoundErr"
#define	kXMapMiTabImportIGridNotFoundExplID		"IGridNotFoundExpl"
#define	kXMapMiTabImportGlobalErrorID           "GlobalErr"
#define	kXMapMiTabImportGlobalExplID            "GlobalExpl"

//----------------------------------------------------------------------------

class bXMapMiTabImport : public bStdXMap{
	public:		
		bXMapMiTabImport						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapMiTabImport				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool edit						(	void* prm);
		
	protected:
		virtual bool import						(	);
		virtual bool import						(	bool dummy);
		virtual bool import						(	import_prm* prm,
													bool silent);
		virtual void field_create				(	bGenericType** tp, 
													bStdTable* MiTab, 
													bArray** f,
													bool silent);
		
	private:
		import_prm	_prm;
};

//----------------------------------------------------------------------------

#endif
