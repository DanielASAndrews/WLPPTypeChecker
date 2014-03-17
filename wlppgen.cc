#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#  define DEBUG(x) do { std::cerr << x << std::endl; } while (0)
#else
#  define DEBUG(x)
#endif


    using std::map;
    using std::pair;
    using std::make_pair;

using namespace std;

const char *terminals[] = {
  "BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID",
  "IF", "INT", "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM",
  "PCT", "PLUS", "PRINTLN", "RBRACE", "RETURN", "RPAREN", "SEMI",
  "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", "RBRACK", "NEW",
  "DELETE", "NULL"
};

typedef pair<const string , int> hash_pair;

map<const string, hash_pair> hashMap;


int isTerminal(const string &sym) {
  int idx;
  for(idx=0; idx<sizeof(terminals)/sizeof(char*); idx++)
    if(terminals[idx] == sym) return 1;
  return 0;
}

// Data structure for storing the parse tree.
class tree {
  public:
    string rule;
    string type;
    vector<string> tokens;
    vector<tree*> children;
    ~tree() { for(int i=0; i<children.size(); i++) delete children[i]; }
};

// Call this to display an error message and exit the program.
void bail(const string &msg) {
  // You can also simply throw a string instead of using this function.
  throw string(msg);
}

// Read and return wlppi parse tree.
tree *readParse(const string &lhs) {
  // Read a line from standard input.
  string line;
  getline(cin, line);

//cout << "getline has succeeded" << endl;
//cout << line << endl;
  
//bool cfail = cin.fail();
//cout << "cfail " << cfail << endl;
 
 if(cin.fail())
{
  //  cerr << "cin has failed" << endl;
    bail("ERROR: Unexpected end of file.");
}
 tree *ret = new tree();
 //cout << "About to tokenize" << endl;
  // Tokenize the line.
  stringstream ss;
  ss << line;
  while(!ss.eof()) {
  //  cout << "We've reached the while" << endl;
    string token;
    ss >> token;
    if(token == "") continue;
    ret->tokens.push_back(token);
  }
  // Ensure that the rule is separated by single spaces.
  for(int idx=0; idx<ret->tokens.size(); idx++) {
    if(idx>0) ret->rule += " ";
    ret->rule += ret->tokens[idx];
  }

  // Recurse if lhs is a nonterminal.
  if(!isTerminal(lhs)) {
    for(int idx=1/*skip the lhs*/; idx<ret->tokens.size(); idx++) {
      ret->children.push_back(readParse(ret->tokens[idx]));
    }
  }
  return ret;
}

// *********  OLD CODE

// //Globals

// //

// tree *parseTree;

// int dclSeen = 0;
// int offset = -4;
// int loopNumber = 0;

// string type = "";
// string strInt = "int";
// string strIntStar = "int*";

// //

// // Compute symbols defined in t.
// void genSymbols(tree *t) {

// //	cout << "tokens[0] " << t->tokens[0] << endl;
// /*	if (t->tokens.size() > 1)
// 	{
// 		cout << "tokens[1] " << t->tokens[1] << endl;
// 	}
// */

// 	if (t->tokens[0] == "dcl")
// 	{
// 		dclSeen = 1;
// 	}

// 	if (t->tokens.size() > 1)
// 	{

// 		if (t->tokens[1] == "int")
// 		{
// 			type = "int";
// 		}

// 		if (t->tokens[1] == "*" && type == "int")
// 		{
// 			type = "int*";
// 		}
// 	}

// 	if (t->tokens[0] == "ID")
// 	{
// 	//	cout << "Found ID, ID is " << t->tokens[1] << endl;
// 	//	cout << "dclSeen is " << dclSeen << endl;
// 	//	cout << "type is " << type << endl;

// 		map<const string, hash_pair>::iterator idExists = hashMap.find(t->tokens[1]);

// 		if (idExists == hashMap.end() && dclSeen == 1)
// 		{
// 			hashMap.insert( make_pair( t->tokens[1], make_pair(type,dclSeen) ) );
// 			dclSeen = 0;
// 			type = "";
// 		}
// 		else if (idExists == hashMap.end() && dclSeen == 0)
// 		{
// 			string problem = "ERROR: " + t->tokens[1] + " was not declared";
//                 	bail(problem);
// 		}
		
// 	/*	if (idExists != hashMap.begin() && idExists != hashMap.end())
// 		{
// 			string tempkey = idExists->first;
//                 	cerr << "This is what exists in the hashmap already " << tempkey << " " << hashMap[tempkey].first << '\n';
// 		}
// 	*/

