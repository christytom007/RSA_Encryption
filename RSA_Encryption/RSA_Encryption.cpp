#include<iostream>
#include<math.h>
#include<string>
#include<stdlib.h>
#include <fstream>
#include<time.h>

using namespace std;
//Prime Number
long int p, q;

//First part of public key : n = p * q
long int n;

//Value Phi : phi = (p - 1) * (q - 1)
long int phi;

//Encryption Key
long int e;

//Decryption key
long int d;

//Temporary Variables
long int j, i;

//Setting string size
const int SIZE = 100;

//This Variable constains message in ASCII code
long int messageASCII[SIZE], messageASCIIFile;

//This Variable contain Encrypted Message
long int en[SIZE], enFile;

//This Variable contain decrypted Message
long int dn[SIZE], dnFile;

//The Original Message
string inputMessage;

//Function to find if the number is prime or not
int prime(long int);

//Function to generate Value of 'e' and 'd'
void findEncryptionDecryptionKeys();

//Function for generate 'd' condition -> d*e = 1 + k * phi 
long int generateDecryptionKey(long int);

//Encryption Function
void encrypt();

//Decryption Function
void decrypt();

//Encryption Function for File
void encryptFile();

//Decryption Function for File
void decryptFile();

//Prime number finding function
int prime(long int pr);

