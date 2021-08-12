#include "Controller.h"
#include "ControllerCoreForCharTree.h"
#include "ControllerCoreForString.h"

#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/split.hpp>

#define INCORRECT_VALUE_ERROR "Incorrect value"
#define STRING_INPUT_ERROR "You need to enter value"

using namespace std;

string Controller::processInput(string input)
{
	boost::trim(input);
	boost::to_lower(input);

	if (boost::starts_with(input, "add")) {
		vector<string> vec;
		boost::split(vec, input, [](char a) {return a == ' '; });

		if (vec.size() <= 1)
			return STRING_INPUT_ERROR;

		if (_core->addToTree(vec[1])) {
			return string("Key " + vec[1] + " added");
		}

		return INCORRECT_VALUE_ERROR;
	}

	if (boost::starts_with(input, "find")) {
		vector<string> vec;
		boost::split(vec, input, [](char a) {return a == ' '; });

		if (vec.size() <= 1)
			return STRING_INPUT_ERROR;

		string error;
		if (_core->findInTree(vec[1], error)) {
			return string("Key " + vec[1] + " is in tree");
		} else {
			return error;
		}

		return INCORRECT_VALUE_ERROR;
	}

	if (boost::starts_with(input, "del")) {
		vector<string> vec;
		boost::split(vec, input, [](char a) {return a == ' '; });

		if (vec.size() <= 1)
			return STRING_INPUT_ERROR;

		string error;
		if (_core->removeFromTree(vec[1], error)) {
			return string("Key " + vec[1] + " removed from tree");
		}
		else {
			return error;
		}

		return INCORRECT_VALUE_ERROR;
	}

	if (boost::starts_with(input, "print")) {
		return _core->printTree();
	}

	if (boost::starts_with(input, "type")) {

		if (boost::contains(input, "int")) {
			vector<string> vec;
			boost::split(vec, input, [](char a) {return a == ' '; });

			if (vec.size() < 3)
				return STRING_INPUT_ERROR;

			try {
				ControllerCore* oldCore = _core;
				_core = new ControllerCoreForIntTree(stoi(vec[2], NULL, 0));
				delete oldCore;
				return "Tree was recreated";
			}
			catch (const std::invalid_argument& exp) {
				return exp.what();
			}

			return INCORRECT_VALUE_ERROR;
		}
		
		if (boost::contains(input, "char")) {
			vector<string> vec;
			boost::split(vec, input, [](char a) {return a == ' '; });

			if (vec.size() < 3)
				return STRING_INPUT_ERROR;

			try {
				ControllerCore* oldCore = _core;
				_core = new ControllerCoreForCharTree(stoi(vec[2], NULL, 0));
				delete oldCore;
				return "Tree was recreated";
			}
			catch (const std::invalid_argument& exp) {
				return exp.what();
			}

			return INCORRECT_VALUE_ERROR;
		}

		if (boost::contains(input, "str")) {
			vector<string> vec;
			boost::split(vec, input, [](char a) {return a == ' '; });

			if (vec.size() < 3)
				return STRING_INPUT_ERROR;

			try {
				ControllerCore* oldCore = _core;
				_core = new ControllerCoreForString(stoi(vec[2], NULL, 0));
				delete oldCore;
				return "Tree was recreated";
			}
			catch (const std::invalid_argument& exp) {
				return exp.what();
			}

			return INCORRECT_VALUE_ERROR;
		}

		return INCORRECT_VALUE_ERROR;
	}

	if (boost::starts_with(input, "randomise")) {
		vector<string> vec;
		boost::split(vec, input, [](char a) {return a == ' '; });

		if (vec.size() <= 1)
			return STRING_INPUT_ERROR;


		try {
			if (int size = stoi(vec[1], NULL, 0)) {
				_core->randomiseTree(size);
				return string("New nodes were added to Tree");
			}
		}
		catch (std::invalid_argument& e) {
			return e.what();
		}

		return INCORRECT_VALUE_ERROR;
	}

	if (boost::starts_with(input, "help"))
		return printHelp();

	if (boost::starts_with(input, "!"))
		exit(0);

	return INCORRECT_VALUE_ERROR;
}

std::string Controller::printHelp()
{
	stringstream result;

	result << "BTree v0.1" << endl;
	result << "1) To add element print: add <element>" << endl;
	result << "2) To find element print: find <element>" << endl;
	result << "3) To remove element print: del <element>" << endl;
	result << "4) To show tree print: print" << endl;
	result << "5) To change tree type print: type <int/char/str> <order>" << endl;
	result << "6) To show this help print: help" << endl;
	result << "7) To create randomTree: randomise size" << endl;
	result << "8) To exit print: !";

	return result.str();
}
