#define _WIN32_WINNT _WIN32_WINNT_WINXP // Enables Windows XP compatibility
#define _CRT_SECURE_NO_WARNINGS			// Ignore the strcpy security warning
#pragma pack(1)
#include "lib_ehm07.h"

/* ========================== */
/*      BUFFER FUNCTIONS      */
/* ========================== */

// --- Populate award names string vector --- //
bool ehm::LoadAwardNames(std::vector<std::string> &award, const bool &lower_case)
{
	std::vector<STAFF_COMPS> buffer;
	std::string award_name;

	// Load the club comps data into the buffer vector
	if(ehm::ReadDB("staff_comp.dat", buffer) == false)
		return false;

	for(auto itr = buffer.begin(); itr < buffer.end(); ++itr)
	{
		award_name = ehm::ASCII((std::string)itr->StaffCompName, lower_case);
		award.push_back(award_name);
	}

	buffer.clear();

	return true;
}

// --- Populate city buffer vector --- //
bool ehm::LoadCityBuffer(std::vector<CITYBUFFER> &cities, const char filename[], const bool &lower_case)
{
	std::vector<CITIES> buffer;
	std::string city_name;

	// Load the city data into the buffer vector
	if(ehm::ReadDB("city.dat", buffer) == false)
		return false;

	for(auto itr = buffer.begin(); itr < buffer.end(); ++itr)
	{
		CITYBUFFER city;

		city.city = ehm::ASCII((std::string)itr->CityName, lower_case);
		city.nation_id = itr->CityNation;

		cities.push_back(city);
	}

	buffer.clear();

	return true;
}

// --- Populate club buffer vector --- //
bool ehm::LoadClubBuffer(std::vector<CLUBBUFFER> &clubs, const char filename[], const bool &lower_case)
{
	std::vector<CLUBS> buffer;
	CLUBBUFFER club_buffer;
	bool nat_flag = false;
	
	// Load the clubs data into the club vector
	if(ehm::ReadDB(filename, buffer) == false)
		return false;

	// Detect whether national teams are being loaded
	if((std::string)filename == "nat_club.dat")
		nat_flag = true;

	for(unsigned short i = 0; i < buffer.size(); ++i)
	{
		club_buffer.club_name = ehm::ASCII((std::string)buffer[i].ClubName, lower_case);
		club_buffer.division = buffer[i].ClubDivision;

		// If the club is a u20 team, add "u20" to the club name
		if(nat_flag == true && i <= 222)
			club_buffer.club_name += " u20";

		clubs.push_back(club_buffer);
	}

	buffer.clear();

	return true;
}

// --- Populate club names string vector --- //
bool ehm::LoadClubNames(std::vector<std::string> &clubs, const char filename[], const bool &lower_case)
{
	std::vector<CLUBS> buffer;
	std::string club_name;
	bool nat_flag = false;

	// Load the clubs data into the club vector
	if(ehm::ReadDB(filename, buffer) == false)
		return false;

	// Detect whether national teams are being loaded
	if((std::string)filename == "nat_club.dat")
		nat_flag = true;

	for(unsigned short i = 0; i < buffer.size(); ++i)
	{
		club_name = ehm::ASCII((std::string)buffer[i].ClubName, lower_case);

		// If the club is a u20 team, add "u20" to the club name
		if(nat_flag == true && i > 222)
			club_name += " u20";

		clubs.push_back(club_name);
	}

	buffer.clear();

	return true;
}

// --- Populate competition names string vector --- //
bool ehm::LoadCompNames(std::vector<std::string> &club_comps, const char filename[], const bool &lower_case)
{
	std::vector<CLUB_COMPS> buffer;
	std::string comp_name;

	// Load the club comps data into the buffer vector
	if(ehm::ReadDB(filename, buffer) == false)
		return false;

	for(auto itr = buffer.begin(); itr < buffer.end(); ++itr)
	{
		comp_name = ehm::ASCII((std::string)itr->ClubCompName, lower_case);
		club_comps.push_back(comp_name);
	}

	buffer.clear();

	return true;
}

// --- Populate nation names string vector --- //
bool ehm::LoadNationNames(std::vector<std::string> &nations, const char filename[], const bool &lower_case)
{
	std::vector<NATIONS> buffer;
	std::string nation_name;

	// Load the nation data into the buffer vector
	if(ehm::ReadDB(filename, buffer) == false)
		return false;

	for(auto itr = buffer.begin(); itr < buffer.end(); ++itr)
	{
		// The full length nation name is used
		nation_name = ehm::ASCII((std::string)itr->NationName, lower_case);
		nations.push_back(nation_name);
	}

	buffer.clear();

	return true;
}

