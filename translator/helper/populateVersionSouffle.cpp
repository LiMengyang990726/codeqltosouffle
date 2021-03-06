#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "populateVersionSouffle.h"
#include "constants.h"
#include "utils.h"

#define VERSION_DL_PREFIX "rules/SelectedVersion"
#define VERSION_DL_POSTFIX ".dl"
#define VERSION_DL "rules/Version.dl"
#define VERSION_OUTPUT_PREFIX "../.facts/"
#define VERSION_OUTPUT_POSTFIX ".facts"
#define VARNAME_PREFIX "SelectedVersion"
using namespace std;

int numVersions = 1;

void writeVersionsCombination(vector<string> inputs)
{
    string filename = VERSION_DL;
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);
    versionDL << "#include \"" << BASE_TYPES_DL_FILE_NAME << "\"" << endl
              << endl;

    for (string input : inputs)
    {
        string varName = VARNAME_PREFIX + input;
        versionDL << ".decl " << varName << "(version: Version)" << endl;
        versionDL << ".input " << varName << endl
                  << endl;
    }

    versionDL << ".decl " << VARNAME_PREFIX << "(";
    for (int i = 0; i < inputs.size(); i++)
    {
        char c = i + 'a';
        if (i == inputs.size() - 1)
        {
            versionDL << c << ": Version)" << endl;
        }
        else
        {
            versionDL << c << ": Version, ";
        }
    }

    versionDL << VARNAME_PREFIX << "(";
    for (int i = 0; i < inputs.size(); i++)
    {
        char c = i + 'a';
        if (i == inputs.size() - 1)
        {
            versionDL << c << ") :- ";
        }
        else
        {
            versionDL << c << ", ";
        }
    }

    int i = 0;
    for (string input : inputs)
    {
        char c = i + 'a';
        string varName = VARNAME_PREFIX + input;
        if (i == inputs.size() - 1)
        {
            versionDL << varName << "(" << c << "). " << endl;
        }
        else
        {
            versionDL << varName << "(" << c << "), ";
        }
        i++;
    }
    versionDL << ".output " << VARNAME_PREFIX << "(IO=file, filename=\"" << VERSION_OUTPUT_PREFIX << "VersionComb" << VERSION_OUTPUT_POSTFIX << "\")" << endl;
}

void writeVersionDL(string varName, string version)
{
    string filename = VERSION_DL_PREFIX + varName + VERSION_DL_POSTFIX;
    writeVersionDLTemplate(filename, varName);

    if (regex_match(version, regex(PLAIN_VERSION_REGEX)))
    {
        writeVersionDLInput(filename, varName, version);
        writeVersionDLDirect(filename, varName);
    }
    else if (regex_match(version, regex(NTH_ANCESTOR_REGEX)))
    {
        string commitId = version.substr(0, version.find("~")) + "\"";
        string nStr = version.substr(version.find("~") + 1, version.size());
        stringstream ss(nStr);
        int n = 0;
        ss >> n;
        writeVersionDLNthAncestor(filename, varName, commitId, n);
    }
    else if (regex_match(version, regex(NTH_PARENT_REGEX)))
    {
        string commitId = version.substr(0, version.find("^")) + "\"";
        string nStr = version.substr(version.find("^") + 1, version.size());
        stringstream ss(nStr);
        int n = 0;
        ss >> n;
        writeVersionDLInput(filename, varName, commitId);
        writeVersionDLNthParent(filename, varName, commitId, n);
    }
    else
    {
        yyerror("invalid version selection");
        return;
    }

    writeVersionDLOutput(filename, varName);
}