int main()
{
	//Common variables used in Main
	int ch, sizeFile = 0; //For checking file Size
	ifstream readFile, checkFile; //Files that are read only
	ofstream writeFile; //File that is write only
	string fileName1, fileName2; //for getting file name
	char oneChar; //For reading one char at a time in file

	//Setting Default Values
	p = 47;
	q = 53;

	do {
		//Display Menu
		cout << (char)201;
		for (int i = 0; i < 116; i++) cout << (char)205;
		cout << (char)187 << endl << (char) 186;
		for (int i = 0; i < 116; i++) cout << " ";
		cout << (char)186 << endl << (char)186;
		cout << "\t\t\t\t\tRSA ENCRYPTION MENU\t\t\t\t\t\t\t     ";
		cout << (char)186 << endl << (char)186;
		for (int i = 0; i < 116; i++) cout << " ";
		cout << (char)186 << endl << (char)200;
		for (int i = 0; i < 116; i++) cout << (char)205;
		cout << (char)188 << endl;

		cout << "\t\t\t\t\t1. ENCRYPT MESSAGE & DECRYPT MESSAGE \n" << endl;
		cout << "\t\t\t\t\t2. ENCRYPT FILE\n" << endl;
		cout << "\t\t\t\t\t3. DECRYPT FILE \n" << endl;
		cout << "\t\t\t\t\t4. CHANGE PRIME NUMBERS \n" << endl;
		cout << "\n\t\t\t\tPLEASE ENTER YOUR CHOICE ( '0' TO EXIT ) : ";
		cin >> ch;// Getting the Choice from user

		switch (ch) {
		case 1:// Message Encryption

			system("cls");//Cleaning the Screen 

			cout << "\nPLEASE ENTER THE MESSAGE YOU WANT TO ENCRYPT\n\n";
			fflush(stdin);//Clean the Input buffer Just in case
			cin.ignore(512, '\n');
			getline(cin, inputMessage);

			//copying message to its ASCII value to another array
			for (i = 0; inputMessage[i] != '\0'; i++)
				messageASCII[i] = inputMessage[i];

			//Generating the First part of the Public key
			n = p * q;
			//Generating the first part of Private Key
			phi = (p - 1) * (q - 1);
			//Generating e & d
			findEncryptionDecryptionKeys();

			//Encrypting the Message
			encrypt();
			//Printing the Encrypted string
			cout << "\n\nTHE ENCRYPTED MESSAGE IS WITH P : "<<p<< " & Q : "<<q<< "\n\n";
			for (i = 0; en[i] != -1; i++)
				printf("%c", en[i]);

			//Decrypting the message
			decrypt();
			//Printing the decrypted Message
			cout << "\n\n\nTHE DECRYPTED MESSAGE IS P : " << p << " & Q : " << q << "\n\n";
			for (i = 0; dn[i] != -1; i++)
				printf("%c", dn[i]);

			break;

		case 2:// ENCRYPT FILE
			system("cls");

			//Getting the file name which you want to encrypt
			cout << "\n\nTHE AVAILABLE FILES IN DIRECTORY....\n\n";
			system("DIR *.txt"); //Displaying available files in directory
		fileEncryptValidation1:;
			cout << "\n\nEnter the Name of the File you want to Encrypt (without .txt) : ";
			cin >> fileName1;
			//adding .txt in the extention
			fileName1 += ".txt";
			//Opening the file 
			readFile.open(fileName1.c_str());
			if (readFile.is_open() == false) {//Checking if the file is open or not
				cout << "\n\CAN NOT OPEN THE SPECIFIED FILE....\n\n";
				goto fileEncryptValidation1;
			}

			//Getting the file name which you want to save the encryption
		fileEncryptValidation2:;
			cout << "\n\nEnter the Name of the File you want to save Encrypted Data (without .txt) : ";
			cin >> fileName2;
			//adding .txt in the extention
			fileName2 += ".txt";
			if (fileName1.compare(fileName2) == 0) {
				//Both read and Write file is same
				cout << "\n\nTHE ENCRYPTION FILE AND DECRYPTION FILES ARE SAME, PLEASE ENTER A DIFFERENT NAME....\n\n"<<fileName2;
				goto fileEncryptValidation2;
			}
			//Opening the file for checking
			checkFile.open(fileName2.c_str(), ios::binary);
			if (checkFile.is_open() == true) {//Checking if there is already a file named as the user input
				cout << "\n\FILE ALREADY EXIST, PLEASE ENTER A DIFFERENT NAME....\n\n";
				checkFile.close();
				goto fileEncryptValidation2;
			}
			//creating the file for writing
			writeFile.open(fileName2.c_str(), ios::binary);
			if (writeFile.is_open() == false) {//Checking if the file is created or not
				cout << "\n\CAN NOT OPEN THE SPECIFIED FILE....\n\n";
				goto fileEncryptValidation2;
			}

			n = p * q;//Generating the First part of the Public key
			phi = (p - 1) * (q - 1);//Generating the first part of Private Key
			findEncryptionDecryptionKeys();//Generating Encryption & decryption keys
			
			while (readFile.get(oneChar)) {//Reading file char by char and encrypting it
				messageASCIIFile = oneChar;
				encryptFile();//encrypt it
				writeFile.write((char*)& enFile, sizeof(enFile));//writing it to file
			}
			//Closing all the files
			readFile.close();
			writeFile.close();
			cout << "\n\n\tFile " << fileName1 << " Succesfully Encrypted and Saved in " << fileName2;
			break;

		case 3://DECRYPT FILE
			system("cls");

			//Getting the file name which you want to decrypt
			cout << "\n\nTHE AVAILABLE FILES IN DIRECTORY....\n\n";
			system("DIR *.txt"); //Displaying available files in directory
		fileDecryptValidation1:;
			cout << "\n\nEnter the Name of the File you want to Decrypt (without .txt) : ";
			cin >> fileName1;
			//adding .txt in the extention
			fileName1 += ".txt";
			//Opening the file 
			readFile.open(fileName1.c_str(), ios::binary);
			if (readFile.is_open() == false) {//Checking if the file is open or not
				cout << "\n\CAN NOT OPEN THE SPECIFIED FILE....\n\n";
				goto fileDecryptValidation1;
			}

			//Getting the file name which you want to save the decrypted message
		fileDecryptValidation2:;
			cout << "\n\nEnter the Name of the File you want to save decrypted Data (without .txt) : ";
			cin >> fileName2;
			//adding .txt in the extention
			fileName2 += ".txt";
			if (fileName1.compare(fileName2) == 0) {
				//Both read and Write file is same
				cout << "\n\nTHE DECRYPTION FILE AND DECRYPTED FILE ARE SAME, PLEASE ENTER A DIFFERENT NAME....\n\n" << fileName2;
				goto fileDecryptValidation2;
			}
			//Opening the file for checking
			checkFile.open(fileName2.c_str(), ios::binary);
			if (checkFile.is_open() == true) {//Checking if there is already a file named as the user input
				cout << "\n\FILE ALREADY EXIST, PLEASE ENTER A DIFFERENT NAME....\n\n";
				checkFile.close();
				goto fileDecryptValidation2;
			}
			//creating the file for writing
			writeFile.open(fileName2.c_str(), ios::binary);
			if (writeFile.is_open() == false) {//Checking if the file is open or not
				cout << "\n\CAN NOT OPEN THE SPECIFIED FILE....\n\n";
				goto fileDecryptValidation2;
			}

			n = p * q; //Generating the First part of the Public key
			phi = (p - 1) * (q - 1); //Generating the first part of Private Key
			findEncryptionDecryptionKeys(); //Generating Encryption & decryption keys

			readFile.seekg(0, ios::end); //Putting the File pointer to the End of file
			sizeFile = (int)readFile.tellg(); //Getting the size of the file
			readFile.seekg(0, ios::beg); //putting back the file pointer in the begining

			while (readFile.tellg() < sizeFile) {

				readFile.read((char*)& messageASCIIFile, sizeof(messageASCIIFile)); //Getting each char from the file
				decryptFile(); //decrypt it
				writeFile << (char)dnFile; //writing it to file
			}
			//Closing all the files
			readFile.close();
			writeFile.close();

			cout << "\n\n\tFile " << fileName1<< " Succesfully Decrypted and Saved in "<< fileName2;
			break;

		case 4://Changeing the Prime number Values

			system("cls");
			//Displaying Current Prime Numbers
			cout << "\nCURRENT PRIME NUMBERS IN USE\n";
			cout << "\n\t P = " << p;
			cout << "\n\t Q = " << q;

			char c;
			cout << "\n\nDo you want to change the Prime Numbers? (Y/N) : ";
			cin >> c;
			if (c == 'N' || c == 'n')
				//Getting out of the Changing prime numbers
				goto outOfSwitch;

			//Getting the New Prime Numbers
		primeValidation1:;//Validation flag
			cout << "\n\nPLEASE ENTER THE NEW PRIME NUMBERS";
			cout << "\n\nENTER 1st PRIME NUMBER ABOVE 50 : ";
			cin >> p;
			//Checking if number is less than 50
			if (p < 50 || cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore(514, '\n');
				cout << "\n\nINVALID INPUT.... PLEASE ENTER AGAIN .....";
				goto primeValidation1;
			}
			//Checking the inputted Prime number
			if (prime(p) == 0)
			{
				cout << "\n\nINPUTTED NUMBER IS NOT A PRIME.....\n\n";
				//Recommending the closest Prime number
				while (1) {
					p++;
					if (prime(p) == 1)
						break;
				}
				cout << "The Closest Prime Number is : " << p << endl;
				char c;
				cout << "\nDo you want to use it ? (Y/N) : ";
				cin >> c;
				if (c == 'n' || c == 'N')
					//Get the First prime number again
					goto primeValidation1;
			}

			//Getting the 2nd Prime Number
		primeValidation2:;
			cout << "\n\nENTER 2nd PRIME NUMBER ABOVE 50 : ";
			cin >> q;
			//Checking if number is less than 50
			if (q < 50 || cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore(514, '\n');
				cout << "\n\nINVALID INPUT.... PLEASE ENTER AGAIN .....";
				goto primeValidation2;
			}
			if (p == q) {
				cout << "\n\nINPUTTED VALUE FOR P & Q IS SAME....PLEASE ENETR AGAIN.....\n";
				//Get the 2nd Prime number again
				goto primeValidation2;
			}
			if (prime(q) == 0)
			{
				cout << "\n\nINPUTTED NUMBER IS NOT A PRIME.....\n\n";
				//Recommending the closest Prime number
				while (1) {
					q++;
					if (prime(q) == 1)
						break;
				}
				//if the new q is same as p
				if (p == q) {
					//Find the next prime number
					while (1) {
						q++;
						if (prime(q) == 1)
							break;
					}
				}
				cout << "The Closest Prime Number is : " << q << endl;
				char c;
				cout << "\nDo you want to use it ? (Y/N) : ";
				cin >> c;
				if (c == 'n' || c == 'N')
					//Get the 2nd prime number again
					goto primeValidation2;
			}

			cout << "\n\n\tPrime Numbers Succesfully updated.....";
			break;

		case 0:
			cout << "\n\nExiting the application....";
			exit(1);
			break;

		default:
			cout << "\n\nInvalid Input....";
		}

		//flag for getting out of switch
	outOfSwitch:;
		cout << "\n\n\n";
		system("pause");//Menu Pause

		system("cls");//Cleaning the Screen

	} while(1);//Infinate Loop

	return 0;
}

