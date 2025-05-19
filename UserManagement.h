#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <iostream>
#include <fstream>
#include <windows.h>
#include "HelperFunctions.h"
#include "InputValidation.h"
using namespace std;

const int MAX_TRIES = 8; // LATER: change value

class User
{
protected:
    int id = -1, day = 0, month = 0, year = 0;
    string firstName = "", lastName = "", phoneNum = "", password = "";
    
    // Static members to track counts
    static int totalUsers;
    static int totalDrivers;
    static int totalPassengers;

public:
    User() {
        totalUsers++;
    }

    User(int id, int day, int month, int year, string firstName, string lastName, string phoneNum, string password) : 
    id(id), day(day), month(month), year(year), firstName(firstName), lastName(lastName), phoneNum(phoneNum), password(password) {
        totalUsers++;
    }

    // Getters
    int getId() const { return id; }
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getPhoneNum() const { return phoneNum; }
    string getPassword() const { return password; }

    // Setters
    void setId(int newId) { id = newId; }
    void setDay(int newDay) { day = newDay; }
    void setMonth(int newMonth) { month = newMonth; }
    void setYear(int newYear) { year = newYear; }
    void setFirstName(const string& newFirstName) { firstName = newFirstName; }
    void setLastName(const string& newLastName) { lastName = newLastName; }
    void setPhoneNum(const string& newPhoneNum) { phoneNum = newPhoneNum; }
    void setPassword(const string& newPassword) { password = newPassword; }

    // Static method to display counts
    static void displayUserCounts() {
        cout << "\n=== User Statistics ===" << endl;
        cout << "Total Users: " << totalUsers << endl;
        cout << "Total Drivers: " << totalDrivers << endl;
        cout << "Total Passengers: " << totalPassengers << endl;
        cout << "=====================\n" << endl;
    }

    // Static method to count users from files
    static void countUsersFromFiles() {
        // Count passengers
        ifstream passengersFile("passengers.csv");
        string line;
        while (getline(passengersFile, line)) {
            totalPassengers++;
            totalUsers++;
        }
        passengersFile.close();

        // Count drivers
        ifstream driversFile("drivers.csv");
        while (getline(driversFile, line)) {
            totalDrivers++;
            totalUsers++;
        }
        driversFile.close();
    }

    // Static method to reset counts
    static void resetCounts() {
        totalUsers = 0;
        totalDrivers = 0;
        totalPassengers = 0;
    }

    virtual void viewProfile()
    {
        system("cls");
        cout << endl
            << "ID: " << id << endl;
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Date of Birth (D/M/Y): " << day << "/" << month << "/" << year << endl;
        cout << "Phone Number: " << phoneNum << endl;
    }

    virtual void updateProfile(bool isPassenger)
    {
        cout << endl
            << "If you do not want to update a field then enter -1 in it" << endl;

        string firstName2, lastName2, password2;
        int day2, month2, year2;
        string phoneNum2;
        int age;

        askName(firstName2, true, true);
        askName(lastName2, false, true);
        askPassword(password2, password, true);

        do
        {
            askDate(1, day2, 1, 31, true);
            askDate(2, month2, 1, 12, true);
            askDate(3, year2, current_year - 120, current_year, true);

            if (day2 == -1)
                day2 = day;
            if (month2 == -1)
                month2 = month;
            if (year2 == -1)
                year2 = year;

            if (!isPassenger)
            {
                age = getAge(day2, month2, year2);
                if (age < 18)
                    cerr << endl
                    << "Error: driver must be at least 18 years old" << endl;
            }
        } while (!isPassenger && age < 18);

        askPhoneNum(phoneNum2, isPassenger, true);

        if (firstName2 != "-")
            firstName = firstName2;
        if (lastName2 != "-")
            lastName = lastName2;
        if (password2 != "-")
            password = password2;
        if (phoneNum2 != "-" && phoneNum2 != "-1")
            phoneNum = phoneNum2;
        if (day2 != -1)
            day = day2;
        if (month2 != -1)
            month = month2;
        if (year2 != -1)
            year = year2;
    }

    void viewHistory(bool);

    bool operator==(const User& other) const {
        return (id == other.id && 
                phoneNum == other.phoneNum);
    }
};