// --- Populate staff buffer vector --- //
bool ehm::LoadStaffBuffer(std::vector<STAFFBUFFER> &staff, std::vector<std::string> &first_names, std::vector<std::string> &second_names, const bool &flush)
{
	std::vector<STAFF> buffer;
	STAFFBUFFER s;

	// Load the .dat file into the buffer
	if(ehm::ReadDB("staff.dat", buffer) == false)
		return false;

	for(auto itr = buffer.begin(); itr < buffer.end(); ++itr)
	{		
		if(itr->StaffFirstName >= VALID)
			s.first_name = first_names[itr->StaffFirstName];
		if(itr->StaffSecondName >= VALID)
			s.second_name = second_names[itr->StaffSecondName];

		s.first_name_id = itr->StaffFirstName;
		s.second_name_id = itr->StaffSecondName;
		s.player_data = itr->StaffPlayerData;
		s.date_of_birth.day = itr->StaffDateOfBirth.day;
		s.date_of_birth.year = itr->StaffDateOfBirth.year;
		s.date_of_birth.leap_year = itr->StaffDateOfBirth.leap_year;
		s.job_for_club = itr->StaffJobForClub;
		s.club_contracted = itr->StaffClubContracted;
		s.club_playing = itr->StaffClubPlaying;

		staff.push_back(s);
	}

	buffer.clear();

	// Clear the name buffers if they're no longer needed
	if(flush == true)
	{
		first_names.clear();
		second_names.clear();
	}

	return true;
}

// --- Populate staff names string buffer vectors --- //
bool ehm::LoadStaffNames(std::vector<std::string> &first_names, std::vector<std::string> &second_names, const bool &lower_case)
{
	const std::string file[] = { "first_names.dat", "second_names.dat" };
	std::vector<NAMES> buffer;
	std::string name;

	// Loop through each names file and add to the relevant vector
	for(short i = 0; i < 2; ++i)
	{
		// Assign file name
		char* filename = new char [file[i].length()+1];
		strcpy(filename, file[i].c_str());

		if(ehm::ReadDB(filename, buffer) == false)
			return false;		

		for(auto itr = buffer.begin(); itr < buffer.end(); ++itr)
		{
			name = ehm::ASCII((std::string)itr->Name, lower_case);

			// Add the name to correct buffer
			(i == 0) ? first_names.push_back(name) : second_names.push_back(name);
		}

		// Clear filename char array and vector buffer
		delete[] filename;
		buffer.clear();
	}
	
	return true;
}


/* ============================== */
/*      COMPARISON FUNCTIONS      */
/* ============================== */

// --- Compare cities --- //
long ehm::CompareCities(const std::string &city_name, const long &nation_id, const std::vector<CITYBUFFER> &cities)
{
	for(auto itr = cities.begin(); itr < cities.end(); ++itr)
		if(city_name == itr->city && nation_id == itr->nation_id)
			return static_cast<long>(itr - cities.begin());

	return NO_MATCH;
}

// --- Compare players and player/coaches --- //
long ehm::ComparePlayers(const std::string &first_name, const std::string &second_name, const short &day, const short &year, const std::vector<STAFFBUFFER> &staff)
{
	// This buffer is used to temporarily store a matched staff id if the matched staff is a non player
	long buffer_staff_id = NO_MATCH;

	for(auto itr = staff.begin(); itr < staff.end(); ++itr)
	{
		// If a match is found
		if(itr->first_name == first_name &&
			itr->second_name == second_name &&
			itr->date_of_birth.day == day &&
			itr->date_of_birth.year == year)
		{
			// If the staff is a player or player/coach then return the staff id
			if(itr->job_for_club == JOB_PLAYER || itr->job_for_club == JOB_PLAYER_COACH)
				return itr - staff.begin();
			// Otherwise store NON_PLAYER as the return value and continue checking for any matching active staff
			else
				buffer_staff_id = NON_PLAYER;
		}
	}

	// If no matching active staff, return NO_MATCH or NON_PLAYER as appropriate
	return buffer_staff_id;
}

