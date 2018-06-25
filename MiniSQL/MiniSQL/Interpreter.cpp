#include "Interpreter.h"

using namespace std;

Interpreter::Interpreter()
{
	sqlcmd.tableInfo.recLength = 0;
	sqlcmd.tableInfo.attriNum = 0;
}

Interpreter::~Interpreter()
{  

}

void Interpreter::readquery(void)
{	
	string temp, s = "";
	tokenizer.clear();

	while (s == "" || (s.length() > 0 && s[s.length() - 1] != ';')) 
	{
		if (s == "")
			cout << "MiniSQL" << "> ";
		else
			cout << "       " << "> ";
		getline(cin, temp);
		if(s == "")
			s = s + trim(temp);
		else
			s = s + " " + trim(temp);
	}
	query = s;
}

int Interpreter::parser(void)
{	
	sqlcmd.clear();

	vector<string>::iterator it = tokenizer.begin();

	int left = 0;
	int right = 0;

	if (*it == "create")
	{
		it++;

		if (*it == "index") 
		{
			sqlcmd.opName = 2;
			it++;
			sqlcmd.indexInfo.indexName = *it;
			it++;
			if (*it != "on")
			{
				cerr << "SQL Syntax Error! " << endl;
				return 1;
			}
			it++;
			sqlcmd.indexInfo.tableName = *it;
			it++;
			if (*it != "(")
			{
				cerr << "SQL Syntax Error! " << endl;
				return 1;
			}
			it++;
			sqlcmd.indexInfo.attributeName = *it;
			it++;
			if (*it != ")")
			{
				cerr << "SQL Syntax Error! " << endl;
				return 1;
			}
		}

		else if (*it == "table") 
		{
			it++;
			sqlcmd.opName = 0;
			sqlcmd.tableInfo.name = *it;
			
			it++;
			vector<string> temp;

			while (*it != ";")
			{
				
				if (*it == "(" || *it == ")")
				{
					it++;
					continue;
				}
				if (*it == "primary")
				{
					it += 3;
					for (vector<Attribute>::iterator t = sqlcmd.tableInfo.attributes.begin(); t != sqlcmd.tableInfo.attributes.end(); t++)
					{
						if (t->name == *it) 
						{
							t->PK = true;
							break;
						}

					}
					break;
				}
				if (*it == ",") 
				{
					if (temp.size() == 2 && temp[1] == "int")
					{
						sqlcmd.tableInfo.attributes.push_back(Attribute(temp[0], 0, false, false));
						sqlcmd.tableInfo.recLength += 4;
						sqlcmd.tableInfo.attriNum++;
						temp.clear();
					}

					else if (temp.size() == 2 && temp[1] == "float")
					{
						sqlcmd.tableInfo.attributes.push_back(Attribute(temp[0], -1, false, false));
						sqlcmd.tableInfo.recLength += 4;
						sqlcmd.tableInfo.attriNum++;
						temp.clear();
					}

					else if (temp.size() == 3 && temp[1] == "int")
					{
						sqlcmd.tableInfo.attributes.push_back(Attribute(temp[0], 0, false, true));
						sqlcmd.tableInfo.recLength += 4;
						sqlcmd.tableInfo.attriNum++;
						temp.clear();
					}
					else if (temp.size() == 3 && temp[1] == "float")
					{
						sqlcmd.tableInfo.attributes.push_back(Attribute(temp[0], -1, false, true));
						sqlcmd.tableInfo.recLength += 4;
						sqlcmd.tableInfo.attriNum++;
						temp.clear();
					}
					else if (temp.size() == 3 && temp[1] == "char")
					{
						sqlcmd.tableInfo.attributes.push_back(Attribute(temp[0], atoi(temp[2].c_str()), false, false));
						sqlcmd.tableInfo.recLength += atoi(temp[2].c_str());
						sqlcmd.tableInfo.attriNum++;
						temp.clear();
					}
					else if (temp.size() == 4 && temp[1] == "char")
					{
						sqlcmd.tableInfo.attributes.push_back(Attribute(temp[0], atoi(temp[2].c_str()), false, true));
						sqlcmd.tableInfo.recLength += atoi(temp[2].c_str());
						sqlcmd.tableInfo.attriNum++;
						temp.clear();
					}
					else {
						cerr << "SQL Syntax Error! " << endl;
						return 1;
					}
				}
				else 
				{
					temp.push_back(*it);
				}
				it++;
			}

		}

		else 
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}
	}

	else if (*it == "select")
	{
		if (tokenizer.size() < 5)
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}

		if (tokenizer[1] != "*" || tokenizer[2] != "from")
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}

		sqlcmd.tableInfo.name = tokenizer[3];
		sqlcmd.opName = 4;

		if (tokenizer[4] == "where")
		{
			it += 5;
			
			while (*(it + 3) != ";")
			{
				Condition * temp = new Condition;
				(*temp).attriName = *it;
				(*temp).cond = *(it + 2);
				if (*(it + 1) == ">") {
					(*temp).opcode = 0;
				}
				if (*(it + 1) == "<") {
					(*temp).opcode = 1;
				}
				if (*(it + 1) == ">=") {
					(*temp).opcode = 2;
				}
				if (*(it + 1) == "<=") {
					(*temp).opcode = 3;
				}
				if (*(it + 1) == "=") {
					(*temp).opcode = 4;
				}
				if (*(it + 1) == "<>") {
					(*temp).opcode = 5;
				}
				sqlcmd.condList.push_back(*temp);
				it += 4;
			}
			Condition * temp = new Condition;
			(*temp).attriName = *it;
			(*temp).cond = *(it + 2);
			if (*(it + 1) == ">") {
				(*temp).opcode = 0;
			}
			if (*(it + 1) == "<") {
				(*temp).opcode = 1;
			}
			if (*(it + 1) == ">=") {
				(*temp).opcode = 2;
			}
			if (*(it + 1) == "<=") {
				(*temp).opcode = 3;
			}
			if (*(it + 1) == "=") {
				(*temp).opcode = 4;
			}
			if (*(it + 1) == "<>") {
				(*temp).opcode = 5;
			}
			sqlcmd.condList.push_back(*temp);
		}
	}

	else if (*it == "drop")
	{
		it++;

		if (*it == "table")
		{
			sqlcmd.opName = 1;
			it++;
			if (*it == ";")
			{
				cerr << "SQL Syntax Error! " << endl;
				return 1;
			}
			sqlcmd.tableInfo.name = *it;
		}

		else if (*it == "index")
		{
			sqlcmd.opName = 3;
			it++;
			if (*it == ";") {
				cerr << "SQL Syntax Error! " << endl;
				return 1;
			}
			sqlcmd.indexInfo.indexName = *it;
		}

		else
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}
	}

	else if (*it == "insert")
	{
		it++;
		sqlcmd.opName = 5;
		if (*it != "into")
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}

		it++;

		sqlcmd.tableInfo.name = *it;

		it++;

		if (*it != "values")
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}
		it++;

		while (*it != ";")
		{
			if (*it != "(" && *it != "," && *it != ")")
			{
				sqlcmd.valuesList.push_back(*it);
			}
			it++;
		}

	}

	else if (*it == "delete")
	{
		it++;
		if (*it != "from")
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}
		it++;

		sqlcmd.tableInfo.name = *it;

		it++;

		if (*it == ";")
		{

		}
		else if (*it == "where")
		{
			it ++;

			while (*(it + 3) != ";")
			{
				Condition * temp = new Condition;
				(*temp).attriName = *it;
				(*temp).cond = *(it + 2);
				if (*(it + 1) == ">") {
					(*temp).opcode = 0;
				}
				if (*(it + 1) == "<") {
					(*temp).opcode = 1;
				}
				if (*(it + 1) == ">=") {
					(*temp).opcode = 2;
				}
				if (*(it + 1) == "<=") {
					(*temp).opcode = 3;
				}
				if (*(it + 1) == "=") {
					(*temp).opcode = 4;
				}
				if (*(it + 1) == "<>") {
					(*temp).opcode = 5;
				}
				sqlcmd.condList.push_back(*temp);
				it += 4;
			}
			Condition * temp = new Condition;
			(*temp).attriName = *it;
			(*temp).cond = *(it + 2);
			if (*(it + 1) == ">") {
				(*temp).opcode = 0;
			}
			if (*(it + 1) == "<") {
				(*temp).opcode = 1;
			}
			if (*(it + 1) == ">=") {
				(*temp).opcode = 2;
			}
			if (*(it + 1) == "<=") {
				(*temp).opcode = 3;
			}
			if (*(it + 1) == "=") {
				(*temp).opcode = 4;
			}
			if (*(it + 1) == "<>") {
				(*temp).opcode = 5;
			}
			sqlcmd.condList.push_back(*temp);
		}
		else 
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}
	}

	else if (*it == "quit" || *it == "exit")
	{
		return 0;
	}

	else if (*it == "execfile")
	{
		if (tokenizer.size() != 3)
		{
			cerr << "SQL Syntax Error! " << endl;
			return 1;
		}
	}

	else 
	{
		cerr << "SQL Syntax Error! " << endl;
		return 1;
	}

	return 0;
	
}

