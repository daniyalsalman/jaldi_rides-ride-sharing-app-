#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "UserManagement.h"
#include <regex>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
using namespace std;

time_t now = time(NULL);
tm* timePtr = localtime(&now);
const int current_year = 1900 + timePtr->tm_year; // tm_year is relative to 1900

bool doesPhoneNumOrNicExist(string str, bool isItPhoneNum, string fileName)
{
    ifstream file_in(fileName);
    istringstream ss;
    bool phoneNumOrNicExists = false;

    if (file_in)
    {
        string line, thisStr;

        int skip = isItPhoneNum ? 6 : 8;

        while (getline(file_in, line))
        {
            ss.clear();
            ss.str(line);

            for (int i = 0; i < skip; i++)
            {
                getline(ss, thisStr, ',');
            }

            getline(ss, thisStr, ',');

            if (thisStr == str)
            {
                phoneNumOrNicExists = true;
                break;
            }
        }
    }

    file_in.close();
    return phoneNumOrNicExists;
}

int countOccurrences(string str, char ch)
{
    int occurrences = 0;

    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ch)
            occurrences++;
    }

    return occurrences;
}

bool containsComma(string str)
{
    return countOccurrences(str, ',') >= 1;
}

bool isValidName(string name)
{
    bool valid = name.length() >= 1 && name.length() <= 30;

    if (!valid)
        return valid;

    for (int i = 0; i < name.length(); i++)
    {
        // includes comma check
        if (!isalnum(name[i]))
        {
            valid = false;
            return valid;
        }
    }

    return valid;
}

bool isPhoneNumValid(string phoneNum)
{
    bool valid = phoneNum.length() == 11;

    if (!valid)
        return false;

    if (phoneNum[0] != '0' || phoneNum[1] != '3')
    {
        valid = false;
        return valid;
    }

    for (int i = 0; i < phoneNum.length(); i++)
    {
        // includes comma check
        if (!isdigit(phoneNum[i]))
        {
            valid = false;
            break;
        }
    }

    return valid;
}

bool isValidNIC(string nic)
{
    bool valid = nic.length() == 13;

    if (!valid)
        return valid;

    if (nic[0] != '4')
    {
        valid = false;
        return valid;
    }

    for (int i = 0; i < nic.length(); i++)
    {
        if (!isdigit(nic[i]))
        {
            // includes comma check
            valid = false;
            return valid;
        }
    }

    return valid;
}

bool isValidPlateNum(string plateNum)
{
    // AA-111
    // AAA-1111
    regex plateNumRegex("[A-Z]{2,3}-[0-9]{3,4}");
    return regex_match(plateNum, plateNumRegex);
}

int menu1()
{
    int opt;

    do
    {
        cout << endl
            << "Enter a number for one of the following commands:" << endl;
        cout << "1) Signup" << endl;
        cout << "2) Login" << endl;
        cout << "3) Exit" << endl;
        cin >> opt;
    } while (opt != 1 && opt != 2 && opt != 3);

    return opt;
}

int menu2()
{
    int opt = 0;

    do
    {
        cout << endl
            << "1) As passenger" << endl;
        cout << "2) As driver" << endl;
        cout << "3) Go back" << endl;
        cin >> opt;
    } while (opt != 1 && opt != 2 && opt != 3);

    return opt;
}

string typesMenu(bool editing)
{
    int opt = 0;

    do
    {
        cout << endl
            << "1) Jaldi Rickshaw [3 passengers]" << endl;
        cout << "2) Jaldi Bike [1 passenger]" << endl;
        cout << "3) JaldiX [A private ride at an everyday price] [4 passengers]" << endl;
        cout << "4) JaldiXL [Affordable rides for groups up to 6] [6 passengers]" << endl;
        cout << "5) Jaldi Black [Premium rides in luxury cars]" << endl;
        cin >> opt;
        if (editing && opt == -1)
            return "-";
    } while (opt < 1 || opt > 5);

    if (opt == 1)
        return "Jaldi Rickshaw";
    else if (opt == 2)
        return "Jaldi Bike";
    else if (opt == 3)
        return "JaldiX";
    else if (opt == 4)
        return "JaldiXL";
    else
        return "Jaldi Black";
}

int getLastId(string fileName)
{
    ifstream file_in(fileName);
    istringstream ss;

    string returnId = "0";

    if (file_in)
    {
        string line, thisIdStr;

        while (getline(file_in, line))
        {
            ss.clear();
            ss.str(line);

            getline(ss, thisIdStr, ',');
            returnId = thisIdStr;
        }
    }

    file_in.close();
    return stoi(returnId);
}

string getCurrentTime()
{
    time_t current_time;
    current_time = time(NULL);
    string time = ctime(&current_time);
    time[time.length() - 1] = ' ';
    return time;
}

istringstream ss;

void getFields(string line, string** fields, int num_fields)
{
    ss.clear();
    ss.str(line);

    for (int i = 0; i < num_fields; i++)
        getline(ss, *(fields[i]), ',');
}

int getFarePerKM(string type)
{
    if (type == "Jaldi Rickshaw")
        return 25;
    else if (type == "Jaldi Bike")
        return 28;
    else if (type == "JaldiX")
        return 35;
    else if (type == "JaldiXL")
        return 45;
    else if (type == "Jaldi Black")
        return 50;
    else
        return 0;
}

int getAge(int day, int month, int year)
{
    struct tm dob = { 0 };

    dob.tm_mday = day;
    dob.tm_mon = month - 1;    // tm_mon is from 0-11
    dob.tm_year = year - 1900; // tm_year is relative to 1900

    time_t then = mktime(&dob);
    time_t now = time(NULL);               //
    const int seconds_per_year = 31536000; // approx

    double seconds = difftime(now, then);
    return int(seconds / seconds_per_year);
}

void addRating(int id, int rating)
{
    ifstream file("drivers.csv");

    ofstream file2("drivers2.csv");
    file2.close();
    file2.open("drivers2.csv", ios::app);
    string line;
    string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr, passwordStr;
    string typeStr, yearOfManufactureStr, modelStr, makeStr, trimLevelStr, plateNumStr, colorStr;
    string nicStr, sumOfRatingsStr, ratedByStr;
    string* fields[18] = { &idStr, &dayStr, &monthStr, &yearStr, &firstNameStr, &lastNameStr, &phoneNumStr, &passwordStr, &nicStr, &typeStr, &yearOfManufactureStr, &makeStr, &modelStr, &trimLevelStr, &plateNumStr, &colorStr, &sumOfRatingsStr, &ratedByStr };

    while (getline(file, line))
    {
        ss.clear();
        ss.str(line);
        getFields(line, fields, 18);

        if (stoi(idStr) == id)
            line = idStr + "," + dayStr + "," + monthStr + "," + yearStr + "," + firstNameStr + "," + lastNameStr + "," + phoneNumStr + "," + passwordStr + "," + nicStr + "," + typeStr + "," + yearOfManufactureStr + "," + makeStr + "," + modelStr + "," + trimLevelStr + "," + plateNumStr + "," + colorStr + "," + to_string(stoi(sumOfRatingsStr) + rating) + "," + to_string(stoi(ratedByStr) + 1);

        file2 << line << "\n";
    }

    file.close();
    file2.close();

    remove("drivers.csv");
    rename("drivers2.csv", "drivers.csv");
}

#endif // HELPER_FUNCTIONS_H 