// Initialize static members
int User::totalUsers = 0;
int User::totalDrivers = 0;
int User::totalPassengers = 0;

class Passenger;
class Driver;

class Booking
{
    int id = -1;
    string status = "available"; // available, not accepted, arriving, arrived, in progress, completed, cancelled
    char pickupLocation = '-', dropoffLocation = '-';
    string type = "";
    int fare = 0;
    int avgDistance = 0;
    Passenger* passenger;
    int passengerId = -1;
    int driverId = -1;
    string bookedAt = "-";
    string startedAt = "-";
    string completedAt = "-";

    const int fare_per_km = 25;

public:
    Booking() {}
    Booking(int id) : id(id) {}
    Booking(int id, char pickupLocation, char dropoffLocation, string type, Passenger* passenger);

    // Getters
    int getId() const { return id; }
    string getStatus() const { return status; }
    char getPickupLocation() const { return pickupLocation; }
    char getDropoffLocation() const { return dropoffLocation; }
    string getType() const { return type; }
    int getFare() const { return fare; }
    int getAvgDistance() const { return avgDistance; }
    int getPassengerId() const { return passengerId; }
    int getDriverId() const { return driverId; }
    string getBookedAt() const { return bookedAt; }
    string getStartedAt() const { return startedAt; }
    string getCompletedAt() const { return completedAt; }

    // Setters
    void setId(int newId) { id = newId; }
    void setStatus(const string& newStatus) { status = newStatus; }
    void setPickupLocation(char newLocation) { pickupLocation = newLocation; }
    void setDropoffLocation(char newLocation) { dropoffLocation = newLocation; }
    void setType(const string& newType) { type = newType; }
    void setFare(int newFare) { fare = newFare; }
    void setAvgDistance(int newDistance) { avgDistance = newDistance; }
    void setPassengerId(int newId) { passengerId = newId; }
    void setDriverId(int newId) { driverId = newId; }
    void setBookedAt(const string& newTime) { bookedAt = newTime; }
    void setStartedAt(const string& newTime) { startedAt = newTime; }
    void setCompletedAt(const string& newTime) { completedAt = newTime; }

    // constructor for loading an already stored booking
    void load(int id, string status, char pickupLocation, char dropoffLocation, string type, int fare, int passengerId, string bookedAt, int driverId, string startedAt, string completedAt)
    {
        this->id = id;
        this->status = status;
        this->pickupLocation = pickupLocation;
        this->dropoffLocation = dropoffLocation;
        avgDistance = abs(dropoffLocation - pickupLocation);
        this->type = type;
        this->fare = fare;
        this->passengerId = passengerId;
        this->bookedAt = bookedAt;
        this->driverId = driverId;
        this->startedAt = startedAt;
        this->completedAt = completedAt;
    }

    friend void updateBooking(Booking& booking, string newStatus, int newDriverId, string newStartedAt, string newCompletedAt);

    bool checkStatus(string givenStatus, string givenStatus2)
    {
        ifstream file("bookings.csv");
        string idStr, statusStr, pickupStr, dropoffStr, driverIdStr;

        string line;
        bool found = false;

        while (getline(file, line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, idStr, ',');

            if (stoi(idStr) == id)
            {
                getline(ss, statusStr, ',');
                getline(ss, pickupStr, ',');
                getline(ss, dropoffStr, ',');

                pickupLocation = pickupStr[0];
                dropoffLocation = dropoffStr[0];
                avgDistance = abs(pickupLocation - dropoffLocation);

                if (statusStr == givenStatus || statusStr == givenStatus2)
                {
                    for (int i = 0; i < 5; i++)
                        getline(ss, driverIdStr, ',');

                    driverId = stoi(driverIdStr);
                    found = true;
                }
            }
        }

        file.close();
        return found;
    }

    void appendToFile();

    void completeRide()
    {
        completedAt = getCurrentTime();
    }

