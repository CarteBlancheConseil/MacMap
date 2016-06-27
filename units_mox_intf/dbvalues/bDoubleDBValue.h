//----------------------------------------------------------------------------
// File : bDoubleDBValue.h
// Project : MacMap
// Purpose : Header file : Double values class
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
// 04/12/2003 creation.
//----------------------------------------------------------------------------

#ifndef __BDOUBLEDBVALUE__
#define __BDOUBLEDBVALUE__

//----------------------------------------------------------------------------

#include "bStdDBValue.h"

//----------------------------------------------------------------------------

class bDoubleDBValue : public bStdDBValue{
	public:
		bDoubleDBValue					();
		virtual ~bDoubleDBValue			();

		virtual bStdDBValue* replicate	();
		
		virtual void	get				(char*		value);
		virtual void	get				(int*		value);
		virtual void	get				(double*	value);
		virtual void	get				(bool*		value);
		virtual void	get				(time_t*	value);
		virtual void	get				(void*		value);

		virtual void	put				(char*		value);
		virtual void	put				(int		value);
		virtual void	put				(double		value);
		virtual void	put				(bool		value);
		virtual void	put				(time_t		value);
		virtual void	put				(void*		value);
		
		virtual bool					operator ==	(bStdDBValue&);
		virtual bool					operator !=	(bStdDBValue&);
		virtual bool					operator <	(bStdDBValue&);
		virtual bool					operator >	(bStdDBValue&);
		virtual bool					operator <=	(bStdDBValue&);
		virtual bool					operator >=	(bStdDBValue&);
		
		virtual bool					operator &&	(bStdDBValue&);
		virtual bool					operator ||	(bStdDBValue&);

		virtual bStdDBValue*			operator +	(bStdDBValue&);
		virtual bStdDBValue*			operator -	(bStdDBValue&);
		virtual bStdDBValue*			operator *	(bStdDBValue&);
		virtual bStdDBValue*			operator /	(bStdDBValue&);
		    
	protected:
		
	private:
		double	val;
};

//----------------------------------------------------------------------------

#endif
