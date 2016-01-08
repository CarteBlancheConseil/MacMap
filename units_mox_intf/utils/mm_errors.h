//----------------------------------------------------------------------------
// File : mm_errors.h
// Project : MacMap
// Purpose : Header file : Localization utils for errors
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
// 03/02/2005 creation.
//----------------------------------------------------------------------------

#ifndef __MM_ERRORS__
#define __MM_ERRORS__

//----------------------------------------------------------------------------

#define	__ERROR_STRING_LENGTH_MAX__	256

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

int error_num		(int family_code, int error_code);
void error_string	(int error_num, char* string);
void b_error_string	(int error_num, char* string, CFBundleRef bndl);

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#define _bStdExtLibRootErr_		10000
enum{
	_bStdExtLibNoErr_,
	_bStdExtLibNoFamilyErr_,
	_bStdExtLibNoCreatorErr_,
	_bStdExtLibNoVersErr_,
	_bStdExtLibNoNameErr_,
	_bStdExtLibNoIdentifierErr_,
	_bStdExtLibNoSignatureErr_,
	_bStdExtLibNoMacMapVersErr_,
	_bStdExtLibBundleAlreadyLoadedErr_,
	_bStdExtLibBundleLoadErr_,
	_bStdExtLibNoInitProcErr_,
	_bStdExtLibNoDestroyProcErr_,
	_bStdExtLibNoInstanceErr_
};

#define _bClassesMgrRootErr_	10040
enum{
	_bClassesMgrNoErr_,
	_bClassesMgrClassNotLoadedErr_
};

#define _bMacMapAppErr_		100000
enum{
	_bMacMapAppNoErr_,
	_bMacMapAppLogInitErr_,
	_bMacMapAppSpecInitErr_,// A retirer
	_bMacMapAppNibInitErr_,
	_bMacMapAppResForkInitErr_,
	_bMacMapDungleInitErr_
};

#define _bDataBaseErr_		110000
enum{
	_bDataBaseNoErr_,
	_bDataBaseReadErr_,
	_bDataBaseWriteErr_,
	_bDataBaseAddFieldErr_,
	_bDataBaseRemoveFieldErr_,
	_bDataBaseModifyFieldErr_,
    _bDataBaseSetDefaultValueErr_,
    _bDataBaseSetHiddenErr_,
    _bDataBaseSetDeleteProtectedErr_,
    _bDataBaseSetWriteProtectedErr_
};

//----------------------------------------------------------------------------

#endif
