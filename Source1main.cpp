#define _CRT_SECURE_NO_WARNINGS
#include "UserManagement.h"
#include "HelperFunctions.h"
#include "InputValidation.h"
#include <string>

int main()
{
	// Count existing users from files
	User::countUsersFromFiles();

	cout << endl
		<< "Welcome to Jaldi Rides!" << endl;

	// Display initial counts from files
	cout << "\nInitial user counts from previous session:" << endl;
	User::displayUserCounts();

	bool programRunning = true;

	while (programRunning)
	{
		int opt1 = menu1();

		if (opt1 == 3)
		{
			cout << endl
				<< "Thank you for using Jaldi Rides!" << endl;
			programRunning = false;
			continue;
		}

		int opt2 = menu2();
		if (opt2 == 3)
		{
			continue;  // Go back to main menu
		}

		if (opt1 == 1)
		{
			string firstName, lastName, phoneNum, password;
			int day, month, year;
			string nic;
			int age;

			askName(firstName, true, false);
			askName(lastName, false, false);
			do
			{
				askDate(1, day, 1, 31, false);
				askDate(2, month, 1, 12, false);
				askDate(3, year, current_year - 120, current_year, false);

				age = getAge(day, month, year);
				if (opt2 == 2)
				{
					if (age < 18)
						cerr << endl
						<< "Error: driver must be at least 18 years old" << endl;
				}
			} while (opt2 == 2 && age < 18);

			// askDOB(day, month, year, opt2 == 1, false);
			askPhoneNum(phoneNum, opt2 == 1, false);

			if (opt2 == 2)
				askNIC(false, nic);

			askPassword(password, "-", false);

			if (opt2 == 2)
			{ // driver
				string type, make, model, trimLevel, plateNum, color;
				int yearOfManufacture;

				askVehicle(false, type, yearOfManufacture, make, model, trimLevel, plateNum, color);

				Vehicle newVehicle(type, yearOfManufacture, make, model, trimLevel, plateNum, color);

				ifstream driversFile("drivers.csv");
				string line;
				bool vehicleExists = false;

				while (getline(driversFile, line))
				{
					string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr, passwordStr;
					string typeStr, yearOfManufactureStr, modelStr, makeStr, trimLevelStr, plateNumStr, colorStr;
					string* fields[18] = { &idStr, &dayStr, &monthStr, &yearStr, &firstNameStr, &lastNameStr, &phoneNumStr, &passwordStr,
										  &typeStr, &yearOfManufactureStr, &modelStr, &makeStr, &trimLevelStr, &plateNumStr, &colorStr };
					getFields(line, fields, 18);

					Vehicle existingVehicle(typeStr, std::stoi(yearOfManufactureStr), modelStr,
						makeStr, trimLevelStr, plateNumStr, colorStr);

					if (newVehicle == existingVehicle)
					{
						vehicleExists = true;
						cerr << "Error: This vehicle is already registered" << endl;
						break;
					}
				}

				if (!vehicleExists)
				{
					int newId = getLastId("drivers.csv") + 1;
					Driver driver(newId, day, month, year, firstName, lastName, phoneNum, password, nic, newVehicle);
					driver.appendToFile();
					cout << endl << "Signed up!" << endl;
				}
			}
			else
			{ // passenger
				int newId = getLastId("passengers.csv") + 1;
				Passenger passenger(newId, day, month, year, firstName, lastName, phoneNum, password);
				passenger.appendToFile();
				cout << endl << "Signed up!" << endl;
			}

			cout << endl
				<< "Signed up!" << endl;
			continue;
		}
		else
		{
			string phoneNum, password;
			cout << endl << "Enter phone number: " << endl;
			cin >> phoneNum;
			cout << endl << "Enter password: " << endl;
			cin >> password;

			bool found = 0;
			if (opt2 == 1)
			{
				ifstream passengers_in("passengers.csv");
				string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr, passwordStr;
				string line;
				bool found = false;
				Passenger currentPassenger;

				if (passengers_in)
				{
					while (!found && getline(passengers_in, line))
					{
						/*
							found	getline	run while loop
							0		0		0
							0		1		1
							1		0		0
							1		1		0
						*/

						string* fields[8] = { &idStr, &dayStr, &monthStr, &yearStr, &firstNameStr, &lastNameStr, &phoneNumStr, &passwordStr };
						getFields(line, fields, 8);

						if (phoneNum == phoneNumStr && password == passwordStr)
						{
							found = 1;
							break;
						}
					}
				}

				passengers_in.close();

				if (!found)
				{
					cerr << endl << "Error: invalid credentials" << endl;
					continue;
				}

				cout << endl << "Logged in as passenger" << endl;
				cout << currentPassenger << endl;

			passengerMenu:
				int opt = currentPassenger.displayMenu();

				if (opt == 1)
				{
					// Create bookings with proper initialization
					Booking currentBooking(1, 'A', 'B', "JaldiX", nullptr);
					currentBooking.setFare(100);
					currentBooking.setAvgDistance(10);

					Booking previousBooking(2, 'C', 'D', "JaldiX", nullptr);
					previousBooking.setFare(150);
					previousBooking.setAvgDistance(15);

					Booking totalStats = currentBooking + previousBooking;

					cout << "\nRide Statistics:" << endl;
					cout << "Total distance: " << totalStats.getAvgDistance() << " km" << endl;
					cout << "Total fare: Rs. " << totalStats.getFare() << endl;
				}
				else if (opt == 2)
					currentPassenger.viewHistory(true);
				else if (opt == 3)
					currentPassenger.viewProfile();
				else if (opt == 4)
					currentPassenger.updateProfile(true);

				if (opt != 5)
				{
					goto passengerMenu;
				}
				else
				{
					cout << endl
						<< "Logged out!" << endl;
					continue;
				}
			}
			else if (opt2 == 2)
			{
				ifstream drivers_in("drivers.csv");
				string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr, passwordStr;
				string typeStr, yearOfManufactureStr, modelStr, makeStr, trimLevelStr, plateNumStr, colorStr;
				string nicStr;
				string line;
				bool found = false;
				Driver currentDriver;

				while (!found && getline(drivers_in, line))
				{
					string* fields[18] = { &idStr, &dayStr, &monthStr, &yearStr, &firstNameStr, &lastNameStr, &phoneNumStr, &passwordStr,
										  &typeStr, &yearOfManufactureStr, &modelStr, &makeStr, &trimLevelStr, &plateNumStr, &colorStr };
					getFields(line, fields, 18);

					if (phoneNum == *fields[6] && password == *fields[7])
					{
						found = true;
						Vehicle vehicle(*fields[9], std::stoi(*fields[10]), *fields[11],
							*fields[12], *fields[13], *fields[14], *fields[15]);
						currentDriver = Driver(stoi(*fields[0]), stoi(*fields[1]),
							stoi(*fields[2]), stoi(*fields[3]),
							*fields[4], *fields[5], *fields[6], *fields[7],
							*fields[8], vehicle);
					}
				}

				drivers_in.close();

				if (!found)
				{
					cerr << endl << "Error: invalid credentials" << endl;
					continue;
				}

				cout << endl << "Logged in as driver" << endl;

				ifstream allDrivers("drivers.csv");
				while (getline(allDrivers, line))
				{
					string* fields[18] = { &idStr, &dayStr, &monthStr, &yearStr, &firstNameStr, &lastNameStr, &phoneNumStr, &passwordStr,
										  &typeStr, &yearOfManufactureStr, &modelStr, &makeStr, &trimLevelStr, &plateNumStr, &colorStr };
					getFields(line, fields, 18);

					if (*fields[6] != phoneNum)
					{
						Vehicle otherVehicle(*fields[9], std::stoi(*fields[10]), *fields[11],
							*fields[12], *fields[13], *fields[14], *fields[15]);
						Driver otherDriver(stoi(*fields[0]), stoi(*fields[1]),
							stoi(*fields[2]), stoi(*fields[3]),
							*fields[4], *fields[5], *fields[6], *fields[7],
							*fields[8], otherVehicle);

						if (currentDriver > otherDriver)
						{
							cout << "Your rating is better than " << otherDriver.getFirstName() << endl;
						}
					}
				}

			driverMenu:
				int opt = currentDriver.displayMenu();

				if (opt == 1)
				{
					int acceptedId;
					bool found;
					Booking booking;

					do
					{
						currentDriver.viewAvailableRidesAndAskId(acceptedId, booking, found);
						if (acceptedId == -1)
							break;

						if (found)
						{
							updateBooking(booking, "accepted", currentDriver.getId(), "-", "-");
							break;
						}
					} while (acceptedId != -1 && found == false);

					if (found)
					{
						int startRide;

						do
						{
							cout << "Enter 1 to start ride: ";
							cin >> startRide;
						} while (startRide != 1);

						string startedAt = getCurrentTime();
						updateBooking(booking, "started", currentDriver.getId(), startedAt, booking.getCompletedAt());

						cout << endl
							<< "Ride started" << endl;

						for (int i = 0; i < booking.getAvgDistance(); i++)
						{
							cout << endl
								<< "Wait... " << i;
							Sleep(1000);
						}
						cout << endl;

						string completedAt = getCurrentTime();
						updateBooking(booking, "completed", currentDriver.getId(), startedAt, completedAt);
						cout << endl
							<< "Ride completed!" << endl;
						cout << endl
							<< "You may see your updated rating after logging in again" << endl;
					}
				}
				else if (opt == 2)
					currentDriver.viewHistory(false);
				else if (opt == 3)
					currentDriver.viewProfile();
				else if (opt == 4)
					currentDriver.updateProfile(false);

				if (opt != 5)
				{
					goto driverMenu;
				}
				else
				{
					cout << endl
						<< "Logged out!" << endl;
					continue;
				}
			}
		}
	}

	// Display final counts including new users created in this session
	cout << "\nFinal user counts including new users from this session:" << endl;
	User::displayUserCounts();

	return 0;
}