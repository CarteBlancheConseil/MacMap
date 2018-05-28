//----------------------------------------------------------------------------
// File : XMLSignatures.h
// Project : MacMap
// Purpose : Header file : XML defines
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
// 17/03/2004 creation.
//----------------------------------------------------------------------------
// Actuellement, on ne sait pas faire dériver gExt de gXMLFormula
// Geog et Calc doivent donc être séparées entre interface et fct

#ifndef __XMLSIGNATURE__
#define __XMLSIGNATURE__

//----------------------------------------------------------------------------

// generic
#define kXMLClassGeneric				'xml '
#define kXMLSubClassGeneric				kXMLClassGeneric

// layers
#define kXMLClassLayer					'layr'
#define kXMLSubClassLayer				kXMLClassLayer

// render
#define kXMLClassRendering				'rndr'
#define kXMLSubClassRendering			kXMLClassRendering
#define kXMLSubClassLowRendering		'rndl'
#define kXMLSubClassStyleIdentification	'rnsi'
#define kXMLSubClassCondition			'rncd'
#define kXMLSubClassGlobals				'rngl'
#define kXMLSubClassGeometry			'rngo'
#define kXMLSubClassStyle				'rnst'

// value
#define kXMLClassValue					'valu'
#define kXMLSubClassValue				kXMLClassValue

// formula
#define kXMLClassFormula				'math'
#define kXMLSubClassFormula				kXMLClassFormula
#define kXMLSubClassOperator			'maop'
#define kXMLSubClassIterator			'mait'
#define kXMLSubClassFunction			'mafu'
#define kXMLSubClassConstant			'mact'
#define kXMLSubClassField				'mafl'
#define kXMLSubClassMacro				'mama'
#define kXMLSubClassOption				'maot'
#define kXMLSubClassGeog				'mago'
#define kXMLSubClassCalc				'maca'

// Externes
#define kXMLSubClassExt					'clss'
#define kXMLSubClassExtGeog				'Geog'
#define kXMLSubClassExtCalc				'Calc'
#define kXMLSubClassExtUserCalc			'UClc'
#define kXMLSubClassExtVar				'Var_'
#define kXMLSubClassExtUserVar			'UVar'
#define kXMLSubClassExtTool				'Tool'
#define kXMLSubClassExtvDef				'vDef'
#define kXMLSubClassExtXMap				'XMap'
#define kXMLSubClassExtMacro			'Macr'
#define kXMLSubClassExtScript			'Scrp'
#define kXMLSubClassExtXBox				'XBox'

#define kvDefUltraSignature				'Ultv'

//----------------------------------------------------------------------------

#endif