void writeVersionDL(string varName, int type, vector<string> versions)
{
    string filename = VERSION_DL_PREFIX + varName + VERSION_DL_POSTFIX;
    writeVersionDLTemplate(filename, varName);

    if (type == MULTIPLE_VERSIONS_TYPE_1)
    {
        for (string version : versions)
        {
            if (regex_match(version, regex(PLAIN_VERSION_REGEX)))
            {
                writeVersionDLInput(filename, varName, version);
                writeVersionDLDirect(filename, varName);
            }
            else if (regex_match(version, regex(NTH_ANCESTOR_REGEX)))
            {
                string commitId = version.substr(0, version.find("~")) + "\"";
                string nStr = version.substr(version.find("~") + 1, version.size());
                stringstream ss(nStr);
                int n = 0;
                ss >> n;
                writeVersionDLNthAncestor(filename, varName, commitId, n);
            }
            else if (regex_match(version, regex(NTH_PARENT_REGEX)))
            {
                string commitId = version.substr(0, version.find("^")) + "\"";
                string nStr = version.substr(version.find("^") + 1, version.size());
                stringstream ss(nStr);
                int n = 0;
                ss >> n;
                writeVersionDLInput(filename, varName, commitId);
                writeVersionDLNthParent(filename, varName, commitId, n);
            }
            else
            {
                yyerror("invalid version selection");
                return;
            }
            numVersions++;
        }
    }
    else if (type == MULTIPLE_VERSIONS_TYPE_2)
    {
        if (versions.size() != 2)
        {
            yyerror("invalid branch comparison in multiple versions selection");
            return;
        }

        // Check the fromCommitId
        string fromCommitId = versions[0];
        bool isFromCommitIdRegex = false;
        string fromVarName = varName + "from";
        string fromFileName = VERSION_DL_PREFIX + fromVarName + VERSION_DL_POSTFIX;
        if (regex_match(fromCommitId, regex(NTH_ANCESTOR_REGEX)))
        {
            string commitId = fromCommitId.substr(0, fromCommitId.find("~")) + "\"";
            string nStr = fromCommitId.substr(fromCommitId.find("~") + 1, fromCommitId.size());
            stringstream ss(nStr);
            int n = 0;
            ss >> n;
            isFromCommitIdRegex = true;
            writeVersionDLTemplate(fromFileName, fromVarName);
            writeVersionDLNthAncestor(fromFileName, fromVarName, commitId, n);
            writeVersionDLOutput(fromFileName, fromVarName);
        }
        else if (regex_match(fromCommitId, regex(NTH_PARENT_REGEX)))
        {
            string commitId = fromCommitId.substr(0, fromCommitId.find("^")) + "\"";
            string nStr = fromCommitId.substr(fromCommitId.find("^") + 1, fromCommitId.size());
            stringstream ss(nStr);
            int n = 0;
            ss >> n;
            isFromCommitIdRegex = true;
            writeVersionDLTemplate(fromFileName, fromVarName);
            writeVersionDLInput(fromFileName, fromVarName, commitId);
            writeVersionDLNthParent(fromFileName, fromVarName, commitId, n);
            writeVersionDLOutput(fromFileName, fromVarName);
        }
        else if (!regex_match(fromCommitId, regex(PLAIN_VERSION_REGEX)))
        {
            yyerror("invalid version selection");
            return;
        }

        // Check the baseCommitId
        string baseCommitId = versions[1];
        bool isBaseCommitIdRegex = false;
        string baseVarName = varName + "base";
        string baseFileName = VERSION_DL_PREFIX + baseVarName + VERSION_DL_POSTFIX;
        if (regex_match(baseCommitId, regex(NTH_ANCESTOR_REGEX)))
        {
            string commitId = baseCommitId.substr(0, baseCommitId.find("~")) + "\"";
            string nStr = baseCommitId.substr(baseCommitId.find("~") + 1, baseCommitId.size());
            stringstream ss(nStr);
            int n = 0;
            ss >> n;
            isBaseCommitIdRegex = true;
            writeVersionDLTemplate(baseFileName, baseVarName);
            writeVersionDLNthAncestor(baseFileName, baseVarName, commitId, n);
            writeVersionDLOutput(baseFileName, baseVarName);
        }
        else if (regex_match(baseCommitId, regex(NTH_PARENT_REGEX)))
        {
            string commitId = baseCommitId.substr(0, baseCommitId.find("^")) + "\"";
            string nStr = baseCommitId.substr(baseCommitId.find("^") + 1, baseCommitId.size());
            stringstream ss(nStr);
            int n = 0;
            ss >> n;
            isBaseCommitIdRegex = true;
            writeVersionDLTemplate(baseFileName, baseVarName);
            writeVersionDLInput(baseFileName, baseVarName, commitId);
            writeVersionDLNthParent(baseFileName, baseVarName, commitId, n);
            writeVersionDLOutput(baseFileName, baseVarName);
        }
        else if (!regex_match(baseCommitId, regex(PLAIN_VERSION_REGEX)))
        {
            yyerror("invalid version selection");
            return;
        }

        writeVersionDLComp(filename, varName,
                           isFromCommitIdRegex ? fromVarName : fromCommitId,
                           isBaseCommitIdRegex ? baseVarName : baseCommitId);
    }
    else
    {
        yyerror("invalid multiple versions selection type");
        return;
    }

    writeVersionDLOutput(filename, varName);
}

