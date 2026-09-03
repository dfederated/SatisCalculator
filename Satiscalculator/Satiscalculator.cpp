#include <list>
#include <filesystem>
#include <cstdio>
#include <stdexcept>
#include <windows.h>
#include "Satiscalculator.h"
#include "Resource.h"
#include "fstream"
#include "string"
#include "json.hpp"
#include "vector"
#include "cstdlib"
#include "utility"
#include "mysql/include/mysqlx/sqlite3.h"
#include "mysql/include/mysqlx/xdevapi.h"
#include "devapi/result.h"
#include "devapi/row.h"

using json = nlohmann::json;
std::filesystem::path donePath("done.json");
std::filesystem::path loginPath("login.json");
std::filesystem::path templogpath("tmp.json");
std::filesystem::path tcheckpath("tcheck.json");
json loginJson;
//SQL Variables
const wchar_t* wbuff;
sqlite3* db;
sqlite3_stmt* stmt;
//SQLite Variables
char* errorMessage = nullptr;
std::wstring drop_table = L"DROP TABLE IF EXISTS ";
//Script Variable Names
const char* RecipeUpdater = "python RecipeUpdater.py";
const char* Calculator = "python Calculator.py";
const char* RecipeList = "python RecipeList.py";
//Variables for Character conversions
const char* RecipeItem;
const wchar_t* rectext_convert;
wchar_t Newtxtboxbuff[256];
wchar_t ntbbuff[256];
const wchar_t* TempTable;
wchar_t loginidbuff[256];
wchar_t loginpassbuff[256];
wchar_t loginaddressbuff[256];
wchar_t loginportbuff[256];
//Strings for Conversions
std::string SelectedRecipeName;
std::wstring SelectedRecipeW;
std::string SelRecipe;
std::string sADD;
std::string sPORT;
std::wstring ID;
std::wstring PASS;
std::string sID;
std::string sPASS;
//New Window HWND Init's
HWND hNewtxtbox;
HWND hNewTableBox;
HWND hRecipeList;
HWND DeleteBtn;
HWND ViewBtn;
HWND NewAcceptButton;
HWND NewDeleteButton;
HWND hNewBackButton;
HWND hCreateButton;
//Building handles
HWND smelter;
HWND constructor;
HWND assembler;
HWND manufacturer;
HWND foundry;
HWND refinery;
HWND blender;
HWND particle;
HWND quantum;
//Login Handles
HWND hLoginUID;
HWND hLoginUP;
HWND hLoginPort;
HWND hLoginAdd;
HWND hLogin;
HWND hLoginExit;
HWND hLoginRemember;
HWND hLoginLocal;
HWND hLoginRemote;
HWND hLoginRemember2;
// Main window handle so child windows can show it again
HWND hMainWnd;
HWND hWnd;
//Initilization of Intergers
int newtxterror = 0;
int outputint = 0;
int createsuccess = 0;
int deletesuccess = 0;
int jsonerror = 0;
int deletepushed = 0;
int view1pressed = 0;
int CreatePushed = 0;
int TableCheckInt = 0;
int index = 0;
int ViewTableError = 0;
int Viewcheck = 0;
int Deletecheck = 0;
int SELECTOR = 0;
int View = 0;
int updatepressed = 0;
int updatedone = 0;
int painter = 0;
int logincheck = 0;
int PORT_INT = 0;
int logintype = 0;
//Dynamic Buttons
//For View
const int ID_BUTTBASE_VIEW = 2000;
const int NUM_VIEW_BUTTONS = 50;
HWND hWndButtons[NUM_VIEW_BUTTONS] = {0};
//For Delete
const int ID_BUTTBASE_DELETE = 2200;
const int NUM_DEL_BUTTONS = 50;
HWND hWndDelButtons[NUM_DEL_BUTTONS];
//For New Window
//UINT For Button States
UINT state;
UINT mainWstate;
// Smelter
const int ID_SMELTER = 2300;
const int NUM_SMELT = 50;
HWND smelterbuttons[NUM_SMELT];
const wchar_t* smelttxt;
// Constructor
const int ID_CONSTRUCTOR = 2400;
const int NUM_CONST = 80;
HWND constructorbuttons[NUM_CONST];
//Assembler
const int ID_ASSEMBLER = 2500;
const int NUM_ASSEMBLER = 80;
HWND assemblerbuttons[NUM_ASSEMBLER];
//Manufacturer
const int ID_MANUFACTURER = 2600;
const int NUM_MANUFACTURER = 80;
HWND manufacturerbuttons[NUM_MANUFACTURER];
//Refinery
const int ID_REFINERY = 2700;
const int NUM_REFINERY = 80;
HWND refinerybuttons[NUM_REFINERY];
//Blender
const int ID_BLENDER = 2800;
const int NUM_BLENDER = 80;
HWND blenderbuttons[NUM_BLENDER];
//Foundry
const int ID_FOUNDRY = 2900;
const int NUM_FOUNDRY = 80;
HWND foundrybuttons[NUM_FOUNDRY];
//Particle Accelerator
const int ID_PARTICLE = 3000;
const int NUM_PARTICLE = 80;
HWND particlebuttons[NUM_PARTICLE];
//Quantum Encoder
const int ID_QUANTUM = 3100;
const int NUM_QUANTUM = 10;
HWND quantumbuttons[NUM_QUANTUM];
// Product Selector Variable
std::wstring PRODUCT_SELECT;
//Table string for New Window Delete Button
std::string Table_Delete;
// Stored rows for view display
std::vector<std::wstring> TableList;
std::vector<std::wstring> rawRows, rawOutput, smeltRows, smeltOutput,quantRows, quantOutput, byRows, byOutput,
						constRows, constOutput, assRows, assOutput, manRows, manOutput, foundRows, foundOutput,
						refRows, refOutput, blenRows, blenOutput, partRows, partOutput;
std::wstring smeltMachines, smeltWatts, constMachines, constWatts, assMachines, assWatts, manMachines, manWatts, foundMachines, foundWatts, refMachines, refWatts, blenMachines, blenWatts, partMachines, partWatts, quantMachines, quantWatts;

//RECT for Creation Window
RECT selectRegion;
//Running python scripts for buttons
json RecListFetch() {
	try {
		std::ifstream check("login.json");
		json upcheck = json::parse(check);
		auto juptmp = upcheck["UPDATE"];
		auto updatecheck = juptmp.get<int>();
		if (updatecheck == 0) {
			system(RecipeUpdater);
			system(RecipeList);
			std::ifstream f("Recipes/RecipeList.json");
			json reclist = json::parse(f);
			return reclist;
			f.close();
			if (std::filesystem::exists(donePath)) {
				updatedone = 0;
			}
			else {
				updatedone = 3;
			}
		}
	}
	catch (const std::exception& e) {
		updatedone = 3;
	}
}
json RListData = RecListFetch();