// --- Compare staff --- //
long ehm::CompareStaff(const std::string &first_name, const std::string &second_name, const short &day, const short &year, const std::vector<STAFFBUFFER> &staff)
{
	// This buffer is used to temporarily store a matched staff id if the matched staff is not active
	long buffer_staff_id = NO_MATCH;

	for(auto itr = staff.begin(); itr < staff.end(); ++itr)
	{
		// If a match is found
		if(itr->first_name == first_name &&
			itr->second_name == second_name &&
			itr->date_of_birth.day == day &&
			itr->date_of_birth.year == year)
		{
			// If the staff is active (i.e. not invalid and not retired) then return the staff id
			if(itr->job_for_club != JOB_INVALID_JOB && itr->job_for_club != JOB_PLAYER_RETIRED)
				return itr - staff.begin();
			// If the staff is inactive, store the staff id and continue checking for any matching active staff
			else
				buffer_staff_id = itr - staff.begin();
		}
	}

	// If no matching active staff, return NO_MATCH or the inactive staff id as appropriate
	return buffer_staff_id;
}

// --- Compare any string (array) --- //
long ehm::CompareStrings(const std::string &target, const std::string names[], const unsigned int &size)
{
	// If the target name is _ufa or _none then return -1 (NONE) instead of proceeding to the for loop
	if(target == "_ufa" || target == "_none")
		return NONE;

	for(unsigned int i = 0; i < size; ++i)
	{
		// If a match is found, return the name id
		if (names[i] == target)
			return static_cast<long>(i);
	}

	return NO_MATCH;
}

// --- Compare any string (vector)--- //
long ehm::CompareStrings(const std::string &target, const std::vector<std::string> &names)
{
	// If the target name is _ufa or _none then return -1 (NONE) instead of proceeding to the for loop
	if(target == "_ufa" || target == "_none")
		return NONE;

	for(auto itr = names.begin(); itr < names.end(); ++itr)
	{
		// If a match is found, return the name id
		if (*itr == target)
			return itr - names.begin();
	}

	return NO_MATCH;
}

/* ======================= */
/*      CSV FUNCTIONS      */
/* ======================= */

// --- Create a new CSV for writing to --- //
bool ehm::CreateCSV(std::fstream &file, const char filename[])
{
	std::cout << std::setw(COL_PROCESS) << "Generating " + (std::string) filename << std::setw(COL_VALUES);

	// Create destination csv file
	file.open(filename, std::ios::out | std::ios::trunc);

	if(!file.is_open())
	{
		ehm::PrintError(filename);
		return false;
	}

	return true;
}

// --- Convert a file from one delimiter type to another --- //
bool ehm::DelimiterConvert(const char filename[], const char &d_out)
{
	// Determine what to delimiter to convert from
	const char d_in = (d_out == ',') ? ';' : ',';

	// Open the file
	std::cout << std::setw(COL_PROCESS) << filename << std::setw(COL_VALUES);
	std::fstream file;		
	file.open (filename, std::ios::in | std::ios::out | std::ios::binary);
	
	if(!file.is_open())
	{
		std::cout << "N/A" << std::setw(COL_STATUS) << "ERROR" << std::endl << "Unable to open " << filename << " for reading" << std::endl;
		return false;
	}

	// Calculate file size
	file.seekg (0, std::ios::end);
	int filesize = static_cast<int>(file.tellg());
	file.seekg (0, std::ios::beg);

	// Dynamically create an array to buffer the entire file to memory. The maximum size for a file is approx 2gb
	char *buffer = new (std::nothrow) char[filesize];
	if(buffer == 0)
	{
		std::cout << filesize << std::setw(COL_STATUS) << "ERROR" << std::endl << "Unable allocate memory for the file buffer" << std::endl;
		return false;
	}

	// Read the file to the buffer
	file.read(buffer, filesize);
	file.close();

	// Boolean flag to determine whether or not any delimiter changes need to be made to the file
	bool flag_modified = false;

	// Replace the d_in delimiter with d_out
	for(int i = 0; i < filesize; ++i)
		if(buffer[i] == d_in)
		{
			buffer[i] = d_out;
			flag_modified = true;
		}

	// Write the converted buffer back to the file if any delimiters were found
	if(flag_modified == true)
	{
		file.open (filename, std::fstream::out | std::fstream::trunc | std::ios::binary);

		if(!file.is_open())
		{
			ehm::PrintError(filename);
			return false;
		}

		file.write (buffer, filesize);
		file.close();

		std::cout << filesize << std::setw(COL_STATUS) << "DONE" << std::endl;
	}

	else
		std::cout << filesize << std::setw(COL_STATUS) << "SKIPPED" << std::endl;

	// Delete the buffer once finished
	delete [] buffer;

	

	return true;
}

