//----------------------------------------------------------------------------
// File : endian.c
// Project : MacMap
// Purpose : C source file : C utils around endianess
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
// 12/06/2008 creation.
//----------------------------------------------------------------------------

#include "endian.h"

int		pt_endian_check_int=1;

// ---------------------------------------------------------------------------
// 
// -----------
void LBSwapWord(void* word, int sz){
int				i;
unsigned char	buff;

	for(i=0;i<sz/2;i++){
		buff=((unsigned char*)word)[i];
		((unsigned char*)word)[i]=((unsigned char*)word)[sz-i-1];
		((unsigned char*)word)[sz-i-1]=buff;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
char LBGet(void){
	if(*((char*)&pt_endian_check_int)){
		return(1);
	}
	else{
		return(0);
	}
}

