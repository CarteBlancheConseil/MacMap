//----------------------------------------------------------------------------
// File : bStdNSPalettevDef.h
// Project : MacMap
// Purpose : Header file : Base class for Cocoa floating window based vDef
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
// 25/03/2011 creation.
//----------------------------------------------------------------------------

#ifndef __bStdNSPalettevDef__
#define __bStdNSPalettevDef__

//----------------------------------------------------------------------------

#include <std_ext/bStdvDef.h>

//----------------------------------------------------------------------------

#ifdef __LITTLE_ENDIAN__
#define kvDefSignature				'feDv'
#else
#define kvDefSignature				'vDef'
#endif

//----------------------------------------------------------------------------

class bStdNSPalettevDef : public bStdvDef{
	public:		
		bStdNSPalettevDef						(	bGenericXMLBaseElement* elt, 
													bGenericMacMapApp* gapp, 
													CFBundleRef bndl);
		virtual ~bStdNSPalettevDef				(	);
		virtual bGenericXMLBaseElement* create	(	bGenericXMLBaseElement* elt);
		
		virtual void open						(	int* flags);
		virtual void close						(	);
		virtual bool event						(	EventRef evt);
		virtual void idle						(	void* prm);
		virtual bool edit						(	void* prm);
		virtual bool process					(	int msg, 
													void* prm);
		virtual bool test						(	void* prm);


		virtual bool test						(	bGenericType* tp);
		virtual bool edit						(	bGenericXMLBaseElement** root, 
													bGenericType* tp);

		virtual void set_bounds					(	CGRect* bnds);//
		virtual void get_bounds					(	CGRect* bnds);//
		virtual void wd_close					(	);

		virtual bool update						(	);

// Accessors
		virtual bArray&	runs					(	){return(_styles);};
		virtual bGenericType* type_get			(	){return(_tp);};
	
		virtual int fld_get						(	){return(_fld);};
		virtual void fld_put					(	int fld){_fld=fld;stl_free();stl_alloc();};

		virtual int pssmax_get					(	){return(_pssmax);};
		virtual void pssmax_put					(	int pssmax){_pssmax=pssmax;};

		virtual int sort_get					(	){return(_sort);};
		virtual void sort_put					(	int sort){_sort=sort;};
	
		virtual double sref_get					(	){return(_sref);};
		virtual void sref_put					(	double sref){_sref=sref;};
	
		virtual double coef_get					(	){return(_coef);};
		virtual void coef_put					(	double coef){_coef=coef;};
	
		virtual int cfld_get					(	){return(_cfld);};
		virtual void cfld_put					(	int cfld){_cfld=cfld;};
	
		virtual int cop_get						(	){return(_cop);};
		virtual void cop_put					(	int cop){_cop=cop;};
	
		virtual void cval_get					(	char* cval){strcpy(cval,_cval);};
		virtual void cval_put					(	char* cval){strcpy(_cval,cval);};
	
		virtual bool cmyk_get					(	){return(_cmyk);};
		virtual void cmyk_put					(	bool cmyk){_cmyk=cmyk;};
		
	protected:

		virtual void make						(	){};
		virtual bool read						(	){return(false);};
		virtual bool write						(	){return(false);};
		virtual void stl_alloc					(	){};
		virtual void stl_free					(	){};
	
		virtual void read_p						(	);
		virtual void write_p					(	);
		virtual bGenericXMLBaseElement* make_p	(	);
		
		virtual void* ccinit					(	);
		
		void*		_controller;
		bool		_openit;
		bool		_wd_open;
		CGRect		_bnds;
	
	
		char		_stylename[256];		
		char		_vers[256];
		UInt32		_sign;
		
		char		_name[256];

		int			_fld;		//<->
		int			_pssmax;	//<->
		int			_sort;		//<->
		
		double		_sref;		//<->
		double		_coef;		//<->
		
		int			_cfld;		//<->
		int			_cop;		//<->
		char		_cval[256];	//<->
		bool		_cmyk;		//<->
		
		bArray		_styles;
		
		int			_style_idx;
	
	private:		
		
};

//----------------------------------------------------------------------------

#endif