// --- Detect delimiter based up a string of text --- //
char ehm::DelimiterDetect(const std::string &text)
{
	unsigned short count = 0;

	for(auto itr = text.begin(); itr < text.end(); ++itr)
		if(*itr == ';')
			++count;

	// If the number of semicolons is greater than zero, return the delimiter as semicolon; otherwise return it as comma
	return (count > 0) ? ';' : ','; 
}

// --- Determine default delimiter based upon system locale --- //
char ehm::DelimiterLocale()
{
	return (std::use_facet<std::numpunct<char>>(std::cout.getloc()).decimal_point() == '.') ? ',' : ';';
}

// --- Parse a string of delimited text (array method) --- //
void ehm::Parse(std::string string_buffer[], const int &max_element, const std::string &csv_string, const char &delimiter)
{
	std::stringstream csv_line(csv_string);

	// Parse each delimiter until the final delimiter
	for(int i = 0; i < (max_element-1); ++i)
		std::getline(csv_line, string_buffer[i], delimiter);

	// After the final delimiter, get the remaining text from the line
	getline(csv_line, string_buffer[max_element-1]);

	return;
}

// --- Parse a string of delimited text (vector method) --- //
void ehm::Parse(std::vector<std::string> &string_buffer, const short &columns, const std::string &csv_string, const char &delimiter)
{
	// Clear and resize the buffer before proceeding
	string_buffer.clear();
	string_buffer.resize(columns);

	// Stringstream the string of text
	std::stringstream csv_line(csv_string);

	// Parse each delimiter until the final delimiter
	for(int i = 0; i < columns; ++i)
		std::getline(csv_line, string_buffer[i], delimiter);

	// After the final delimiter, get the remaining text from the line
	getline(csv_line, string_buffer[columns-1]);

	return;
}

// --- Read a CSV delimited file into vector of strings --- //
char ehm::ReadCSV(const char filename[], std::vector<std::vector<std::string>> &vect, const short &columns, const short &header_rows, const bool &setting)
{
	std::cout << "Loading " + (std::string) filename << std::setw(COL_VALUES);

	std::fstream file (filename, std::ios::in);

	// Abort if the file could not be opened
	if(!file.is_open())
	{
		ehm::PrintError(filename);
		return ERROR;
	}

	// Calculate file size
	file.seekg (0, std::ios::end);
	std::streamoff filesize = file.tellg();
	file.seekg (0, std::ios::beg);

	// Buffers
	std::string csv_string;				// Line of text from the csv file
	std::vector<std::string> buffer;	// Buffer for storing the parsed data
	buffer.reserve(columns);			// Reserve the correct number of elements in the buffer according to the number columns of data
	char flag = SUCCESS;				// Flag used to return the setting or file open success (depending on whether the setting has been requested in the function call)
	char d	= ',';						// Delimiter type. Default as comma to start with. It can be changed later using DelimiterDetect().

	if(setting == false)
	{
		// Skip the two header rows
		for(short i = 0; i < header_rows; ++i)
			std::getline(file, csv_string);

		// Detect the d based upon the header row
		d = ehm::DelimiterDetect(csv_string);
	}
	else
	{
		// Get the first header row...
		std::getline(file, csv_string);

		// Detect the d based upon the header row
		d = ehm::DelimiterDetect(csv_string);

		// ...and check the first column for the setting text
		ehm::Parse(buffer, columns, csv_string, d);

		// Check for settings that will result in a TRUE flag
		if(!buffer[0].empty() && (ehm::StripText(buffer[0], true) == "imperial"))
			flag = TRUE;

		// Skip and remaining header rows
		for(short i = 1; i < header_rows; ++i)
			std::getline(file, csv_string);
	}

	while(file.tellg() < filesize)
	{
		// Get the line of csv and parse it
		std::getline(file, csv_string);
		ehm::Parse(buffer, columns, csv_string, d);

		// Only add the data to the vector if both the first two columns of data are present (i.e. not empty)
		if(!buffer[0].empty() && !buffer[1].empty())
			vect.push_back(buffer);
	}

	file.close();

	std::cout << vect.size() << std::setw(COL_STATUS) << "DONE" << std::endl;

	return flag;
}


