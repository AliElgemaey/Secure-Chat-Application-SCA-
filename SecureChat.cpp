#include <string>
#include <sstream>
#include <list>
#include <vector>
#include<fstream>
#include <iostream>
using namespace std;

const string usersFile = "users.txt";
const string messagesFile = "messages.txt";

/*
	Class: RSACipher
    This class implements the RSA encryption 
	and decryption algorithm.
    It provides methods to generate RSA keys, 
	encrypt messages, and decrypt messages.
*/

class RSACipher{
public:
	// Static members to hold prime numbers (p,q), modulus (n), 
	// public exponent (e), and private exponent (d)
	static int p, q, n, e, d;
	    
	// this method computes the greatest common divisor of two numbers
	static int gcd(int num1, int num2) {
		if (num2 == 0)
			return num1;
		return gcd(num2, num1 % num2);
	}

	/**
	This function generates RSA encryption and decryption keys.
    It calculates the modulus 'n', 
	public exponent 'e', and private exponent 'd'.
    It iterates through possible values of 'e' 
	and 'd' until finding ones that satisfy RSA requirements.
	*/
	static void generateKeys() {
		n = p * q;
		int phi = (p - 1) * (q - 1);
		for (e = 2; e < phi; e++) {
			if (gcd(e, phi) == 1)
				break;
		}
		for (d = 2; d < phi; d++) {
			if ((d * e) % phi == 1)
				break;
		}
	}

	static long long int modExp(long long int base, long long int exponent, long long int modulus) {
		// Initialize the result to 1
		long long int result = 1;
		// Take base modulo modulus to handle large values of base
		base = base % modulus;
		while (exponent > 0) {
			if (exponent % 2 == 1)
				result = (result * base) % modulus;
			// Right shift exponent to divide it by 2
			exponent = exponent >> 1;
			// Square base: base = (base * base) mod modulus
			base = (base * base) % modulus;
		}
		return result;
	}
	static string encrypt(string message) {
		string encryptedMessage = "";
		for (char c : message) {
			long long int m = c;
			long long int cryptedCh = modExp(m, e, n);
			encryptedMessage += to_string(cryptedCh) + " ";
		}
		return encryptedMessage;
	}

	static string decrypt(string message) {
		string decryptedMassage = "";
		for (string::size_type i = 0; i < message.length(); i++) {
			string num_str = "";
			while (message[i] != ' ' && i < message.length()) {
				num_str += message[i];
				i++;
			}
			long long int crypted = stoll(num_str);
			long long int decrypted = modExp(crypted, d, n);
			decryptedMassage += static_cast<char>(decrypted);
		}
		return decryptedMassage;
	}
};

int RSACipher::p = 11;
int RSACipher::q = 13;
int RSACipher::n;
int RSACipher::e;
int RSACipher::d;

/**
This class represents a User in this chat application.
It stores the username, encrypted password, and an authorized key.
*/
class User {
public:
    string userName;
    string password;
    string authorizedKey;
};

/**
This class represents a node in the hash table.
It holds a User object and its associated key.
*/
class UserNode {
public:
    User value;
    string key;
	UserNode(const string& key, const User& value){
		this->value = value;
		this->key = key;
	}
};

/**
This class implements a hash table to store User Info.
*/
class UserHashTable {
private:
   // Vector of linked lists for storing user nodes
    vector<list<UserNode*>> userHashTable;
	// Number of users in the hash table
    int size;
    int getHash(const string& key) const {
        return hash<string>()(key) % userHashTable.size();
    }
public:
    // constructor to initialize all users hashtable
    UserHashTable(int initSize = 10) : size(0) {
        userHashTable.resize(initSize);
    }
    // destructor to clear all users hashtable
    ~UserHashTable() {
        clear();
    }

    // Adds a new user to the hash table.
    bool add(const string& key, const User& value) {
		int index = getHash(key);
        for (UserNode* node : userHashTable[index]) {
            if (node->key == key) {
                // If a user with the same key already exists, do not add them
                return false;
            }
        }
		//new user so add it
        userHashTable[index].push_back(new UserNode(key,value));
        size++;
		return true;
    }

    // Checks if a user exists in the hashtable of all users or not.
    bool isUserExist(const string& userName,const string& password,const string& authorizedKey) {
		int index = getHash(userName);
        for (UserNode* node : userHashTable[index]) {
			if (node->key == userName && node->value.password == password && node->value.authorizedKey == authorizedKey) {
                return true;
            }
        }
        return false;
    }

    // empty all users hashtable
    void clear() {
		userHashTable.clear();
        size = 0;
    }

