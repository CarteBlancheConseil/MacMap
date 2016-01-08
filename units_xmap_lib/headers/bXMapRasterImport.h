//----------------------------------------------------------------------------
// File : bXMapRasterImport.h
// Project : MacMap
// Purpose : Header file : XMap class, raster imports
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
// 13/12/2006 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapRasterImport__
#define __bXMapRasterImport__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/bArray.h>
#include <MacMapSuite/bStdFile.h>

//----------------------------------------------------------------------------

#define	kXMapRasterImportMessageID				"rasterimport"
#define	kXMapRasterImportInvalidGridErrorID		"GridErr"
#define	kXMapRasterImportIGridNotFoundErrorID	"IGridNotFoundErr"
#define	kXMapRasterImportIGridNotFoundExplID	"IGridNotFoundExpl"
#define	kXMapRasterImportGlobalErrorID          "GlobalErr"
#define	kXMapRasterImportGlobalExplID           "GlobalExpl"

enum{
	kRasterImportKindBotRight	=1,
	kRasterImportKindScale		=2,
	kRasterImportKindTFW		=10,
	kRasterImportKindTAB		=11,
	kRasterImportKindKML		=12,// unused
	kRasterImportKindMMR		=20,// unused
};

enum{
	kXMapRasterImportSignature			='iRST',
	kXMapRasterImportKFrameID			=2,
	kXMapRasterImportXID				=10,
	kXMapRasterImportXUnitID			=11,
	kXMapRasterImportYID				=12,
	kXMapRasterImportYUnitID			=13,
	kXMapRasterImportKind1ID			=20,
	kXMapRasterImportKind2ID			=21,
	kXMapRasterImportXpID				=200,
	kXMapRasterImportXpUnitID			=201,
	kXMapRasterImportYpID				=202,
	kXMapRasterImportYpUnitID			=203,
	kXMapRasterImportScaleID			=210,
	kXMapRasterImportPopKindID			='PopK'
};

//----------------------------------------------------------------------------

class bXMapRasterImport : public bStdXMapWithModalIntf{
	public:		
		bXMapRasterImport						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bXMapRasterImport				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool edit						(	void* prm);
		virtual bool wd_event					(	EventRef evt, 
													WindowRef wd);
		virtual void wd_init					(	WindowRef wd);
		
	protected:
		virtual bool import						(	);
		virtual bool import						(	import_prm* prm);

		virtual bool parse_tfw					(	bStdFile& f);
		virtual bool parse_tab					(	bStdFile& f);
		
	private:
		import_prm	_prm;
		int			_ik;
		d2dvertex	_tl;
		d2dvertex	_br;
		double		_scl;
		double		_xsz;
		double		_ysz;
};

//----------------------------------------------------------------------------

#endif