/* ================================ */
/*      DATABASE I/O FUNCTIONS      */
/* ================================ */

// --- Open a database .dat file for writing --- //
bool ehm::OpenDB(std::fstream &file, const char filename[], const bool &truncate, const bool &silent)
{
	if(silent == false)
		std::cout << std::setw(COL_PROCESS) << "Updating " + (std::string) filename << std::setw(COL_VALUES);

	// Truncate and write
	if(truncate == true)
		file.open(filename, std::fstream::out | std::fstream::trunc | std::ios::binary);

	// Append and write
	else
		file.open(filename,  std::ios::in | std::fstream::out | std::fstream::ate | std::ios::binary);

	// Abort if the file cannot be opened
	if(!file.is_open())
	{
		ehm::PrintError(filename);
		return false;
	}

	return true;
}

// --- Load a database .dat file and populate a vector of the relevant struct
template <typename T> bool ehm::ReadDB(const char filename[], std::vector<T> &vect)
{
	std::cout << std::setw(COL_PROCESS) << "Loading " + (std::string) filename << std::setw(COL_VALUES);

	std::fstream file (filename, std::ios::in | std::ios::binary);

	// Calculate file size
	file.seekg (0, std::ios::end);
	std::streamoff filesize = file.tellg();
	file.seekg (0, std::ios::beg);

	// Struct buffer
	T struct_buffer;

	// Abort if the file cannot be opened
	if(!file.is_open())
	{
		ehm::PrintError(filename);
		return false;
	}

	// Add the data to the vector of structs
	while(file.tellg() < filesize)
	{
		file.read((char*)&struct_buffer, sizeof(T));
		vect.push_back(struct_buffer);
	}

	file.close();

	std::cout << vect.size() << std::setw(COL_STATUS) << "DONE" << std::endl;

	return true;
}

// --- Open a database .dat file for writing and write a vector of structs to the file --- //
template <typename T> bool ehm::WriteDB(const char filename[], const std::vector<T> &vect, const bool &truncate)
{
	// Open the file
	std::fstream file;

	if(ehm::OpenDB(file, filename, truncate) == false)
		return false;

	// Write the vector of structs
	for(unsigned long i = 0; i < vect.size(); ++i)
		file.write((char*)&vect[i], sizeof(T));

	file.close();

	std::cout << vect.size() << std::setw(COL_STATUS) << "DONE" << std::endl;

	return true;
}

/* ======================== */
/*      DATE FUNCTIONS      */
/* ======================== */

// --- Convert a standard date to an EHM date --- //
short ehm::EHMDate(short &day, short &month, const bool &leap_year)
{
	short month_lengths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if(leap_year == true)
		month_lengths[1] = 29;

	short date = 0;

	// Prevent any dates out of range prior to processing.
	if(day < 1) day = 1;
	else if(day > 31) day = 31;

	if(month < 1) month = 1;
	else if(month > 12) month = 12;

	for(short i = 0; i < (month-1); ++i) date += month_lengths[i];

	// The first day of the year is zero. Therefore you must subtract one from the date.
	day += (date - 1);

	// Prevent any dates out of range after processing.
	if(day < 0) day = 0;
	else if(day > 365) day = 365;

	return day;
}