    void getFromFile()
    {
        ifstream file("bookings.csv");
        string idStr, statusStr, pickupLocationStr, dropoffLocationStr, typeStr,
            fareStr, passengerIdStr, bookedAtStr, driverIdStr, startedAtStr, completedAtStr;
        string line;
        bool found = false;

        string* fields[11] = { &idStr, &statusStr, &pickupLocationStr, &dropoffLocationStr, &typeStr, &fareStr, &passengerIdStr, &bookedAtStr, &driverIdStr, &startedAtStr, &completedAtStr };
        while (getline(file, line))
        {
            ss.clear();
            ss.str(line);
            getFields(line, fields, 11);
            // getline(ss, idStr, ',');

            if (stoi(idStr) == id)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            status = statusStr;
            pickupLocation = pickupLocationStr[0];
            dropoffLocation = dropoffLocationStr[0];
            type = typeStr;
            fare = stoi(fareStr);
            passengerId = stoi(passengerIdStr);
            bookedAt = bookedAtStr;
            driverId = stoi(driverIdStr);
            startedAt = startedAtStr;
            completedAt = completedAtStr;
        }

        file.close();
    }

    void display(bool);

    Booking operator+(const Booking& other) const {
        Booking combined;
        combined.fare = this->fare + other.fare;
        combined.avgDistance = this->avgDistance + other.avgDistance;
        return combined;
    }
};

void updateBooking(Booking& booking, string newStatus, int newDriverId, string newStartedAt, string newCompletedAt)
{
    ifstream file("bookings.csv");

    ofstream file2("bookings2.csv");
    file2.close();
    file2.open("bookings2.csv", ios::app);
    // string idStr, statusStr, pickupLocationStr, dropoffLocationStr, typeStr,
    // fareStr, passengerIdStr, bookedAtStr, driverIdStr, completedAtStr;

    // string *fields[10] = {&idStr, &statusStr, &pickupLocationStr, &dropoffLocationStr, &typeStr, &fareStr, &passengerIdStr, &bookedAtStr, &driverIdStr, &completedAtStr};

    string line, idStr;
    while (getline(file, line))
    {
        ss.clear();
        ss.str(line);
        getline(ss, idStr, ',');

        if (stoi(idStr) == booking.id)
            line = to_string(booking.id) + "," + newStatus + "," + booking.pickupLocation + "," + booking.dropoffLocation + "," + booking.type + "," + to_string(booking.fare) + "," + to_string(booking.passengerId) + "," + booking.bookedAt + "," + to_string(newDriverId) + "," + newCompletedAt;

        file2 << line << "\n";
    }

    file.close();
    file2.close();

    remove("bookings.csv");
    rename("bookings2.csv", "bookings.csv");
}

class Passenger : public User
{
public:
    Passenger() : User() {
        totalPassengers++;
    }

    Passenger(int id, int day, int month, int year, string firstName, string lastName, string phoneNum, string password)
        : User(id, day, month, year, firstName, lastName, phoneNum, password) {
        totalPassengers++;
    }

    void appendToFile()
    {
        ofstream file("passengers.csv", ios::app);
        file << id << "," << day << "," << month << "," << year << "," << firstName << "," << lastName << "," << phoneNum << "," << password << "\n";
        file.close();
    }

    int displayMenu()
    {
        int opt = 0;

        do
        {
            cout << endl
                << "1) Book a ride" << endl;
            cout << "2) View history" << endl;
            cout << "3) View profile" << endl;
            cout << "4) Update profile" << endl;
            cout << "5) Logout" << endl;
            cin >> opt;
        } while (opt > 5 || opt < 1);

        return opt;
    }

    void viewProfile() override
    {
        this->User::viewProfile();
        // LATER: more data?
    }

    void updateProfile(bool isPassenger) override
    {
        this->User::updateProfile(isPassenger);
        ifstream file("passengers.csv");

        ofstream file2("passengers2.csv");
        file2.close();
        file2.open("passengers2.csv", ios::app);

        string line, idStr;
        while (getline(file, line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, idStr, ',');

            if (stoi(idStr) == id)
                line = to_string(id) + "," + to_string(day) + "," + to_string(month) + "," + to_string(year) + "," + firstName + "," + lastName + "," + phoneNum + "," + password;

            file2 << line << "\n";
        }

        file.close();
        file2.close();

        remove("passengers.csv");
        rename("passengers2.csv", "passengers.csv");
    }