    // Saves all user data to a file.
	void writeData(){
		ofstream outFile(usersFile);
		string key;
		if (outFile.is_open()) {
			// Iterate through each index in hashtable
			for(int index = 0 ; index < userHashTable.size(); index++){
				// Iterate through each node in current index in hashtable
				for (UserNode* node : userHashTable[index]) {
					// if node is not null, Write his/here data to the file
					if(node == NULL)
						continue;
					outFile << node->value.userName  << "," << node->value.password << "," << node->value.authorizedKey<<"\n";
				}
			}
			outFile.close();
			cout<<"User Data has been written to "<<usersFile<<" successfully\n";
		}
	}
};


////////////////////////////////
////////////////////////////////

/**
This class represents a Message in this chat application.
It stores the senderName, receiverName, and message it self.
*/
class Message {
public:
    string senderName;
    string receiverName;
    string message;
};

/**
This class represents a node in the hash table.
It holds a Message object and its associated key (receiverName).
*/
class MessageNode {
public:
    Message value;
    string key;
	MessageNode(const string& key, const Message& value){
		this->value = value;
		this->key = key;
	}
};

/**
This class implements a hash table to store User Info.
*/
class MessageHashTable {
private:
    // Vector of linked lists for storing message nodes
    vector<list<MessageNode*>> messageHashTable;
	// Number of messages in the hash table
    int size;
    int getHash(const string& key) const {
        return hash<string>()(key) % messageHashTable.size();
    }
public:
    MessageHashTable(int initSize = 10) : size(0) {
        messageHashTable.resize(initSize);
    }

    ~MessageHashTable() {
        clear();
    }

    // Adds a message to the hash table or updates an existing message if the key already exists.
	void add(const string& key, const Message& value) {
		int index = getHash(key);
		for (MessageNode* node : messageHashTable[index]) {
            if (node->key == key) {
				node->value.message += value.message;
            }
        }
		// If no node with the key is found, add a new node with the key and message value to the linked list
		messageHashTable[index].push_back(new MessageNode(key,value));
        size++;
    }

    // Retrieves the message associated with the given key from the hash table.
    string getMessage(const string& key) {
		int index = getHash(key);
        for (MessageNode* node : messageHashTable[index]) {
            if (node->key == key) {
				return node->value.message;
            }
        }
        return "";
    }

    // empty all messages hashtable
    void clear() {
		messageHashTable.clear();
        size = 0;
    }

    // Saves all message data to a file.
	void writeData(){
		ofstream outFile(messagesFile);
		string key;
		if (outFile.is_open()) {
			for(int index = 0 ; index < messageHashTable.size(); index++){
				for (MessageNode* node : messageHashTable[index]) {
					if(node == NULL)
						continue;
					outFile << node->value.senderName << "," << node->value.receiverName << "," << node->value.message<<"\n";
				}
			}
			outFile.close();
			cout<<"Messgaes Data has been written to "<<messagesFile<<" successfully\n";
		}
	}
};


// This Class representing a secure chat application.
class SecureChatApplication {
private:
// Hash table for storing user information
	UserHashTable usersHashTable;
	// Hash table for storing message information
	MessageHashTable messagesHashTable;
	// Current user logged in to the application
	string currUser;

public:
	SecureChatApplication(){
		readUsersData();
		readMessagesData();
		currUser = "";
	}

    // Logs in to the application using the provided credentials.
    void login() {
		string userName, password, authorizedKey;
		cout << "Enter User Name: ";
        getline(cin, userName);
        cout << "Enter Password: ";
        getline(cin, password);
        cout << "Enter Authorized Key: ";
        getline(cin, authorizedKey);
		// Encrypt password using RSA encryption
		password = RSACipher::encrypt(password);
		// Encrypt authorized key using RSA encryption
		authorizedKey = RSACipher::encrypt(authorizedKey);
		// Check if the user exists in the hash table with the provided info
		if(usersHashTable.isUserExist(userName,password,authorizedKey)){
			currUser = userName;
			cout<<"You have successfully login\n";
			chatMenu();
		}
		else{
			cout<<"Invalid Login Data\n";
		}
    }

	void signUp() {
		string userName, password, repassword, authorizedKey;
		cout << "Enter User Name: ";
        getline(cin, userName);
        cout << "Enter Password: ";
        getline(cin, password);
		cout << "Enter Re-Password: ";
        getline(cin, repassword);
		if(password != repassword){
			cout<<" password must match re-password, fail to signup\n";
			return;
		}
        cout << "Enter Authorized Key: ";
        getline(cin, authorizedKey);
		
		password = RSACipher::encrypt(password);
		authorizedKey = RSACipher::encrypt(authorizedKey);
		User userObj;
		userObj.userName = userName;
		userObj.password = password;
		userObj.authorizedKey = authorizedKey;
		if(usersHashTable.add(userName,userObj)){
			cout<<"You have successfully SignUp\n";
		}
		else{
			cout<<"Invalid Signup Data this user name is already exists try to login\n";
		}
    }

