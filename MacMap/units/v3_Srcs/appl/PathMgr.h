//----------------------------------------------------------------------------
// File : PathMgr.h
// Project : MacMap
// Purpose : Header file : Paths to types managements (adding types, removing, etc...)
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2002 Carte Blanche Conseil.
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
// 09/09/2002 creation.
//----------------------------------------------------------------------------

#ifndef _PATH_MGR_
#define _PATH_MGR_

// ---------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------

long	PATH_count		(	void);
bool	PATH_get		(	long n, 
							int* dbtype,
							char* path);
char*	PATH_getExt		(	long n,
							int* pthType, 
							const char* ext);
long    PATH_add		(	int pthType,
							const char* path);
bool	PATH_change		(	long n,
							const char* path);
bool	PATH_rmv		(	long n);
bool	PATH_make		(	);

// ---------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

// ---------------------------------------------------------------------------

#endif


