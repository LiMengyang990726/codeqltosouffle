#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include "populateVersionDL.h"
#include "utils.h"

#define VERSION_DL_PREFIX "rules/version"
#define VERSION_DL_POSTFIX ".dl"
#define VERSION_OUTPUT_PREFIX "output/"
#define VERSION_OUTPUT_POSTFIX ".facts"
using namespace std;

void writeVersionDL(string range, string name) {
    writeVersionDLTemplate(name);

    if (regex_match(range, regex("^\"[a-zA-Z0-9]{40}\"$"))) {
        writeVersionDLDirect(range, name);
    } else if (regex_match(range, regex("^\"[a-zA-Z0-9]{40}~[0-9]+\"$"))) {
        string nStr = range.substr(range.find("~")+1, range.size());
        stringstream ss(nStr);
        int n = 0;
        ss >> n;
        writeVersionDLNthAncestor(range, name, n);
    } else if (regex_match(range, regex("^\"[a-zA-Z0-9]{40}\\^[0-9]+\"$"))) {
        string nStr = range.substr(range.find("^")+1, range.size());
        stringstream ss(nStr);
        int n = 0;
        ss >> n;
        writeVersionDLNthParent(range, name, n);
    } else {
        yyerror("invalid version selection");
    }
}

void writeVersionDLDirect(string range, string name) {
    ofstream versionDL;
    versionDL.open(VERSION_DL_PREFIX + name + VERSION_DL_POSTFIX, ios_base::app);

    string varName = "SelectedVersion" + name;
    versionDL << ".decl " << varName << "(version: Version)" << endl;
    versionDL << varName << "(version) :- Inputversion(version)." << endl;
    versionDL << ".output " << varName << "(IO=file, filename=\"" << VERSION_OUTPUT_PREFIX << varName << VERSION_OUTPUT_POSTFIX << "\")" << endl;

    versionDL.close();
}

void writeVersionDLNthAncestor(string range, string name, int n) {
    ofstream versionDL;
    versionDL.open(VERSION_DL_PREFIX + name + VERSION_DL_POSTFIX, ios_base::app);

    for (int i = 0; i < n-1; i++) {
        string varName = "IntermediateVersion" + to_string(i);
        versionDL << ".decl " << varName << "(version: Version)" << endl;
        versionDL << varName << "(version) :- History(X, version, DEFAULT_PARENT_INDEX), InputVersion(X)." << endl << endl;
    }

    string varName = "SelectedVersion" + name;
    string lastIntermediateVersionName = "IntermediateVersion" + to_string(n-2);
    versionDL << ".decl " << varName << "(version: Version)" << endl;
    versionDL << varName << "(version) :- History(X, version, DEFAULT_PARENT_INDEX), " << lastIntermediateVersionName << "(X)." << endl;
    versionDL << ".output " << varName << "(IO=file, filename=\"" << VERSION_OUTPUT_PREFIX << varName << VERSION_OUTPUT_POSTFIX << "\")" << endl;

    versionDL.close();
}

void writeVersionDLNthParent(string range, string name, int n) {
    ofstream versionDL;
    versionDL.open(VERSION_DL_PREFIX + name + VERSION_DL_POSTFIX, ios_base::app);

    string varName = "SelectedVersion" + name;
    versionDL << ".decl " << varName << "(version: Version)" << endl;
    versionDL << varName << "(version) :- History(X, version, "<< n << "), InputVersion(X)." << endl;
    versionDL << ".output " << varName << "(IO=file, filename=\"" << VERSION_OUTPUT_PREFIX << varName << VERSION_OUTPUT_POSTFIX << "\")" << endl;

    versionDL.close();
}

void writeVersionDLTemplate (string name) {
    ofstream versionDL;
    versionDL.open(VERSION_DL_PREFIX + name + VERSION_DL_POSTFIX, ios_base::app);
    
    versionDL << "#include \"types.dl\"" << endl;
    versionDL << "#include \"objectMapping.dl\"" << endl;
    versionDL << "#define DEFAULT_PARENT_INDEX 0" << endl;
    versionDL << "#define CURRENT_VERSION \"c0a13d8cc528a449967e83b7d0f4043787597e81\"" << endl << endl; /* todo */

    versionDL << ".decl History(child: Version, parent: Version, index: Int)" << endl;
    versionDL << ".input History" << endl << endl;

    versionDL << ".decl InputVersion(version: Version)" << endl;
    versionDL << "InputVersion(CURRENT_VERSION)." << endl << endl;

    versionDL.close();
}