    // Displays the chat menu and allows user interaction.
	void chatMenu(){
		bool exitMenu;
		int choice;
		for(exitMenu = false ; exitMenu != true;) {
			cout << "\n\n**** Chat Menu ****\n";
			cout << "1. Send Message\n";
			cout << "2. Show Recieved Messages\n";
			cout << "3. Log out\n";
			cout << "your choice [1-3]: ";
			// Read user's choice from the console
			cin >> choice;
			// Ignore any remaining newline characters in the input buffer
			cin.ignore();
			switch (choice) {
				case 1: 
					sendMessage();
					break;
				case 2: 
					showRecievedMessages();
					break;
				case 3:
					exitMenu = true;
					break;
				default:
					cout << "Invalid choice. try again [1-3].\n";
					break;
			}
		}
	}

    // Sends a message to the specified receiver.
	void sendMessage(){
		string receiverName, message;
        cout << "Enter receiver name: ";
        getline(cin, receiverName);
        cout << "Enter message: ";
        getline(cin, message);
		// Create a Message object
		Message messageObj;
		message = "\n"+currUser+": "+message;
		// Encrypt the message using RSA encryption
		message = RSACipher::encrypt(message);
		messageObj.senderName = currUser;
		messageObj.receiverName = receiverName;
		messageObj.message = message;
		// Add the message to the message hash table
		messagesHashTable.add(receiverName,messageObj);
		cout << "Message sent successfully to "<<receiverName<<"\n";
	}

    // Displays received messages for the current user.
	void showRecievedMessages(){
		// Retrieve the encrypted message for the current user
		string message = messagesHashTable.getMessage(currUser);
		// Display message for no received messages
		if(message == ""){
			cout<<"You did not received any message yet !!!\n";
			return;
		}
		// Decrypt and display the received messages
		cout<<"Your messages are\n";
		cout<<RSACipher::decrypt(message)<<endl;
	}

    // Reads user data from a file and populates the user hash table.
	void readUsersData(){
		ifstream inFile(usersFile);
		if (inFile.is_open()) {
			cout<<"Loading Users Data from file  ......\n";
			string line;
			User userObj;
			while (getline(inFile, line)) {
				stringstream ss(line);
				// Temporary string to store parsed values
				string temp;
				// Extract username from the line
				getline(ss, userObj.userName, ',');
				getline(ss, userObj.password, ',');
				getline(ss, userObj.authorizedKey, ',');
				// Add user to the user hash table
				usersHashTable.add(userObj.userName, userObj);
			}
			inFile.close();
		}
	}

    // Reads message data from a file and populates the message hash table.
	void readMessagesData(){
		ifstream inFile(messagesFile);
		if (inFile.is_open()) {
			cout<<"Loading Messages Data from file  ......\n";
			// Variable to store each line read from the file
			string line;
			// Temporary Message object to store message information
			Message messageObj;
			while (getline(inFile, line)) {
				stringstream ss(line);
				string temp;
				getline(ss, messageObj.senderName, ',');
				getline(ss, messageObj.receiverName, ',');
				getline(ss, messageObj.message, ',');
				messagesHashTable.add(messageObj.receiverName, messageObj);
			}
			inFile.close();
		}
	}
	
    // Writes message data to a file.
	void writeMessagesData(){
		// Call the writeData() method of the message hash table
		messagesHashTable.writeData();
	}
    // Writes user data to a file.
	void writeUsersData(){
		// Call the writeData() method of the user hash table
		usersHashTable.writeData();
	}
};

// The main function of the Secure Chat Application System (SCAS).
int main() {
    cout << "Welcome to Our Secure Chat Application System (SCAS)\n\n";
	RSACipher::generateKeys();
    SecureChatApplication sca;
	// Flag to control the loop
    bool closeApp;
	// Variable to store user's choice
	int choice;
	// Main menu loop
    for(closeApp = false ; closeApp != true;) {
        cout << "\n\n**** SCAS Main Menu ****\n";
        cout << "1. Login\n";
        cout << "2. SignUp\n";
        cout << "3. Exit\n";
        cout << "your choice [1-3]: ";
		// Read user's choice
        cin >> choice;
		// Ignore any remaining newline characters in the input buffer
        cin.ignore();
        switch (choice) {
            case 1: 
				sca.login();
                break;
            case 2: 
                sca.signUp();
                break;
			case 3:
				closeApp = true;
				break;
            default:
                cout << "Invalid choice. try again [0-3].\n";
                break;
        }
    }
	
	// Write user data to file
	sca.writeUsersData();
	// Write message data to file
	sca.writeMessagesData();
	system("pause");
    return 0;
}