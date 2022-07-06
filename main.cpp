#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;

bool strCanBeInt(string string){
    for (int i=0;i<string.length();i++) {
    	char n = string[i];
        if (n != '0' && n != '1' && n != '2' && n != '3' && n != '4' && n != '5'
            && n != '6' && n != '7' && n != '8' && n != '9') {
            return false;
        }
    }
    return true;
}

int strToInt(string string) {
    int integer = 0;
    int numInt;
    for (int i=0;i<string.length();i++) {
    	char n = string[i];
        if(n == '0') numInt = 0;
        if(n == '1') numInt = 1;
        if(n == '2') numInt = 2;
        if(n == '3') numInt = 3;
        if(n == '4') numInt = 4;
        if(n == '5') numInt = 5;
        if(n == '6') numInt = 6;
        if(n == '7') numInt = 7;
        if(n == '8') numInt = 8;
        if(n == '9') numInt = 9;
        if (integer){
            integer *= 10;
        }
        integer += numInt;
    }
    return integer;
}

string trim(string& str)
{
    str.erase(str.find_last_not_of(' ')+1);         //suffixing spaces
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    return str;
}

string toUpper(const string & s){
    string ret(s.size(), char());
    for(unsigned int i = 0; i < s.size(); ++i)
        ret[i] = (s[i] <= 'z' && s[i] >= 'a') ? s[i]-('a'-'A') : s[i];
    return ret;
}

string toLower(const string & s){
    string ret(s.size(), char());
    for(unsigned int i = 0; i < s.size(); ++i)
        ret[i] = (s[i] >= 'A' && s[i] <= 'Z') ? s[i]-('A'-'a') : s[i];
    return ret;
}


string getTime(){
 time_t _tm =time(NULL );

struct tm * curtime = localtime ( &_tm );

  return asctime(curtime);
}

bool startsWith(string first, string second){
    first = toLower(first);
    second = toLower(second);
	bool value = first.length() >= second.length();
	for(int i=0; i < second.length(); i++){
//		cout << first[i] << " " << second[i] << " " << i << " " << (first[i] == second[i]) << endl;
		if(first[i] != second[i]){
			value = 0;
		}
	}
//	cout << value << " " << first << " " << second << endl;
	return value;
}

void helpMenu(){
	cout << endl << "===========================================================================================" << endl << endl;
	cout << "*                                                  HELP MENU" << endl << endl;
	cout << endl << "===========================================================================================" << endl << endl;
	
	cout << "add <Location>                                       : Add new location" << endl;
	cout << "delete <Location>                                    : Delete an existing location" << endl;
	cout << "record <Location> <disease> <cases>                  : Record a disease and its cases" << endl;
	cout << "list locations                                       : List all existing locations" << endl;
	cout << "list diseases                                        : List Existing diseases in locations" << endl;
	cout << "where disease                                        : Find where disease exists" << endl;
	cout << "cases <location><disease>                            : Find cases of a disease in location" << endl;
	cout << "cases <disease>                                      : Find total cases of a given disease" << endl;
	cout << "help                                                 : Prints user manual" << endl;
	cout << "Exit                                                 : Exit the program" << endl;
}

string getCommandPostfix(string userInput, string command){
	string data = "";
	for(int i=0; i < userInput.length(); i++){
		if(i > command.length() -1){
			data += userInput[i];
		}
	}
	return data;
}

bool isRecordinput(string userInput){
	if(!startsWith(userInput, "record ")){
		return 0;
	}
	
	string parameters = getCommandPostfix(userInput, "record ");
	string locationParam = "";
	string diseaseParam = "";
	string casesParam = "";
	int j= 1;
	for(int i=0; i < parameters.length(); i++){
		if(parameters[i] == ' '){
			j++;
		}else {
			if(j == 1) locationParam += parameters[i];
			if(j == 2) diseaseParam += parameters[i];
			if(j == 3) casesParam += parameters[i];
		}
	}
	
	return locationParam != "" && diseaseParam != "" && casesParam != "";
}