void writeVersionDLDirect(string filename, string varName)
{
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);

    versionDL << VARNAME_PREFIX << varName << "(version) :- InputVersion" << numVersions << "(version)." << endl
              << endl;

    versionDL.close();
}

void writeVersionDLNthAncestor(string filename, string varName, string commitId, int n)
{
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);

    versionDL << "CommitDist(" << commitId << ", " << commitId << ", 0)." << endl;
    versionDL << VARNAME_PREFIX << varName << "(version) :- CommitDist(" << commitId << ", version, " << n << ")." << endl
              << endl;

    versionDL.close();
}

void writeVersionDLNthParent(string filename, string varName, string commitId, int n)
{
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);

    versionDL << VARNAME_PREFIX << varName << "(version) :- History(" << commitId << ", version, " << (n - 1) << "), InputVersion" << numVersions << "(" << commitId << ")." << endl
              << endl;

    versionDL.close();
}

void writeVersionDLComp(string filename, string varName, string from, string base)
{
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);

    string fieldFrom = "";
    string ruleFrom = "";
    string fieldBase = "";
    string ruleBase = "";
    if (!regex_match(from, regex(PLAIN_VERSION_REGEX)))
    {
        versionDL << ".decl SelectedVersion" << from << "(version: Version)" << endl;
        versionDL << ".input SelectedVersion" << from << endl << endl;
        ruleFrom = ", SelectedVersion" + from + "(" + from + ")";
        fieldFrom = from;
    } 
    else 
    {
        versionDL << "#define target_from " << from << endl;
        versionDL << "Reachable(target_from, target_from)." << endl;
        fieldFrom = "target_from";
    }

    if (!regex_match(base, regex(PLAIN_VERSION_REGEX)))
    {
        versionDL << ".decl SelectedVersion" << base << "(version: Version)" << endl;
        versionDL << ".input SelectedVersion" << base << endl << endl;
        ruleBase = ", SelectedVersion" + base + "(" + base + ")";
        fieldBase = base;
    }
    else 
    {
        versionDL << "#define target_base " << base << endl;
        versionDL << "Reachable(target_base, target_base)." << endl;
        fieldBase = "target_base";
    }

    versionDL << VARNAME_PREFIX << varName << "(version) :- Reachable(" << fieldFrom << ", version)"
              << ", !Reachable(" << fieldBase << ", version)"
              << ruleFrom 
              << ruleBase
              << "."
              << endl;

    versionDL.close();
}

void writeVersionDLTemplate(string filename, string varName)
{
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);

    versionDL << "#include \""<< BASE_TYPES_DL_FILE_NAME << "\"" << endl;
    versionDL << "#include \"" << FACT_TYPES_DL_FILE_NAME << "\"" << endl 
              << endl;

    versionDL << ".decl " << VARNAME_PREFIX << varName << "(version: Version)" << endl
              << endl;

    versionDL.close();
}

void writeVersionDLInput(string filename, string varName, string commitId)
{
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);

    versionDL << ".decl InputVersion" << numVersions << "(version: Version)" << endl;
    versionDL << "InputVersion" << numVersions << "(" << commitId << ")." << endl
              << endl;

    versionDL.close();
}

void writeVersionDLOutput(string filename, string varName)
{
    ofstream versionDL;
    versionDL.open(filename, ios_base::app);

    versionDL << ".output " << VARNAME_PREFIX << varName << "(IO=file, filename=\"" << VERSION_OUTPUT_PREFIX << VARNAME_PREFIX << varName << VERSION_OUTPUT_POSTFIX << "\")" << endl;

    versionDL.close();
}