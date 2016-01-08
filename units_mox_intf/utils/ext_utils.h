//----------------------------------------------------------------------------
// File : ext_utils.h
// Project : MacMap
// Purpose : Header file : C utils for externs
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
// 13/02/2006 creation.
//----------------------------------------------------------------------------

#ifndef __ext_utils__
#define __ext_utils__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/wtable.h>
#include <MacMapSuite/wproj.h>
#include <QuickTime/QuickTimeComponents.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

UInt32				UniqueSignature						(	bGenericMacMapApp* gapp);
bGenericType*		NthTypeOfKind						(	bGenericMacMapApp* gapp,
															int idx,
															int kind);
	
void				PopulatePopupControlWithScales		(	bGenericMacMapApp* gapp, 
															ControlRef c, 
															int current);
void				PopulatePopupControlWithDistUnits	(	bGenericMacMapApp* gapp, 
															ControlRef c, 
															int current);
void				PopulatePopupControlWithFields		(	bGenericType* tp, 
															ControlRef c, 
															int start, 
															int current);
void				PopulatePopupControlWithTypedFields	(	bGenericType* tp, 
															ControlRef c, 
															int start, 
															int current,
															int* types,
															int n);
void				PopulatePopupControlWithConstraints	(	bGenericType* tp, 
															int field,
															ControlRef c, 
															int current);

void				PopulatePopupControlWithOperators	(	bGenericMacMapApp* gapp, 
															ControlRef c, 
															int current);
float*				GetDash								(	bGenericMacMapApp* gapp,
															bGenericType* tp,
															const char* name,
															int* count);
OSType				GetImageKind						(	const char* name);
CGPDFDocumentRef	GetPDFPattern						(	bGenericMacMapApp* gapp,
															bGenericType* tp,
															const char* name);
CGPDFDocumentRef	GetPDFIcon							(	bGenericMacMapApp* gapp,
															bGenericType* tp,
															const char* name);
CGImageRef			GetImageIcon						(	bGenericMacMapApp* gapp,
															bGenericType* tp,
															const char* name);

CGImageRef			GetRaster							(	bGenericMacMapApp* gapp,
															bGenericGeoElement* o);

UInt32				GetSignatureFromBundle				(	CFBundleRef bndl);
UInt32				GetSignature						(	bGenericXMLBaseElement* elt);
void				GetSignatureString					(	bGenericXMLBaseElement* elt, 
															char* sign);
void				GetName								(	bGenericXMLBaseElement* elt,
															char* name);
// Precision coefficient with prefs units
double				GetPrecisionCoef					(	bGenericMacMapApp* gapp);
// Precision coefficient without prefs units
double				GetRawPrecisionCoef					(	bGenericMacMapApp* gapp);
// Conversion to precision model with prefs units
double				Measure_d2i							(	bGenericMacMapApp* gapp, 
															double m);
// Conversion from precision model with prefs units
double				Measure_i2d							(	bGenericMacMapApp* gapp, 
															double m);
// Pointer comparison
int					ObjRefComp							(	const void* a,
															const void* b);
// ID comparison
int					ObjIDComp							(	const void* a,
															const void* b);
// Iterator Stuff
void				SetFlagWrapper						(	void* o,
															bool b);
bool				GetFlagWrapper						(	void* o);
void				GetBoundsWrapper					(	void* o,
															ivx_rect* vxr);
// Ext/User Ext Utils
int					RecoverGeog							(	bGenericMacMapApp* gapp,
															const char* formula);
int					RecoverCalc							(	bGenericMacMapApp* gapp,
															const char* formula);
int					RecoverVar							(	bGenericMacMapApp* gapp,
															const char* formula);
int					RecoverMacro						(	bGenericMacMapApp* gapp,
															const char* formula);
int					RecoverScript						(	bGenericMacMapApp* gapp,
															const char* formula);
int					RecoverScale						(	bGenericMacMapApp* gapp,
															bGenericUnit* u);

// Object init
int					InitWithStyle						(	bGenericMacMapApp* gapp,
															int style_index,
															bGenericGeoElement* o);

int					FieldInView							(	bGenericMacMapApp* gapp,
															bGenericType* tp,
															int field);
															
int					GetBufferFromXMLParam				(	bGenericXMLBaseElement* root,
															void** buffer,
															int* bsz);
int					GetXMLParamFromBuffer				(	bGenericMacMapApp* gapp,
															bGenericXMLBaseElement** root,
															void* buffer,
															int bsz);
bGenericXMLBaseElement* 
					GetXMLParamFromBundle				(	bGenericMacMapApp* gapp,
															CFBundleRef bndl,
															const char* name);
	
void				SetMarginToCurview					(	bGenericMacMapApp* gapp, 
															double d);
void				SaveCurrentMargins					(	bGenericMacMapApp* gapp, 
															bArray** margins);
int					RestoreCurrentMargins				(	bGenericMacMapApp* gapp, 
															bArray* margins);
	
void				IdleApp								(	);

/*Boolean				db_file_filter						(	AEDesc* item,
															void* xinfo, 
															void* cba, 
															NavFilterModes mode);*/

long				GetBestScaleIndexForValue			(	bGenericMacMapApp* gapp,
															double val);

void				GetBasePath							(	bGenericMacMapApp* gapp,
															char* path);
//----------------------------------------------------------------------------
// CGRect/ivx_rect conversion
//----------------------------------------------------------------------------
CGRect				ivr2cgr								(	bGenericMacMapApp* gapp, 
															ivx_rect ivr);
ivx_rect			cgr2ivr								(	bGenericMacMapApp* gapp, 
															CGRect cgr);

		
//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
// Getting a name from a window displaying a text field
bool				GetAName							(	char* name, 
															const char* wtitle,
															bool cncl);
// Getting a value from a window displaying a text field, for a field
bool				GetAValue							(	bGenericType* tp,
															int field,
															const char* name,
															char* value);

//----------------------------------------------------------------------------
// Proj Stuff
//----------------------------------------------------------------------------
// Getting a proj srid from a window displaying a list
bool				GetAProj							(	int* srid, 
															bool uselonglat);
// Getting a proj name from srid
bool				GetProjName							(	int srid, 
															char* name);

//----------------------------------------------------------------------------
								
#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#define	Convert(a,b)		_gapp->locConverter()->convert((a),(b))

#define btst(a,n)			(((a)&(1L<<(n)))!=0)
#define bset(a,n)			((a)|=(1L<<(n)))
#define bclr(a,n)			((a)&=(-1L^(1L<<(n))))

//----------------------------------------------------------------------------

#endif