// Sql Table Fetch Function
size_t tcount; // Count of Tables
size_t machinecount;
std::string Tableclean; //Name of Table to grab
int RDI = 0; //RowData Iterator
int MWI = 0; // MWData Iterator
std::vector<std::wstring> MachineList = {L"Raw", L"BYPRODUCT", L"Smelter", L"Constructor", L"Assembler", L"Manufacturer", L"Foundry", L"Refinery", L"Blender", L"Particle", L"Quantum"};
// Function to display targeted Table/Recipe.
void tableFetch() {
	if (logintype == 0) {
		mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
		mysqlx::Schema myDB = mysession.getSchema("satisfactory");
		try {
			mysqlx::Table checktable = myDB.getTable(Tableclean, true);
			TableCheckInt = 1;
		}
		catch (const mysqlx::Error& e) {
			TableCheckInt = 0;
		}
		if (CreatePushed == 0) { TableCheckInt = 0; }
			if (TableCheckInt == 0) {
				try {
					if (CreatePushed == 1) { system(Calculator); }
					RDI = 0;
					MWI = 0;
					// prepare storage for rows to be painted
					byRows.clear();
					byOutput.clear();
					rawRows.clear();
					rawOutput.clear();
					smeltRows.clear();
					smeltOutput.clear();
					smeltMachines.clear();
					smeltWatts.clear();
					constRows.clear();
					constOutput.clear();
					constMachines.clear();
					constWatts.clear();
					assRows.clear();
					assOutput.clear();
					assMachines.clear();
					assWatts.clear();
					manRows.clear();
					manOutput.clear();
					manMachines.clear();
					foundRows.clear();
					foundOutput.clear();
					foundMachines.clear();
					foundWatts.clear();
					refRows.clear();
					refOutput.clear();
					refMachines.clear();
					refWatts.clear();
					blenRows.clear();
					blenOutput.clear();
					blenMachines.clear();
					blenWatts.clear();
					partRows.clear();
					partOutput.clear();
					partMachines.clear();
					partWatts.clear();
					quantRows.clear();
					quantOutput.clear();
					quantMachines.clear();
					quantWatts.clear();

					mysqlx::Table ViewTable = myDB.getTable(Tableclean);

					for (int i = 0; i < MachineList.size(); i++) {

						mysqlx::RowResult Product = ViewTable.select("Product").where("ID = :id").bind("id", MachineList[i]).execute();
						mysqlx::RowResult Output = ViewTable.select("Output").where("ID = :id").bind("id", MachineList[i]).execute();
						if (Product.count() != 0 && Output.count() != 0) {
							for (mysqlx::Row row : Product.fetchAll()) {
								std::string tmp = row[0].get<std::string>();
								int len = MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0);
								if (len > 0) {
									std::wstring wtmp(len - 1, L'\0');
									MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, &wtmp[0], len);
									if (MachineList[i] == L"Raw") { rawRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"BYPRODUCT") { byRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Smelter") { smeltRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Constructor") { constRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Assembler") { assRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Manufacturer") { manRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Foundry") { foundRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Refinery") { refRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Blender") { blenRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Particle") { partRows.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Quantum") { quantRows.push_back(std::move(wtmp)); }
										
								}
							}
							RDI++;
							for (mysqlx::Row row : Output.fetchAll()) {
								float outputValue = row[0].get<float>();
								char buffer[32];
								sprintf_s(buffer, sizeof(buffer), "%.1f", outputValue);
								std::string tmp(buffer);
								int len = MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0);
								if (len > 0) {
									std::wstring wtmp(len - 1, L'\0');
									MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, &wtmp[0], len);
									if (MachineList[i] == L"Raw") { rawOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"BYPRODUCT") { byOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Smelter") { smeltOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Constructor") { constOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Assembler") { assOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Manufacturer") { manOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Foundry") { foundOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Refinery") { refOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Blender") { blenOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Particle") { partOutput.push_back(std::move(wtmp)); }
									if (MachineList[i] == L"Quantum") { quantOutput.push_back(std::move(wtmp)); }
								}
							}
							RDI++;
							if (RDI >= 5) {
								mysqlx::RowResult Machines = ViewTable.select("Machines").where("ID = :id").bind("id", MachineList[i]).execute();
								mysqlx::RowResult Wattage = ViewTable.select("Wattage").where("ID = :id").bind("id", MachineList[i]).execute();
									
								float smeltMsum = 0;
								for (mysqlx::Row row : Machines.fetchAll()) {
									float machinesValue = row[0].get<float>();
									smeltMsum = smeltMsum + machinesValue;
									char buffer[32];
									sprintf_s(buffer, sizeof(buffer), "%.1f", smeltMsum);
									std::string tmp(buffer);
									int len = MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0);
									if (len > 0) {
										std::wstring wtmp(len - 1, L'\0');
										MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, &wtmp[0], len);
										if (MachineList[i] == L"Smelter") { smeltMachines = wtmp; }
										if (MachineList[i] == L"Constructor") { constMachines = wtmp; }
										if (MachineList[i] == L"Assembler") { assMachines = wtmp; }
										if (MachineList[i] == L"Manufacturer") { manMachines = wtmp; }
										if (MachineList[i] == L"Foundry") { foundMachines = wtmp; }
										if (MachineList[i] == L"Refinery") { refMachines = wtmp; }
										if (MachineList[i] == L"Blender") { blenMachines = wtmp; }
										if (MachineList[i] == L"Particle") { partMachines = wtmp; }
										if (MachineList[i] == L"Quantum") { quantMachines = wtmp; }
									}
								}
								float smeltWsum = 0;
								for (mysqlx::Row row : Wattage.fetchAll()) {
									float wattValue = row[0].get<float>();
									smeltWsum = smeltWsum + wattValue;
									char buffer[32];
									sprintf_s(buffer, sizeof(buffer), "%.1f", smeltWsum);
									std::string tmp(buffer);
									int len = MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0);
									if (len > 0) {
										std::wstring wtmp(len - 1, L'\0');
										MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, &wtmp[0], len);
										if (MachineList[i] == L"Smelter") { smeltWatts = wtmp; }
										if (MachineList[i] == L"Constructor") { constWatts = wtmp; }
										if (MachineList[i] == L"Assembler") { assWatts = wtmp; }
										if (MachineList[i] == L"Manufacturer") { manWatts = wtmp; }
										if (MachineList[i] == L"Foundry") { foundWatts = wtmp; }
										if (MachineList[i] == L"Refinery") { refWatts = wtmp; }
										if (MachineList[i] == L"Blender") { blenWatts = wtmp; }
										if (MachineList[i] == L"Particle") { partWatts = wtmp; }
										if (MachineList[i] == L"Quantum") { quantWatts = wtmp; }
									}
								}
							}
						}
					}
				}
				catch (const std::runtime_error& e) {
					jsonerror = 1;
					InvalidateRect(hWnd, NULL, FALSE);
			}
		}
	}
	
	if (logintype == 1) {
		if (CreatePushed == 1) { system(Calculator); }
		RDI = 0;
		MWI = 0;
		// prepare storage for rows to be painted
		byRows.clear();
		byOutput.clear();
		rawRows.clear();
		rawOutput.clear();
		smeltRows.clear();
		smeltOutput.clear();
		smeltMachines.clear();
		smeltWatts.clear();
		constRows.clear();
		constOutput.clear();
		constMachines.clear();
		constWatts.clear();
		assRows.clear();
		assOutput.clear();
		assMachines.clear();
		assWatts.clear();
		manRows.clear();
		manOutput.clear();
		manMachines.clear();
		foundRows.clear();
		foundOutput.clear();
		foundMachines.clear();
		foundWatts.clear();
		refRows.clear();
		refOutput.clear();
		refMachines.clear();
		refWatts.clear();
		blenRows.clear();
		blenOutput.clear();
		blenMachines.clear();
		blenWatts.clear();
		partRows.clear();
		partOutput.clear();
		partMachines.clear();
		partWatts.clear();
		quantRows.clear();
		quantOutput.clear();
		quantMachines.clear();
		quantWatts.clear();
		machinecount = MachineList.size();
		for (int i = 0; i < machinecount; i++) {
			
			std::string CurrentMachine(MachineList[i].begin(), MachineList[i].end());
			std::string lProduct("SELECT Product FROM " + Tableclean + " WHERE ID = \'" + CurrentMachine + "\'");
			std::string lOutput("SELECT Output FROM " + Tableclean + " WHERE ID = \'" + CurrentMachine + "\'");
			std::string lMachines("SELECT Machines FROM " + Tableclean + " WHERE ID = \'" + CurrentMachine + "\'");
			std::string lWattage("SELECT Wattage FROM " + Tableclean + " WHERE ID = \'" + CurrentMachine + "\'");
			if (sqlite3_open("satisfactory.db", &db) != SQLITE_OK) {
				DebugBreak();
			}

			if (sqlite3_prepare_v2(db, lProduct.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
				DebugBreak();
			}
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				const unsigned char* text = sqlite3_column_text(stmt, 0);
				if (text != nullptr) {
					std::string tmp(reinterpret_cast<const char*>(text));
					int len = MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0);
					if (len > 0) {
						std::wstring wtmp(len - 1, L'\0');
						MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, &wtmp[0], len);
						if (MachineList[i] == L"Raw") { rawRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"BYPRODUCT") { byRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Smelter") { smeltRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Constructor") { constRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Assembler") { assRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Manufacturer") { manRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Foundry") { foundRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Refinery") { refRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Blender") { blenRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Particle") { partRows.push_back(std::move(wtmp)); }
						if (MachineList[i] == L"Quantum") { quantRows.push_back(std::move(wtmp)); }
					}
				}
			}
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			RDI++;
			sqlite3_prepare_v2(db, lOutput.c_str(), -1, &stmt, nullptr);
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				float outputValue = static_cast<float>(sqlite3_column_double(stmt, 0));
				char buffer[32];
				sprintf_s(buffer, sizeof(buffer), "%.1f", outputValue);
				std::string tmp(buffer);
				std::wstring wtmp(MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0) - 1, L'\0');
				if (MachineList[i] == L"Raw") { rawOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"BYPRODUCT") { byOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Smelter") { smeltOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Constructor") { constOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Assembler") { assOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Manufacturer") { manOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Foundry") { foundOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Refinery") { refOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Blender") { blenOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Particle") { partOutput.push_back(std::move(wtmp)); }
				if (MachineList[i] == L"Quantum") { quantOutput.push_back(std::move(wtmp)); }
			}
			sqlite3_finalize(stmt);
			RDI++;
			if (RDI >= 5) {
				sqlite3_prepare_v2(db, lMachines.c_str(), -1, &stmt, nullptr);
				while (sqlite3_step(stmt) == SQLITE_ROW) {
					float machineValue = static_cast<float>(sqlite3_column_double(stmt, 0));
					char buffer[32];
					sprintf_s(buffer, sizeof(buffer), "%.1f", machineValue);
					std::string tmp(buffer);
					std::wstring wtmp(MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0) - 1, L'\0');
					if (MachineList[i] == L"Smelter") { smeltMachines = wtmp; }
					if (MachineList[i] == L"Constructor") { constMachines = wtmp; }
					if (MachineList[i] == L"Assembler") { assMachines = wtmp; }
					if (MachineList[i] == L"Manufacturer") { manMachines = wtmp; }
					if (MachineList[i] == L"Foundry") { foundMachines = wtmp; }
					if (MachineList[i] == L"Refinery") { refMachines = wtmp; }
					if (MachineList[i] == L"Blender") { blenMachines = wtmp; }
					if (MachineList[i] == L"Particle") { partMachines = wtmp; }
					if (MachineList[i] == L"Quantum") { quantMachines = wtmp; }
				}
				sqlite3_finalize(stmt);
				MWI++;
				sqlite3_prepare_v2(db, lWattage.c_str(), -1, &stmt, nullptr);
				while (sqlite3_step(stmt) == SQLITE_ROW) {
					float wattValue = static_cast<float>(sqlite3_column_double(stmt, 0));
					char buffer[32];
					sprintf_s(buffer, sizeof(buffer), "%.1f", wattValue);
					std::string tmp(buffer);
					std::wstring wtmp(MultiByteToWideChar(CP_UTF8, 0, tmp.c_str(), -1, NULL, 0) - 1, L'\0');
					if (MachineList[i] == L"Smelter") { smeltWatts = wtmp; }
					if (MachineList[i] == L"Constructor") { constWatts = wtmp; }
					if (MachineList[i] == L"Assembler") { assWatts = wtmp; }
					if (MachineList[i] == L"Manufacturer") { manWatts = wtmp; }
					if (MachineList[i] == L"Foundry") { foundWatts = wtmp; }
					if (MachineList[i] == L"Refinery") { refWatts = wtmp; }
					if (MachineList[i] == L"Blender") { blenWatts = wtmp; }
					if (MachineList[i] == L"Particle") { partWatts = wtmp; }
					if (MachineList[i] == L"Quantum") { quantWatts = wtmp; }
				}
				sqlite3_finalize(stmt);
				MWI++;
			}
		}
		sqlite3_close(db);
	}
}

std::vector<std::wstring> Local_tableFetch(sqlite3* db) {
	std::vector<std::wstring> local_tables;
	sqlite3_stmt* stmt;
	const char* sql = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY name;";
	sqlite3_open("satisfactory.db", &db);
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			if (text != nullptr) {
				const char* tmp = (reinterpret_cast<const char*>(text));
				int tmp_size = MultiByteToWideChar(CP_UTF8, 0, tmp, -1, nullptr, 0);
				std::wstring wtmp(tmp_size - 1, L'\0');
				MultiByteToWideChar(CP_UTF8, 0, tmp, -1, &wtmp[0], tmp_size);
				local_tables.push_back(wtmp);
			}
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return local_tables;
}


//Defining window processors
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ViewWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LoginProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//Bools to check if a window is open, and if it was registered.
bool window1open, ViewWinOpen, NewWindowOpen, LoginOpen = false; //View window No loger used
bool winClass1Registered, ViewWinClassRegistered, NewWinClassRegistered, LoginRegistered = false;
//enum that decides which window to open
enum windowtoopenenumt {none, ViewWindow, NewWindow, LoginWindow};
windowtoopenenumt windowtoopenenum = none;

//Declaring other windows
void createviewwindow(WNDCLASSEX& wc, HWND& hWnd, HINSTANCE hInstance, int nShowCmd);
void createNewWindow(WNDCLASSEX& wc, HWND& hWnd, HINSTANCE hIsntance, int nShowCmd);
//Declaring Login Window
void createLoginWindow(WNDCLASSEX& wc, HWND& hWnd, HINSTANCE hInstance, int nShowCmd);
//Creating Main Window Structure
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	bool endprogram = false;
	WNDCLASSEX WClViewWin;
	HWND hWndViewWin;
	WNDCLASSEX WClNewWin;
	HWND hWndNewWin;
	WNDCLASSEX WClLoginWin;
	HWND hWndLogWin;

	MSG msg;
	WNDCLASSEX WClMain;
	ZeroMemory(&WClMain, sizeof(WNDCLASSEX));
	WClMain.cbClsExtra = NULL;
	WClMain.cbSize = sizeof(WNDCLASSEX);
	WClMain.cbClsExtra = NULL;
	WClMain.hbrBackground = (HBRUSH)COLOR_WINDOW;
	WClMain.hCursor = LoadCursor(NULL, IDC_ARROW);
	WClMain.hIcon = NULL;
	WClMain.hIconSm = NULL;
	WClMain.hInstance = hInstance;
	WClMain.lpfnWndProc = (WNDPROC)WndProc;
	WClMain.lpszClassName = L"Main";
	WClMain.lpszMenuName = NULL;
	WClMain.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&WClMain))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window class failed",
			L"Window Class Failed",
			MB_ICONERROR);
	}
	HWND hWnd = CreateWindowEx(NULL,
		WClMain.lpszClassName,
		L"SatisCalculator",
		WS_OVERLAPPEDWINDOW,
		200,
		150,
		640,
		480,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);
	// store main window handle for child windows to reference
	hMainWnd = hWnd;
	//Button Font
	HFONT hButtonFont = CreateFont(
		42,                    // Height (adjust this value for larger/smaller text)
		0,                     // Width (0 = auto)
		0, 0,                  // Angle
		FW_BOLD,             // Weight
		FALSE, FALSE, FALSE,   // Italic, Underline, Strikeout
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Algerian"
	);
	ViewBtn = CreateWindowEx(NULL,
		L"BUTTON",
		L"View",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_PUSHLIKE | BS_AUTOCHECKBOX,
		10,
		300,
		180,
		60,
		hWnd,
		(HMENU)ViewButton,
		(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
		NULL);
	HWND UpdateBtn = CreateWindowEx(NULL,
		L"BUTTON",
		L"Update",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		10,
		10,
		180,
		60,
		hWnd,
		(HMENU)UpdateButton,
		GetModuleHandle(NULL),
		NULL);
	HWND NewBtn = CreateWindowEx(NULL,
		L"BUTTON",
		L"New",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		10,
		200,
		180,
		60,
		hWnd,
		(HMENU)NewButton,
		GetModuleHandle(NULL),
		NULL);
	DeleteBtn = CreateWindowEx(NULL,
		L"BUTTON",
		L"Delete",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_PUSHLIKE | BS_AUTOCHECKBOX,
		10,
		360,
		180,
		60,
		hWnd,
		(HMENU)DeleteButton,
		GetModuleHandle(NULL),
		NULL);
	HWND MainLoginBtn = CreateWindowEx(NULL, L"BUTTON", L"Login",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 75, 180, 60, hWnd,
		(HMENU)MAINLOGIN, GetModuleHandle(NULL), NULL);
	SendMessage(DeleteBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	SendMessage(NewBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	SendMessage(ViewBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	SendMessage(UpdateBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	SendMessage(MainLoginBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);


	if (!hWnd)
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			L"Window Creation Failed",
			L"Window Creation Failed",
			MB_ICONERROR);
	}
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);
	bool endloop = false;
	while (endloop == false) {
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (windowtoopenenum != none) {
			switch (windowtoopenenum) {
			case ViewWindow:
				if (ViewWinOpen == false) {
					createviewwindow(WClViewWin, hWndViewWin, hInstance, nShowCmd);
				}
				break;
			case NewWindow:
				if (NewWindowOpen == false) {
					createNewWindow(WClNewWin, hWndNewWin, hInstance, nShowCmd);
				}
				break;
			case LoginWindow:
				if (LoginOpen == false) {
					createLoginWindow(WClLoginWin, hWndLogWin, hInstance, nShowCmd);
				}
				break;
			}
			windowtoopenenum = none;
		}

		if (window1open == false && ViewWinOpen == false && NewWindowOpen == false && LoginOpen == false) {
			std::filesystem::path tempremoval = "tmp.json";
			std::filesystem::remove(tempremoval);
			endloop = true;
		}
	}
}

//Creating View Window
void createviewwindow(WNDCLASSEX& wc, HWND& hWnd, HINSTANCE hInstance, int nShowCmd) {
	//Main Window
	if (ViewWinClassRegistered == false) {
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = (WNDPROC)ViewWinProc;
		wc.lpszClassName = L"wc2";
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wc))
		{
			int nResult = GetLastError();
			MessageBox(NULL,
				L"Window Class Creation Failed",
				L"Window Class Creation Failed",
				MB_ICONERROR);
		}
		else
			ViewWinClassRegistered = true;
	}
	hWnd = CreateWindowEx(NULL,
		wc.lpszClassName,
		L"View Screen",
		WS_OVERLAPPEDWINDOW,
		200,
		170,
		960,
		960,
		NULL,
		NULL,
		hInstance,
		NULL);
	HFONT hButtonFont = CreateFont(
		42,                    // Height (adjust this value for larger/smaller text)
		0,                     // Width (0 = auto)
		0, 0,                  // Angle
		FW_BOLD,             // Weight
		FALSE, FALSE, FALSE,   // Italic, Underline, Strikeout
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Algerian"
	);
	HWND ViewBackButton = CreateWindowEx(NULL, L"BUTTON", L"Back",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 180, 60, hWnd,
		(HMENU)VIEWBACKBTN, GetModuleHandle(NULL), NULL);
	SendMessage(ViewBackButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	if (!hWnd)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"View Window Creation Failed",
			L"View Window Creation Failed",
			MB_ICONERROR);
	}
	ShowWindow(hWnd, nShowCmd);
}