// 		if (idExists != hashMap.end() && dclSeen == 1)
// 		{
// 			string problem = "ERROR: " + t->tokens[1] + " was already declared";
//                         bail(problem);
// 		}
         
// 	}

//        // Recurse if children still exist.

// //	cout << "Try to recurse on the children" << endl;
//   	if(t->children.size() != 0) {

// //	cout << "Recurse on the children, size of children: " << t->children.size() << endl;

// 		for(int idx=0; idx < t->children.size(); ++idx) 
//                 {
//                         genSymbols(t->children[idx]);
//                 }
//     	}
//   }

// *********  OLD CODE

//Globals

tree *parseTree;

int dclSeen = 0;
int offset = -4;
int loopNumber = 0;
string type = "";
string strInt = "int";
string strIntStar = "int*";

// Compute symbols defined in t.
void genSymbols(tree *t) {

  if (t->tokens[0] == "dcl")
  {
    dclSeen = 1;
  }

  if (t->tokens.size() > 2)
  {
    if (t->tokens[1] == "INT" && t->tokens[2] == "STAR")
                {
                        type = strIntStar; 
                        t->type = strIntStar;
                }
  }
  else if (t->tokens.size() > 1)
  {
    if (t->tokens[1] == "INT")
    {
      type = strInt;
      t->type = strInt;
    }
  }

  if (t->tokens[0] == "ID")
  {

    map<const string, hash_pair>::iterator idExists = hashMap.find(t->tokens[1]);

    if (idExists == hashMap.end() && dclSeen == 1)
    {
      hashMap.insert( make_pair( t->tokens[1], make_pair(type,offset) ) );
      dclSeen = 0;
      type = "";
      offset -= 4;
    }
    else if (idExists == hashMap.end() && dclSeen == 0)
    {
      string problem = "ERROR: " + t->tokens[1] + " was not declared";
                  bail(problem);
    }

    if (idExists != hashMap.end() && dclSeen == 1)
    {
      string problem = "ERROR: " + t->tokens[1] + " was already declared";
                        bail(problem);
    }
         
  }

       // Recurse if children still exist.
    if(t->children.size() != 0) {

    for(int idx=0; idx < t->children.size(); ++idx) 
                {
                        genSymbols(t->children[idx]);
                }
      }
  }

string retu = "";

void printTree(tree *t)
{
	retu = retu + t->rule + "\n";

	if(!isTerminal(t->tokens[0]))
	{
    		for(int idx=0; idx < t->children.size(); idx++)
		{
      			printTree(t->children[idx]);
    		}
	}
}

bool exprSeen = false;

//Type Generators

//predeclarations

string exprType(tree* t);
string factorType(tree* t);
string termType(tree* t);
string lValueType(tree* t);
string idType(tree* t);


string exprType(tree* t)
{

  if (t->children.size() == 1)
  {

    DEBUG("Rule: expr → term");
    DEBUG("Calculate the term type");

    string type = termType(t->children[0]);
    t->type = type;
    return type;
  }
  else if (t->children.size() == 3)
  {

    DEBUG("Rule: expr → expr PLUS | MINUS term");
    DEBUG("Calculate the expr type");
    DEBUG("Calculate the term type");

    string exprType1 = exprType(t->children[0]);
    string termType1 = termType(t->children[2]);

      if (t->tokens[2] == "PLUS")
      {

        DEBUG("Rule: expr → expr PLUS term");

        if (exprType1 == strInt && termType1 == strInt)
        {

          DEBUG("exprType = INT && termType = INT");
          DEBUG("So type = INT");

          t->type = strInt;
          return strInt;
        }
        else if (exprType1 == strIntStar && termType1 == strInt)
        {

          DEBUG("exprType = INT* && termType = INT");
          DEBUG("So type = INT*");

          t->type = strIntStar;
          return strIntStar;
        }
        else if (exprType1 == strInt && termType1 == strIntStar)
        {

          DEBUG("exprType = INT && termType = INT*");
          DEBUG("So type = INT*");

          t->type = strIntStar;
          return strIntStar;
        }
        else {

           DEBUG("ERROR");

          string problem = "ERROR: with types of expr + term";
                        bail(problem);
        }
      }
      else if (t->tokens[2] == "MINUS")
      {

         DEBUG("Rule: expr → expr MINUS term");

        if (exprType1 == strInt && termType1 == strInt)
        {

          DEBUG("exprType = INT && termType = INT");
          DEBUG("So type = INT");

          t->type = strInt;
          return strInt;
        }
        else if(exprType1 == strIntStar && termType1 == strInt)
        {

           DEBUG("exprType = INT* && termType = INT");
          DEBUG("So type = INT*");

          t->type = strIntStar;
          return strIntStar;
        }
        else if (exprType1 == strIntStar && termType1 == strIntStar)
        {

           DEBUG("exprType = INT* && termType = INT*");
          DEBUG("So type = INT");

          t->type = strInt;
          return strInt;
        }
        else {


           DEBUG("ERROR");

          string problem = "ERROR: with types of expr - term";
                        bail(problem);
        }
      }

  }
  string tmp = "";
      return tmp;
}

string factorType(tree* t)
{
      if (t->children.size() == 1)
  {

    DEBUG("Rule: factor → ID | factor → NUM | factor → NULL");

        if (t->tokens[1] == "ID")
    {

            DEBUG("Rule: factor → ID");
             DEBUG("Calculate the id type");

          string type = idType(t->children[0]);
          t->type = type;
          return type;
        }
        else if (t->tokens[1] == "NUM")
    {

            DEBUG("Rule: factor → NUM");
             DEBUG("So type = INT");

          t->type = strInt;
          return strInt;
        }
        else if (t->tokens[1] == "NULL")
    {
            DEBUG("Rule: factor → NULL");
            DEBUG("So type = INT*");

          t->type = strIntStar;
          return strIntStar;

    }
    else {

      DEBUG("ERROR");

      string problem = "ERROR: with types of factor";
                        bail(problem);

    }
  }
      else if (t->children.size() == 2)
  {

       DEBUG("Rule: factor → AMP lvalue | factor → STAR factor ");

        if (t->tokens[1] == "AMP")
    {

        DEBUG("Rule: factor → AMP lvalue");
         DEBUG("Calculate the lvalue type");

          t->type = strIntStar;
          string type = lValueType(t->children[1]);
          if (type == strInt)
      {

          DEBUG("lvalue = INT");
            DEBUG("So type = INT*");

        return strIntStar;
      }
        }
        else if (t->tokens[1] == "STAR")
    {

      DEBUG("Rule: factor → STAR factor ");
       DEBUG("Calculate the factor type");
          t->type = strInt;
          string type = factorType(t->children[1]);
          if (type == strIntStar)
      {
           DEBUG("factor = INT*");
            DEBUG("So type = INT");

            return strInt;
      }
        }
        else {

          DEBUG("ERROR");

          string problem = "ERROR: with types of factor |&|* lvalue | factor";
                        bail(problem);
        }
      }
      else if (t->children.size() == 3)
  {

       DEBUG("Rule: factor → LPAREN expr RPAREN ");
         DEBUG("Calculate the expr type");

        string type =  exprType(t->children[1]);
        t->type = type;
        return type;
      }
      else if (t->children.size() == 5)
  {

     DEBUG("Rule: factor → NEW INT LBRACK expr RBRACK");
         DEBUG("Calculate the expr type");
        string type = exprType(t->children[3]);
        t->type = strIntStar;
        if (type == strInt)
    {
            DEBUG("expr = INT");
            DEBUG("So type = INT*");

          return strIntStar;
        }
        else {

          DEBUG("ERROR");
          string problem = "ERROR: with types of NEW INT ( expr )";
                        bail(problem);
        }

      }
  string tmp = "";
      return tmp;
}

string termType(tree* t)
{
  if (t->children.size() == 1)
  {
     DEBUG("Rule: term → factor");
         DEBUG("Calculate the factor type");
        string type = factorType(t->children[0]);
        t->type = type;
        return type;
      }
      else if (t->children.size() == 3)
  {
    DEBUG("Rule: term → term STAR factor | term → term SLASH factor | term → term PCT factor"); 
    DEBUG("Calculate the term type");
    DEBUG("Calculate the factor type");

    string termType1 = termType(t->children[0]);
    string factorType1 = factorType(t->children[2]);
    t->type = strInt;

        if (termType1 == strInt && factorType1 == strInt)
    {
           DEBUG("termType = INT && factorType = INT");
          DEBUG("So type = INT");

          return strInt;
    }
    else {

      DEBUG("ERROR");
      string problem = "ERROR: with types of term *|/|\% factor";
                        bail(problem);
    }
  }
    string tmp = "";
    return tmp;
}