int prime(long int pr)
{//Function for checking if the Number is Prime number or not
	int i;

	j = sqrt(pr);//Finding Squre root to reduce calculations
	for (i = 2; i <= j; i++)
	{
		if (pr % i == 0)//Finding the Multiple
			return 0;
	}
	return 1;
}


void findEncryptionDecryptionKeys()
{//Finding value of e & d

	for (i = 2; i < phi; i++)
	{
		// e must be co-prime to phi and smaller than phi 

		if (phi % i == 0)// e shoould not be multiple of phi (Making the program faster)
			continue;
		// Cheking if 'i' is prime
		if (prime(i) == 1 && i != p && i != q)
		{
			//'e' have to be prime and not 'p' and 'q'
			e = i;

			// Private key (d stands for decrypt) 
			// choosing d such that it satisfies 
			// d * e = 1 + k * phi
			int temD = generateDecryptionKey(e);

			//checking if Generated Decryption key is not less than 0
			if (temD > 0)
			{
				d = temD;
				break;
			}
		}
	}
}

long int generateDecryptionKey(long int x)
{//This function Generate the Decryption key

	long int k = 1;
	while (1)
	{
		k = k + phi;
		if (k % x == 0)
			return (k / x);
	}
}


void encrypt()
{//Encrypting the Values
	long int pt, k, len;
	i = 0;

	//Finding the length of the message
	len = inputMessage.length();

	while (i != len)
	{
		pt = messageASCII[i] - 96; //Reducing the ASCII code to integer value 96 | 97 - 'a'
		k = 1; //k is constant Value 1
		for (j = 0; j < e; j++)
		{
			//Applaying public Key to Encrypt
			k = k * pt;
			k = k % n;
		}
		//Adding 96 to make it from 'a' in ASCII
		en[i] = k + 96;
		i++;
	}
	//Putting -1 in the end of the Message to show the string ending
	en[i] = -1;
}
void decrypt()
{//Decrypting the Values
	long int ct, k;
	i = 0;
	while (en[i] != -1)
	{//until i reach -1
		ct = en[i] - 96; //Reducing the ASCII code to integer value 96 | 97 - 'a'
		k = 1; //k is constant Value 1
		for (j = 0; j < d; j++)
		{
			k = k * ct;
			k = k % n;
		}
		dn[i] = k + 96; //Adding 96 to make it from 'a' in ASCII
		i++;
	}
	dn[i] = -1; //Putting -1 in the end of the Message to show the string ending
}

void encryptFile() {
	//Encrypting the Values
	long int pt, k;

	pt = messageASCIIFile - 96; //Reducing the ASCII code to integer value 96 | 97 - 'a'
	k = 1; //k is constant Value 1
	for (j = 0; j < e; j++)
	{
		//Applaying public Key to Encrypt
		k = k * pt;
		k = k % n;
	}
	//Adding 96 to make it from 'a' in ASCII
	enFile = k + 96;
}

void decryptFile() {
	long int ct, k;
	//Finding the length of the message

	ct = messageASCIIFile - 96; //Reducing the ASCII code to integer value 96 | 97 - 'a'
	k = 1; //k is constant Value 1
	for (j = 0; j < d; j++)
	{
		k = k * ct;
		k = k % n;
	}
	dnFile = k + 96; //Adding 96 to make it from 'a' in ASCII
}