//----------------------------------------------------------------------------
// File : bXMapCalcCell.h
// Project : MacMap
// Purpose : Header file : MapCalculator editor cells classes
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
// 03/05/2007 creation.
//----------------------------------------------------------------------------

#ifndef __bXMapCalcCell__
#define __bXMapCalcCell__

//----------------------------------------------------------------------------

#include <std_ext/bStdXMapWithModalIntf.h>
#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/bStdBaseAccessor.h>

//----------------------------------------------------------------------------

enum{
	kXMapCalcEditorKindNone		=0,
	kXMapCalcEditorKindOperator	=1,
	kXMapCalcEditorKindIterator	=2,
	kXMapCalcEditorKindFunction	=3,
	kXMapCalcEditorKindConstant	=4,
	kXMapCalcEditorKindField	=5,
	kXMapCalcEditorKindCalc		=6,
	kXMapCalcEditorKindMacro	=7
};

//----------------------------------------------------------------------------

class bXMapCalcCell{
	public:		
		bXMapCalcCell							(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCell					(	);

		static void set_gorigin					(	CGPoint cgp);
		static CGPoint gorigin					(	){return(_go);};
		static bool parse						(	bGenericXMLBaseElement* root, 
													void* prm,
													int indent);
													
		static bXMapCalcCell* get_select		(	){return(_gsel);};
		
		virtual void select						(	bool flg);
		
		virtual bool click						(	CGPoint cgp);

		virtual void draw						(	CGContextRef ctx, 
													CGPoint go);
		virtual void rect						(	CGRect* cgr);

		virtual void read						(	bGenericXMLBaseElement* root);
		virtual void write						(	bArray& arr,
													int indent);
		virtual bool add						(	bXMapCalcCell* cell);
		virtual bool remove						(	bXMapCalcCell* cell);
		
		virtual int estimate					(	);
		
		virtual bXMapCalcCell* left				(	){return(_lft);};
		virtual void set_left					(	bXMapCalcCell* cell);
		virtual bXMapCalcCell* right			(	){return(_rgt);};
		virtual void set_right					(	bXMapCalcCell* cell);
		virtual bXMapCalcCell* parent			(	){return(_par);};
		virtual void set_parent					(	bXMapCalcCell* cell){_par=cell;};
		virtual int kind						(	){return(_knd);};
		virtual CGPoint origin					(	){return(_o);};
		
	protected:													
		static CGPoint				_go;
		static bGenericMacMapApp*	_gapp;
		static bXMapCalcCell*		_gsel;
		
		bXMapCalcCell*				_par;
		bXMapCalcCell*				_rgt;
		bXMapCalcCell*				_lft;
		CGPoint						_o;
		int							_knd;
		int							_cnt;
		char						_name[256];
		float						_r,_g,_b;
		int							_lref;
		
		
	private:
};

//----------------------------------------------------------------------------

typedef struct parse_prm{
	bArray*			stack;
	int				cur;
	bXMapCalcCell*	root;
}parse_prm;

//----------------------------------------------------------------------------

class bXMapCalcCellDeadEnd : public bXMapCalcCell{
	public:		
		bXMapCalcCellDeadEnd					(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCellDeadEnd			(	);
		virtual void rect						(	CGRect* cgr);
		virtual void write						(	bArray& arr,
													int indent);
		
	protected:													
	
	private:
};

//----------------------------------------------------------------------------

class bXMapCalcCellOperator : public bXMapCalcCell{
	public:		
		bXMapCalcCellOperator					(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCellOperator			(	);
		static bool parse						(	bGenericXMLBaseElement* root, 
													bXMapCalcCell** clrt);
		virtual bool add						(	bXMapCalcCell* cell);
		
	protected:													
	
	private:
};

//----------------------------------------------------------------------------

class bXMapCalcCellIterator : public bXMapCalcCell{
	public:		
		bXMapCalcCellIterator					(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCellIterator			(	);
		static bool parse						(	bGenericXMLBaseElement* root, 
													bXMapCalcCell** clrt);
		virtual bool add						(	bXMapCalcCell* cell);
				
	protected:													
	
	private:
};

//----------------------------------------------------------------------------

class bXMapCalcCellFunction : public bXMapCalcCell{
	public:		
		bXMapCalcCellFunction					(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCellFunction			(	);
		static bool parse						(	bGenericXMLBaseElement* root, 
													bXMapCalcCell** clrt);
		virtual bool add						(	bXMapCalcCell* cell);
		
	protected:													
	
	private:
};

//----------------------------------------------------------------------------

class bXMapCalcCellConstant : public bXMapCalcCell{
	public:		
		bXMapCalcCellConstant					(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCellConstant			(	);
		static bool parse						(	bGenericXMLBaseElement* root, 
													bXMapCalcCell** clrt);
		virtual void write						(	bArray& arr,
													int indent);
		
	protected:													
	
	private:
};

//----------------------------------------------------------------------------

class bXMapCalcCellField : public bXMapCalcCell{
	public:		
		bXMapCalcCellField						(	bGenericMacMapApp* gapp,
													const char* name,
													bGenericType* tp);
		virtual ~bXMapCalcCellField				(	);
		static bool parse						(	bGenericXMLBaseElement* root, 
													bXMapCalcCell** clrt);
		virtual void write						(	bArray& arr,
													int indent);
		
	protected:													
	
	private:
		bGenericType*	_tp;
};

//----------------------------------------------------------------------------

class bXMapCalcCellCalc : public bXMapCalcCell{
	public:		
		bXMapCalcCellCalc						(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCellCalc				(	);
		static bool parse						(	bGenericXMLBaseElement* root, 
													bXMapCalcCell** clrt);
		virtual void write						(	bArray& arr,
													int indent);
		
	protected:													
	
	private:
};

//----------------------------------------------------------------------------

class bXMapCalcCellMacro : public bXMapCalcCell{
	public:		
		bXMapCalcCellMacro						(	bGenericMacMapApp* gapp,
													const char* name);
		virtual ~bXMapCalcCellMacro				(	);
		static bool parse						(	bGenericXMLBaseElement* root, 
													bXMapCalcCell** clrt);
		virtual bool add						(	bXMapCalcCell* cell);
		virtual void write						(	bArray& arr,
													int indent);
		
	protected:													
	
	private:
};

//----------------------------------------------------------------------------

#endif
