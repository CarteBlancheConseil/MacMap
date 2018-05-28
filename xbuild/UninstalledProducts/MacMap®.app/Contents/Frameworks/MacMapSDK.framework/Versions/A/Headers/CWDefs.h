//----------------------------------------------------------------------------
// File : CWDefs.h
// Project : MacMap
// Purpose : Header file : MacMapSDK defines
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
// 19/01/2009 creation.
//----------------------------------------------------------------------------

#ifndef __CWDefs__
#define __CWDefs__

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

typedef void*		CWCallback;					// App Callback

typedef char		CWCharData[256];			// Std Char Data
typedef char		CWFileName[FILENAME_MAX];	// Std Filename
typedef char		CWPath[PATH_MAX];			// Std Path

//----------------------------------------------------------------------------

typedef void*		CWXMLTree;					// XML Trees

//----------------------------------------------------------------------------

typedef void*		CWUnit;						// Units (distances, scales,...)

//----------------------------------------------------------------------------

typedef void*		CWDocument;					// Document

//----------------------------------------------------------------------------

typedef void*		CWObject;					// MacMap¨ objects

// Std field indexes, user fields begins at 13
enum{
	kCWObject_ID_		=1,
	kCWObject_Name_		=9,
	kCWObject_SubType_	=10,
	kCWObject_Color_	=11,
	kCWObject_Dir_		=12
};

//----------------------------------------------------------------------------

typedef void*		CWType;						// MacMap¨ types

// Type kinds
enum{
	kCWTypeKindPoint	=1,
	kCWTypeKindPolyline	=2,
	kCWTypeKindText		=3,
	kCWTypeKindPolygon	=4,
	kCWTypeKindRaster	=5
};

// Base kind
enum{
	kCWBaseKindLocal		='bLDB',
};

// Database fields kinds
enum{
	kCWTypeFieldKindChar	='CHAR',
	kCWTypeFieldKindBool	='BOOL',
	kCWTypeFieldKindInteger	='INT_',
	kCWTypeFieldKindDouble	='DBLE',
	kCWTypeFieldKindDate	='DATE',
	kCWTypeFieldKindTime	='TIME',
	kCWTypeFieldKindBinary	='BIN_'
};

//----------------------------------------------------------------------------

typedef void*		CWLayer;					// MacMap¨ layers

//----------------------------------------------------------------------------

typedef void*		CWMapWindow;

//----------------------------------------------------------------------------

typedef void*		CWEvent;

// Theses values can be received in Events Handlers, or 
// in Event procs of your plugins (see CWXMap_EventPtr, in CWXMapWrappers.h)
enum{
	kCWEventClassMacMap			='Map_',	// Returned by GetEventClass (CarbonEventsCore.h)
	kCWEventKindMacMapDataBase	='MpDt'		// Returned by GetEventKind (CarbonEventsCore.h)
};

// Event actions 
enum{
	kCWEventActionCreate		='crea',	// Something have been created
	kCWEventActionModify		='modi',	// Something have been modified
	kCWEventActionDestroy		='kill'		// Something have been destroyed
};

// Event kinds
enum{
	kCWEventKindGeoElement		='gobj',	// Object event
	kCWEventKindDataStruct		='data',	// Table structure event
	kCWEventKindTypeElement		='type',	// Type event
	kCWEventKindUserExt			='uExt'		// User function event (script, macro, calc, var,...)
};

// Struct for field events
typedef struct CWEventfield_info{
	int			fidx;
	int			fid;
	int			kind;
	int			len;
	int			decs;
	CWCharData	name;
	CWType*		tp;
}CWEventfield_info;

//----------------------------------------------------------------------------

typedef void*		CWGraphicContext;		// Unused actually

enum{
	kCWGraphicContextCG			='CG__',
	kCWGraphicContextPDF		='PDF_',
	kCWGraphicContextPrint		='PRNT',
	kCWGraphicContextKML		='KML_'
};

//----------------------------------------------------------------------------

enum{
	kCWExtProcessCallFromIntf		= 1,	// parameters set with intf
	kCWExtProcessCallWithParams		= 2,	// call with prm
	kCWExtProcessCallWithXMLTree	= 3,	// params describes as xml tree
	kCWExtProcessCallGetData		= 32,	// read params
	kCWExtProcessCallSetData		= 33	// write params
};

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif