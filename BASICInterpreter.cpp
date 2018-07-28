#include<iostream>
#include<stdlib.h>
#include<map>
#include <string>
#include <sstream>
#include <vector>
#include<cstring>
#include<vector>
#include<fstream>
#include<ctype.h>
#include<bits/stdc++.h>

using namespace std;

//Exception declarations
class EndQuotesException{};
class InitialQuotesException{};
class NumberInputException{};
class ENDIFException{};
class GOTOEXException{};
class WrongstatementException{};
class Nonexistentvariable{};

//Abstract base class//
class base{

public:
	//virtual function is overidden by each of the derived classes
	virtual void execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines)=0;
};

//Derived IF statement Class//
class IF:public base
{
private:
	string input;    //Input string to IF
	string A,B;      //string to store separated arguments
	int A1,A2;       //Arguments converted into integer for comparison
	int operator1;   //operator

public:
	//execute Function shared with all the derived classes and base class//
	void execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines);

	void checkOperator();
	int checkCondition(int A,int B,int Operator);
	inline bool isInteger(const std::string & s);
	int checkVariable(map<string, double> variables);
	int findEndIF(std::vector<std::vector<string> >arr,int line,int lines);
	~IF();
};

//Checks the conditions once the operator and the Arguments are assigned as integers
int IF::checkCondition(int A,int B,int Operator){
	if (Operator==1){if(A>B){return 1;} else{return 0;}}
	if(Operator==2){if(A<B){return 1;} else{return 0;}}
	if(Operator==3){if(A==B){return 1;} else{return 0;}}
	return 0;
}
//Checks the operator present within the string 'input'
void IF::checkOperator(){
	char str[input.length()];
	strcpy(str,input.c_str());
	int i;
	char operators1[3]={'>','<','='};
	char *pch;
	string words[2];
	int found;
	for(i=0;i<3;i++){
		found=input.find(operators1[i]);
		if(input[found]>0){
			operator1=i+1;
		}}
	i=0;
	pch=strtok(str,">=<");
	while(pch !=NULL){
		words[i]=pch;
		pch=strtok(NULL,">=<");
		i++;
	}
	A=words[0];
	B=words[1];

}
//checks whether an argument is an integer or not
inline bool IF:: isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
	char * p ;
	strtol(s.c_str(), &p, 10) ;
	return (*p == 0) ;
}

//check variables uses isInteger to check whether the argument is a variable or number and assigns A and B accordignly
int IF::checkVariable(map<string, double> variables){

	if(isInteger(A)==false){

		A1=variables[A];
	}
	else{
		std::istringstream ss(A);
		ss >> A1;
	}
	if(isInteger(B)==false){
		A2=variables[B];
	}
	else{
		std::istringstream ss(B);
		ss >> A2;
	}
}
//Counts EndiF from the IF line and checks whether there are enough and ENDIF and returns the positions of ENDIF
int IF::findEndIF(std::vector<std::vector<string> >arr,int line,int lines){

	int countifs=1;
	int i;
	for(i=line+1;i<lines;i++)
	{
		if(arr[i][1]=="IF")
		{
			countifs++;
		}
		if(arr[i][1]=="ENDIF"){
			countifs--;
			if(countifs==0){return i;}
		}
	}
	if(countifs!=0){

		line=lines+1;           //increments line in program class to end of program to exit//
		throw ENDIFException{}; //throws exception if not enough ENDIFs found//
		return 0;
	}
}

//Execute function checks the operator first and assigns the string A1 and B1 and checks if these are variables//
//The checkCondition is then called to evaluate the condition//
void IF::execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines){

	input=arr[line][2];
	checkOperator();
	checkVariable(variables);
	if(checkCondition(A1,A2,operator1)==1&&findEndIF(arr,line,lines)>0){

		line++;   //uses the reference from execute to increment line number in program class to proceed to the next line
	}
	if(checkCondition(A1,A2,operator1)==0&&findEndIF(arr,line,lines)>0){

		line=findEndIF(arr,line,lines);  //if If statement not satisfied, line number set to the corresponding ENDIF
	}
}





//LET FUNCTION
class let:public base{
private:
	string input;
	int operator1;
	string A;       //Arguments extracted as strings/
	string B;
	int A1;         //Arguments assigned as integers//
	int A2;
	string Result;  //Variable being assigned the value used as the key for MAP//
	int inresult;   //assigned value for key in map//


public:
	void checkOperator();
	inline bool isInteger(const std::string & s);
	int checkVariable(map<string,double> &variables,int Operator,int &line ,int lines);
	int compute(int A,int B,int Operator);
	void execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines);
	~let();

};
//Same process as the one in IF, checks Operator such as *,+..//
void let::checkOperator(){
	char str[input.length()];
	strcpy(str,input.c_str());
	int i;
	char operators1[4]={'+','*','-','/'};
	char *pch;
	int equal;
	string words[3];
	int found;
	found=0;
	for(i=0;i<4;i++){
		found=input.find(operators1[i]);
		if(input[found]>0){
			operator1=i+1;
		}}
	i=0;

	equal=input.find('=');//

	pch=strtok(str,"+*-/=");
	while(pch !=NULL){
		words[i]=pch;
		pch=strtok(NULL,"+*-/=");
		i++;
	}
	if (found==0){
		Result=words[0];
		operator1=5;
		A=words[1];
	}
	else{
		Result=words[0];
		A=words[1];         //assigns arguments to strings A and B
		B=words[2];
	}


}
//checks whether an argument is an integer or not
inline bool let:: isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
	char * p ;
	strtol(s.c_str(), &p, 10) ;
	return (*p == 0) ;
}
//check variables uses isInteger to check whether the argument is a variable or number and assigns A and B accordignly
int let::checkVariable(map<string,double> &variables,int Operator,int &line,int lines){

	if(isInteger(A)==false)
	{
		line=lines+1; throw Nonexistentvariable();
	}

	else{
		std::istringstream ss(A);
		ss >> A1;
	}

	if(Operator!=5){
		if(isInteger(B)==false)
		{
			A2=variables[B];
		}
		else{
			std::istringstream ss(B);
			ss >> A2;
		}
	}
}
//uses the operator assigned to compute the let
int let::compute(int A,int B,int Operator){
	if (Operator==1){return A+B;}
	if(Operator==2){return A*B;}
	if(Operator==3){return A-B;}
	if(Operator==4){return A/B;}
	if(Operator==0){return A;}

	return -1;
	//else{return A;}
}
//execute function
void let::execute(std::vector<std::vector<string> >arr,map<string,double> &variables,int &line,int lines){

	input=arr[line][2];  //assigns string Input
	checkOperator();
	checkVariable(variables,operator1,line,lines);
	inresult=compute(A1,A2,operator1);    //stores the computed result into inresult
	variables[Result]=inresult;           //inresult is assigned to map with key//
	if(inresult!=-1){
		line++;
	}
	else{line=lines+1;}    //increments line to the end of the program//
}







//INPUT FUNCTION
class INPUT:public base
{
public:
	void execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines){

		//extract value from string and assign to map
		string key = arr[line][2];
		//checks if inputed value is integer//
		if (isInteger(key)==false){
			double val;
			cin >> val;
			variables[key] = val;
			line++;
		}
		else{
			line=lines+1;
			throw NumberInputException();  //throws exeception
		}

	}
	inline bool isInteger(const std::string & s);

	~INPUT();
};
//checks whether an argument is an integer or not
inline bool INPUT:: isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
	char * p ;
	strtol(s.c_str(), &p, 10) ;
	return (*p == 0) ;
}






//GOTO CLASS
class GOTO:public base{
public:
	inline bool isInteger(const std::string & s);
	void execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines);
	~GOTO();
};
//checks whether an argument is an integer or not
inline bool GOTO::isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

	char * p ;
	strtol(s.c_str(), &p, 10) ;

	return (*p == 0) ;
}
//execute checks if line number is a number and finds the corresponding line with the same characters in the stored vector
void GOTO::execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines)
{
	string element = arr[line][2];
	int temp;
	temp=line;
	int i;
	if (isInteger(element)==1)
	{
		for(i=0;i<lines;i++)
		{
			if(arr[i][0]==element)
			{
				line=i;
			}
		}
		if(temp==line)
		{

			line=lines+1;
			throw GOTOEXException();
		}
	}
	else{throw GOTOEXException();}

}






//PRINT Class
class Print:public base{
private:
	int i=2; //counter
public:

	void execute(std::vector<std::vector<string> >arr,map<string, double> &variables,int &line,int lines){
		int length = arr[line].size()-1;
		if(variables.find(arr[line][2])==variables.end()){

			//Check that there are quotation marks in the beginning of the statement
			if(arr[line][2].at(0)== '\"'){

				//Check that there are quotation marks at the end of the statement
				if (arr[line][length][arr[line][length].size() - 1]== '\"'){

					//remove quotes from first element of string to print
					arr[line][2].erase(
							remove( arr[line][2].begin(), arr[line][2].end(), '\"' ),
							arr[line][2].end()
					);

					//remove quotes from last element of string to print
					arr[line][length].erase(
							remove(arr[line][length].begin(), arr[line][length].end(), '\"' ),
							arr[line][length].end()
					);

					//Check for newline character and print correct strings
					while (i != length+1){
						string str2 = arr[line][i];
						int pos = 0;
						bool state = true;
						while (pos != str2.length())
						{
							if(str2.at(pos) == '\\' && str2.at(pos+1) == 'n')
							{
								cout << endl;
								state =false;
								pos++;
							}
							else
							{
								cout << str2.at(pos);
							}
							pos++;
						}
						if (state == true)
						{
							cout << " ";
						}
						i++;
					}
				}

				else
				{
					line=lines+1;
					throw EndQuotesException();
				}
			}
			else{

				line=lines+1;
				throw InitialQuotesException();
			}
		}
		else{
			cout <<variables[arr[line][2]];
		}
		line++;
	}
	~Print();
};










//MAIN PROGRAM CLASS
class program{

private:
	string Filename;
	std::vector<std::vector<string> >arr;
	map<string, double> mymap;
	std::map<int,base*> lines;
	int line=0;
	int length=0;
public:

	void File(const char* const FileName);
	string *separate(string line,int *count2);
	void create();
	int errorcheck(int i);
	void execute();

};
//Create function checks the second element within the vector and assigns a pointer to a new (derived Class) for functions.
// The the pointer is then stored within the map(for base class pointers) and line executed with the execute function //
//Create passes on the required references and variables to the base class pointer execute//
//The execute function within the derived class overrides this and carries out its process.//
void program::create(){

	if(arr[line][1]=="LET")
	{
		let* ptr=new let();
		lines[line]=ptr;
		lines[line]->execute(arr,mymap,line,length);
	}

	if(arr[line][1]=="IF")
	{
		IF* ptr=new IF();
		lines[line]=ptr;
		lines[line]->execute(arr,mymap,line,length);
	}
	if(arr[line][1]=="ENDIF")
	{
		line++;
	}

	if(arr[line][1]=="INPUT")
	{
		INPUT* ptr=new INPUT();
		lines[line]=ptr;
		lines[line]->execute(arr,mymap,line,length);
	}
	if(arr[line][1]=="GOTO")
	{
		GOTO* ptr=new GOTO();
		lines[line]=ptr;
		lines[line]->execute(arr,mymap,line,length);

	}

	if(arr[line][1]=="PRINT")
	{
		Print* ptr=new Print();
		lines[line]=ptr;
		lines[line]->execute(arr,mymap,line,length);
	}
}
//separates the each line by spaces and returns the words within a line as an array.
string *program::separate(string line,int *count2)
{

	string buff;
	string *words;
	int count1=0;
	int i;
	stringstream ss(line);
	vector<string>tokens;
	while(ss>>buff){
		tokens.push_back((buff));
		count1++;
	}
	words=new string[count1];

	for(i=0;i<count1;i++)
	{
		words[i]=tokens[i];
	}
	*count2=count1;

	return words;
}

//uses the function separate to create a 2D vector and does this for each of the lines
void program::File(const char* const FileName)
{

	std::vector<string> sub;
	std::ifstream infile(FileName);
	string *words;
	string line1;
	int words2;
	length=0;

	while (std::getline(infile, line1))
	{
		length++;
		string *words=separate(line1 ,&words2);
		for(int j=0;j<words2;j++)
		{
			sub.push_back(words[j]);
		}


		arr.push_back(sub);
		sub.clear();
		if(errorcheck((length-1))==0){
			line=length+1;
			throw WrongstatementException();

		}

	}
}

//INITIAL ERRORCHECKING
int program::errorcheck(int i){

	if(arr[i][1]=="PRINT"||"LET"||"GOTO"||"IF"||"INPUT"||"ENDIF") //checks that the statements entered are correct
	{
		return 1;
	}
	return 0;
}

//EXECUTE FUNCTION- STARTS THE LINE READING AND STATEMENT EXECUTION
void program::execute()
{
	while(line<length)
	{    //while loop goes through each line and executes the statement
		create(); //creates the nodes dynamically
	}
	lines.clear();   //clears the map once program in is finished//
}

int main(int argc, char** argv)
{
	program a;


	//catches exception for errors in different classes
	try {
		a.File(argv[1]);
		a.execute();
	}
	catch (EndQuotesException& e){
		cerr << "Missing end quotes" << endl;
	}
	catch (InitialQuotesException& e){
		cerr << "Missing beginning quotes"<<endl;
	}
	catch (NumberInputException& e){
		cerr << "Invalid key, cannot be integer"<<endl;
	}
	catch (ENDIFException& e){
		cerr << "Non matching IFs and ENDIFs"<<endl;
	}
	catch (GOTOEXException& e){
		cerr << "LINE number does not exist"<<endl;
	}
	catch (WrongstatementException& e)
	{
		cerr << "Incorrect statement"<<endl; //catches exception for wrong statement
	}

}



