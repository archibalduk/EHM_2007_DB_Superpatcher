#pragma pack(1)
#include "main_dbpatcher.h"

// Static class members
bool cDBPatch::apply_superpatch_ = true;

int main()
{
	// Left align all text
	std::cout << setiosflags(std::ios::left)
			  << "http://www.ehmtheblueline.com" << std::endl
			  << std::endl
			  << "EHM Database Patcher (EHM SuperPatch) v0.1" << std::endl
			  << "by Archibalduk" << std::endl
			  << std::endl
			  << std::endl
			  << "Select an option:" << std::endl
			  << std::endl
			  << "1) Patch the database for use with the SuperPatch" << std::endl
			  << "2) Restore the database for use with unmodified/vanilla EHM" << std::endl
			  << std::endl;

	// User input option and set patch method
	short option = 0;
	std::cin >> option;
	cDBPatch::SetPatchType(option);
	
	// Vector buffers
	std::vector<cDBPatch> db_patch;			// Comp ID changes to be made
	std::vector<CLUBS> clubs;				// Clubs
	std::vector<CLUB_COMPS> club_comps;		// Club competitions
	
	// Apply the patch
	if(cDBPatch::Apply(db_patch, clubs, club_comps))
		std::cout << std::endl << "*** Patch applied successfully ***" << std::endl;
	else
		std::cout << std::endl << "*** Error encountered ***" << std::endl;

	std::cout << std::endl << "Press ENTER to close this window." << std::endl;
	std::cin.ignore();
	std::cin.get();

	return 0;
}

// Class constructor
cDBPatch::cDBPatch(int team_id, int comp_id_vanilla, int comp_id_patched)
{
	team_id_ = team_id;
	comp_id_vanilla_ = comp_id_vanilla;
	comp_id_patched_ = comp_id_patched;

	error_flag_ = false;
}

// Update competition IDs
bool cDBPatch::UpdateCompID(std::vector<CLUBS> &clubs, std::vector<CLUB_COMPS> &club_comps)
{
	// Check that the Club ID is valid before proceeding
	if(team_id_ >= static_cast<int>(clubs.size()))
	{
		std::cout << "UNKNOWN TEAM" << std::endl;
		error_flag_ = true;
	}
	else
		std::cout << ehm::ASCII((std::string) clubs[team_id_].ClubName, false) << std::endl;

	// Check that the Vanilla Comp ID is valid before proceeding
	if(comp_id_vanilla_ >= static_cast<int>(club_comps.size()))
	{
		std::cout << "ERROR";
		error_flag_ = true;
	}
	else
		std::cout << ehm::ASCII((std::string) club_comps[comp_id_vanilla_].ClubCompNameShort, false);

	// Display the appropriate arrow depending on whether the SuperPatch or vanilla settings are applied.
	if(apply_superpatch_)
		std::cout << " -> ";
	else
		std::cout << " <- ";

	// Check that the SuperPatch Comp ID is valid before proceeding
	if(comp_id_patched_ >= static_cast<int>(club_comps.size()))
	{
		std::cout << "ERROR";
		error_flag_ = true;
	}
	else
		std::cout << ehm::ASCII((std::string) club_comps[comp_id_patched_].ClubCompNameShort, false);

	// Apply the updated competition ID if no errors have been flagged
	if(!error_flag_)
	{
		if(apply_superpatch_ == true)
			clubs[team_id_].ClubDivision = comp_id_patched_;
		else
			clubs[team_id_].ClubDivision = comp_id_vanilla_;

		std::cout << std::endl << std::endl;
	}
	else
		std::cout << " (SKIPPED)" << std::endl << std::endl;
		
	return error_flag_;
}

// Apply the patch
bool cDBPatch::Apply(std::vector<cDBPatch> &db_patch, std::vector<CLUBS> &clubs, std::vector<CLUB_COMPS> &club_comps)
{
	// Load the database
	ehm::PrintHeading("LOADING DATABASE");
	if(ehm::ReadDB("club.dat", clubs) == false || ehm::ReadDB("club_comp.dat", club_comps) == false)
		return false;
		
	// Load the patch changes to be made
	ehm::PrintHeading("APPLYING CHANGES", true);
	cDBPatch::LoadSettings(db_patch);
	
	for(auto itr = db_patch.begin(); itr < db_patch.end(); ++itr)
		itr->UpdateCompID(clubs, club_comps);
		
	// Save the database (club.dat)
	ehm::PrintHeading("SAVING DATABASE");
	if(ehm::WriteDB("club.dat", clubs, true) == false)
		return false;

	return true;
}