void recordCases(string userInput){
	string parameters = getCommandPostfix(userInput, "record ");
	string locationParam = "";
	string diseaseParam = "";
	string casesParam;
	int j= 1;
	for(int i=0; i < parameters.length(); i++){
		if(parameters[i] == ' '){
			j++;
		}else {
			if(j == 1) locationParam += parameters[i];
			if(j == 2) diseaseParam += parameters[i];
			if(j == 3) casesParam += parameters[i];
		}
	}
	
	fstream data;
	string lctn;
	bool found = 0;
	data.open("locations.txt", ios::in);
	while(data >> lctn){
		if(toLower(lctn) == toLower(locationParam)){
			found = 1;
		}
	}
	data.close();
	
	if(found == 0){
		cout << "Location with name '" << locationParam << "' not found in the system." << endl;
		return;
	}

    if(!strCanBeInt(casesParam)){
        cout << "Cases must be a number" << endl;
        return;
    }

    bool updated = 0;

    fstream data2;
    string tempdisease, templocation;
    int tempcases;
    data.open("cases.txt", ios::in);
    data2.open("temp_cases.txt", ios::app | ios::in);
    while(data >> tempcases >> tempdisease >> templocation){
        if(toLower(templocation) == toLower(locationParam) && toLower(tempdisease) == toLower(diseaseParam)){
            tempcases = tempcases + strToInt(casesParam);
            updated = 1;
        }
        data2 << tempcases << " " << tempdisease << " " << templocation << endl;
    }
    data2.close();
    data.close();

    remove("cases.txt");
    rename("temp_cases.txt", "cases.txt");

    if(updated == 1) return;

	data.open("cases.txt", ios::app | ios::in);
	data << strToInt(casesParam) << " " << diseaseParam << " " << locationParam << endl;
	data.close();
}

bool isCasesinput(string userInput){
	if(!startsWith(userInput, "cases ")){
		return 0;
	}
	
	string parameters = getCommandPostfix(userInput, "cases ");
	string firstParam = "";
	string secondParam = "";
	string thirdParam = "";
	int j= 1;
	for(int i=0; i < parameters.length(); i++){
		if(parameters[i] == ' '){
			j++;
		}else {
			if(j == 1) firstParam += parameters[i];
			if(j == 2) secondParam += parameters[i];
			if(j == 3) thirdParam += parameters[i];
		}
	}
	
	return thirdParam == "" && firstParam != "";
}

void casesPreview(string userInput){
	string parameters = getCommandPostfix(userInput, "cases ");
	string firstParam = "";
	string secondParam = "";
	string thirdParam = "";
	int j= 1;
	for(int i=0; i < parameters.length(); i++){
		if(parameters[i] == ' '){
			j++;
		}else {
			if(j == 1) firstParam += parameters[i];
			if(j == 2) secondParam += parameters[i];
			if(j == 3) thirdParam += parameters[i];
		}
	}
	
	string locationParam = secondParam != "" ? firstParam : "";
	string diseaseParam = secondParam == "" ? firstParam : secondParam;
	
	fstream data;
	
	string lctn;
	bool found = 0;
	data.open("locations.txt", ios::in);
	while(data >> lctn){
		if(toLower(lctn) == toLower(locationParam)){
			found = 1;
		}
	}
	data.close();
	
	if(found == 0 && locationParam != ""){
		cout << "Location with name '" << locationParam << "' not found in the system." << endl;
		return;
	}
	
	int total = 0;
	
	string dsease, lctnname;
	int cses;
	
	data.open("cases.txt", ios::in);
	while(data >> cses >> dsease >> lctnname){
		bool isLocationSupported = locationParam == "" ? 1 : toLower(locationParam) == toLower(lctnname);
		if(isLocationSupported && toLower(diseaseParam) == toLower(dsease)){
			total += cses;
		}
	}
	data.close();
	if(locationParam == ""){
		cout << "Total cases of '" << toUpper(diseaseParam) << "' = " << total << endl;
	}else{
		cout << "Cases of " << toUpper(diseaseParam) << " at " << toUpper(locationParam) << " are: " << total << endl;
	}
}

void localizeDisease(string userInput){
	string diseaseName = getCommandPostfix(userInput, "where ");
	fstream data;
	string dsease, lctnname;
	int cases;
	bool found = 0;
	
	data.open("cases.txt", ios::in);
	
	while(data >> cases >> dsease >> lctnname){
		if(toLower(dsease) == toLower(diseaseName)){
			cout << "[" << toLower(lctnname) << "]" << endl;
			found = 1;
		}
	}
	if(found == 0){
		cout << "No location with this disease" << endl;
	}
}

void listDiseases(){
	fstream data;
	string dsease, lctnname;
	int cses;
	
	vector<string> diseases;
	
	data.open("cases.txt", ios::in);
	int found = 0;
	while(data >> cses >> dsease >> lctnname){
		diseases.push_back(toUpper(dsease));
		found = 1;
	}
	data.close();
    sort( diseases.begin(), diseases.end() );
	diseases.erase( unique( diseases.begin(), diseases.end() ), diseases.end() );
	for(int i=0; i< diseases.size(); i++){
		cout << diseases[i] << endl;
	}
	if(found == 0){
		cout << "There are no diseases registered for cases in the system." << endl;
	}
}

