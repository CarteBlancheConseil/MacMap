//----------------------------------------------------------------------------
// File : bvDefPalettePropWindowController.h
// Project : MacMap
// Purpose : Header file : Base subclass for proportionnal vDef window controller
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
// 02/01/2013 creation.
//----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <std_ext/bStdNSPalettevDefWindowController.h>
#import <std_ext/vDefPalettePreview.h>

//----------------------------------------------------------------------------

@interface bvDefPalettePropWindowController : bStdNSPalettevDefWindowController{
// Interface
// Std Cocoa Palette vDef
	IBOutlet vDefPalettePreview*	_prvw;		// Preview
	IBOutlet NSTextField*			_tnm_fld;	// Nom du type
	IBOutlet NSTabView*				_tab;		// Rubriques
	 
// Statistiques
	IBOutlet NSPopUpButton*			_fld_pop;	// Champ analysé
	IBOutlet NSPopUpButton*			_prp_pop;	// Proportionnalité
	IBOutlet NSPopUpButton*			_oor_pop;	// Dépassement de seuil
	IBOutlet NSPopUpButton*			_min_pop;	// Calcul du min
	IBOutlet NSTextField*			_min_fld;	// Minimum	
	IBOutlet NSPopUpButton*			_max_pop;	// Calcul du max
	IBOutlet NSTextField*			_max_fld;	// Maximum	

// Taille	
	IBOutlet NSTextField*			_szn_fld;	// Minimum	
	IBOutlet NSTextField*			_szx_fld;	// Maximum	

// Apparence	
	IBOutlet NSPopUpButton*			_fil_pop;	// Switch remplissage
	IBOutlet NSTabView*				_fil_tab;	// Tab remplissages
	IBOutlet NSColorWell*			_fil_clr;	// Couleur
	IBOutlet NSButton*				_fil_img;	// Pattern
	
	
// Gradient
// Std Cocoa Palette vDef
	IBOutlet NSTableView*			_g_run_tbl;	// StyleRuns
	IBOutlet vDefPalettePreview*	_g_prvw;	// Preview
	
	IBOutlet NSPopUpButton*			_g_fld_pop;	// Champ analysé
	IBOutlet NSPopUpButton*			_g_dsc_pop;	// Méthode de discrétisation
	IBOutlet NSTextField*			_g_ncl_fld;	// Nombre de classes	
	IBOutlet NSPopUpButton*			_g_min_pop;	// Calcul du min
	IBOutlet NSTextField*			_g_min_fld;	// Minimum	
	IBOutlet NSPopUpButton*			_g_max_pop;	// Calcul du max
	IBOutlet NSTextField*			_g_max_fld;	// Maximum	
	IBOutlet NSButton*				_g_dsc_btn;	// Discrétiser
	IBOutlet NSButton*				_g_add_btn;	// Ajouter une classe
	IBOutlet NSButton*				_g_rmv_btn;	// Supprimer une classe
	
	IBOutlet NSPopUpButton*			_g_fil_pop;	// Switch remplissage
	IBOutlet NSTabView*				_g_fil_tab;	// Tab remplissages
	IBOutlet NSColorWell*			_g_fil_clr;	// Couleur
	IBOutlet NSButton*				_g_fil_img;	// Pattern

	IBOutlet NSPopUpButton*			_g_clr_pop;	// Colorimétrie

	
	
// Visibilité
	IBOutlet NSPopUpButton*			_smn_pop;	// Echelle min
	IBOutlet NSPopUpButton*			_smx_pop;	// Echelle max
	
// Options : general
	IBOutlet NSPopUpButton*			_o_srf_pop;	// Echelle de référence
	IBOutlet NSPopUpButton*			_o_unt_pop;	// Unité
	IBOutlet NSButton*				_o_cls_chk;	// CMYK
	
// Options : conditions
	IBOutlet NSButton*				_o_dif_chk; // Afficher si
	IBOutlet NSPopUpButton*			_o_cnf_pop;	// Champ condition
	IBOutlet NSPopUpButton*			_o_cno_pop;	// Opérateur
	IBOutlet NSTextField*			_o_val_fld;	// Valeur
		
// Options : offset
	IBOutlet NSButton*				_o_off_chk; // Offset
	IBOutlet NSPopUpButton*			_o_xof_pop;	// Champ offset X
	IBOutlet NSPopUpButton*			_o_yof_pop;	// Champ offset Y
	
// Update du style
	IBOutlet NSButton*				_upd_btn;
	
	
	
	
	NSInteger						_last_click;
	BOOL							_hprev;
	double							_qmin;
	double							_qmax;
	int								_nbclass;
	
