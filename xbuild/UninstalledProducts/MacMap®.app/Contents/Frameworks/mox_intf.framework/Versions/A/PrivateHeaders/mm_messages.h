//----------------------------------------------------------------------------
// File : mm_messages.h
// Project : MacMap
// Purpose : Header file : Localization utils (messages, words, etc...)
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

#ifndef __mm_messages__
#define __mm_messages__

//----------------------------------------------------------------------------

#define	__MESSAGE_STRING_LENGTH_MAX__	256

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

void message_string		(	const char* msg_id, 
							char* string, 
							int maj);
void b_message_string	(	const char* msg_id, 
							char* string, 
							CFBundleRef bndl, 
							int maj);
void message_cfstring	(	const char* msg_id, 
							CFStringRef* string);
void b_message_cfstring	(	const char* msg_id, 
							CFStringRef* string, 
							CFBundleRef bndl);
void get_localized_name	(	char* string, 
							CFBundleRef bndl);

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#define	kMsgOk				"ok"
#define	kMsgCancel			"cancel"
#define	kMsgPlural			"plural"
#define	kMsgObjects			"objects"
#define	kMsgObject			"object"
#define	kMsgTypes			"types"
#define	kMsgType			"type"
#define	kMsgCard			"card"
#define	kMsgCards			"cards"
#define	kMsgKind			"kind"
#define	kMsgUnit			"unit"
#define	kMsgResolution		"resolution"
#define	kMsgNoSelection		"no selection"
#define	kMsgNoDocument		"no document"
#define	kMsgProgress		"progress"
#define	kMsgOptions			"options"
#define	kMsgCopy			"copy"
#define	kMsgUntitled		"untitled"
#define	kMsgNewView			"new view"
#define	kMsgNewStyle		"new style"
#define	kMsgVirtual			"virtual"
#define	kMsgName			"name"
#define	kMsgColor			"color"
#define	kMsgSubType			"subtype"
#define	kMsgFlag			"flag"
#define	kMsgDirection		"direction"

#define	kMsgKindPoint		"point_kind"
#define	kMsgKindLine		"line_kind"
#define	kMsgKindText		"text_kind"
#define	kMsgKindArea		"area_kind"
#define	kMsgKindRaster		"raster_kind"

#define	kMsgSubTypeDefault	"defaultsubtype"
#define	kMsgBlack			"black"
#define	kMsgWhite			"white"
#define	kMsgRed				"red"
#define	kMsgGreen			"green"
#define	kMsgBlue			"blue"
#define	kMsgCyan			"cyan"
#define	kMsgMagenta			"magenta"
#define	kMsgYellow			"yellow"

#define	kMsgNavigation		"navigation"
#define	kMsgGeometry		"geometry"
#define	kMsgPresentation	"presentation"

#define	kMsgOpen			"open"
#define	kMsgClose			"close"
#define	kMsgNew				"new"

#define	kMsgLength			"length"
#define	kMsgArea			"area"
#define	kMsgDecimals		"decimals"
#define	kMsgFormat			"format"

#define	kMsgAngstroms		"angstroms"
#define	kMsgNanometers		"nanometers"
#define	kMsgMicrometers		"micrometers"
#define	kMsgMillimeters		"millimeters"
#define	kMsgCentimeters		"centimeters"
#define	kMsgInches			"inches"
#define	kMsgDecimeters		"decimeters"
#define	kMsgFeet			"feet"
#define	kMsgYards			"yards"
#define	kMsgMeters			"meters"
#define	kMsgDecameters		"decameters"
#define	kMsgHectometers		"hectometers"
#define	kMsgKilometers		"kilometers"
#define	kMsgMiles			"miles"

#define	kMsgAngstromsAB		"angstromsAB"
#define	kMsgNanometersAB	"nanometersAB"
#define	kMsgMicrometersAB	"micrometersAB"
#define	kMsgMillimetersAB	"millimetersAB"
#define	kMsgCentimetersAB	"centimetersAB"
#define	kMsgInchesAB		"inchesAB"
#define	kMsgDecimetersAB	"decimetersAB"
#define	kMsgFeetAB			"feetAB"
#define	kMsgYardsAB			"yardsAB"
#define	kMsgMetersAB		"metersAB"
#define	kMsgDecametersAB	"decametersAB"
#define	kMsgHectometersAB	"hectometersAB"
#define	kMsgKilometersAB	"kilometersAB"
#define	kMsgMilesAB			"milesAB"

#define	kMsgASeconds		"Aseconds"
#define	kMsgAMinutes		"Aminutes"
#define	kMsgDegrees			"degrees"

#define	kMsgASecondsAB		"AsecondsAB"
#define	kMsgAMinutesAB		"AminutesAB"
#define	kMsgDegreesAB		"degreesAB"

#define	kMsgTSeconds		"Tseconds"
#define	kMsgTMinutes		"Tminutes"
#define	kMsgHours			"hours"
#define	kMsgDays			"days"
#define	kMsgMonths			"months"
#define	kMsgYears			"years"
#define	kMsgCenturies		"centuries"

#define	kMsgTSecondsAB		"TsecondsAB"
#define	kMsgTMinutesAB		"TminutesAB"
#define	kMsgHoursAB			"hoursAB"
#define	kMsgDaysAB			"daysAB"
#define	kMsgMonthsAB		"monthsAB"
#define	kMsgYearsAB			"yearsAB"
#define	kMsgCenturiesAB		"centuriesAB"

//----------------------------------------------------------------------------

#endif