// Creating New Window
void createNewWindow(WNDCLASSEX& nwc, HWND& hWnd, HINSTANCE hInstance, int nShowCmd) {
	if (NewWinClassRegistered == false) {
		ZeroMemory(&nwc, sizeof(WNDCLASSEX));
		nwc.cbClsExtra = NULL;
		nwc.cbSize = sizeof(WNDCLASSEX);
		nwc.cbWndExtra = NULL;
		nwc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		nwc.hCursor = LoadCursor(NULL, IDC_ARROW);
		nwc.hIcon = NULL;
		nwc.hIconSm = NULL;
		nwc.hInstance = hInstance;
		nwc.lpfnWndProc = (WNDPROC)NewWinProc;
		nwc.lpszClassName = L"nwc2";
		nwc.lpszMenuName = NULL;
		nwc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&nwc))
		{
			int nResult = GetLastError();
			MessageBox(NULL,
				L"Window Class Creation Failed",
				L"Window Class Creation Failed",
				MB_ICONERROR);
		}
		else
			NewWinClassRegistered = true;
	}
	hWnd = CreateWindowEx(NULL,
		nwc.lpszClassName,
		L"Creation Screen",
		WS_OVERLAPPEDWINDOW,
		200,
		170,
		520,
		840,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!hWnd)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"View Window Creation Failed",
			L"View Window Creation Failed",
			MB_ICONERROR);
	}
	hCreateButton = CreateWindowEx(NULL,
		L"BUTTON",
		L"Create",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		310,
		200,
		180,
		60,
		hWnd,
		(HMENU)CreateButton,
		GetModuleHandle(NULL),
		NULL);

	// Make button text bigger
	HFONT hButtonFont = CreateFont(
		42,                    // Height (adjust this value for larger/smaller text)
		0,                     // Width (0 = auto)
		0, 0,                  // Angle
		FW_BOLD,             // Weight
		FALSE, FALSE, FALSE,   // Italic, Underline, Strikeout
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Algerian"
	);
	SendMessage(hCreateButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	hNewBackButton = CreateWindowEx(NULL,
		L"BUTTON",
		L"Back",
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD | BS_DEFPUSHBUTTON,
		220,
		60,
		120,
		60,
		hWnd,
		(HMENU)NewBackButton,
		GetModuleHandle(NULL),
		NULL);
	NewAcceptButton = CreateWindowEx(NULL, L"BUTTON", L"Accept",
		WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 440, 160, 60, hWnd,
		(HMENU)NEWACCEPTBTN, GetModuleHandle(NULL), NULL);
	NewDeleteButton = CreateWindowEx(NULL, L"BUTTON", L"Delete",
		WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 520, 160, 60, hWnd,
		(HMENU)NEWDELETEBTN, GetModuleHandle(NULL), NULL);
	SendMessage(hNewBackButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	SendMessage(NewAcceptButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	SendMessage(NewDeleteButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	hNewtxtbox = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10, 80, 200, 25,
		hWnd, (HMENU)1, NULL, NULL);
	hNewTableBox = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10, 30, 200, 25,
		hWnd, (HMENU)2, NULL, NULL);


	smelter = CreateWindowEx(NULL, L"BUTTON", L"Smelter",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		10, 130, 95, 24, hWnd,
		(HMENU)SMELTBTN, GetModuleHandle(NULL), NULL);
	constructor = CreateWindowEx(NULL, L"BUTTON", L"Constructor",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		110, 130, 95, 24, hWnd,
		(HMENU)CONSTRUCTBTN, GetModuleHandle(NULL), NULL);
	assembler = CreateWindowEx(NULL, L"BUTTON", L"Assembler",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		210, 130, 95, 24, hWnd,
		(HMENU)ASSEMBLERBTN, GetModuleHandle(NULL), NULL);
	manufacturer = CreateWindowEx(NULL, L"BUTTON", L"Manufacturer",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		10, 170, 95, 24, hWnd,
		(HMENU)MANUFACTURERBTN, GetModuleHandle(NULL), NULL);
	refinery = CreateWindowEx(NULL, L"BUTTON", L"Refinery",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		110, 170, 95, 24, hWnd,
		(HMENU)REFINERYBTN, GetModuleHandle(NULL), NULL);
	blender = CreateWindowEx(NULL, L"BUTTON", L"Blender",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		210, 170, 95, 24, hWnd,
		(HMENU)BLENDERBTN, GetModuleHandle(NULL), NULL);
	foundry = CreateWindowEx(NULL, L"BUTTON", L"Foundry",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		10, 210, 95, 24, hWnd,
		(HMENU)FOUNDRYBTN, GetModuleHandle(NULL), NULL);
	particle = CreateWindowEx(NULL, L"BUTTON", L"Particle Acc",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		110, 210, 95, 24, hWnd,
		(HMENU)PARTICLEBTN, GetModuleHandle(NULL), NULL);
	quantum = CreateWindowEx(NULL, L"BUTTON", L"Quantum Enc",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		210, 210, 95, 24, hWnd,
		(HMENU)QUANTUMBTN, GetModuleHandle(NULL), NULL);
	int currentY = 270;
	int padding = 40;
	int startY = 270;
	int startX = 10;
	std::ifstream f("Recipes/smelter.json");
	json data = json::parse(f);
	const size_t smeltcount = data.size();
	if (data["0"] != 0) {
		for (int interdel = 0; interdel < smeltcount; ++interdel) {
			int currentY = startY + interdel * (24 + padding);
			std::string intconversion = std::to_string(interdel);

			const auto table0name = data[intconversion].get<std::string>();
			size_t tblsize = table0name.length() + 1;
			std::vector<wchar_t> wc(tblsize);
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, wc.data(), tblsize, table0name.c_str(), _TRUNCATE);
			const wchar_t* wstr = wc.data();

			smelterbuttons[interdel] = CreateWindowEx(NULL,
				L"BUTTON",
				wstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX,
				currentY,
				100,
				60,
				hWnd,
				(HMENU)(INT_PTR)(ID_SMELTER + interdel),
				(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
				NULL);
		}
	}
	f.close();
	f.clear();
	std::ifstream conf("Recipes/constructor.json");
	json condata = json::parse(conf);
	const size_t constcount = condata.size();
	if (condata["0"] != 0) {
		for (int i = 0; i < constcount; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string intconversion = std::to_string(i);
			const auto rname = condata[intconversion].get<std::string>();
			size_t rsize = rname.length() + 1;
			std::vector<wchar_t> wc(rsize);
			size_t convertChars = 0;
			mbstowcs_s(&convertChars, wc.data(), rsize, rname.c_str(), _TRUNCATE);
			const wchar_t* wstr = wc.data();
			constructorbuttons[i] = CreateWindowEx(NULL, L"BUTTON", wstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_CONSTRUCTOR + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}

	}
	conf.close();
	conf.clear();
	std::ifstream assf("Recipes/assembler.json");
	json assdata = json::parse(assf);
	const size_t asscount = assdata.size();
	if (assdata["0"] != 0) {
		for (int i = 0; i < asscount; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string inconversion = std::to_string(i);
			const auto assname = assdata[inconversion].get<std::string>();
			size_t asize = assname.length() + 1;
			std::vector<wchar_t> wc(asize);
			size_t convertAss = 0;
			mbstowcs_s(&convertAss, wc.data(), asize, assname.c_str(), _TRUNCATE);
			const wchar_t* wstr = wc.data();
			assemblerbuttons[i] = CreateWindowEx(NULL, L"BUTTON", wstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_ASSEMBLER + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
	}
	assf.close();
	assf.clear();
	std::ifstream foundf("Recipes/foundry.json");
	json foundata = json::parse(foundf);
	const size_t foundcount = foundata.size();
	if (foundata["0"] != 0) {
		for (int i = 0; i < foundcount; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string fconvert = std::to_string(i);
			const auto foundname = foundata[fconvert].get<std::string>();
			size_t fsize = foundname.length() + 1;
			std::vector<wchar_t> fwc(fsize);
			size_t convertF = 0;
			mbstowcs_s(&convertF, fwc.data(), fsize, foundname.c_str(), _TRUNCATE);
			const wchar_t* fwstr = fwc.data();
			foundrybuttons[i] = CreateWindowEx(NULL, L"BUTTON", fwstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_FOUNDRY + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
	}
	foundf.close();
	foundf.clear();
	std::ifstream manf("Recipes/manufacturer.json");
	json mandata = json::parse(manf);
	const size_t mansize = mandata.size();
	if (mandata["0"] != 0) {
		for (int i = 0; i < mansize; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string mconvert = std::to_string(i);
			const auto manname = mandata[mconvert].get<std::string>();
			size_t msize = manname.length() + 1;
			std::vector<wchar_t> mwc(msize);
			size_t convertm = 0;
			mbstowcs_s(&convertm, mwc.data(), msize, manname.c_str(), _TRUNCATE);
			const wchar_t* mwstr = mwc.data();
			manufacturerbuttons[i] = CreateWindowEx(NULL, L"BUTTON", mwstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_MANUFACTURER + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
	}
	manf.close();
	manf.clear();
	std::ifstream ref("Recipes/refinery.json");
	json refdata = json::parse(ref);
	const size_t refsize = refdata.size();
	if (refdata["0"] != 0) {
		for (int i = 0; i < refsize; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string refconvert = std::to_string(i);
			const auto refname = refdata[refconvert].get<std::string>();
			size_t resize = refname.length() + 1;
			std::vector<wchar_t> rewc(resize);
			size_t convertref = 0;
			mbstowcs_s(&convertref, rewc.data(), resize, refname.c_str(), _TRUNCATE);
			const wchar_t* mwstr = rewc.data();
			refinerybuttons[i] = CreateWindowEx(NULL, L"BUTTON", mwstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_REFINERY + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
	}
	ref.close();
	ref.clear();
	std::ifstream bf("Recipes/blender.json");
	json blendata = json::parse(bf);
	const size_t blendsize = blendata.size();
	if (blendata["0"] != 0) {
		for (int i = 0; i < blendsize; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string blendconvert = std::to_string(i);
			const auto blendname = blendata[blendconvert].get<std::string>();
			size_t blsize = blendname.length() + 1;
			std::vector<wchar_t> bwc(blsize);
			size_t convertbl = 0;
			mbstowcs_s(&convertbl, bwc.data(), blsize, blendname.c_str(), _TRUNCATE);
			const wchar_t* bwstr = bwc.data();
			blenderbuttons[i] = CreateWindowEx(NULL, L"BUTTON", bwstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_BLENDER + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
	}
	bf.close();
	bf.clear();
	std::ifstream pf("Recipes/particles.json");
	json partdata = json::parse(pf);
	const size_t partsize = partdata.size();
	if (partdata["0"] != 0) {
		for (int i = 0; i < partsize; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string partconvert = std::to_string(i);
			const auto partname = partdata[partconvert].get<std::string>();
			size_t prsize = partname.length() + 1;
			std::vector<wchar_t> pwc(prsize);
			size_t convertp = 0;
			mbstowcs_s(&convertp, pwc.data(), prsize, partname.c_str(), _TRUNCATE);
			const wchar_t* pwstr = pwc.data();
			particlebuttons[i] = CreateWindowEx(NULL, L"BUTTON", pwstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_PARTICLE + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
	}
	pf.close();
	pf.clear();
	std::ifstream qf("Recipes/quantum.json");
	json qdata = json::parse(qf);
	const size_t quantsize = qdata.size();
	if (qdata["0"] != 0) {
		for (int i = 0; i < quantsize; ++i) {
			if (i >= 8) {
				startX = 120;
				currentY = startY + (i - 8) * (24 + padding);
			}
			if (i >= 16) {
				startX = 230;
				currentY = startY + (i - 16) * (24 + padding);
			}
			if (i >= 24) {
				startX = 340;
				currentY = startY + (i - 24) * (24 + padding);
			}
			if (i >= 32) {
				startX = 450;
				currentY = startY + (i - 32) * (24 + padding);
			}
			if (i >= 40) {
				startX = 560;
				currentY = startY + (i - 40) * (24 + padding);
			}
			if (i >= 48) {
				startX = 670;
				currentY = startY + (i - 48) * (24 + padding);
			}
			if (i >= 56) {
				startX = 780;
				currentY = startY + (i - 56) * (24 + padding);
			}
			if (i >= 64) {
				startX = 880;
				currentY = startY + (i - 64) * (24 + padding);
			}
			if (i >= 72) {
				startX = 990;
				currentY = startY + (i - 72) * (24 + padding);
			}
			if (i < 8) {
				startX = 10;
				currentY = startY + i * (24 + padding);
			}
			std::string qconvert = std::to_string(i);
			const auto qname = qdata[qconvert].get<std::string>();
			size_t qsize = qname.length() + 1;
			std::vector<wchar_t> qwc(qsize);
			size_t convertq = 0;
			mbstowcs_s(&convertq, qwc.data(), qsize, qname.c_str(), _TRUNCATE);
			const wchar_t* qwstr = qwc.data();
			quantumbuttons[i] = CreateWindowEx(NULL, L"BUTTON", qwstr,
				WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_CENTER | BS_MULTILINE,
				startX, currentY, 100, 60, hWnd,
				(HMENU)(INT_PTR)(ID_QUANTUM + i), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
		}
	}
	qf.close();
	qf.clear();
	ShowWindow(hWnd, nShowCmd);
}

//Creating Login Window
void createLoginWindow(WNDCLASSEX& wc, HWND& hWnd, HINSTANCE hInstance, int nShowCmd) {
	if (LoginRegistered == false) {
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = (WNDPROC)LoginProc;
		wc.lpszClassName = L"lcw";
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wc))
		{
			int nResult = GetLastError();
			MessageBox(NULL,
				L"Window Class Creation Failed",
				L"Window Class Creation Failed",
				MB_ICONERROR);
		}
		else
			LoginRegistered = true;
	}
	hWnd = CreateWindowEx(NULL,
		wc.lpszClassName,
		L"Login",
		WS_OVERLAPPEDWINDOW,
		500,
		300,
		200,
		160,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!hWnd)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			L"View Window Creation Failed",
			L"View Window Creation Failed",
			MB_ICONERROR);
	}
	hLoginUID = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"EDIT", L"",
		WS_CHILD | ES_AUTOHSCROLL,
		40, 30, 200, 25,
		hWnd, (HMENU)3, NULL, NULL);
	hLoginUP = CreateWindowEx(WS_EX_CLIENTEDGE,
		L"EDIT", L"",
		WS_CHILD | ES_AUTOHSCROLL,
		40, 80, 200, 25,
		hWnd, (HMENU)4, NULL, NULL);
	hLoginAdd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
		WS_CHILD | ES_AUTOHSCROLL,
		40, 130, 200, 25, hWnd,
		(HMENU)5, NULL, NULL);
	hLoginPort = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
		WS_CHILD | ES_AUTOHSCROLL,
		40, 180, 200, 25, hWnd,
		(HMENU)6, NULL, NULL);
	hLogin = CreateWindowEx(NULL, L"BUTTON", L"Login",
		WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
		40, 230, 80, 25, hWnd,
		(HMENU)LOGINBTN, GetModuleHandle(NULL), NULL);
	hLoginExit = CreateWindowEx(NULL, L"BUTTON", L"Back",
		WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
		40, 260, 80, 25, hWnd,
		(HMENU)LOGINEXIT, GetModuleHandle(NULL), NULL);
	hLoginRemember = CreateWindowEx(NULL, L"BUTTON", L"Remember User?",
		WS_TABSTOP | WS_CHILD | BS_AUTOCHECKBOX | BS_CENTER | BS_MULTILINE,
		140, 220, 100, 30, hWnd,
		(HMENU)LOGINREMEMBER, GetModuleHandle(NULL), NULL);
	hLoginLocal = CreateWindowEx(NULL, L"BUTTON", L"Local",
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		5, 40, 80, 40, hWnd,
		(HMENU)LOGINLOCAL, GetModuleHandle(NULL), NULL);
	hLoginRemote = CreateWindowEx(NULL, L"BUTTON", L"Remote",
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		100, 40, 80, 40, hWnd,
		(HMENU)LOGINREMOTE, GetModuleHandle(NULL), NULL);
	//hLoginRemember2 = CreateWindowEx(NULL, L"BUTTON", L"Remember Selection",
	//	WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_CENTER | BS_MULTILINE,
	//	10, 100, 100, 30, hWnd,
	//	(HMENU)LOGINREMEMBER2, GetModuleHandle(NULL), NULL);
	if (std::filesystem::exists(loginPath)) {
		std::ifstream f(loginPath);
		json logintmp = json::parse(f);
		if (logintmp.contains("ID")) {
			auto jidtmp = logintmp["ID"];
			auto jaddtmp = logintmp["ADD"];
			auto jporttmp = logintmp["PORT"];
			auto jpasstmp = logintmp["PASS"];
			std::string idtmp = jidtmp.get<std::string>();
			std::wstring widtmp(idtmp.begin(), idtmp.end());
			std::string addtmp = jaddtmp.get<std::string>();
			std::wstring waddtmp(addtmp.begin(), addtmp.end());
			std::string porttemp = jporttmp.get<std::string>();
			std::wstring wportemp(porttemp.begin(), porttemp.end());
			std::string passtemp = jpasstmp.get<std::string>();
			std::wstring wpasstemp(passtemp.begin(), passtemp.end());
			SendMessage(hLoginUID, WM_SETTEXT, 0, (LPARAM)widtmp.c_str());
			SendMessage(hLoginAdd, WM_SETTEXT, 0, (LPARAM)waddtmp.c_str());
			SendMessage(hLoginPort, WM_SETTEXT, 0, (LPARAM)wportemp.c_str());
			SendMessage(hLoginUP, WM_SETTEXT, 0, (LPARAM)wpasstemp.c_str());
		}
	}




	ShowWindow(hWnd, SW_SHOW);
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}
// Windows Process Funtions

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		window1open = true;
		if (logincheck == 0) {
			if (LoginOpen == false)
			{
				ShowWindow(hWnd, SW_HIDE);
				windowtoopenenum = LoginWindow;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		break;
	case WM_DESTROY:
		window1open = false;
		NewWindowOpen = false;
		ViewWinOpen = false;
		LoginOpen = false;
		break;
	case WM_PAINT:
		if (updatedone == 2) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
			HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
			TextOutW(hdc, 20, 160, L"Update Failed.", 15);
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
			updatedone = 0;
		}
		if (updatedone == 1) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
			HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
			TextOutW(hdc, 20, 160, L"Updating Complete.", 18);
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
			updatedone = 0;
		}
		if (updatedone == 3) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
			HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
			TextOutW(hdc, 20, 160, L"Update Required.", 17);
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
			updatedone = 0;
		}
		if (logincheck == 4) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
			HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
			TextOutW(hdc, 10, 160, L"Login Successful.", 17);
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
			logincheck = 6;
			Sleep(2000);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (logincheck == 5) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
			HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
			TextOutW(hdc, 10, 160, L"Please Login to Use Calculator.", 31);
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
			logincheck = 6;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_COMMAND:
		switch LOWORD(wParam)
		{
		case MAINLOGIN:
			painter = 0;
			logincheck = 9;
			for (int i = 0; i < 49; i++) {
				DestroyWindow(hWndDelButtons[i]);
				hWndDelButtons[i] = NULL;
				DestroyWindow(hWndButtons[i]);
				hWndButtons[i] = NULL;
			}
			ShowWindow(hLogin, SW_HIDE);
			ShowWindow(hLoginExit, SW_HIDE);
			ShowWindow(hLoginAdd, SW_HIDE);
			ShowWindow(hLoginPort, SW_HIDE);
			ShowWindow(hLoginRemember, SW_HIDE);
			ShowWindow(hLoginUID, SW_HIDE);
			ShowWindow(hLoginUP, SW_HIDE);
			ShowWindow(hLoginRemote, SW_SHOW);
			ShowWindow(hLoginLocal, SW_SHOW);
			windowtoopenenum = LoginWindow;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		case UpdateButton:
			if (std::filesystem::exists(donePath)) {
				std::filesystem::remove(donePath);
			}
			system(RecipeUpdater);
			system(RecipeList);
			if (std::filesystem::exists(donePath)) {
				updatedone = 1;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else {
				updatedone = 2;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case NewButton:
			CheckDlgButton(hWnd, DeleteButton, BST_UNCHECKED);
			CheckDlgButton(hWnd, ViewButton, BST_UNCHECKED);
			SetWindowPos(hWnd, NULL,
				0, 0, 640, 480,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			for (int i = 0; i < 49; i++) {
				ShowWindow(hWndButtons[i], SW_HIDE);
			}
			for (int i = 0; i < 49; i++) {
				ShowWindow(hWndDelButtons[i], SW_HIDE);
			}
			if (NewWindowOpen == false)
			{
				// hide the main window while the New window is open
				ShowWindow(hWnd, SW_HIDE);
				windowtoopenenum = NewWindow;
			}
			break;
		case ViewButton:
			mainWstate = IsDlgButtonChecked(hWnd, ViewButton);
			if (mainWstate == BST_CHECKED) {
				CheckDlgButton(hWnd, DeleteButton, BST_UNCHECKED);
				int padding = 10;
				int startY = 10;
				int startX = 200;
				if (logintype == 0){
					mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
					mysqlx::Schema myDB = mysession.getSchema("satisfactory");
					std::list<mysqlx::Table> tables = myDB.getTables();
					int tablecount = tables.size();

					int exitcounter = 0;
					int currentY = startY + exitcounter * (24 + padding);
					for (const mysqlx::Table& table : tables) {
						if (exitcounter >= 10) {
							startX = 315;
							currentY = startY + (exitcounter - 10) * (24 + padding);
						}
						if (exitcounter >= 20) {
							startX = 430;
							currentY = startY + (exitcounter - 20) * (24 + padding);
						}
						if (exitcounter >= 30) {
							startX = 545;
							currentY = startY + (exitcounter - 30) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 680, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 40) {
							startX = 760;
							currentY = startY + (exitcounter - 40) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 890, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 50) {
							startX = 875;
							currentY = startY + (exitcounter - 50) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 1005, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter < 10) {
							currentY = startY + exitcounter * (24 + padding);
						}
						std::string tmp = table.getName();
						std::wstring wtmp(tmp.begin(), tmp.end());
						hWndButtons[exitcounter] = CreateWindowEx(NULL, L"BUTTON", wtmp.c_str(),
							WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
							startX, currentY, 100, 24, hWnd,
							(HMENU)(INT_PTR)(ID_BUTTBASE_VIEW + exitcounter), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
						++exitcounter;
						currentY = startY + exitcounter * (24 + padding);
					}
				}
				if (logintype == 1) {
					std::vector<std::wstring> wideTables = Local_tableFetch(db);
					int exitcounter = 0;
					int currentY = startY + exitcounter * (24 + padding);
					tcount = wideTables.size();
					for (int i = 0; i < tcount; i++) {
						if (exitcounter >= 10) {
							startX = 315;
							currentY = startY + (exitcounter - 10) * (24 + padding);
						}
						if (exitcounter >= 20) {
							startX = 430;
							currentY = startY + (exitcounter - 20) * (24 + padding);
						}
						if (exitcounter >= 30) {
							startX = 545;
							currentY = startY + (exitcounter - 30) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 680, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 40) {
							startX = 760;
							currentY = startY + (exitcounter - 40) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 890, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 50) {
							startX = 875;
							currentY = startY + (exitcounter - 50) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 1005, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter < 10) {
							currentY = startY + exitcounter * (24 + padding);
						}
						hWndButtons[exitcounter] = CreateWindowEx(NULL, L"BUTTON", wideTables[i].c_str(),
							WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
							startX, currentY, 100, 24, hWnd,
							(HMENU)(INT_PTR)(ID_BUTTBASE_VIEW + exitcounter), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
						++exitcounter;
						currentY = startY + exitcounter * (24 + padding);
					}
					
				}

				for (int i = 0; i < 49; i++) {
					ShowWindow(hWndDelButtons[i], SW_HIDE);
				}
				for (int i = 0; i < 49; i++){
					ShowWindow(hWndButtons[i], SW_SHOW);
				}
			}
			else {
				for (int i = 0; i < 49; i++) {
					ShowWindow(hWndButtons[i], SW_HIDE);
				}
				SetWindowPos(hWnd, NULL,
					0, 0, 640, 480,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		case DeleteButton:
			mainWstate = IsDlgButtonChecked(hWnd, DeleteButton);
			if (mainWstate == BST_CHECKED) {
				CheckDlgButton(hWnd, ViewButton, BST_UNCHECKED);
				int padding = 10;
				int startY = 10;
				int startX = 200;
				if (logintype == 0) {
					mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
					mysqlx::Schema myDB = mysession.getSchema("satisfactory");
					std::list<mysqlx::Table> tables = myDB.getTables();
					int tablecount = tables.size();
					int exitcounter = 0;
					int currentY = startY + exitcounter * (24 + padding);
					for (const mysqlx::Table& table : tables) {
						if (exitcounter >= 10) {
							startX = 315;
							currentY = startY + (exitcounter - 10) * (24 + padding);
						}
						if (exitcounter >= 20) {
							startX = 430;
							currentY = startY + (exitcounter - 20) * (24 + padding);
						}
						if (exitcounter >= 30) {
							startX = 545;
							currentY = startY + (exitcounter - 30) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 680, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 40) {
							startX = 760;
							currentY = startY + (exitcounter - 40) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 890, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 50) {
							startX = 875;
							currentY = startY + (exitcounter - 50) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 1005, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter < 10) {
							currentY = startY + exitcounter * (24 + padding);
						}
						std::string tmp = table.getName();
						std::wstring wtmp(tmp.begin(), tmp.end());
						hWndDelButtons[exitcounter] = CreateWindowEx(NULL,
							L"BUTTON", wtmp.c_str(),
							WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
							startX, currentY, 100, 24, hWnd,
							(HMENU)(INT_PTR)(ID_BUTTBASE_DELETE + exitcounter),
							(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
							NULL);
						++exitcounter;
						currentY = startY + exitcounter * (24 + padding);
					}
				}
				if (logintype == 1) {
					std::vector<std::wstring> wideTables = Local_tableFetch(db);
					int exitcounter = 0;
					int currentY = startY + exitcounter * (24 + padding);
					for (int i = 0; i < wideTables.size(); i++) {
						if (exitcounter >= 10) {
							startX = 315;
							currentY = startY + (exitcounter - 10) * (24 + padding);
						}
						if (exitcounter >= 20) {
							startX = 430;
							currentY = startY + (exitcounter - 20) * (24 + padding);
						}
						if (exitcounter >= 30) {
							startX = 545;
							currentY = startY + (exitcounter - 30) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 680, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 40) {
							startX = 760;
							currentY = startY + (exitcounter - 40) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 890, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 50) {
							startX = 875;
							currentY = startY + (exitcounter - 50) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 1005, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter < 10) {
							currentY = startY + exitcounter * (24 + padding);
						}
						hWndDelButtons[exitcounter] = CreateWindowEx(NULL,
							L"BUTTON", wideTables[i].c_str(),
							WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
							startX, currentY, 100, 24, hWnd,
							(HMENU)(INT_PTR)(ID_BUTTBASE_DELETE + exitcounter),
							(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
							NULL);
						++exitcounter;
						currentY = startY + exitcounter * (24 + padding);
					}
				}

				for (int i = 0; i < 49; i++) {
					ShowWindow(hWndButtons[i], SW_HIDE);
				}
				for (int i = 0; i < 49; i++) {
					ShowWindow(hWndDelButtons[i], SW_SHOW);
				}
			}
			else {
				for (int i = 0; i < 49; i++) {
					ShowWindow(hWndDelButtons[i], SW_HIDE);
				}
				SetWindowPos(hWnd, NULL,
					0, 0, 640, 480,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		default:
			if (LOWORD(wParam) >= ID_BUTTBASE_VIEW && LOWORD(wParam) < ID_BUTTBASE_VIEW + NUM_VIEW_BUTTONS) {
				int btnIndex = LOWORD(wParam) - ID_BUTTBASE_VIEW;
				if (logintype == 0) {
					mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
					mysqlx::Schema myDB = mysession.getSchema("satisfactory");
					std::list<mysqlx::Table> tables = myDB.getTables();
					int tablecount = tables.size();
					if (btnIndex != (int)tablecount) {
						wchar_t viewText[256];
						GetDlgItemText(hWnd, LOWORD(wParam), viewText, sizeof(viewText) / sizeof(viewText[0]));
						const auto tableName = viewText;
						std::wstring temptable = tableName;
						Tableclean = std::string(temptable.begin(), temptable.end());
						tableFetch();
						// mark paint-needed after gathering rows
						InvalidateRect(hWnd, NULL, TRUE);
						ViewTableError = 2;
						if (ViewWinOpen == false)
						{
							// hide the main window while the New window is open
							ShowWindow(hMainWnd, SW_HIDE);
							windowtoopenenum = ViewWindow;
						}
					}
				}
				if (logintype == 1) {
					wchar_t viewText[256];
					GetDlgItemText(hWnd, LOWORD(wParam), viewText, sizeof(viewText) / sizeof(viewText[0]));
					const auto tableName = viewText;
					std::wstring temptable = tableName;
					Tableclean = std::string(temptable.begin(), temptable.end());
					tableFetch();
					// mark paint-needed after gathering rows
					InvalidateRect(hWnd, NULL, TRUE);
					ViewTableError = 2;
					if (ViewWinOpen == false)
					{
						// hide the main window while the New window is open
						ShowWindow(hMainWnd, SW_HIDE);
						windowtoopenenum = ViewWindow;
					}
				}
				break;
			}
			if (LOWORD(wParam) >= ID_BUTTBASE_DELETE && LOWORD(wParam) < ID_BUTTBASE_DELETE + NUM_DEL_BUTTONS) {
				int btnDelIndex = LOWORD(wParam) - ID_BUTTBASE_DELETE;
				if (logintype == 0){
					mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
					mysqlx::Schema myDB = mysession.getSchema("satisfactory");
					std::list<mysqlx::Table> tables = myDB.getTables();
					int tablecount = tables.size();
					if (btnDelIndex != (int)tablecount) {
						wchar_t viewText[256];
						GetDlgItemText(hWnd, LOWORD(wParam), viewText, sizeof(viewText) / sizeof(viewText[0]));
						const auto tableName = viewText;
						std::wstring wtable(tableName);
						std::string tabname(wtable.begin(), wtable.end());
						mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
						mysqlx::Schema myDB = mysession.getSchema("satisfactory");
						mysqlx::Table ViewTable = myDB.getTable(tableName);
						std::string ToDrop = "DROP TABLE IF EXISTS satisfactory.";
						ToDrop = ToDrop + tabname;
						myDB.getSession().sql(ToDrop).execute();
						for (int i = 0; i < 49; i++) {
							DestroyWindow(hWndDelButtons[i]);
							hWndDelButtons[i] = NULL;
							DestroyWindow(hWndButtons[i]);
							hWndButtons[i] = NULL;
						}
						int padding = 10;
						int startY = 10;
						int startX = 200;
						std::list<mysqlx::Table> tables = myDB.getTables();
						int tablecount = tables.size();
						int exitcounter = 0;
						int currentY = startY + exitcounter * (24 + padding);
						for (const mysqlx::Table& table : tables) {
							if (exitcounter >= 10) {
								startX = 315;
								currentY = startY + (exitcounter - 10) * (24 + padding);
							}
							if (exitcounter >= 20) {
								startX = 430;
								currentY = startY + (exitcounter - 20) * (24 + padding);
							}
							if (exitcounter >= 30) {
								startX = 545;
								currentY = startY + (exitcounter - 30) * (24 + padding);
								SetWindowPos(hWnd, NULL,
									0, 0, 680, 480,
									SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
							}
							if (exitcounter >= 40) {
								startX = 760;
								currentY = startY + (exitcounter - 40) * (24 + padding);
								SetWindowPos(hWnd, NULL,
									0, 0, 890, 480,
									SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
							}
							if (exitcounter >= 50) {
								startX = 875;
								currentY = startY + (exitcounter - 50) * (24 + padding);
								SetWindowPos(hWnd, NULL,
									0, 0, 1005, 480,
									SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
							}
							if (exitcounter < 10) {
								currentY = startY + exitcounter * (24 + padding);
							}
							std::string tmp = table.getName();
							std::wstring wtmp(tmp.begin(), tmp.end());
							hWndDelButtons[exitcounter] = CreateWindowEx(NULL,
								L"BUTTON", wtmp.c_str(),
								WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
								startX, currentY, 100, 24, hWnd,
								(HMENU)(INT_PTR)(ID_BUTTBASE_DELETE + exitcounter),
								(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
								NULL);
							++exitcounter;
							currentY = startY + exitcounter * (24 + padding);
						}
					}

					for (int i = 0; i < 49; i++) {
						ShowWindow(hWndDelButtons[i], SW_SHOW);
					}
					InvalidateRect(hWnd, NULL, TRUE);

				}
				if (logintype == 1) {
					wchar_t viewText[256];
					GetDlgItemText(hWnd, LOWORD(wParam), viewText, sizeof(viewText) / sizeof(viewText[0]));
					const auto tableName = viewText;
					std::wstring wtable(drop_table + tableName);
					std::string drop_cmd(wtable.begin(), wtable.end());
					int rc = sqlite3_open("satisfactory.db", &db);
					rc = sqlite3_exec(db, drop_cmd.c_str(), nullptr, nullptr, &errorMessage);
					if (rc != SQLITE_OK) {
						DebugBreak();
						return rc;
					}
					sqlite3_close(db);
					for (int i = 0; i < 49; i++) {
						DestroyWindow(hWndDelButtons[i]);
						hWndDelButtons[i] = NULL;
						DestroyWindow(hWndButtons[i]);
						hWndButtons[i] = NULL;
					}
					std::vector<std::wstring> wideTables = Local_tableFetch(db);
					int padding = 10;
					int startY = 10;
					int startX = 200;
					int exitcounter = 0;
					int currentY = startY + exitcounter * (24 + padding);
					for (int i = 0; i < wideTables.size(); i++) {
						if (exitcounter >= 10) {
							startX = 315;
							currentY = startY + (exitcounter - 10) * (24 + padding);
						}
						if (exitcounter >= 20) {
							startX = 430;
							currentY = startY + (exitcounter - 20) * (24 + padding);
						}
						if (exitcounter >= 30) {
							startX = 545;
							currentY = startY + (exitcounter - 30) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 680, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 40) {
							startX = 760;
							currentY = startY + (exitcounter - 40) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 890, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter >= 50) {
							startX = 875;
							currentY = startY + (exitcounter - 50) * (24 + padding);
							SetWindowPos(hWnd, NULL,
								0, 0, 1005, 480,
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
						}
						if (exitcounter < 10) {
							currentY = startY + exitcounter * (24 + padding);
						}
						hWndDelButtons[exitcounter] = CreateWindowEx(NULL,
							L"BUTTON", wideTables[i].c_str(),
							WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
							startX, currentY, 100, 24, hWnd,
							(HMENU)(INT_PTR)(ID_BUTTBASE_DELETE + exitcounter),
							(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
							NULL);
						++exitcounter;
						currentY = startY + exitcounter * (24 + padding);
					}
					for (int i = 0; i < 49; i++) {
						ShowWindow(hWndDelButtons[i], SW_SHOW);
					}
					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	}
}

LRESULT CALLBACK ViewWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE: {
		ViewWinOpen = true;
		ShowWindow(hMainWnd, SW_HIDE);
		UpdateWindow(hWnd);
		break;
	}
	case WM_PAINT: 
		if (ViewTableError == 2) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			int currentY = 10;
			int startX = 200;
			// Particle Accelerator
			if (!partRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Particle Accelerator:", 21);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, partMachines.c_str(), (int)partMachines.length());
				TextOutW(hdc, 690, currentY, partWatts.c_str(), (int)partWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < partRows.size(); ++i) {
					const std::wstring& partProduct = partRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, partProduct.c_str(), (int)partProduct.length());
				}
				for (size_t i = 0; i < partOutput.size(); ++i) {
					const std::wstring& partAmount = partOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, partAmount.c_str(), (int)partAmount.length());
				}
				currentY += (int)partRows.size() * 20 + 20;
			}

			// Quantum Encoder
			if (!quantRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Quantum Encoder:", 16);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, quantMachines.c_str(), (int)quantMachines.length());
				TextOutW(hdc, 690, currentY, quantWatts.c_str(), (int)quantWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < quantRows.size(); ++i) {
					const std::wstring& quantProduct = quantRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, quantProduct.c_str(), (int)quantProduct.length());
				}
				for (size_t i = 0; i < quantOutput.size(); ++i) {
					const std::wstring& quantAmount = quantOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, quantAmount.c_str(), (int)quantAmount.length());
				}
				currentY += (int)quantRows.size() * 20 + 20;
			}

			// Blender
			if (!blenRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Blender:", 8);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, blenMachines.c_str(), (int)blenMachines.length());
				TextOutW(hdc, 690, currentY, blenWatts.c_str(), (int)blenWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < blenRows.size(); ++i) {
					const std::wstring& blenProduct = blenRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, blenProduct.c_str(), (int)blenProduct.length());
				}
				for (size_t i = 0; i < blenOutput.size(); ++i) {
					const std::wstring& blenAmount = blenOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, blenAmount.c_str(), (int)blenAmount.length());
				}
				currentY += (int)blenRows.size() * 20 + 20;
			}

			// Refinery
			if (!refRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Refinery:", 9);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, refMachines.c_str(), (int)refMachines.length());
				TextOutW(hdc, 690, currentY, refWatts.c_str(), (int)refWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < refRows.size(); ++i) {
					const std::wstring& refProduct = refRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, refProduct.c_str(), (int)refProduct.length());
				}
				for (size_t i = 0; i < refOutput.size(); ++i) {
					const std::wstring& refAmount = refOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, refAmount.c_str(), (int)refAmount.length());
				}
				currentY += (int)refRows.size() * 20 + 20;
			}

			// Manufacturer
			if (!manRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Manufacturer:", 13);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, manMachines.c_str(), (int)manMachines.length());
				TextOutW(hdc, 690, currentY, manWatts.c_str(), (int)manWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < manRows.size(); ++i) {
					const std::wstring& manProduct = manRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, manProduct.c_str(), (int)manProduct.length());
				}
				for (size_t i = 0; i < manOutput.size(); ++i) {
					const std::wstring& manAmount = manOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, manAmount.c_str(), (int)manAmount.length());
				}
				currentY += (int)manRows.size() * 20 + 20;
			}

			// Assembler
			if (!assRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Assembler:", 10);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, assMachines.c_str(), (int)assMachines.length());
				TextOutW(hdc, 690, currentY, assWatts.c_str(), (int)assWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < assRows.size(); ++i) {
					const std::wstring& assProduct = assRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, assProduct.c_str(), (int)assProduct.length());
				}
				for (size_t i = 0; i < assOutput.size(); ++i) {
					const std::wstring& assAmount = assOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, assAmount.c_str(), (int)assAmount.length());
				}
				currentY += (int)assRows.size() * 20 + 20;
			}

			// Constructor
			if (!constRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Constructor:", 12);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, constMachines.c_str(), (int)constMachines.length());
				TextOutW(hdc, 690, currentY, constWatts.c_str(), (int)constWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < constRows.size(); ++i) {
					const std::wstring& constProduct = constRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, constProduct.c_str(), (int)constProduct.length());
				}
				for (size_t i = 0; i < constOutput.size(); ++i) {
					const std::wstring& constAmount = constOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, constAmount.c_str(), (int)constAmount.length());
				}
				currentY += (int)constRows.size() * 20 + 20;
			}

			// Smelter
			if (!smeltRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Smelter:", 8);
				TextOutW(hdc, 430, currentY, L"Machines:", 9);
				TextOutW(hdc, 610, currentY, L"Watts:", 6);
				TextOutW(hdc, 550, currentY, smeltMachines.c_str(), (int)smeltMachines.length());
				TextOutW(hdc, 690, currentY, smeltWatts.c_str(), (int)smeltWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < smeltRows.size(); ++i) {
					const std::wstring& smeltProduct = smeltRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, smeltProduct.c_str(), (int)smeltProduct.length());
				}
				for (size_t i = 0; i < smeltOutput.size(); ++i) {
					const std::wstring& smeltAmount = smeltOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, smeltAmount.c_str(), (int)smeltAmount.length());
				}
				currentY += (int)smeltRows.size() * 20 + 20;
			}

			// Raw
			if (!rawRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"Raw:", 4);
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < rawRows.size(); ++i) {
					const std::wstring& rawProduct = rawRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, rawProduct.c_str(), (int)rawProduct.length());
				}
				for (size_t i = 0; i < rawOutput.size(); ++i) {
					const std::wstring& rawoutput = rawOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, rawoutput.c_str(), (int)rawoutput.length());
				}
				currentY += (int)rawRows.size() * 20 + 20;
			}
			if (!byRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, startX, currentY, L"BYPRODUCTS:", 11);
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < byRows.size(); ++i) {
					const std::wstring& rawProduct = byRows[i];
					TextOutW(hdc, startX, currentY + (int)i * 20, rawProduct.c_str(), (int)rawProduct.length());
				}
				for (size_t i = 0; i < byOutput.size(); ++i) {
					const std::wstring& rawoutput = byOutput[i];
					TextOutW(hdc, 370, currentY + (int)i * 20, rawoutput.c_str(), (int)rawoutput.length());
				}
			}
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
		}
		break;
	case WM_DESTROY: 
		ViewWinOpen = false;
		// show the main window again when the New window is closed
		if (hMainWnd)
		{
			ShowWindow(hMainWnd, SW_SHOW);
			SetForegroundWindow(hMainWnd);
		}
		break;
	case WM_COMMAND: 
		switch LOWORD(wParam) {
			case View1: {
				view1pressed = 1;
				ViewWinOpen = false;
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
			case VIEWBACKBTN:
				ViewWinOpen = false;
				if (hMainWnd)
				{
					ShowWindow(hMainWnd, SW_SHOW);
					SetForegroundWindow(hMainWnd);
				}
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK NewWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		if (View == 0) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			TextOutW(hdc, 10, 10, L"Enter: Table Name", 18);
			TextOutW(hdc, 10, 60, L"Enter: Output", 13);
			TextOutW(hdc, 10, 110, L"Select Product", 14);
			TextOutW(hdc, 310, 130, L"Product Selected:", 17);
			// Create a larger font for product display
			HFONT hFont = CreateFontW(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial");
			HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
			if (SELECTOR == 1) {
				TextOutW(hdc, 310, 160, PRODUCT_SELECT.c_str(), (int)PRODUCT_SELECT.length());
				if ((int)PRODUCT_SELECT.length() >= 25) {
					UINT paintstate = IsDlgButtonChecked(hWnd, MANUFACTURERBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 620, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}
				else {
					UINT paintstate = IsDlgButtonChecked(hWnd, MANUFACTURERBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 580, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}
				if ((int)PRODUCT_SELECT.length() >= 18) {
					UINT paintstate = IsDlgButtonChecked(hWnd, BLENDERBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 580, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, SMELTBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 580, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, FOUNDRYBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 580, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, QUANTUMBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 580, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, PARTICLEBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 580, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}
				else {
					UINT paintstate = IsDlgButtonChecked(hWnd, BLENDERBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 520, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, SMELTBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 520, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, FOUNDRYBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 520, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, QUANTUMBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 520, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
					paintstate = IsDlgButtonChecked(hWnd, PARTICLEBTN);
					if (paintstate == BST_CHECKED) {
						SetWindowPos(hWnd, NULL,
							0, 0, 520, 840,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					}
				}
			}
			if (TableCheckInt == 1) {
				TextOutW(hdc, 220, 30, L"Table Already Exists", 20);
				TableCheckInt = 0;
			}

			if (createsuccess == 1) {
				TextOutW(hdc, 220, 30, L"Table created Successfully.", 27);
				createsuccess = 0;
			}
			if (newtxterror == 1) {
				TextOutW(hdc, 220, 30, L"Enter a valid Number", 20);
				newtxterror = 0;
			}
			if (jsonerror == 1) {
				TextOutW(hdc, 220, 30, L"Failed to Load Recipe.", 22);
				jsonerror = 0;
			}
			if (deletesuccess == 1) {
				TextOutW(hdc, 220, 30, L"Table Deleted.", 13);
				deletesuccess = 0;
			}
			if (SELECTOR == 2) {
				TextOutW(hdc, 220, 30, L"Select a Recipe", 15);
				SELECTOR = 0;
			}
			SelectObject(hdc, hOldFont);
			DeleteObject(hFont);
			EndPaint(hWnd, &ps);
		}
		if (View == 1) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			int currentY = 10;
			// Particle Accelerator
			if (!partRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Particle Accelerator:", 21);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, partMachines.c_str(), (int)partMachines.length());
				TextOutW(hdc, 680, currentY, partWatts.c_str(), (int)partWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < partRows.size(); ++i) {
					const std::wstring& partProduct = partRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, partProduct.c_str(), (int)partProduct.length());
				}
				for (size_t i = 0; i < partOutput.size(); ++i) {
					const std::wstring& partAmount = partOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, partAmount.c_str(), (int)partAmount.length());
				}
				currentY += (int)partRows.size() * 20 + 20;
			}

			// Quantum Encoder
			if (!quantRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Quantum Encoder:", 16);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, quantMachines.c_str(), (int)quantMachines.length());
				TextOutW(hdc, 680, currentY, quantWatts.c_str(), (int)quantWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < quantRows.size(); ++i) {
					const std::wstring& quantProduct = quantRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, quantProduct.c_str(), (int)quantProduct.length());
				}
				for (size_t i = 0; i < quantOutput.size(); ++i) {
					const std::wstring& quantAmount = quantOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, quantAmount.c_str(), (int)quantAmount.length());
				}
				currentY += (int)quantRows.size() * 20 + 20;
			}

			// Blender
			if (!blenRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Blender:", 8);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, blenMachines.c_str(), (int)blenMachines.length());
				TextOutW(hdc, 680, currentY, blenWatts.c_str(), (int)blenWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < blenRows.size(); ++i) {
					const std::wstring& blenProduct = blenRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, blenProduct.c_str(), (int)blenProduct.length());
				}
				for (size_t i = 0; i < blenOutput.size(); ++i) {
					const std::wstring& blenAmount = blenOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, blenAmount.c_str(), (int)blenAmount.length());
				}
				currentY += (int)blenRows.size() * 20 + 20;
			}

			// Refinery
			if (!refRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Refinery:", 9);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, refMachines.c_str(), (int)refMachines.length());
				TextOutW(hdc, 680, currentY, refWatts.c_str(), (int)refWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < refRows.size(); ++i) {
					const std::wstring& refProduct = refRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, refProduct.c_str(), (int)refProduct.length());
				}
				for (size_t i = 0; i < refOutput.size(); ++i) {
					const std::wstring& refAmount = refOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, refAmount.c_str(), (int)refAmount.length());
				}
				currentY += (int)refRows.size() * 20 + 20;
			}

			// Manufacturer
			if (!manRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Manufacturer:", 13);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, manMachines.c_str(), (int)manMachines.length());
				TextOutW(hdc, 680, currentY, manWatts.c_str(), (int)manWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < manRows.size(); ++i) {
					const std::wstring& manProduct = manRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, manProduct.c_str(), (int)manProduct.length());
				}
				for (size_t i = 0; i < manOutput.size(); ++i) {
					const std::wstring& manAmount = manOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, manAmount.c_str(), (int)manAmount.length());
				}
				currentY += (int)manRows.size() * 20 + 20;
			}

			// Assembler
			if (!assRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Assembler:", 10);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, assMachines.c_str(), (int)assMachines.length());
				TextOutW(hdc, 680, currentY, assWatts.c_str(), (int)assWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < assRows.size(); ++i) {
					const std::wstring& assProduct = assRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, assProduct.c_str(), (int)assProduct.length());
				}
				for (size_t i = 0; i < assOutput.size(); ++i) {
					const std::wstring& assAmount = assOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, assAmount.c_str(), (int)assAmount.length());
				}
				currentY += (int)assRows.size() * 20 + 20;
			}

			// Constructor
			if (!constRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Constructor:", 12);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, constMachines.c_str(), (int)constMachines.length());
				TextOutW(hdc, 680, currentY, constWatts.c_str(), (int)constWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < constRows.size(); ++i) {
					const std::wstring& constProduct = constRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, constProduct.c_str(), (int)constProduct.length());
				}
				for (size_t i = 0; i < constOutput.size(); ++i) {
					const std::wstring& constAmount = constOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, constAmount.c_str(), (int)constAmount.length());
				}
				currentY += (int)constRows.size() * 20 + 20;
			}

			// Smelter
			if (!smeltRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Smelter:", 8);
				TextOutW(hdc, 420, currentY, L"Machines:", 9);
				TextOutW(hdc, 600, currentY, L"Watts:", 6);
				TextOutW(hdc, 540, currentY, smeltMachines.c_str(), (int)smeltMachines.length());
				TextOutW(hdc, 680, currentY, smeltWatts.c_str(), (int)smeltWatts.length());
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < smeltRows.size(); ++i) {
					const std::wstring& smeltProduct = smeltRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, smeltProduct.c_str(), (int)smeltProduct.length());
				}
				for (size_t i = 0; i < smeltOutput.size(); ++i) {
					const std::wstring& smeltAmount = smeltOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, smeltAmount.c_str(), (int)smeltAmount.length());
				}
				currentY += (int)smeltRows.size() * 20 + 20;
			}

			// Raw
			if (!rawRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"Raw:", 4);
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < rawRows.size(); ++i) {
					const std::wstring& rawProduct = rawRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, rawProduct.c_str(), (int)rawProduct.length());
				}
				for (size_t i = 0; i < rawOutput.size(); ++i) {
					const std::wstring& rawoutput = rawOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, rawoutput.c_str(), (int)rawoutput.length());
				}
				currentY += (int)rawRows.size() * 20 + 20;
			}
			// Byproducts
			if (!byRows.empty()) {
				HFONT hBoldFont = CreateFontW(0, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
				TextOutW(hdc, 190, currentY, L"BYPRODUCTS:", 11);
				SelectObject(hdc, hOldFont);
				DeleteObject(hBoldFont);
				currentY += 25;
				for (size_t i = 0; i < byRows.size(); ++i) {
					const std::wstring& rawProduct = byRows[i];
					TextOutW(hdc, 190, currentY + (int)i * 20, rawProduct.c_str(), (int)rawProduct.length());
				}
				for (size_t i = 0; i < byOutput.size(); ++i) {
					const std::wstring& rawoutput = byOutput[i];
					TextOutW(hdc, 360, currentY + (int)i * 20, rawoutput.c_str(), (int)rawoutput.length());
				}
			}

			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
		}
		break;
	case WM_CREATE:
		NewWindowOpen = true;
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		break;
	case WM_DESTROY:
		newtxterror = 0;
		TableCheckInt = 0;
		CreatePushed = 0;
		createsuccess = 0;
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		NewWindowOpen = false;
		// show the main window again when the New window is closed
		if (hMainWnd)
		{
			ShowWindow(hMainWnd, SW_SHOW);
			SetForegroundWindow(hMainWnd);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case NEWACCEPTBTN:
			if (View == 1) {
				//Show Creation Buttons
				ShowWindow(smelter, SW_SHOW);
				ShowWindow(constructor, SW_SHOW);
				ShowWindow(assembler, SW_SHOW);
				ShowWindow(manufacturer, SW_SHOW);
				ShowWindow(foundry, SW_SHOW);
				ShowWindow(refinery, SW_SHOW);
				ShowWindow(blender, SW_SHOW);
				ShowWindow(particle, SW_SHOW);
				ShowWindow(quantum, SW_SHOW);
				ShowWindow(hNewTableBox, SW_SHOW);
				ShowWindow(hNewtxtbox, SW_SHOW);
				ShowWindow(hNewBackButton, SW_SHOW);
				ShowWindow(hCreateButton, SW_SHOW);
				SetWindowText(hNewTableBox, L"");
				SetWindowText(hNewtxtbox, L"");
				SELECTOR = 0;
				PRODUCT_SELECT.clear();
				//Hide Accept, Delete
				ShowWindow(NewAcceptButton, SW_HIDE);
				ShowWindow(NewDeleteButton, SW_HIDE);
				//Set Window Size Back
				SetWindowPos(hWnd, NULL,
					0, 0, 520, 840,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				View = 0;
				createsuccess = 1;
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			}
			break;
		case NEWDELETEBTN:
			if (View == 1) {
				//Show Creation Buttons
				ShowWindow(smelter, SW_SHOW);
				ShowWindow(constructor, SW_SHOW);
				ShowWindow(assembler, SW_SHOW);
				ShowWindow(manufacturer, SW_SHOW);
				ShowWindow(foundry, SW_SHOW);
				ShowWindow(refinery, SW_SHOW);
				ShowWindow(blender, SW_SHOW);
				ShowWindow(particle, SW_SHOW);
				ShowWindow(quantum, SW_SHOW);
				ShowWindow(hNewTableBox, SW_SHOW);
				ShowWindow(hNewtxtbox, SW_SHOW);
				ShowWindow(hNewBackButton, SW_SHOW);
				ShowWindow(hCreateButton, SW_SHOW);
				//Hide Accept, Delete
				ShowWindow(NewAcceptButton, SW_HIDE);
				ShowWindow(NewDeleteButton, SW_HIDE);
				//Set Window Size Back
				SetWindowPos(hWnd, NULL,
					0, 0, 520, 840,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				//Deleting Table Entry
				if (logintype == 0) {
					mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
					mysqlx::Schema myDB = mysession.getSchema("satisfactory");
					mysqlx::Table ViewTable = myDB.getTable(Table_Delete);
					std::string ToDrop = "DROP TABLE IF EXISTS satisfactory.";
					ToDrop = ToDrop + Table_Delete;
					myDB.getSession().sql(ToDrop).execute();
				}
				if (logintype == 1) {
					std::string dropcmd = "DROP TABLE IF EXISTS " + Table_Delete;
					int rc = sqlite3_open("satisfactory.db", &db);
					rc = sqlite3_exec(db, dropcmd.c_str(), nullptr, nullptr, &errorMessage);
					if (rc != SQLITE_OK) {
						DebugBreak();
						return rc;
					}
					sqlite3_close(db);
				}
				
				View = 0;
				deletesuccess = 1;
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			}
			break;
			case SMELTBTN:
				state = IsDlgButtonChecked(hWnd, SMELTBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i < NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i < NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					SELECTOR = 0;
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
			case CONSTRUCTBTN:
				state = IsDlgButtonChecked(hWnd, CONSTRUCTBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 690, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					SELECTOR = 0;
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
			case ASSEMBLERBTN:
				state = IsDlgButtonChecked(hWnd, ASSEMBLERBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(
						hWnd, NULL, 0, 0, 910, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
					);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
			case FOUNDRYBTN:
				state = IsDlgButtonChecked(hWnd, FOUNDRYBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER -1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					SELECTOR = 0;
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
			case MANUFACTURERBTN:
				state = IsDlgButtonChecked(hWnd, MANUFACTURERBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_SHOW);
						UpdateWindow(hWnd);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 580, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
						UpdateWindow(hWnd);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				break;
			case REFINERYBTN:
				state = IsDlgButtonChecked(hWnd, REFINERYBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 580, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				break;
			case BLENDERBTN:
				state = IsDlgButtonChecked(hWnd, BLENDERBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					SELECTOR = 0;
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				break;
			case PARTICLEBTN:
				state = IsDlgButtonChecked(hWnd, PARTICLEBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
				}
				else {
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					SELECTOR = 0;
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				UpdateWindow(hWnd);
				break;
			case QUANTUMBTN:
				state = IsDlgButtonChecked(hWnd, QUANTUMBTN);
				if (state == BST_CHECKED) {
					CheckDlgButton(hWnd, SMELTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_SMELT - 1; i++) {
						ShowWindow(smelterbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_CONST - 1; i++) {
						ShowWindow(constructorbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_ASSEMBLER -1; i++) {
						ShowWindow(assemblerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
						ShowWindow(foundrybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
						ShowWindow(manufacturerbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_REFINERY - 1; i++) {
						ShowWindow(refinerybuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_BLENDER - 1; i++) {
						ShowWindow(blenderbuttons[i], SW_HIDE);
					}
					CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
					for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
						ShowWindow(particlebuttons[i], SW_HIDE);
					}
					InvalidateRect(hWnd, NULL, FALSE);
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_SHOW);
					}
					SELECTOR = 0;
					SetWindowPos(hWnd, NULL,
						0, 0, 520, 840,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
					
				}
				else {
					for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
						ShowWindow(quantumbuttons[i], SW_HIDE);
					}
					
					SELECTOR = 0;
					ShowWindow(hNewtxtbox, SW_SHOW);
					ShowWindow(hNewTableBox, SW_SHOW);
					InvalidateRect(hWnd, NULL, FALSE);
				}
				UpdateWindow(hWnd);
				break;
			case NewBackButton:
				newtxterror = 0;
				TableCheckInt = 0;
				CreatePushed = 0;
				createsuccess = 0;
				SELECTOR = 0;
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				NewWindowOpen = false;
				// show the main window again when the New window is closed
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				if (View != 1) {
					if (hMainWnd)
					{
						ShowWindow(hMainWnd, SW_SHOW);
						SetForegroundWindow(hMainWnd);
					}
				}
				break;
			case CreateButton:
				InvalidateRect(hWnd, NULL, FALSE);
				// Indicate we should paint the selected text and refresh window
				CreatePushed = 1;
				createsuccess = 0;
				if (PRODUCT_SELECT == L"") {
					SELECTOR = 2;
				}
				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);
				//Getting Table Name
				GetWindowTextW(hNewTableBox, ntbbuff, 256);
				if (PRODUCT_SELECT != L"") {
					std::wstring Table(ntbbuff);
					Tableclean = std::string(Table.begin(), Table.end());
					Table_Delete = Tableclean;
					size_t tblsize = Tableclean.length() + 1;
					std::vector<wchar_t> wc(tblsize);
					size_t convertedChars = 0;
					mbstowcs_s(&convertedChars, wc.data(), tblsize, Tableclean.c_str(), _TRUNCATE);
					const wchar_t* Temp2Table = wc.data();

					//Getting Output
					newtxterror = 0;
					GetWindowTextW(hNewtxtbox, Newtxtboxbuff, 256);
					std::wstring Output(Newtxtboxbuff);
					std::string SELECTED_PRODUCT(PRODUCT_SELECT.begin(), PRODUCT_SELECT.end());
					int outputint = 0;
					newtxterror = 0;
					if (Output.empty()) {
						newtxterror = 1; // empty input
						InvalidateRect(hWnd, NULL, TRUE);
					}
					else {
						wchar_t* endptr = nullptr;
						errno = 0;
						long val = wcstol(Output.c_str(), &endptr, 10);
						outputint = static_cast<int>(val);
					}
					if (outputint == 0) {
						newtxterror = 1;
						InvalidateRect(hWnd, NULL, FALSE);
					}
					if (newtxterror == 0) {
						nlohmann::json tdata;
						tdata["Table"] = Tableclean;
						tdata["Output"] = outputint;
						tdata["Product"] = SELECTED_PRODUCT;
						std::ofstream output_file("temptable.json");

						if (!output_file.is_open()) {
							newtxterror = 3;
						}
						else {
							output_file << tdata;
							output_file.close();
						}
						if (CreatePushed == 1) {
							InvalidateRect(hWnd, NULL, FALSE);
							if (logintype == 0) {
								mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
								mysqlx::Schema myDB = mysession.getSchema("satisfactory");
								try {
									mysqlx::Table checktable = myDB.getTable(Tableclean, true);
									TableCheckInt = 1;
								}
								catch (const mysqlx::Error& e) {
									TableCheckInt = 0;
								}
							}
							if (logintype == 1) {
								std::vector<std::wstring> wideTables = Local_tableFetch(db);
								for (int i = 0; i < wideTables.size(); i++) {
									if (Table == wideTables[i]) {
										TableCheckInt = 1;
									}
								}
							}
							if (TableCheckInt == 0) {
								tableFetch();
								try {
									//Hiding Everything to Display the New Table
									CheckDlgButton(hWnd, CONSTRUCTBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_CONST - 1; i++) {
										ShowWindow(constructorbuttons[i], SW_HIDE);
									}
									CheckDlgButton(hWnd, ASSEMBLERBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_ASSEMBLER - 1; i++) {
										ShowWindow(assemblerbuttons[i], SW_HIDE);
									}
									CheckDlgButton(hWnd, FOUNDRYBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_FOUNDRY - 1; i++) {
										ShowWindow(foundrybuttons[i], SW_HIDE);
									}
									CheckDlgButton(hWnd, MANUFACTURERBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_MANUFACTURER - 1; i++) {
										ShowWindow(manufacturerbuttons[i], SW_HIDE);
									}
									CheckDlgButton(hWnd, REFINERYBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_REFINERY - 1; i++) {
										ShowWindow(refinerybuttons[i], SW_HIDE);
									}
									CheckDlgButton(hWnd, BLENDERBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_BLENDER - 1; i++) {
										ShowWindow(blenderbuttons[i], SW_HIDE);
									}
									CheckDlgButton(hWnd, PARTICLEBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_PARTICLE - 1; i++) {
										ShowWindow(particlebuttons[i], SW_HIDE);
									}
									CheckDlgButton(hWnd, QUANTUMBTN, BST_UNCHECKED);
									for (int i = 0; i <= NUM_QUANTUM - 1; i++) {
										ShowWindow(quantumbuttons[i], SW_HIDE);
									}
									InvalidateRect(hWnd, NULL, FALSE);
									for (int i = 0; i < NUM_SMELT - 1; i++) {
										ShowWindow(smelterbuttons[i], SW_HIDE);
									}
									//Hide Creation Buttons
									ShowWindow(smelter, SW_HIDE);
									ShowWindow(constructor, SW_HIDE);
									ShowWindow(assembler, SW_HIDE);
									ShowWindow(manufacturer, SW_HIDE);
									ShowWindow(foundry, SW_HIDE);
									ShowWindow(refinery, SW_HIDE);
									ShowWindow(blender, SW_HIDE);
									ShowWindow(particle, SW_HIDE);
									ShowWindow(quantum, SW_HIDE);
									ShowWindow(hNewTableBox, SW_HIDE);
									ShowWindow(hNewtxtbox, SW_HIDE);
									ShowWindow(hNewBackButton, SW_HIDE);
									ShowWindow(hCreateButton, SW_HIDE);
									//Show Accept, Delete
									ShowWindow(NewAcceptButton, SW_SHOW);
									ShowWindow(NewDeleteButton, SW_SHOW);
									//Set Window Size
									SetWindowPos(
										hWnd, NULL, 0, 0, 910, 840,
										SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
									);
									View = 1;
									InvalidateRect(hWnd, NULL, TRUE);
									UpdateWindow(hWnd);
								}
								catch (const std::runtime_error& e) {
									jsonerror = 1;
									InvalidateRect(hWnd, NULL, FALSE);
								}
							CreatePushed = 0;
							}
						}
					}
				}
				break;
			default:
				if (LOWORD(wParam) >= ID_SMELTER && LOWORD(wParam) < ID_SMELTER + NUM_SMELT) {
					SELECTOR = 1;
					wchar_t smelttext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), smelttext, sizeof(smelttext) / sizeof(smelttext[0]));
					PRODUCT_SELECT = smelttext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_CONSTRUCTOR && LOWORD(wParam) < ID_CONSTRUCTOR + NUM_CONST) {
					SELECTOR = 1;
					wchar_t consttext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), consttext, sizeof(consttext) / sizeof(consttext[0]));
					PRODUCT_SELECT = consttext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_ASSEMBLER && LOWORD(wParam) < ID_ASSEMBLER + NUM_ASSEMBLER) {
					SELECTOR = 1;
					wchar_t assemblertext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), assemblertext, sizeof(assemblertext) / sizeof(assemblertext[0]));
					PRODUCT_SELECT = assemblertext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_FOUNDRY && LOWORD(wParam) < ID_FOUNDRY + NUM_FOUNDRY) {
					SELECTOR = 1;
					wchar_t foundrytext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), foundrytext, sizeof(foundrytext) / sizeof(foundrytext[0]));
					PRODUCT_SELECT = foundrytext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_MANUFACTURER && LOWORD(wParam) < ID_MANUFACTURER + NUM_MANUFACTURER) {
					SELECTOR = 1;
					wchar_t manufacturertext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), manufacturertext, sizeof(manufacturertext) / sizeof(manufacturertext[0]));
					PRODUCT_SELECT = manufacturertext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_REFINERY && LOWORD(wParam) < ID_REFINERY + NUM_REFINERY) {
					SELECTOR = 1;
					wchar_t refinerytext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), refinerytext, sizeof(refinerytext) / sizeof(refinerytext[0]));
					PRODUCT_SELECT = refinerytext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_BLENDER && LOWORD(wParam) < ID_BLENDER + NUM_BLENDER) {
					SELECTOR = 1;
					wchar_t blendertext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), blendertext, sizeof(blendertext) / sizeof(blendertext[0]));
					PRODUCT_SELECT = blendertext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_PARTICLE && LOWORD(wParam) < ID_PARTICLE + NUM_PARTICLE) {
					SELECTOR = 1;
					wchar_t particletext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), particletext, sizeof(particletext) / sizeof(particletext[0]));
					PRODUCT_SELECT = particletext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
				if (LOWORD(wParam) >= ID_QUANTUM && LOWORD(wParam) < ID_QUANTUM + NUM_QUANTUM) {
					SELECTOR = 1;
					wchar_t quantumtext[256];
					GetDlgItemText(hWnd, LOWORD(wParam), quantumtext, sizeof(quantumtext) / sizeof(quantumtext[0]));
					PRODUCT_SELECT = quantumtext;
					selectRegion.left = 310;
					selectRegion.top = 140;
					selectRegion.right = 640;
					selectRegion.bottom = 200;
					InvalidateRect(hWnd, &selectRegion, TRUE);
					break;
				}
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}