	BOOL							_uclss;
	BOOL							_area;
}

//----------------------------------------------------------------------------
// Actions proportionnel
-(IBAction)doChooseField:(id)sender;
-(IBAction)doChooseProportionnality:(id)sender;
-(IBAction)doChooseOutOfRangeKind:(id)sender;
-(IBAction)doChooseMin:(id)sender;
-(IBAction)doPutMin:(id)sender;
-(IBAction)doChooseMax:(id)sender;
-(IBAction)doPutMax:(id)sender;

// Actions classes
-(IBAction)doChooseClassField:(id)sender;
-(IBAction)doChooseClassMethod:(id)sender;
-(IBAction)doPutClassCount:(id)sender;
-(IBAction)doChooseClassMin:(id)sender;
-(IBAction)doPutClassMin:(id)sender;
-(IBAction)doChooseClassMax:(id)sender;
-(IBAction)doPutClassMax:(id)sender;
-(IBAction)doComputeClass:(id)sender;
-(IBAction)doAddClass:(id)sender;
-(IBAction)doRemoveClass:(id)sender;
-(IBAction)doChooseClassFillKind:(id)sender;
-(IBAction)doChooseClassFillColor:(id)sender;
-(IBAction)doChooseClassFillPattern:(id)sender;
-(IBAction)doChooseClassColorimetry:(id)sender;


-(IBAction)doPutSize:(id)sender;
-(IBAction)doChooseColor:(id)sender;

-(IBAction)doChooseScaleMin:(id)sender;
-(IBAction)doChooseScaleMax:(id)sender;

-(IBAction)doClickPreview:(id)sender;//

// Actions options
-(IBAction)doChooseScaleRef:(id)sender;
-(IBAction)doChooseUnit:(id)sender;
-(IBAction)doChooseColorSpace:(id)sender;

-(IBAction)doCheckCondition:(id)sender;
-(IBAction)doChooseCondField:(id)sender;
-(IBAction)doChooseCondOperator:(id)sender;
-(IBAction)doPutCondValue:(id)sender;

-(IBAction)doCheckOffset:(id)sender;
-(IBAction)doChooseOffsetXField:(id)sender;
-(IBAction)doChooseOffsetYField:(id)sender;

// Actions appliquer
-(IBAction)doApply:(id)sender;

//----------------------------------------------------------------------------
// Gestion table
-(NSInteger)	numberOfRowsInTableView:(NSTableView*)aTableView;
-(id)			tableView:(NSTableView *)aTableView 
				objectValueForTableColumn:(NSTableColumn*)aTableColumn 
				row:(NSInteger)rowIndex;
-(void)			tableView:(NSTableView *)tableView 
				setObjectValue:(id)object 
				forTableColumn:(NSTableColumn *)tableColumn 
				row:(NSInteger)row;
-(void)			tableViewSelectionDidChange:(NSNotification*)notification;

//----------------------------------------------------------------------------
// Gestion Interface
-(void)updateUI;
-(void)updateSizeMax;
-(void)checkIntfAvailability;
-(void)plot2Intf;
-(void)convertColor;
-(void)drawPreview:(CGRect)rect context:(CGContextRef)ctx;
-(void)clickPreview:(CGPoint)point modifiers:(NSUInteger)mod;

//----------------------------------------------------------------------------
// Fonctions de calcul des tailles max
-(double)sizeMax;
-(double)widthMax;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------