string lValueType(tree* t)
{
  if (t->children.size() == 1)
  {
     DEBUG("Rule: lvalue → ID");
    DEBUG("Calculate the ID type");

    string type = idType(t->children[0]);
    t->type = type;
    return type;
  }
  else if (t->children.size() == 2)
  {

     DEBUG("Rule: lvalue → STAR factor");
    DEBUG("Calculate the factor type");
    string type = factorType(t->children[1]);
    t->type = strInt;

    if (type == strIntStar)
    {
            DEBUG("factorType = INT*");
          DEBUG("So type = INT*");
      return strInt;
    }
    else {
      DEBUG("ERROR");
      string problem = "ERROR: with types of lvalue * factor";
                        bail(problem);
    }
  }
  else if (t->children.size() == 3)
  {


     DEBUG("Rule: lvalue → LPAREN lvalue RPAREN ");
        DEBUG("Calculate the lvalue type");
    string type = lValueType(t->children[1]);
    t->type = type;
        return type;
  }
      string tmp = "";
    return tmp;
}

string idType(tree* t)
{
   map<const string, hash_pair>::iterator it;
         it = hashMap.find(t->tokens[1]);
         string key = it->first;
         string type = hashMap[key].first;

         DEBUG("So the ID: " + key + " has type: " + type);

         return type;
}


// Generate the code for the parse tree t.
void genCode(tree *t)
{
	if (t->tokens[0] == "expr"){exprSeen = true;}
	// rule: factor -> id
	//if (t->token[0] == "factor" && t->children.size() == 0){add
}

void checkTypes(tree *t){

string result;

 t = t->children[1];

//     cout << "parseTree->rule " << t->rule << endl;

//     vector<string> strCpy = t->tokens;

//         cout << "parseTree->tokens the first one: " << strCpy[0] << endl;
//         cout << "parseTree->tokens the second one: " << strCpy[1] << endl;

//   vector<tree*> ptr = t->children;

//   cout << "children->rule the first one: " << ptr[0]->rule << endl;
//   cout << "children->rule the second one: " << ptr[2]->rule << endl;

//   tree* ptrToExpr = t;
//   ptrToExpr = ptrToExpr->children[11];
//   vector<string> strExpr = ptrToExpr->tokens;
//   vector<tree*> ptrChildren = ptrToExpr->children;

// cout << "This is the output for expr: " << endl;


// cout << "parseTree->rule " << ptrToExpr->rule << endl;

        
//         cout << "parseTree->tokens the first one: " << strExpr[0] << endl;
//         cout << "parseTree->tokens the second one: " << strExpr[1] << endl;


//   cout << "children->rule the first one: " << ptrChildren[0]->rule << endl;
//   cout << "children->rule the second one: " << ptrChildren[2]->rule << endl;


//  cout << "children->childrenofBOF " << ptr[0]->children.size() << endl;
//  cout << "children->childrenofprocedure " << ptr[1]->children.size() << endl;


for (int i = 0; i < t->tokens.size(); i++) {

    cout << t->tokens[i] << endl;

    if (t->tokens[i] == "expr"){

      DEBUG("token is expr");

      t = t->children[i-1];

      result = exprType(t);

   }
   else if(t->tokens[i] == "factor"){
      
      DEBUG("token is factor");

      result = factorType(t);
   }
   else if(t->tokens[i] == "term"){
      DEBUG("token is term");

      result = termType(t);
   }
   else if(t->tokens[i] == "lvalue"){

      DEBUG("token is lvalue");
      result = lValueType(t);
   }
} 

 return;

}


int main()
{

// Main program.
  try {
    parseTree = readParse("S");
  //  cout << "parseTree->rule " << parseTree->rule << endl;

//	vector<string> strCpy = parseTree->tokens;

  //      cout << "parseTree->tokens " << strCpy[0] << endl;
//        cout << "parseTree->tokens " << strCpy[1] << endl;

//	vector<tree*> ptr = parseTree->children;

//	cout << "children->rule " << ptr[0]->rule << endl;
//	cout << "children->rule " << ptr[2]->rule << endl;

//	cout << "children->childrenofBOF " << ptr[0]->children.size() << endl;
//	cout << "children->childrenofprocedure " << ptr[1]->children.size() << endl;

    genSymbols(parseTree);

     //Print out Symbol Table
     
    /*

     for (map<const string,hash_pair>::iterator it=hashMap.begin(); it != hashMap.end(); ++it)
    {
        string key = it->first;
        cerr << key << " " << hashMap[key].first << '\n';
    }

    */

    printTree(parseTree);

    cout << retu << endl;

    checkTypes(parseTree);


        
    //genCode(parseTree);
  } catch(string msg) {
    cerr << msg << endl;
  }
  if (parseTree) delete parseTree;


return 0;

}
