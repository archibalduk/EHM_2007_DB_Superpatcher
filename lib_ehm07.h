#ifndef LIB_EHM07_H
#define LIB_EHM07_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <new>
#include <sstream>
#include <string>
#include <vector>

#include "database_flags.h"
#include "database_types.h"

// Enumerator to represent various data states
enum ENUM_EHM_FLAGS
{
	ERROR = -127,		// Error code for when returning an integer/short from a function, etc
	NO_MATCH,			// Data cannot be matched when comparing with existing data
	BLANK,				// Blank data field
	NON_PLAYER,			// Data should not be processed because the staff is not a player
	INACTIVE,			// Staff is either invalid or retired
	NONE = -1,			// No pointer set - e.g. where no competition has been assigned to a team or player career history entry
	UFA = -1,			// Free agent club_ptrs should be regarded as -1 in the database
	TRUE = -1,			// EHM uses -1 to represent bool true for SOME data fields
	NO_PTR = 0,			/* The staff awards table doesn't display unlinked players with an ID of -1
						   (this might be a bug with EHM because it seems as though an ID of -1 was intended - it certainly ought to have been).
						   Therefore we'll use an ID of 0 in order to ensure they appear. */
	VALID = 0,			// Any valid ID/ptr in the database should be zero or higher
	SUCCESS = 1			// Indicates that a file has been loaded successfully
};

// Column widths for the CLI display
enum ENUM_COLUMN_WIDTHS
{
	COL_PROCESS = 45,	// Left-most column showing the process currently running
	COL_VALUES = 12,	// Middle column showing how many records, etc were loaded/processed
	COL_STATUS = 8,		// Right-most column showing the outcome/status of the process
	COL_MENU = 40		// Left-hand column for the main menu in order to allow a dual-column display
};

// City buffer struct
struct CITYBUFFER
{
	std::string	city;
	long		nation_id;
};

// Club buffer struct
struct CLUBBUFFER
{
	std::string club_name;
	long		division;
};

// Staff buffer struct
struct STAFFBUFFER
{
	long		player_data;
	std::string	first_name;
	std::string	second_name;
	long		first_name_id;
	long		second_name_id;
	SI_DATE		date_of_birth;
	char		job_for_club;
	long		club_contracted;
	long		club_playing;
};

// ehm:: namespace is used for all functions below
namespace ehm
{
	// Buffer Functions
	bool LoadAwardNames(std::vector<std::string> &, const bool & = true);
	bool LoadCityBuffer(std::vector<CITYBUFFER> &, const char [], const bool & = true);
	bool LoadClubBuffer(std::vector<CLUBBUFFER> &, const char [], const bool & = true);
	bool LoadClubNames(std::vector<std::string> &, const char [], const bool & = true);
	bool LoadCompNames(std::vector<std::string> &, const char [], const bool & = true);
	bool LoadNationNames(std::vector<std::string> &, const char [], const bool & = true);
	bool LoadStaffBuffer(std::vector<STAFFBUFFER> &, std::vector<std::string> &, std::vector<std::string> &, const bool & = true);
	bool LoadStaffNames(std::vector<std::string> &, std::vector<std::string> &, const bool & = true);

	// Comparision Functions
	long CompareCities(const std::string &, const long &, const std::vector<CITYBUFFER> &);
	long ComparePlayers(const std::string &, const std::string &, const short &, const short &, const std::vector<STAFFBUFFER> &);
	long CompareStaff(const std::string &, const std::string &, const short &, const short &, const std::vector<STAFFBUFFER> &);
	long CompareStrings(const std::string &, const std::string[], const unsigned int &);
	long CompareStrings(const std::string &, const std::vector<std::string> &);

	// CSV Functions
	bool CreateCSV(std::fstream &, const char []);
	bool DelimiterConvert(const char [], const char &);
	char DelimiterDetect(const std::string &);
	char DelimiterLocale();
	void Parse(std::string[], const int &, const std::string &, const char &);
	void Parse(std::vector<std::string> &, const short &, const std::string &, const char &);
	char ReadCSV(const char [], std::vector<std::vector<std::string>> &, const short &, const short &, const bool & = false);

	// Database I/O Functions
	bool OpenDB(std::fstream &, const char [], const bool & = false, const bool & = false);
	template <typename T> bool ReadDB(const char [], std::vector<T> &);
	template <typename T> bool WriteDB(const char [], const std::vector<T> &, const bool & = false);

	// Date Functions
	short EHMDate(short &, short &, const bool &);
	bool LeapYearCheck(const short &);
	void StandardDate(short, short &, short &, const bool &);

	// General Functions
	long IndexSize(char [], unsigned int);
	bool IndexUpdate();
	void PrintError(const char []);
	void PrintHeading(const char [], const bool & = false);

	// Text Functions
	std::string ASCII(std::string &, const bool & = true);
	void LowerCase(std::string &);
	std::string StripText(std::string &, const bool & = true);
}


/* ============================================================== */
/*      DATABASE I/O FUNCTIONS (IMPLEMENTATION OF TEMPLATES)      */
/* ============================================================== */

// Load a database .dat file for reading into a vector of structs
template bool ehm::ReadDB(const char [], std::vector<CITIES> &);
template bool ehm::ReadDB(const char [], std::vector<CLUBS> &);
template bool ehm::ReadDB(const char [], std::vector<CLUB_COMP_HISTORIES> &);
template bool ehm::ReadDB(const char [], std::vector<CLUB_COMPS> &);
template bool ehm::ReadDB(const char [], std::vector<DB_CLUB_RECORDS> &);
template bool ehm::ReadDB(const char [], std::vector<DB_PLAYER_INFO> &);
template bool ehm::ReadDB(const char [], std::vector<DB_PLAYER_RIGHTS> &);
template bool ehm::ReadDB(const char [], std::vector<DB_STAFF_INFO> &);
template bool ehm::ReadDB(const char [], std::vector<DRAFTED_PLAYERS> &);
template bool ehm::ReadDB(const char [], std::vector<NAMES> &);
template bool ehm::ReadDB(const char [], std::vector<NATIONS> &);
template bool ehm::ReadDB(const char [], std::vector<NON_PLAYERS> &);
template bool ehm::ReadDB(const char [], std::vector<OFFICIALS> &);
template bool ehm::ReadDB(const char [], std::vector<PLAYERS> &);
template bool ehm::ReadDB(const char [], std::vector<STAFF> &);
template bool ehm::ReadDB(const char [], std::vector<STAFF_COMPS> &);
template bool ehm::ReadDB(const char [], std::vector<STAFF_COMP_HISTORIES> &);
template bool ehm::ReadDB(const char [], std::vector<STAFF_HISTORIES> &);
template bool ehm::ReadDB(const char [], std::vector<STAFF_LANGUAGES> &);
template bool ehm::ReadDB(const char [], std::vector<STAFF_PREFERENCES> &);

// Write the vector of structs to a database .dat file
template bool ehm::WriteDB(const char [], const std::vector<CLUBS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<CLUB_COMP_HISTORIES> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<DB_CLUB_RECORDS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<DB_PLAYER_INFO> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<DB_PLAYER_RIGHTS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<DB_STAFF_INFO> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<DRAFTED_PLAYERS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<NAMES> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<NATIONS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<NON_PLAYERS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<OFFICIALS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<PLAYERS> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<STAFF> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<STAFF_COMP_HISTORIES> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<STAFF_HISTORIES> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<STAFF_LANGUAGES> &, const bool &);
template bool ehm::WriteDB(const char [], const std::vector<STAFF_PREFERENCES> &, const bool &);

#endif