void addLocation(string userInput){
	string locationName = getCommandPostfix(userInput, "add ");
	fstream data;
	string lctn;            
	
	bool found = 0;
	data.open("locations.txt", ios::in);
	while(data >> lctn){
		if(toLower(locationName) == toLower(lctn)){
			cout << "Location " << toUpper(lctn) << " Already exists!" << endl;
			found = 1;
			break;
		}
	}
	data.close();
	
	if(found == 1) return;
	
	data.open("locations.txt", ios::app);
	data << locationName << endl;
	data.close();
	cout << "Location " << toUpper(locationName) << " is Successfully added!" << endl;
}

void deleteLocation(string userInput){
	string locationName = getCommandPostfix(userInput, "delete ");
	fstream data1;
	fstream data2;
	bool found = 0;
	
	string lctn;
	
	data1.open("locations.txt", ios::in);
	data2.open("temp_locations.txt", ios::app | ios::in);
	while(data1 >> lctn){
		if(toLower(lctn) == toLower(locationName)){
			found = 1;
	        cout << "Location " << toUpper(locationName) << " is Successfully deleted!" << endl;
		} else {
			data2 << lctn << endl;
		}
	}
	data1.close();
	data2.close();
	
	string dsease, lctnname;
	int css;
	
	data1.open("cases.txt", ios::in);
	data2.open("temp_cases.txt", ios::app | ios::in);
	
	while(data1 >> css >> dsease >> lctnname){
		if(toLower(lctnname) != toLower(locationName)){
			data2 << css << " " << dsease << " " << lctnname << endl;
		}
	}
	data1.close();
	data2.close();
	
	remove("locations.txt");
	rename("temp_locations.txt", "locations.txt");
	remove("cases.txt");
	rename("temp_cases.txt", "cases.txt");
	
	if(found == 0){
		cout << "Location with name '" << locationName << "' not found in the system." << endl;
	}
}

void listLocations(){
	bool found = 0;
	fstream data;
	
	string lctn;
	
	vector<string> locations;
	
	data.open("locations.txt", ios::in);
	
	while(data >> lctn){
		locations.push_back(toUpper(lctn));
		found = 1;
	}
	
	data.close();
	if(found == 0){
		cout << "There are no locations in the system." << endl;
	}
    sort( locations.begin(), locations.end() );
	for(int i=0; i< locations.size(); i++){
		cout << locations[i] << endl;
	}
	
}

void input(){
	string userInput = "";
	cout << "Console >";
	getline(cin, userInput);
    userInput = trim(userInput);
	if(userInput == "") return;
    if(toLower(userInput) == "help"){
		helpMenu();
    } else if(toLower(userInput) == "exit"){
    	cout << "Exiting..." << endl;
    	exit(0);
	}else if(toLower(userInput) == "add" || toLower(userInput) == "add "){
		cout << "You should provide location name to register" << endl;	
	}else if(startsWith(userInput, "add ")){
		addLocation(userInput);
	}else if((toLower(userInput)) == "delete" || (toLower(userInput)) == "delete "){
		cout << "You should provide location name to delete" << endl;	
	}else if(startsWith(userInput, "delete ")){
		deleteLocation(userInput);
	}else if((toLower(userInput)) == "list locations"){
		listLocations();
	} else if(isRecordinput(userInput)){
		recordCases(userInput); 
	}else if((toLower(userInput)) == "record" || (toLower(userInput)) == "record "){
		cout << "You should provide location, disease and cases" << endl;	
	} else if(startsWith(userInput, "record ")){
		cout << "Invalid parameters" << endl;
	}else if((toLower(userInput)) == "where" || (toLower(userInput)) == "where "){
		cout << "You should provide disease name to look location for" << endl;	
	}else if(startsWith(userInput, "where ")){
		localizeDisease(userInput);
	}else if((toLower(userInput)) == "list diseases"){
		listDiseases();
	} else if(isCasesinput(userInput)){
		casesPreview(userInput); 
	}else if((toLower(userInput)) == "cases" || (toLower(userInput)) == "cases "){
		cout << "You should provide district and disease or disease" << endl;	
	} else if(startsWith(userInput, "cases ")){
		cout << "Invalid parameters" << endl;
	} else {
		cout << "Unknown command" << endl;
    }
}


void menu(){
    cout << "================================================" << endl << endl;
    cout << "*    Welcome to Disease Cases Reporting System!    *" << endl << endl;
    cout << "*   ***************************************   *" << endl << endl;
    cout << "*                                 *" << endl << endl;
    cout << "* It is developed by 'ABIZERA Oreste' as practical *" << endl << endl;
    cout << "* evaluation for the end of Year 3.                          *" << endl << endl;
    cout << "================================================" << endl << endl;

    cout << "Starting Time: " << getTime() << endl << endl;
    cout << "Need a help? Type 'help' then press Enter key." << endl << endl;
    while(true){
        input();
    }
}

int main(){
    menu();
    return 0;
}
