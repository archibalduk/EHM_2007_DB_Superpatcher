#include "main_dbpatcher.h"

// Changes to be made to the database
void cDBPatch::LoadSettings(std::vector<cDBPatch> &db_patch)
{
	// --- AUSTRIA --- //
	db_patch.push_back( cDBPatch(1797, 103, 15) );	// EC Dornbirn Bulldogs:				Nationalliga			-> Austrian Erste Bank Eishockey Liga
	db_patch.push_back( cDBPatch(402, 26, 15) );	// HDD Olimpija Ljubljana:				Slovenian Elite League	-> Austrian Erste Bank Eishockey Liga
	db_patch.push_back( cDBPatch(52, 102, 15) );	// Orli Znojmo:							Czech First Division	-> Austrian Erste Bank Eishockey Liga
	db_patch.push_back( cDBPatch(487, 103, 15) );	// HC Tiroler Wasserkraft Innsbruck:	Nationalliga			-> Austrian Erste Bank Eishockey Liga

	// --- SLOVENIA --- //
	db_patch.push_back( cDBPatch(652, 502, 26) );	// HK Partizan Beograd:					Serbian League			-> Slovenian Elite League
	
	return;
}