void Interpreter::execute(void)
{

}

void Interpreter::executefile(void)
{
	file.open(tokenizer[1], ios::in);

	if (!file)
	{
		cerr << "Cannot open file \"" << tokenizer[1] << "\"!" << endl;
		return;
	}

	string line;


	while (!file.eof())
	{
		getline(file, line);
		tokenizer.clear();
		query = trim(line);
		execute();
	}
	file.close();
}

string & Interpreter::trim(string & str)
{
	string res = "";

	vector<string>::iterator it;

	vector<string> token;

	if (str != "")
	{
		str.erase(0, str.find_first_not_of(" \n\r\t"));
		str.erase(str.find_last_not_of(" \n\r\t") + 1);
	}
	else
		return str;

	str.append(" ");

	for (int i = 0; i < str.length(); i++)
	{
		char ch = str[i];

		if (ch == '(')
		{
			if (res != "") 
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
				
			res = "";
			token.push_back("(");
			tokenizer.push_back("(");
		}

		else if (ch == ')')
		{
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";
			token.push_back(")");
			tokenizer.push_back(")");
		}

		else if (ch == '*')
		{
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";
			token.push_back("*");
			tokenizer.push_back("*");
		}

		else if (ch == ';')
		{
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";
			token.push_back(";");
			tokenizer.push_back(";");
		}

		else if (ch == ',')
		{
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";
			token.push_back(",");
			tokenizer.push_back(",");
		}

		else if (ch == '=')
		{
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";
			token.push_back("=");
			tokenizer.push_back("=");
		}

		else if (ch == '>')
		{
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";

			if (str[i + 1] == '=')
			{
				token.push_back(">=");
				tokenizer.push_back(">=");
				i++;
			}
			else 
			{
				token.push_back(">");
				tokenizer.push_back(">");
			}
				
		}

		else if (ch == '<')
		{
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";

			if (str[i + 1] == '=')
			{
				token.push_back("<=");
				tokenizer.push_back("<=");
				i++;
			}
			else if (str[i + 1] == '>') 
			{
				token.push_back("<>");
				tokenizer.push_back("<>");
				i++;
			}
			else
			{
				token.push_back("<");
				tokenizer.push_back("<");
			}

		}


		else if (ch != ' ' && ch != '\n' && ch != '\t' && ch != '\r' && ch != '"' && ch != 0x27)
		{
			res += ch;
		}

		else {
			if (res != "")
			{
				token.push_back(res);
				tokenizer.push_back(res);
			}
			res = "";
		}
	}
	res = "";
	it = token.begin();
	res += *it;

	for (it = it + 1; it != token.end(); it++)
		res += " " + *it;

	str = res;
	return str;
}

void Interpreter::token_debug(void)
{	
	int i = 0;
	cout << "-----------------------------" << endl;
	cout << "Token: " << endl;
	for (vector<string>::iterator it = tokenizer.begin(); it != tokenizer.end(); it++)
	{
		cout << "Token " << i++ << ": " << *it << endl;
	}
	cout << "-----------------------------" << endl;
}

void Interpreter::parse_debug(void)
{
	sqlcmd.tableInfo.show();
}
