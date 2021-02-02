#ifndef MAIN_DBPATCHER_H
#define MAIN_DBPATCHER_H

#define _WIN32_WINNT _WIN32_WINNT_WINXP // Enables Windows XP compatibility

#pragma pack(1)

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "database_flags.h"
#include "database_types.h"
#include "lib_ehm07.h"

class cDBPatch
{
private:
	long team_id_;
	long comp_id_vanilla_;	// Competition ID for unmodified / vanilla EHM database
	long comp_id_patched_;	// Competition ID for SuperPatched EHM
	bool error_flag_;

	// Static members
	static bool apply_superpatch_;	// Flag to determine whether to apply SuperPatch settings (true) or vanilla settings (false)
	
public:
	cDBPatch(int, int, int);
	~cDBPatch() {}

	// Member functions
	bool UpdateCompID(std::vector<CLUBS> &, std::vector<CLUB_COMPS> &);

	// Static functions
	static bool Apply(std::vector<cDBPatch> &, std::vector<CLUBS> &, std::vector<CLUB_COMPS> &);
	static void LoadSettings(std::vector<cDBPatch> &);
	static void SetPatchType(short &option) { apply_superpatch_ = (option == 1) ? true : false; }	// Set the patch type according to the user's option
};

#endif