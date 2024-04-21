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
		long long int result = 1;
		base = base % modulus;
		while (exponent > 0) {
			if (exponent % 2 == 1)
				result = (result * base) % modulus;
			exponent = exponent >> 1;
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
    vector<list<UserNode*>> userHashTable;
    int size;
    int getHash(const string& key) const {
        return hash<string>()(key) % userHashTable.size();
    }
public:
    UserHashTable(int initSize = 10) : size(0) {
        userHashTable.resize(initSize);
    }

    ~UserHashTable() {
        clear();
    }

    bool add(const string& key, const User& value) {
		int index = getHash(key);
        for (UserNode* node : userHashTable[index]) {
            if (node->key == key) {
                // user already exists,do not add him/her
                return false;
            }
        }
		//new user so add it
        userHashTable[index].push_back(new UserNode(key,value));
        size++;
		return true;
    }

    bool isUserExist(const string& userName,const string& password,const string& authorizedKey) {
		int index = getHash(userName);
        for (UserNode* node : userHashTable[index]) {
			if (node->key == userName && node->value.password == password && node->value.authorizedKey == authorizedKey) {
                return true;
            }
        }
        return false;
    }

    void clear() {
		userHashTable.clear();
        size = 0;
    }

	void writeData(){
		ofstream outFile(usersFile);
		string key;
		if (outFile.is_open()) {
			for(int index = 0 ; index < userHashTable.size(); index++){
				for (UserNode* node : userHashTable[index]) {
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
    vector<list<MessageNode*>> messageHashTable;
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

	void add(const string& key, const Message& value) {
		int index = getHash(key);
		for (MessageNode* node : messageHashTable[index]) {
            if (node->key == key) {
				node->value.message += value.message;
            }
        }
		//new user so add it
		messageHashTable[index].push_back(new MessageNode(key,value));
        size++;
    }

    string getMessage(const string& key) {
		int index = getHash(key);
        for (MessageNode* node : messageHashTable[index]) {
            if (node->key == key) {
				return node->value.message;
            }
        }
        return "";
    }

    void clear() {
		messageHashTable.clear();
        size = 0;
    }

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



class SecureChatApplication {
private:
	UserHashTable usersHashTable;
	MessageHashTable messagesHashTable;
	string currUser;

public:
	SecureChatApplication(){
		readUsersData();
		readMessagesData();
		currUser = "";
	}

    void login() {
		string userName, password, authorizedKey;
		cout << "Enter User Name: ";
        getline(cin, userName);
        cout << "Enter Password: ";
        getline(cin, password);
        cout << "Enter Authorized Key: ";
        getline(cin, authorizedKey);
		password = RSACipher::encrypt(password);
		authorizedKey = RSACipher::encrypt(authorizedKey);
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

	void chatMenu(){
		bool exitMenu;
		int choice;
		for(exitMenu = false ; exitMenu != true;) {
			cout << "\n\n**** Chat Menu ****\n";
			cout << "1. Send Message\n";
			cout << "2. Show Recieved Messages\n";
			cout << "3. Log out\n";
			cout << "your choice [1-3]: ";
			cin >> choice;
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

	void sendMessage(){
		string receiverName, message;
        cout << "Enter receiver name: ";
        getline(cin, receiverName);
        cout << "Enter message: ";
        getline(cin, message);
		Message messageObj;
		message = "\n"+currUser+": "+message;
		message = RSACipher::encrypt(message);
		messageObj.senderName = currUser;
		messageObj.receiverName = receiverName;
		messageObj.message = message;
		messagesHashTable.add(receiverName,messageObj);
		cout << "Message sent successfully to "<<receiverName<<"\n";
	}

	void showRecievedMessages(){
		string message = messagesHashTable.getMessage(currUser);
		if(message == ""){
			cout<<"You did not received any message yet !!!\n";
			return;
		}
		cout<<"Your messages are\n";
		cout<<RSACipher::decrypt(message)<<endl;
	}

	void readUsersData(){
		ifstream inFile(usersFile);
		if (inFile.is_open()) {
			cout<<"Loading Users Data from file  ......\n";
			string line;
			User userObj;
			while (getline(inFile, line)) {
				stringstream ss(line);
				string temp;
				getline(ss, userObj.userName, ',');
				getline(ss, userObj.password, ',');
				getline(ss, userObj.authorizedKey, ',');
				usersHashTable.add(userObj.userName, userObj);
			}
			inFile.close();
		}
	}

	void readMessagesData(){
		ifstream inFile(messagesFile);
		if (inFile.is_open()) {
			cout<<"Loading Messages Data from file  ......\n";
			string line;
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
	

	void writeMessagesData(){
		messagesHashTable.writeData();
	}

	void writeUsersData(){
		usersHashTable.writeData();
	}
};


int main() {
    cout << "Welcome to Our Secure Chat Application System (SCAS)\n\n";
	RSACipher::generateKeys();
    SecureChatApplication sca;
    bool closeApp;
	int choice;
    for(closeApp = false ; closeApp != true;) {
        cout << "\n\n**** SCAS Main Menu ****\n";
        cout << "1. Login\n";
        cout << "2. SignUp\n";
        cout << "3. Exit\n";
        cout << "your choice [1-3]: ";
        cin >> choice;
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
	
	sca.writeUsersData();
	sca.writeMessagesData();
	system("pause");
    return 0;
}