    void bookARide()
    {
        system("cls");
        char pickup, dropoff;

        do
        {
            cout << endl
                << "Enter pickup location (enter any location from A to Z): ";
            cin >> pickup;

            cout << "Enter dropoff location (make sure not to use the same location as pickup): ";
            cin >> dropoff;

            pickup = toupper(pickup);
            dropoff = toupper(dropoff);
        } while (pickup < 'A' || pickup > 'Z' || dropoff < 'A' || dropoff > 'Z' || pickup == dropoff);

        cout << endl
            << "Enter a number to select the type of vehicle that you want to book: " << endl;
        string type = typesMenu(false);

        int newId = getLastId("bookings.csv") + 1;
        Booking booking(newId, pickup, dropoff, type, this);
        booking.appendToFile();

        // wait for X seconds OR until a driver accepts a ride
        int tries = 0;
        bool found = false;

        cout << endl
            << "Checking every five seconds if any driver accepted the ride...";

        // LATER: check if user cancels the search; in that case update status to unavailable
        while (tries < MAX_TRIES && !found)
        {
            cout << endl
                << "Wait... ";
            for (int i = 1; i <= 5; i++)
            {
                cout << i << " ";
                Sleep(1000);
            }

            found = booking.checkStatus("accepted", "started");

            if (!found)
                tries++;
            else
                found = true;
        }

        if (found)
        {
            cout << endl
                << "Ride found!" << endl;

            booking.getFromFile();
            booking.display(true);

            int tries2 = 0;
            bool started = false;

            while (tries2 < MAX_TRIES && !started)
            {

                cout << endl
                    << "Wait... ";
                for (int i = 1; i <= 5; i++)
                {
                    cout << i << " ";
                    Sleep(1000);
                }
                cout << endl;

                started = booking.checkStatus("started", "");

                if (!started)
                    tries2++;
                else
                {
                    cout << endl
                        << "Ride started" << endl;
                    for (int i = 0; i < booking.getAvgDistance() + 5; i++)
                    {
                        cout << endl
                            << "Wait... " << i;
                        Sleep(1000);
                    }
                    cout << endl;

                    bool completed = booking.checkStatus("completed", "");
                    if (completed)
                    {
                        cout << endl
                            << "Ride completed!" << endl;

                        int rating;

                        do
                        {
                            cout << endl
                                << "Enter rating out of 5: " << endl;
                            cin >> rating;
                        } while (rating > 5 || rating < 0);

                        addRating(booking.getDriverId(), rating);
                        cout << endl
                            << "Thank you";
                    }
                }
            }

            if (!found)
            {
                updateBooking(booking, "unavailable", -1, "-", "-");
                // LATER: check that if ctrl+c is pressed then the line above can be made to run or not
                cout << endl
                    << endl
                    << "Your ride was not started by the driver" << endl
                    << "Please try again later" << endl;
            }
            else
            {
            }
        }
        else
        {
            updateBooking(booking, "unavailable", -1, "-", "-");
            // LATER: check that if ctrl+c is pressed then the line above can be made to run or not
            cout << endl
                << endl
                << "Your ride was not accepted by any driver" << endl
                << "Please try again later" << endl;
        }
    }

    void displayToUser()
    {
        cout << endl
            << "Passenger ID: " << id << endl;
        cout << "Passenger Name: " << firstName << " " << lastName << endl;
        cout << "Passenger Phone Number: " << phoneNum << endl;
    }

    void setId(int id) { this->id = id; }

    void getFromFile()
    {

        ifstream file("passengers.csv");
        string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr;
        string line;
        string* fields[7] = { &idStr, &dayStr, &monthStr, &yearStr, &firstNameStr, &lastNameStr, &phoneNumStr };
        bool found = false;

        while (getline(file, line))
        {
            ss.clear();
            ss.str(line);
            getFields(line, fields, 7);

            if (stoi(idStr) == id)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            this->id = stoi(idStr);
            this->day = stoi(dayStr);
            this->month = stoi(monthStr);
            this->year = stoi(yearStr);
            this->firstName = firstNameStr;
            this->lastName = lastNameStr;
            this->phoneNum = phoneNumStr;
        }

        file.close();
    }

    friend ostream& operator<<(ostream& os, const Passenger& passenger) {
        os << "Passenger: " << passenger.firstName << " " << passenger.lastName 
           << "\nPhone: " << passenger.phoneNum 
           << "\nID: " << passenger.id;
        return os;
    }
};

class Vehicle
{
    friend class Driver;

    string type = "", make = "", model = "", trimLevel = "", plateNum = "", color = "";
    int yearOfManufacture = 0;

public:
    Vehicle() {}

