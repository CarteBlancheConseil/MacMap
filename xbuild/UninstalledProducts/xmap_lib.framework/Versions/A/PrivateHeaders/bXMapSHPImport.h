//----------------------------------------------------------------------------
// File : bXMapSHPImport.h
// Project : MacMap
// Purpose : Header file : XMap class, Shapefile import
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
// 22/03/2005 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapSHPImport__
#define __bXMapSHPImport__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

#define	kXMapSHPImportFieldCreateMessageID	"FieldCreate"
#define	kXMapSHPImportObjectCreateMessageID	"ObjCreate"
#define	kXMapSHPImporOpenSHPErrorID			"OpenSHPErr"
#define	kXMapSHPImporOpenDBFErrorID			"OpenDBFErr"
#define	kXMapSHPImportInvalidGridErrorID	"GridErr"
#define	kXMapSHPImportFieldCreateErrorID	"FieldCreateErr"
#define	kXMapSHPImportObjectCreateErrorID	"ObjCreateErr"
#define	kXMapSHPImportExplCreateErrorID		"ExplErr"
#define	kXMapSHPImportIGridNotFoundErrorID	"IGridNotFoundErr"
#define	kXMapSHPImportIGridNotFoundExplID	"IGridNotFoundExpl"
#define	kXMapSHPImportGlobalErrorID         "GlobalErr"
#define	kXMapSHPImportGlobalExplID          "GlobalExpl"

//----------------------------------------------------------------------------

class bXMapSHPImport : public bStdXMap{
	public:		
		bXMapSHPImport							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapSHPImport					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool edit						(	void* prm);
		
	protected:
		virtual bool import						(	);
		virtual bool import						(	bool dummy);
		virtual bool import						(	import_prm* prm,
													bool silent);
		virtual void field_create				(	bGenericType* tp, 
													bStdTable* dbf, 
													bArray& f,
													bool silent);
		
	private:
		import_prm	_prm;
};

//----------------------------------------------------------------------------

#endif
