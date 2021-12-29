#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Source.h"
using namespace std;

bool in_array(const std::string& value, const std::vector<std::string>& array)
{
	return std::find(array.begin(), array.end(), value) != array.end();
}

int main() {
	ifstream inputFile;
	ofstream outputFile;
	inputFile.open("input2.txt");
	outputFile.open("output2.txt", ios::trunc);
	vector<string> myKeywords = { "break", "case", "char", "const", "default", "do", "double", "else", "float", "for", "if", "int", "long", "return", "short", "switch", "unsigned", "void", "volatile", "while" };
	vector<string> myOperators = { "+", "-", "*", "/", "%", "<", ">", "<=", ">=", "==", "!=", "!","&&", "||", "="};
	vector<string> mySeparators = { ",", ":", ";", " "};

	if (inputFile.is_open()) { 
		string line;
		int counter = 0;
		vector<string> lines;
		while (getline(inputFile, line)) {

			if (line != "")
				lines.push_back(line);
		}
		inputFile.close();
		
		string delimiter = " ";
		vector<vector<string>> splitLines(lines.size());
		size_t pos = 0;
		string token;
		string delimiter2 = "	";
		size_t pos2 = 0;

		for (int i = 0; i < lines.size(); i++) {
			string s = lines[i];
			if (s.rfind("	", 0) == 0) {
				pos2 = s.find(delimiter2);
				if (pos2 != std::string::npos)
				{
					// If found then erase it from string
					s.erase(pos2, delimiter2.length());
				}
			}
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				splitLines[i].push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			splitLines[i].push_back(s);
		}
		
		vector<string> outputText(splitLines.size());
		for (int i = 0; i < splitLines.size(); i++)
		{
			string line;
			string outputline;
			for (std::vector<std::string>::const_iterator ii = splitLines[i].begin(); ii != splitLines[i].end(); ++ii)
				line += *ii + " ";
			outputline = "line " + to_string(i + 1) + ": " + line;
			if(outputline != " " || outputline != "\n")
				outputText.push_back(outputline);
			for (int j = 0; j < splitLines[i].size(); j++)
			{
				if (in_array(splitLines[i][j], myKeywords))
				{
					outputline = "('" + splitLines[i][j] +"', keyword)";
					outputText.push_back(outputline);
				}
				else if (in_array(splitLines[i][j], myOperators))
				{
					outputline = "('" + splitLines[i][j] + "', operator)";
					outputText.push_back(outputline);
				}
				else if (in_array(splitLines[i][j], mySeparators))
				{
					outputline = "('" + splitLines[i][j] + "', separator)";
					outputText.push_back(outputline);
				}
				else if (splitLines[i][j].rfind("//", 0) == 0)
				{
					string commentLine;
					int skipCounter = 0;
					for (std::vector<std::string>::const_iterator ii = splitLines[i].begin(); ii != splitLines[i].end(); ++ii) {
						commentLine += *ii + " ";
						skipCounter++;
					}
					outputline = "('" + commentLine + "', comment)";
					outputText.push_back(outputline);
					j = j + skipCounter-1;
				}
				else if (splitLines[i][j].rfind("#define", 0) == 0)
				{
					string constLine;
					int skipCounter = 0;
					for (std::vector<std::string>::const_iterator ii = splitLines[i].begin(); ii != splitLines[i].end(); ++ii) {
						constLine += *ii + " ";
						skipCounter++;
					}
					outputline = "('" + constLine + "', constant)";
					outputText.push_back(outputline);
					j = j + skipCounter - 1;
				}
				else if (!in_array(splitLines[i][j], myOperators)) {
					if (!in_array(splitLines[i][j], myKeywords)) {
						if (!in_array(splitLines[i][j], mySeparators)) {
							if (splitLines[i][j].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos)
							{
								outputline = "('" + splitLines[i][j] + "', unknown)";
								outputText.push_back(outputline);
							}
							else {
								outputline = "('" + splitLines[i][j] + "', identificator)";
								outputText.push_back(outputline);
							}
						}
					}
				}
			}
			outputText.push_back("\n");
		}

		for (int i = 0; i < outputText.size(); i++)
		{
			if (!outputText[i].length() == 0) {
				cout << outputText[i] << " ";
				cout << endl;
			}
		}

		if (outputFile.is_open()) {
			for (int i = 0; i < outputText.size(); i++)
			{
				if (!outputText[i].length() == 0) {
					outputFile << outputText[i] << " ";
					outputFile << endl;
				}
			}
			outputFile.close();
		}
	}

	return 0;
}