LRESULT CALLBACK LoginProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_PAINT:
		if (painter == 1) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			TextOutW(hdc, 40, 10, L"User:", 5);
			TextOutW(hdc, 40, 60, L"Password:", 9);
			TextOutW(hdc, 40, 110, L"Address:", 8);
			TextOutW(hdc, 40, 160, L"Port:", 5);
			if (logincheck == 3) {
				TextOutW(hdc, 80, 10, L"Login Failed.", 13);
				logincheck = 0;
			}
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
		}
		if (painter == 0) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetBkMode(hdc, TRANSPARENT);
			TextOutW(hdc, 10, 10, L"Select Database Type:", 22);
			EndPaint(hWnd, &ps);
			UpdateWindow(hWnd);
		}
		break;
	case WM_CREATE:
		LoginOpen = true;
		if(hMainWnd){ ShowWindow(hMainWnd, SW_HIDE); }
		if (logincheck == 9) {
			SetWindowPos(hWnd, NULL,
				0, 0, 200, 160,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			logincheck = 0;
		}
		break;
	case WM_DESTROY:
		LoginOpen = false;
		if (logincheck != 4) { logincheck = 5; }
		if (hMainWnd)
		{
			ShowWindow(hMainWnd, SW_SHOW);
			SetForegroundWindow(hMainWnd);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case LOGINLOCAL:
			LoginOpen = false;
			logintype = 1;
			logincheck = 4;
			loginJson["TYPE"] = 1;
			loginJson["ID"] = sID;
			loginJson["ADD"] = sADD;
			loginJson["PORT"] = sPORT;
			loginJson["PASS"] = sPASS;
			if (logintype == 1) {
				std::ofstream of(templogpath);
				if (!of.is_open()) {
					logincheck = 5;
				}
				else {
					of << loginJson;
					of.close();
				}
			}

			PostMessage(hWnd, WM_CLOSE, 0, 0);
			if (hMainWnd)
			{
				ShowWindow(hMainWnd, SW_SHOW);
				SetForegroundWindow(hMainWnd);
			}
			break;
		case LOGINREMOTE:
			painter = 1;
			ShowWindow(hLogin, SW_SHOW);
			ShowWindow(hLoginExit, SW_SHOW);
			ShowWindow(hLoginAdd, SW_SHOW);
			ShowWindow(hLoginPort, SW_SHOW);
			ShowWindow(hLoginRemember, SW_SHOW);
			ShowWindow(hLoginUID, SW_SHOW);
			ShowWindow(hLoginUP, SW_SHOW);
			ShowWindow(hLoginRemote, SW_HIDE);
			ShowWindow(hLoginLocal, SW_HIDE);
			ShowWindow(hLoginRemember2, SW_HIDE);
			SetWindowPos(hWnd, NULL, 0, 0, 300, 400, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			InvalidateRect(hWnd, NULL, TRUE);
			loginJson["TYPE"] = 0;
			break;
		case LOGINBTN:
			try {
				if (logincheck == 0) {
					logintype = 0;
					GetWindowTextW(hLoginAdd, loginaddressbuff, 256);
					GetWindowTextW(hLoginPort, loginportbuff, 256);
					std::wstring ADD(loginaddressbuff);
					std::wstring PORT(loginportbuff);
					sADD = std::string(ADD.begin(), ADD.end());
					sPORT = std::string(PORT.begin(), PORT.end());
					PORT_INT = std::stoi(sPORT);
					GetWindowTextW(hLoginUID, loginidbuff, 256);
					GetWindowTextW(hLoginUP, loginpassbuff, 256);
					ID = loginidbuff;
					PASS = loginpassbuff;
					sID = std::string(ID.begin(), ID.end());
					sPASS = std::string(PASS.begin(), PASS.end());
					mysqlx::Session mysession(sADD, PORT_INT, sID, sPASS);
					logincheck = 4;
					loginJson["ID"] = sID;
					loginJson["ADD"] = sADD;
					loginJson["PORT"] = sPORT;
					loginJson["PASS"] = sPASS;
					loginJson["UPDATE"] = 1;
					std::ofstream of(templogpath);
					if (!of.is_open()) {
						logincheck = 5;
					}
					else {
						of << loginJson;
						of.close();
					}
					state = IsDlgButtonChecked(hWnd, LOGINREMEMBER);
					if (state == BST_CHECKED) {
						std::ofstream outfile(loginPath);
						if (!outfile.is_open()) {
							logincheck = 5;
						}
						else {
							outfile << loginJson;
							outfile.close();
						}
					}
					
				}
				if (logincheck == 4) {
					LoginOpen = false;
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					if (hMainWnd)
					{
						ShowWindow(hMainWnd, SW_SHOW);
						SetForegroundWindow(hMainWnd);
					}
					logincheck = 4;
					break;
				}
				
			}
			catch (const mysqlx::Error& e) {
				logincheck = 3;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case LOGINEXIT:
			painter = 0;
			ShowWindow(hLogin, SW_HIDE);
			ShowWindow(hLoginExit, SW_HIDE);
			ShowWindow(hLoginAdd, SW_HIDE);
			ShowWindow(hLoginPort, SW_HIDE);
			ShowWindow(hLoginRemember, SW_HIDE);
			ShowWindow(hLoginUID, SW_HIDE);
			ShowWindow(hLoginUP, SW_HIDE);
			ShowWindow(hLoginRemote, SW_SHOW);
			ShowWindow(hLoginLocal, SW_SHOW);
			ShowWindow(hLoginRemember2, SW_SHOW);
			SetWindowPos(hWnd, NULL,
				0, 0, 200, 200,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}