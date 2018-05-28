//----------------------------------------------------------------------------
// File : bXMapMMTExport.h
// Project : MacMap
// Purpose : Header file : XMap class, MacMap Text format export
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
// 30/12/2008 creation.
//----------------------------------------------------------------------------
// SCRIPT_COMPATIBLE
//----------------------------------------------------------------------------

#ifndef __bXMapMMTExport__
#define __bXMapMMTExport__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMap.h>

//----------------------------------------------------------------------------

#define	kXMapMMTExportProcessingID	"MMTExportProcess"

//----------------------------------------------------------------------------

class bXMapMMTExport : public bStdXMap{
	public:		
		bXMapMMTExport							(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapMMTExport					(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool edit						(	void* prm);
		
	protected:
	
	private:
		OSStatus choose_location				(	char* path);
		void dump								(	char* path);
		void objs_dump							(	bArray& arr);
		void objs_dump							(	bArray& arr, 
													bArray& sel);
		static int obj_dump						(	void *o, 
													void *prm);
		void field_create						(	bGenericType* tp, 
													bStdTable* dbf, 
													bArray* f);
};

//----------------------------------------------------------------------------

#endif