    Vehicle(string type, int yearOfManufacture, string make, string model, string trimLevel, string plateNum, string color)
    {
        this->type = type;
        this->yearOfManufacture = yearOfManufacture;
        this->make = make;
        this->model = model;
        this->trimLevel = trimLevel;
        this->plateNum = plateNum;
        this->color = color;
    }

    // Getters
    string getType() const { return type; }
    int getYearOfManufacture() const { return yearOfManufacture; }
    string getMake() const { return make; }
    string getModel() const { return model; }
    string getTrimLevel() const { return trimLevel; }
    string getPlateNum() const { return plateNum; }
    string getColor() const { return color; }

    // Setters
    void setType(const string& newType) { type = newType; }
    void setYearOfManufacture(int newYear) { yearOfManufacture = newYear; }
    void setMake(const string& newMake) { make = newMake; }
    void setModel(const string& newModel) { model = newModel; }
    void setTrimLevel(const string& newTrimLevel) { trimLevel = newTrimLevel; }
    void setPlateNum(const string& newPlateNum) { plateNum = newPlateNum; }
    void setColor(const string& newColor) { color = newColor; }

    void viewData()
    {
        cout << endl
            << "Vehicle Data: " << endl;
        cout << "Name: " << yearOfManufacture << " " << make << " " << model << " " << trimLevel << endl;
        cout << "Type: " << type << endl;
        cout << "Color: " << color << endl;
        cout << "Plate Number: " << plateNum << endl;
    }

    void setDetails(string type, int yearOfManufacture, string model, string make, string trimLevel, string plateNum, string color)
    {
        this->type = type;
        this->yearOfManufacture = yearOfManufacture;
        this->model = model;
        this->make = make;
        this->trimLevel = trimLevel;
        this->plateNum = plateNum;
        this->color = color;
    }

    bool operator==(const Vehicle& other) const {
        return (plateNum == other.plateNum);
    }
};

class Driver : public User
{
    int sumOfRatings = 0, ratedBy = 0;
    Vehicle vehicle;
    string nic = "";

public:
    Driver() : User() {
        totalDrivers++;
    }

    Driver(int id, int day, int month, int year, string firstName, string lastName, string phoneNum, string password, string nic, Vehicle vehicle)
        : User(id, day, month, year, firstName, lastName, phoneNum, password), sumOfRatings(0), ratedBy(0), nic(nic), vehicle(vehicle) {
        totalDrivers++;
    }

    // Getters
    int getSumOfRatings() const { return sumOfRatings; }
    int getRatedBy() const { return ratedBy; }
    string getNic() const { return nic; }
    Vehicle getVehicle() const { return vehicle; }
    float getRating() const { return (ratedBy == 0) ? 0 : (float)sumOfRatings / (float)ratedBy; }

    // Setters
    void setSumOfRatings(int newSum) { sumOfRatings = newSum; }
    void setRatedBy(int newCount) { ratedBy = newCount; }
    void setNic(const string& newNic) { nic = newNic; }
    void setVehicle(const Vehicle& newVehicle) { vehicle = newVehicle; }

    void appendToFile()
    {
        ofstream file("drivers.csv", ios::app);
        file << id << "," << day << "," << month << "," << year << "," << firstName << "," << lastName << ","
            << phoneNum << "," << password << "," << nic << "," << vehicle.type << "," << vehicle.yearOfManufacture << "," << vehicle.make << "," << vehicle.model << "," << vehicle.trimLevel << "," << vehicle.plateNum << "," << vehicle.color << "," << sumOfRatings << "," << ratedBy << "\n";
        file.close();
    }

    int displayMenu()
    {
        int opt = 0;

        do
        {
            cout << endl
                << "1) View available rides" << endl;
            cout << "2) View history" << endl;
            cout << "3) View profile" << endl;
            cout << "4) Update profile" << endl;
            cout << "5) Logout" << endl;
            cin >> opt;
        } while (opt > 5 || opt < 1);

        // if (opt == 3)
        //     viewProfile();

        return opt;
    }

    void viewProfile() override
    {
        this->User::viewProfile();
        cout << "NIC: " << nic << endl;
        cout << "Average Rating: " << getRating() << endl;
        cout << "Rated by: " << ratedBy << " passengers" << endl;
        vehicle.viewData();
        // LATER: more data?
    }