// --- Check whether a given year is a leap year --- //
bool ehm::LeapYearCheck(const short &year)
{
	return((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

// --- Convert an EHM date to a standard date. ehm_day is passed by value because temporary changes must be made to it during the calculation process --- //
void ehm::StandardDate(short ehm_day, short &day, short &month, const bool &leap_year)
{
	short month_length[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	short month_days = 0;
	month = 0;

	if(leap_year == true) month_length[1] = 29;

	// The first SI_DATE day of the year is zero. Therefore you must add one to the si_days before conversion.
	++ehm_day;

	while(ehm_day > month_days) month_days += month_length[(month)++];

	day = ehm_day - month_days + month_length[(month)-1];

	// Prevent any dates out of range (well, at least the day won't exceed 31).
	if(day < 0) day = 0;
	else if(day > 31) day = 31;

	if(month < 1) month = 1;
	else if(month > 12) month = 12;

	return;
}


/* =========================== */
/*      GENERAL FUNCTIONS      */
/* =========================== */

// -- Calculate number of entries in a database file for updating index.dat --- //
long ehm::IndexSize(char filename[], unsigned int structure)
{
	std::fstream file (filename, std::ios::in | std::ios::binary);

	if(!file.is_open())
		return -1;

	file.seekg (0, std::ios::end);
	return static_cast<unsigned int>(file.tellg()) / structure;
}

// -- Update index.dat --- //
bool ehm::IndexUpdate()
{
	// Array to store index values for each database file
	long index[35];

	std::cout << std::setw(COL_PROCESS) << "Updating index.dat" << std::setw(COL_VALUES);

	// Calculate index values for each database file
	index[0] = ehm::IndexSize("club.dat", sizeof(CLUBS));
	index[1] = ehm::IndexSize("nat_club.dat", sizeof(CLUBS));
	index[2] = ehm::IndexSize("colour.dat", sizeof(COLOURS));
	index[3] = ehm::IndexSize("continent.dat", sizeof(CONTINENTS));
	index[4] = ehm::IndexSize("nation.dat", sizeof(NATIONS));
	index[5] = ehm::IndexSize("officials.dat", sizeof(OFFICIALS));
	index[6] = ehm::IndexSize("stadium.dat", sizeof(ARENAS));
	index[7] = ehm::IndexSize("staff.dat", sizeof(STAFF));
	index[8] = ehm::IndexSize("nonplayer.dat", sizeof(NON_PLAYERS));
	index[9] = ehm::IndexSize("player.dat", sizeof(PLAYERS));
	index[10] = ehm::IndexSize("staff_comp.dat", sizeof(STAFF_COMPS));
	index[11] = ehm::IndexSize("city.dat", sizeof(CITIES));
	index[12] = ehm::IndexSize("club_comp.dat", sizeof(CLUB_COMPS));
	index[13] = ehm::IndexSize("nation_comp.dat", sizeof(CLUB_COMPS));
	index[14] = ehm::IndexSize("first_names.dat", sizeof(NAMES));
	index[15] = ehm::IndexSize("second_names.dat", sizeof(NAMES));
	index[16] = ehm::IndexSize("staff_history.dat", sizeof(STAFF_HISTORIES));
	index[17] = ehm::IndexSize("staff_comp_history.dat", sizeof(STAFF_COMP_HISTORIES));
	index[18] = ehm::IndexSize("club_comp_history.dat", sizeof(CLUB_COMP_HISTORIES));
	index[19] = ehm::IndexSize("nation_comp_history.dat", sizeof(CLUB_COMP_HISTORIES));
	index[20] = ehm::IndexSize("affiliations.dat", sizeof(AFFILIATIONS));
	index[21] = ehm::IndexSize("retired_numbers.dat", sizeof(RETIRED_NUMBERS));
	index[22] = ehm::IndexSize("states_provinces.dat", sizeof(STATES_PROVINCES));
	index[23] = ehm::IndexSize("injuries.dat", sizeof(INJURIES));
	index[24] = ehm::IndexSize("staff_preferences.dat", sizeof(STAFF_PREFERENCES));
	index[25] = ehm::IndexSize("currencies.dat", sizeof(CURRENCIES));
	index[26] = ehm::IndexSize("club_records.dat", sizeof(DB_CLUB_RECORDS));
	index[27] = ehm::IndexSize("club_histories.dat", sizeof(CLUB_HISTORIES));
	index[28] = ehm::IndexSize("drafts.dat", sizeof(DRAFTS));
	index[29] = ehm::IndexSize("drafted_players.dat", sizeof(DRAFTED_PLAYERS));
	index[30] = ehm::IndexSize("player_rights.dat", sizeof(DB_PLAYER_RIGHTS));
	index[31] = ehm::IndexSize("stage_names.dat", sizeof(STAGE_NAMES));
	index[32] = ehm::IndexSize("staff_languages.dat", sizeof(STAFF_LANGUAGES));
	index[33] = ehm::IndexSize("player_info.dat", sizeof(DB_PLAYER_INFO));
	index[34] = ehm::IndexSize("staff_info.dat", sizeof(DB_STAFF_INFO));

	std::cout << "N/A" << std::setw(COL_STATUS);

	// Check for any errors returned from the IndexSize function (and abort before writing to index.dat)
	for(short i = 0; i < 35; ++i)
	{
		if(index[i] < 0)
		{
			std::cout << "ERROR" << std::endl << "Unable to open all .dat files" << std::endl;
			return false;
		}
	}

	// Write to index.dat
	std::fstream file_index ("index.dat", std::ios::in | std::ios::out | std::ios::binary);

	if(!file_index.is_open())
	{
		ehm::PrintError("index.dat");
		return false;
	}

	// File position marker
	std::streamoff pos = 63;

	for(short i = 0; i < 35; ++i)
	{
		file_index.seekp (pos, std::ios::beg);
		file_index.write ((char*)&index[i], sizeof(index[i]));
		pos += 63;
	}

	file_index.close();

	std::cout << "DONE" << std::endl;

	return true;
}

// --- Print a file open error message using the standard column layout --- //
void ehm::PrintError(const char filename[])
{
	std::cout << "N/A" << std::setw(COL_STATUS) << "ERROR" << std::endl << "Unable to open " + (std::string) filename << std::endl;
	return;
}

// --- Print heading for listing progress details --- //
void ehm::PrintHeading(const char title[], const bool &summary)
{
	std::cout << std::endl << std::endl
			  << title << std::endl;

	// Print underline text
	for(unsigned short i = 0; i < strlen(title); ++i)
		std::cout << "=";
	
	std::cout << std::endl << std::endl;

	// Only print the column heading if the heading is a main title rather than just a summary
	if(summary == false)
	{
		std::cout << std::setw(COL_PROCESS) << "PROCESS" << std::setw(COL_VALUES) << "RECORDS" << std::setw(COL_STATUS) << "STATUS" << std::endl
				  << std::setw(COL_PROCESS);
	}

	return;
}

/* ======================== */
/*      TEXT FUNCTIONS      */
/* ======================== */

// --- Convert SI Font text to ASCII text. The bLowerCase bool enables conversion to lower case --- //
std::string ehm::ASCII(std::string &ehm_text, const bool &lower_case)
{
	// Don't process the string if it is empty
	if(ehm_text.empty())
		return ehm_text;

	// Multidimensional array of SI font characters ([i][0]) and ASCII characters ([i][1])
	const int font[][2] =
	{		
		{ '\xBC', '\x43' },	// C - Caron - Uppercase
		{ '\xA0', '\x63' },	// c - Caron - Lowercase
		{ '\xBE', '\x44' },	// D - Caron - Uppercase
		{ '\xBB', '\x64' },	// d - Caron - Lowercase
		{ '\x86', '\x45' },	// E - Caron - Uppercase
		{ '\x90', '\x65' }, // e - Caron - Lowercase
		{ '\xB1', '\x4E' },	// N - Caron - Uppercase
		{ '\x8F', '\x6E' },	// n - Caron - Lowercase
		{ '\xB3', '\x52' },	// R - Caron - Uppercase
		{ '\xA7', '\x72' },	// r - Caron - Lowercase		
		{ '\x8A', '\x53' },	// S - Caron - Uppercase
		{ '\xBD', '\x53' },	// S - Caron - Uppercase
		{ '\x9A', '\x73' },	// s - Caron - Lowercase
		{ '\x87', '\x55' },	// U - Ring Above - Uppercase
		{ '\x9E', '\x7A' },	// z - Caron - Lowercase
		{ '\x9F', '\x7A' },	// z - Caron - Lowercase		
		{ '\x8E', '\x5A' },	// Z - Caron - Uppercase		
		{ ',', '_'},		// Comma delimiter
		{ ';', '_'}			// Semicolon delimiter
	};

	short font_size = sizeof(font)/sizeof(*font);

	// Check for SI Font characters
	for(short i = 0; i < font_size; ++i)
		replace(ehm_text.begin(), ehm_text.end(), font[i][0], font[i][1]);

	// Remove trademark symbol if present at the end of the string
	if(ehm_text[ehm_text.length()-1] == '\x99')
		ehm_text.erase(ehm_text.length()-1);

	// Convert the text to lower case
	if(lower_case == true)
		ehm::LowerCase(ehm_text);

	return ehm_text;
}

// --- Convert text to lowercase --- //
void ehm::LowerCase(std::string &text)
{
	// Multidimensional array of uppercase ([i][0]) to lowercase characters ([i][1])
	const int font[][2] =
	{
		{ '\xC0', '\xE0' },	// A - Grave - Uppercase
		{ '\xC1', '\xE1' },	// A - Acute - Uppercase
		{ '\xC2', '\xE2' },	// A - Circumflex - Uppercase
		{ '\xC3', '\xE3' },	// A - Tilde - Uppercase
		{ '\xC4', '\xE4' },	// A - Umlauts - Uppercase
		{ '\xC5', '\xE5' },	// A - Ring Above - Uppercase
		{ '\xC6', '\xE6' },	// AE - Joined - Uppercase
		{ '\xC7', '\xE7' },	// C - Cedilla - Uppercase
		{ '\xC8', '\xE8' },	// E - Grave - Uppercase
		{ '\xC9', '\xE9' },	// E - Acute - Uppercase
		{ '\xCA', '\xEA' },	// E - Circumflex - Uppercase
		{ '\xCB', '\xEB' },	// E - Umlauts - Uppercase
		{ '\xCC', '\xEC' },	// I - Grave - Uppercase
		{ '\xCD', '\xED' },	// I - Acute - Uppercase
		{ '\xCE', '\xEE' },	// I - Circumflex - Uppercase
		{ '\xCF', '\xEF' },	// I - Umlauts - Uppercase
		{ '\xD1', '\xF1' },	// N - Tilde - Uppercase
		{ '\xD2', '\xF2' },	// O - Grave - Uppercase
		{ '\xD3', '\xF3' },	// O - Acute - Uppercase
		{ '\xD4', '\xF4' },	// O - Circumflex - Uppercase
		{ '\xD5', '\xF5' },	// O - Tilde - Uppercase
		{ '\xD6', '\xF6' },	// O - Umlauts - Uppercase
		{ '\xD8', '\xF8' },	// O - Stroke - Uppercase
		{ '\xD9', '\xF9' },	// U - Grave - Uppercase
		{ '\xDA', '\xFA' },	// U - Acute - Uppercase
		{ '\xDB', '\xFB' },	// U - Circumflex - Uppercase
		{ '\xDC', '\xFC' },	// U - Umlauts - Uppercase
		{ '\xDD', '\xFD' },	// Y - Acute - Uppercase
		{ '\x9F', '\xFF' },	// Y - Umlauts - Uppercase
		{ '\xDE', '\xFE' }	// ? - Thorn - Uppercase
	};

	short font_size = sizeof(font)/sizeof(*font);

	// Convert text to lowercase
	int(*pf)(int)=tolower;
	transform(text.begin(), text.end(), text.begin(), pf);

	// Convert additional accented characters to lowercase
	for(short i = 0; i < font_size; ++i)
		replace(text.begin(), text.end(), font[i][0], font[i][1]);

	return;
}

// --- Strip ASCII text by converting carons and removing trailing 'TM' symbols. The lower_case bool enables conversion to lower case --- //
std::string ehm::StripText(std::string &text, const bool &lower_case)
{
	// Don't process the string if it is empty
	if(text.empty())
		return text;

	// Before proceeding, check for and remove any text qualifiers. Some software encloses text within speech marks or apostrophes in csv files
	if((text[0] == '\"' && text[text.length()-1] == '\"') || (text[0] == '\'' && text[text.length()-1] == '\''))
	{
		text.erase(text.begin());
		text.erase(text.length()-1);
	}

	// Multidimensional array of strip characters ([i][0]) and replace characters ([i][1])
	const int font[][2] =
	{		
		{ '\x8A', '\x53' },	// S - Caron - Uppercase
		{ '\x9A', '\x73' },	// s - Caron - Lowercase
		{ '\x8E', '\x5A' },	// Z - Caron - Uppercase
		{ '\x9E', '\x7A' }	// z - Caron - Lowercase
	};

	short font_size = sizeof(font)/sizeof(*font);

	// Check for bad characters
	for(short i = 0; i < font_size; ++i)
		replace(text.begin(), text.end(), font[i][0], font[i][1]);

	// Remove trademark symbol if present at the end of the string
	if(text[text.length()-1] == '\x99')
		text.erase(text.length()-1);

	// Convert the text to lower case
	if(lower_case == true)
		ehm::LowerCase(text);

	// If the text string consists of solely a space then erase it. Single space strings are likely to be an artefact from text qualified cells that were subsequently blanked in the spreadsheet software.
	if(text == " ")
		text.clear();

	return text;
}