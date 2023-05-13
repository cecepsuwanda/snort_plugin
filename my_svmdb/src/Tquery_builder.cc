#include "Tquery_builder.h"



Tquery_builder::Tquery_builder()
{
	try {

		auto data = parse_ini_file("config.ini");
		auto db_host = data["database"]["host"];
		auto db_name = data["database"]["name"];
		auto db_user = data["database"]["user"];
		auto db_pass = data["database"]["password"];

		_url = db_host;
		_user = db_user;
		//_user = "cecepsuwanda";
		_pass = db_pass;
		_database = db_name;

	}
	catch (const std::exception& ex) {
		// Handle any exceptions that may occur
		cerr << "Error: " << ex.what() << endl;
	}

	_con = NULL;
	_result = NULL;
}

Tquery_builder::~Tquery_builder()
{
	//close_connection();
}

map<string, map<string, string>> Tquery_builder::parse_ini_file(const string& filename)
{
	map<string, map<string, string>> data;

	ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file: " + filename);
	}

	string line;
	string current_section;
	while (getline(file, line)) {

		line.erase(line.find_last_not_of(" \t\r\n") + 1);

		if (line.empty() || line[0] == ';') {
			continue;
		}

		// Check if the line contains a section
		if (line[0] == '[' && line[line.size() - 1] == ']') {
			current_section = line.substr(1, line.size() - 2);
			map<string, string> key_value;
			data.insert(pair<string, map<string, string>>(current_section, key_value));
		}
		else {
			// Split the line into a key-value pair
			istringstream iss(line);
			string key, value;
			if (getline(iss, key, '=')) {
				if (getline(iss, value)) {
					// Remove any leading or trailing whitespace from the value
					value.erase(0, value.find_first_not_of(" \t\r\n"));
					value.erase(value.find_last_not_of(" \t\r\n") + 1);

					key.erase(0, key.find_first_not_of(" \t\r\n"));
					key.erase(key.find_last_not_of(" \t\r\n") + 1);

					auto itr = data.find(current_section);
					itr->second.insert(pair<string, string>(key, value));

					// Add the key-value pair to the current section
					//data[current_section].insert(pair<string,string>(key,value));
				}
			}
		}
	}

	return data;
}

void Tquery_builder::finish_with_error()
{
	string tmp = string(mysql_error(_con)) + " \n";
	perror(tmp.c_str());
	mysql_close(_con);
}

void Tquery_builder::open_connection()
{
	_con = mysql_init(NULL);

	if (_con == NULL)
	{
		perror(" mysql_init() failed \n");
	} else {
		if (mysql_real_connect(_con, _url.c_str(), _user.c_str(), _pass.c_str(), _database.c_str(), 0, NULL, 0) == NULL)
		{
			finish_with_error();
		}
	}
}

bool Tquery_builder::query(string sql)
{
	bool hsl = true;
	if (mysql_query(_con, sql.c_str()))
	{
		hsl = false;
		cout << sql << endl;
		finish_with_error();
	}
	return hsl;
}

bool Tquery_builder::get_result()
{
	bool hsl = true;
	_result = mysql_store_result(_con);
	if (_result == NULL)
	{
		hsl = false;
		finish_with_error();
	}
	return hsl;
}

int Tquery_builder::get_jml_row()
{
	int hsl = 0;
	if (_result != NULL)
	{
		hsl = mysql_num_rows(_result);
	}
	return hsl;
}

int Tquery_builder::get_jml_col()
{
	int hsl = 0;
	if (_result != NULL)
	{
		hsl = mysql_num_fields(_result);
	}
	return hsl;
}

vector<string> Tquery_builder::get_column_name()
{
	int jml_col = get_jml_col();
	vector<string> tmp;
	if (_result != NULL)
	{
		for (int i = 0; i < jml_col; ++i)
		{
			MYSQL_FIELD *field = mysql_fetch_field_direct(_result, i);
			tmp.push_back(field->name);
		}
	}
	return tmp;
}

vector<string> Tquery_builder::get_column_type()
{
	int jml_col = get_jml_col();
	vector<string> tmp;
	if (_result != NULL)
	{
		for (int i = 0; i < jml_col; ++i)
		{
			MYSQL_FIELD *field = mysql_fetch_field_direct(_result, i);
			if (field->type == FIELD_TYPE_VAR_STRING)
			{
				tmp.push_back("symbolic.");
			} else {
				tmp.push_back("continuous.");
			}
		}
	}
	return tmp;
}

vector<string> Tquery_builder::fetch_row()
{
	int jml_col = get_jml_col();
	vector<string> tmp;
	if (_result != NULL)
	{
		MYSQL_ROW row = mysql_fetch_row(_result);
		if (row != NULL) {
			for (int i = 0; i < jml_col; i++)
			{
				tmp.push_back(row[i]);
			}
		}
	}
	return tmp;
}

void Tquery_builder::go_to_no_record(int no_rec)
{
	if (_result != NULL)
	{
		mysql_data_seek(_result, no_rec);
	}
}

void Tquery_builder::close_connection()
{
	if (_result != NULL) {
		mysql_free_result(_result);
		_result = NULL;
	}
	if (_con != NULL) {
		mysql_close(_con);
		_con = NULL;
	}
}