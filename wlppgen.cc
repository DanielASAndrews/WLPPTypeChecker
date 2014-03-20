#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>



//Uncomment  to run the debug version
//#define DEBUG_BUILD


#ifdef DEBUG_BUILD
#  define DEBUG(x) do { std::cerr << x << std::endl; } while (0)
#else
#  define DEBUG(x)
#endif


using std::map;
using std::pair;
using std::make_pair;
using namespace std;

//Globals

int dclSeen = 0;
int offset = -4;
int loopNumber = 0;
string type = "";
string strInt = "int";
string strIntStar = "int*";
string wellTyped = "wellTyped";

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


// Data structure for storing types involved with a wain procedure
struct wainProcedureTypes {
  string secondDcl;
  string dcls;
  string statements;
  string expr;
};

tree *parseTree;

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

 if(cin.fail()){

    bail("ERROR: Unexpected end of file.");
  }
  tree *ret = new tree();

  // Tokenize the line.
  stringstream ss;
  ss << line;
  while(!ss.eof()) {
 
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

//Lvalue(ID)

//Lvalue(*a)

//Lvalue(*E)

bool isLValueValid(tree* t){

  bool isLValueValid = false;

//  if (t->children[])

  return isLValueValid;


}

//Type Generators

//predeclarations

string exprType(tree* t);
string factorType(tree* t);
string termType(tree* t);
string lValueType(tree* t);
string idType(tree* t);
string statementType(tree* t);
string statementsType(tree* t);
string testType(tree* t);
string dclType(tree* t);
string dclsType(tree* t);

string exprType(tree* t){

  if (t->children.size() == 1)
  {

    DEBUG("Rule: expr → term");
    DEBUG("Calculate the term type");

    string type = termType(t->children[0]);
    t->type = type;
     DEBUG ("So return type of the term is: " + type);
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

string factorType(tree* t){

  if (t->children.size() == 1){

    DEBUG("Rule: factor → ID | factor → NUM | factor → NULL");

    if (t->tokens[1] == "ID"){

      DEBUG("Rule: factor → ID");
      DEBUG("Calculate the id type");

      string type = idType(t->children[0]);
      t->type = type;

      DEBUG ("So return type of the id is: " + type);

      return type;
    }
    else if (t->tokens[1] == "NUM"){

      DEBUG("Rule: factor → NUM");
      DEBUG("So type = INT");

      t->type = strInt;
      return strInt;
    }
    else if (t->tokens[1] == "NULL"){
      
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
  else if (t->children.size() == 2){

    DEBUG("Rule: factor → AMP lvalue | factor → STAR factor ");

    if (t->tokens[1] == "AMP"){

      DEBUG("Rule: factor → AMP lvalue");
      DEBUG("Calculate the lvalue type");

      t->type = strIntStar;
      string type = lValueType(t->children[1]);
      if (type == strInt){

        DEBUG("lvalue = INT");
        DEBUG("So type = INT*");
        return strIntStar;
      }
    }
    else if (t->tokens[1] == "STAR"){

      DEBUG("Rule: factor → STAR factor ");
      DEBUG("Calculate the factor type");
      
      t->type = strInt;
      string type = factorType(t->children[1]);
      DEBUG ("So return type of the factor → STAR factor is: " + type);
      
      if (type == strIntStar){
        DEBUG("factor = INT*");
        DEBUG("So type = INT");

        return strInt;
      }
      else {
        DEBUG("ERROR");

        string problem = "ERROR: with types of factor |&|* lvalue | factor";
        bail(problem);
      }
    }
  }
  else if (t->children.size() == 3){
    DEBUG("Rule: factor → LPAREN expr RPAREN ");
    DEBUG("Calculate the expr type");

    string type =  exprType(t->children[1]);
    t->type = type;
    return type;
  }
  else if (t->children.size() == 5){

    DEBUG("Rule: factor → NEW INT LBRACK expr RBRACK");
    DEBUG("Calculate the expr type");
    string type = exprType(t->children[3]);
    t->type = strIntStar;
    
    if (type == strInt){
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

string termType(tree* t){
  if (t->children.size() == 1)
  {
     DEBUG("Rule: term → factor");
         DEBUG("Calculate the factor type");
        string type = factorType(t->children[0]);
        t->type = type;
         DEBUG ("So return type of the term → factor is: " + type);
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

string lValueType(tree* t){
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

string idType(tree* t){
   map<const string, hash_pair>::iterator it;
         it = hashMap.find(t->tokens[1]);
         string key = it->first;
         string type = hashMap[key].first;

         DEBUG("So the ID: " + key + " has type: " + type);

         return type;
}

string statementType(tree* t){

  if (t->children.size() == 4){

    DEBUG("Rule: statement → lvalue BECOMES expr SEMI");
    DEBUG("Calculate the lvalue type");
    DEBUG("Calculate the term type");

    string type1 = lValueType(t->children[0]);
    string type2 = exprType(t->children[2]);

    if (type1 == strInt && type2 == strInt){
      t->type = strInt;
      DEBUG ("So return type of the statement is: " + t->type);
      return strInt;
    }
    else if (type1 == strIntStar && type2 == strIntStar){
      t->type = strIntStar;
      DEBUG ("So return type of the statement is: " + t->type);
      return strIntStar;
    }
    else {
      DEBUG("ERROR");
      string problem = "ERROR: with types of lvalue BECOMES expr SEMI";
      bail(problem);
    }
  }
  else if (t->children.size() == 5){
    if(t->tokens[0] == "PRINTLN"){

      DEBUG("Rule: statement → PRINTLN LPAREN expr RPAREN SEMI");
      DEBUG("Calculate the expr type");
      
      string type = exprType(t->children[2]);

      if(type == strInt){
        t->type = type;
        DEBUG ("So return type of the statement is: " + t->type);
        return type;
      }
      else {
        DEBUG("ERROR");
        string problem = "ERROR: with types of PRINTLN LPAREN expr RPAREN SEMI";
        bail(problem);
      }
    }
    else if(t->tokens[1] == "DELETE"){

      DEBUG("Rule: statement → DELETE LBRACK RBRACK expr SEMI");
      DEBUG("Calculate the expr type");

      string type = exprType(t->children[3]);

      if(type == strIntStar){
        t->type = type;
        DEBUG ("So return type of the statement is: " + t->type);
        return type;
      }
      else {
        DEBUG("ERROR");
        string problem = "ERROR: with types of DELETE LBRACK RBRACK expr SEMI";
        bail(problem);
      }
    }
  }
  else if (t->children.size() == 7){

    DEBUG("Rule: statement → WHILE LPAREN test RPAREN LBRACE statements RBRACE");
    DEBUG("Calculate the test type");
    DEBUG("Calculate the statements type");

    string type1 = testType(t->children[2]);
    string type2 = statementsType(t->children[5]);

    t->type = wellTyped;
    DEBUG ("So return type of the statement is: " + t->type);
    return t->type;
  }
  else if (t->children.size() == 11){

    DEBUG("Rule: statement → IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE");
    DEBUG("Calculate the test type");
    DEBUG("Calculate the statements type");
    DEBUG("Calculate the statements type");

    string type1 = testType(t->children[2]);
    string type2 = statementsType(t->children[5]);
    string type3 = statementsType(t->children[9]);

    t->type = wellTyped;
    DEBUG ("So return type of the statement is: " + t->type);
    return t->type;
  }

  string tmp = "";
  return tmp;
}

string statementsType(tree* t){
  
  if (t->children.size() == 0){

    DEBUG("Rule: statements → ");
    t->type = wellTyped;
    DEBUG ("So return type of the statements is: " + t->type);
    return t->type;
  }
  else if (t->children.size() == 2){

    DEBUG("Rule: statement → statements statement");
    DEBUG("Calculate the statements type");
    DEBUG("Calculate the statment type");

    string type1 = statementsType(t->children[0]);
    string type2 = statementType(t->children[1]);

    t->type = wellTyped;
    DEBUG ("So return type of the statements is: " + t->type);
    return t->type;
  }

  string tmp = "";
  return tmp;
}

string testType(tree* t){

  if (t->children.size() == 3){

    DEBUG("Rule: test → expr EQ|NE|LT|LE|GE|GT expr");
    DEBUG("Calculate the expr type");
    DEBUG("Calculate the expr type");

    string type1 = exprType(t->children[0]);
    string type2 = exprType(t->children[2]);

    if (type1 == strInt && type2 == strInt){
      t->type = wellTyped;
      DEBUG ("So return type of the test is: " + t->type);
      return t->type;
    }
    else if (type1 == strIntStar && type2 == strIntStar){
      t->type = wellTyped;
      DEBUG ("So return type of the test is: " + t->type);
      return t->type;
    }
    else {
        DEBUG("ERROR");
        string problem = "ERROR: with types of expr EQ|NE|LT|LE|GE|GT expr";
        bail(problem);
    }
  }

  string tmp = "";
  return tmp;
}

string dclType(tree* t){

  if(t->children.size() == 2){

    DEBUG("Rule: dcl → type ID");
    DEBUG("Calculate the ID type");

    string type = idType(t->children[1]);
    t->type = type;
    DEBUG ("So return type of the test is: " + t->type);
    return t->type;
  }
  else {
    DEBUG("ERROR");
    string problem = "ERROR: with types of type ID";
    bail(problem);
  }
  string tmp = "";
  return tmp;
}

string dclsType(tree* t){
  if(t->children.size() == 0){

    DEBUG("Rule: dcls → ");
    t->type = wellTyped;
    DEBUG ("So return type of the dcls is: " + t->type);
    return t->type;
  }
  else if (t->children.size() == 5){

    if(t->tokens[4] == "NUM"){

      DEBUG("Rule: dcls → dcls dcl BECOMES NUM SEMI");
      DEBUG("Calculate the dcls type");
      DEBUG("Calculate the dcl type");

      string type1 = dclsType(t->children[0]);
      string type2 = dclType(t->children[1]);

      if(type1 == wellTyped && type2 == strInt){

        t->type = wellTyped;
        DEBUG ("So return type of the dcls is: " + t->type);
        return t->type;
      }
      else {
        DEBUG("ERROR");
        string problem = "ERROR: with types of dcls dcl BECOMES NUM SEMI";
        bail(problem);
      }
    }
    else if(t->tokens[4] == "NULL"){

      DEBUG("Rule: dcls → dcls dcl BECOMES NULL SEMI");
      DEBUG("Calculate the dcls type");
      DEBUG("Calculate the dcl type");

      string type1 = dclsType(t->children[0]);
      string type2 = dclType(t->children[1]);

      if(type1 == wellTyped && type2 == strIntStar){

        t->type = wellTyped;
        DEBUG ("So return type of the dcls is: " + t->type);
        return t->type;
      }
      else {
        DEBUG("ERROR");
        string problem = "ERROR: with types of dcls dcl BECOMES NULL SEMI";
        bail(problem);
      }
    }

  }

  string tmp = "";
  return tmp;
}

// Generate the code for the parse tree t.
void genCode(tree *t)
{
	if (t->tokens[0] == "expr"){exprSeen = true;}
	// rule: factor -> id
	//if (t->token[0] == "factor" && t->children.size() == 0){add
}

wainProcedureTypes checkTypes(tree *t){

  wainProcedureTypes procedure;

  string procSecondDclType;
  string procDclsType;
  string procStatementsType;
  string procExprType;

  tree* parseTreeCopy = t;

  //Print the tokens
  /*
  for (int i = 0; i < t->tokens.size(); i++) {
    cout << t->tokens[i] << endl;
  }
  */

  DEBUG("Check the second dcl {It should be an int}");

  parseTreeCopy = parseTreeCopy->children[5];

  procSecondDclType = dclType(parseTreeCopy);

  //We need to reset the parseTree;

  parseTreeCopy = t;

  DEBUG("Result of second dcl is: " + procSecondDclType); 


  DEBUG("Check the dcls tree");

  parseTreeCopy = parseTreeCopy->children[8];

  procDclsType = dclsType(parseTreeCopy);

  //We need to reset the parseTree;

  parseTreeCopy = t;

  DEBUG("Result of dcls is: " + procDclsType); 


  DEBUG("Check the statement tree");

  parseTreeCopy = parseTreeCopy->children[9];

  procStatementsType = statementsType(parseTreeCopy);

  //We need to reset the parseTree;

  parseTreeCopy = t;

  DEBUG("Result of statements is: " + procStatementsType); 

  
  DEBUG("Check the expr tree");

  parseTreeCopy = parseTreeCopy->children[11];

  procExprType = exprType(parseTreeCopy);

  //We need to reset the parseTree;

  parseTreeCopy = t;

  DEBUG("Result of expr is: " + procExprType);

  procedure.secondDcl = procSecondDclType;
  procedure.dcls = procDclsType;
  procedure.statements = procStatementsType;
  procedure.expr = procExprType;

 return procedure;

}

bool checkWLPPProcedure(tree* t){

  bool isWLPPProcedureValid;

  wainProcedureTypes procedure = checkTypes(t->children[1]);

  bool isSecondDclTypeValid = (procedure.secondDcl == strInt);
  bool isDclsTypeValid = (procedure.dcls == wellTyped);
  bool isStatementsTypeValid = (procedure.statements == wellTyped);
  bool isExprTypeValid = (procedure.expr == strInt);

  DEBUG("Our final procedure check is:");
  DEBUG("isSecondDclTypeValid: ");
  DEBUG(isSecondDclTypeValid);
  DEBUG("isDclsTypeValid: ");
  DEBUG(isDclsTypeValid);
  DEBUG("isStatementsTypeValid: ");
  DEBUG(isStatementsTypeValid);
  DEBUG("isExprTypeValid: ");
  DEBUG(isExprTypeValid);

  if (isSecondDclTypeValid && isDclsTypeValid && isStatementsTypeValid && isExprTypeValid){
    isWLPPProcedureValid = true;

    DEBUG("isWLPPProcedureValid: ");
    DEBUG(isWLPPProcedureValid);
  }
  else {
    isWLPPProcedureValid = false;

    DEBUG("isWLPPProcedureValid: ");
    DEBUG(isWLPPProcedureValid);
  }

  return isWLPPProcedureValid;
}


int main()
{

// Main program.
  try {
    parseTree = readParse("S");

    genSymbols(parseTree);

     //Print out Symbol Table
     
    /*

     for (map<const string,hash_pair>::iterator it=hashMap.begin(); it != hashMap.end(); ++it)
    {
        string key = it->first;
        cerr << key << " " << hashMap[key].first << '\n';
    }

    */

    //printTree(parseTree);

    //cout << retu << endl;

    bool isWLPPValid = checkWLPPProcedure(parseTree);

    if(isWLPPValid){
      DEBUG("WLPP Program is Valid!");
    }
    else {
      DEBUG("ERROR");
      string problem = "ERROR: with types of INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE";
      bail(problem);
    }
        
    //genCode(parseTree);
  } catch(string msg) {
    cerr << msg << endl;
  }
  if (parseTree) delete parseTree;


return 0;

}