    void updateProfile(bool isPassenger) override
    {
        this->User::updateProfile(isPassenger);

        string nic2;
        string type2, make2, model2, trimLevel2, plateNum2, color2;
        int yearOfManufacture2;

        askNIC(true, nic2);
        askVehicle(true, type2, yearOfManufacture2, make2, model2, trimLevel2, plateNum2, color2);

        if (nic2 != "-1" && nic != "-")
            nic = nic2;

        if (type2 != "-")
            vehicle.type = type2;

        if (yearOfManufacture2 != -1)
            vehicle.yearOfManufacture = yearOfManufacture2;

        if (make2 != "-")
            vehicle.make = make2;

        if (model2 != "-")
            vehicle.model = model2;

        if (trimLevel2 != "-")
            vehicle.trimLevel = trimLevel2;

        if (plateNum2 != "-")
            vehicle.plateNum = plateNum2;

        if (color2 != "-")
            vehicle.color = color2;

        ifstream file("drivers.csv");

        ofstream file2("drivers2.csv");
        file2.close();
        file2.open("drivers2.csv", ios::app);

        string line, idStr;
        while (getline(file, line))
        {
            ss.clear();
            ss.str(line);
            getline(ss, idStr, ',');

            if (stoi(idStr) == id)
                line = to_string(id) + "," + to_string(day) + "," + to_string(month) + "," + to_string(year) + "," + firstName + "," + lastName + "," + phoneNum + "," + password + "," + nic + "," + vehicle.type + "," + to_string(vehicle.yearOfManufacture) + "," + vehicle.make + "," + vehicle.model + "," + vehicle.trimLevel + "," + vehicle.plateNum + "," + vehicle.color + "," + to_string(sumOfRatings) + "," + to_string(ratedBy);

            file2 << line << "\n";
        }

        file.close();
        file2.close();

        remove("drivers.csv");
        rename("drivers2.csv", "drivers.csv");
    }

    void viewAvailableRidesAndAskId(int& acceptedId, Booking& booking, bool& found)
    {
        system("cls");

        ifstream file("bookings.csv");
        string line;

        string idStr, statusStr, pickupStr, dropoffStr, typeStr, fareStr, passengerIdStr, driverIdStr, bookedAtStr, startedAtStr, completedAtStr;
        string* fields[11] = { &idStr, &statusStr, &pickupStr, &dropoffStr, &typeStr, &fareStr, &passengerIdStr, &bookedAtStr, &driverIdStr, &startedAtStr, &completedAtStr };

        acceptedId = -1;
        found = false;

        if (file)
        {
            while (getline(file, line))
            {
                getFields(line, fields, 11);

                if (statusStr == "available" && (this->vehicle).type == typeStr)
                {
                    booking.load(stoi(idStr), statusStr, pickupStr[0], dropoffStr[0], typeStr, stoi(fareStr), stoi(passengerIdStr), bookedAtStr, stoi(driverIdStr), startedAtStr, completedAtStr);
                    booking.display(false);
                }
            }
        }

        cout << endl
            << "Enter a booking ID to accept it (enter -1 to cancel): ";
        cin >> acceptedId;

        if (acceptedId < 0)
            acceptedId = -1;

        else // if acceptedId >= 0
        {
            ifstream file("bookings.csv");
            string line, idStr, statusStr;

            while (getline(file, line))
            {
                ss.clear();
                ss.str(line);
                getline(ss, idStr, ',');
                getline(ss, statusStr, ',');

                if (stoi(idStr) == acceptedId && statusStr == "available")
                    found = true;
            }
        }

        file.close();
    }

    void setId(int id)
    {
        this->id = id;
    }

    void getFromFile()
    {

        ifstream file("drivers.csv");
        string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr, passwordStr;
        string typeStr, yearOfManufactureStr, modelStr, makeStr, trimLevelStr, plateNumStr, colorStr;
        string nicStr;
        string line;
        string* fields[16] = { &idStr, &dayStr, &monthStr, &yearStr, &firstNameStr, &lastNameStr, &phoneNumStr, &passwordStr, &nicStr, &typeStr, &yearOfManufactureStr, &makeStr, &modelStr, &trimLevelStr, &plateNumStr, &colorStr };
        bool found = false;

        while (getline(file, line))
        {
            ss.clear();
            ss.str(line);
            getFields(line, fields, 16);

            if (stoi(idStr) == id)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            this->id = stoi(idStr);
            this->day = stoi(dayStr);
            this->month = stoi(monthStr);
            this->year = stoi(yearStr);
            this->firstName = firstNameStr;
            this->lastName = lastNameStr;
            this->phoneNum = phoneNumStr;
            this->password = passwordStr;
            this->nic = nicStr;
            vehicle.setDetails(typeStr, stoi(yearOfManufactureStr), modelStr, makeStr, trimLevelStr, plateNumStr, colorStr);
        }

        file.close();
    }

    void displayToUser()
    {
        cout << endl
            << "Driver ID: " << id << endl;
        cout << "Driver Name: " << firstName << " " << lastName << endl;
        cout << "Driver Rating: " << getRating() << "/5" << endl;
        cout << "Driver Rated By: " << ratedBy << " passengers" << endl;
        cout << "Driver Phone Number: " << phoneNum << endl;
        cout << "Driver NIC: " << nic << endl;
    }

    bool operator>(const Driver& other) const {
        return this->getRating() > other.getRating();
    }
};

Booking::Booking(int id, char pickupLocation, char dropoffLocation, string type, Passenger* passenger)
{
    this->id = id;
    this->pickupLocation = pickupLocation;
    this->dropoffLocation = dropoffLocation;
    this->type = type;
    this->passenger = passenger;
    this->passengerId = passenger->getId();
    this->fare = getFarePerKM(type) * abs(dropoffLocation - pickupLocation);
    bookedAt = getCurrentTime();
}

void Booking::appendToFile()
{
    ofstream file("bookings.csv", ios::app);
    file << id << "," << status << "," << pickupLocation << "," << dropoffLocation << "," << type << ","
        << fare << "," << passengerId << "," << bookedAt << "," << driverId << "," << completedAt << "\n";
    file.close();
}

void Booking::display(bool fromPassenger)
{
    cout << endl
        << "Booking ID: " << id;

    if (fromPassenger)
    {
        if (driverId != -1)
        {
            Driver driver;
            driver.setId(driverId);
            driver.getFromFile();
            driver.displayToUser();
        }
    }
    else
    {
        if (passengerId != -1)
        {
            Passenger p;
            p.setId(passengerId);
            p.getFromFile();
            p.displayToUser();
        }
    }
    cout << "Status: " << status;
    cout << endl
        << "Fare: Rs. " << fare;
    cout << endl
        << "Pickup Location: " << pickupLocation;
    cout << endl
        << "Dropoff Location: " << dropoffLocation;
    cout << endl
        << "Average Distance: " << avgDistance << " km";
    cout << endl
        << "Vehicle Type: " << type;
    cout << endl
        << "Booked at: " << bookedAt << endl;
}

void User::viewHistory(bool isPassenger)
{
    ifstream file("bookings.csv");
    string line;

    string idStr, statusStr, pickupStr, dropoffStr, typeStr, fareStr, passengerIdStr, driverIdStr, bookedAtStr, startedAtStr, completedAtStr;
    Booking booking;

    if (file)
    {
        while (getline(file, line))
        {
            string* fields[11] = { &idStr, &statusStr, &pickupStr, &dropoffStr, &typeStr, &fareStr, &passengerIdStr, &bookedAtStr, &driverIdStr, &startedAtStr, &completedAtStr };
            getFields(line, fields, 11);

            if ((!isPassenger && stoi(driverIdStr) == id) || (isPassenger && stoi(passengerIdStr) == id))
            {
                booking.load(stoi(idStr), statusStr, pickupStr[0], dropoffStr[0], typeStr, stoi(fareStr), stoi(passengerIdStr), bookedAtStr, stoi(driverIdStr), startedAtStr, completedAtStr);
                booking.display(isPassenger);
            }
        }

        file.close();
    }
}

// Add string comparison operators
bool operator==(const string& str1, const string& str2) {
    return str1.compare(str2) == 0;
}

bool operator!=(const string& str1, const string& str2) {
    return str1.compare(str2) != 0;
}

#endif // USER_MANAGEMENT_H 