#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std::filesystem;
// using::std::getline;

#define version 0.1111
#define ERROR_TPL -1
#define ERROR_DIR -2
#define ERROR_FILE -3
#define ERROR_ARGS -4



std::string TemplatesWords[]{
    "#FILENAME#",
    "#MAINFILECPP#"};

std::string Commands[]{
    "cconfig",
    "--man",
    "-tpl",
    "--version",
    "-pp", // FullPathToProject
    "-np", // NameProject
    "save",
    "{CURRENT_DIRECTORY}",
    "-lg"
};

std::string CommandsForChange[]{
    "\"program\":",
    "\"args\":"
};

std::string PreDefVariables[]{

    "${userHome}", // - the path of the user's home folder
    "${workspaceFolder}", // - the path of the folder opened in VS Code
    "${workspaceFolderBasename}", // - the name of the folder opened in VS Code without any slashes (/)
    "${file}",// - the current opened file
    "${fileWorkspaceFolder}", // - the current opened file's workspace folder
    "${relativeFile}", // - the current opened file relative to workspaceFolder
    "${relativeFileDirname}", // - the current opened file's dirname relative to workspaceFolder
    "${fileBasename}", // - the current opened file's basename
    "${fileBasenameNoExtension}", // - the current opened file's basename with no file extension
    "${fileExtname}", // - the current opened file's extension
    "${fileDirname}", // - the current opened file's folder path
    "${fileDirnameBasename}", // - the current opened file's folder name
    "${cwd}", // - the task runner's current working directory upon the startup of VS Code
    "${lineNumber}", // - the current selected line number in the active file
    "${selectedText}", // - the current selected text in the active file
    "${execPath}",// - the path to the running VS Code executable
    "${defaultBuildTask}", // - the name of the default build task
    "${pathSeparator}", // - the character used by the operating system to separate components in file paths
    "${/}" // - shorthand for ${pathSeparator}

};

int manual()
{
    std::ifstream File("/usr/share/turn-screw/manual.txt");
    if (!File.is_open())
    {
        std::cout << "ERROR: Could not open manual!\n"
                  << std::endl;
        return 1;
    }
    std::string Line;
    while (getline(File, Line))
        std::cout << Line << std::endl;
    File.close();
    return 0;
}

std::pair<int, int> FindTemplateWord(std::string &Line)
{
    std::pair<int, int> Indexs;
    Indexs.first = 0;
    Indexs.second = 0;
    for (int i = 0; i < Line.size(); i++)
    {
        if (Line[i] == '#' && Indexs.first == 0)
        {
            Indexs.first = i;
        }
        else if (Line[i] == '#' && Indexs.first != 0)
        {
            Indexs.second = i;
        }
    }
    if (Indexs.first == 0 || Indexs.second == 0)
    {
        // std::cout << "Finded nothing" << std::endl;
        Indexs.first = 0;
        Indexs.second = 0;
    }
    return Indexs;
}
std::pair<int, int> FindStringToChangeTemplateWord(std::string &Line)
{
    std::pair<int, int> Indexes;
    std::vector<char> StringLine;
    bool TrueSubString = true;
    int LastIndex = 0;
    for(char S:Line)    
        StringLine.push_back(S);
        std::vector<char>::iterator it;
    for(std::string Str:CommandsForChange)
    {
        it = find_end(StringLine.begin(), StringLine.end(), Str.c_str(), Str.c_str()+Str.length()-1);
        if(it!=StringLine.end())
        {
            std::cout << "Done!" << std::endl;
            
            Indexes.first = it-StringLine.begin();
            Indexes.second = Indexes.first+Str.length();
            std::string Word(Line.c_str()+Indexes.first, Line.c_str()+Indexes.second);
            
            for(std::string Str:CommandsForChange)
            {
                if(Str == Word)
                    {std::cout << "Command is found!" << std::endl;
                    for(int i = Indexes.second; i < Line.length(); i++)
                    {
                        std::string Symb = "";
                        Symb += Line[i];
                        if(Symb == "\"") {Indexes.second = i; break;}
                    }
                    if(LastIndex == 0)
                    {
                    LastIndex = Indexes.second;
                    Repeat:
                    }
                    for(std::string Str: PreDefVariables)
                    {
        
                        it = find_end(StringLine.begin()+LastIndex, StringLine.end(), Str.c_str(), Str.c_str()+Str.length()-1);
                        if(it!=StringLine.end())
                        {
                            int FirstIndex = it-StringLine.begin();
                            int SecondIndex = FirstIndex+Str.length();
                            std::string StrVar(Line.c_str()+FirstIndex, Line.c_str()+SecondIndex);
                            if(Str == StrVar)
                                std::cout << StrVar << std::endl;
                            if(TrueSubString == true)
                            {
                                if(LastIndex == SecondIndex)
                                {TrueSubString = false;}
                                LastIndex = SecondIndex; 
                                goto Repeat;
                            }
                            for(int i = LastIndex; i < Line.length(); i++)
                            {
                                std::string StrSymbol = "";
                                StrSymbol += Line[i];
                                if(StrSymbol == "/")
                                {
                                    Indexes.first = LastIndex;
                                    Indexes.second = i;
                                    return Indexes;
                                }
                            }
                        }
                    }
                    }
                    else {std::cout << "Command is not found!" << std::endl;}
            }
            for(int i = LastIndex; i < Line.length(); i++)
            {
                std::string Symbol = "";
                Symbol += Line[i];
                if(Symbol == "\"")
                {
                    Indexes.first = LastIndex+1;
                    Indexes.second = i;
                    break;
                }
                else if(Line.length()-1 == i)
                {
                    Indexes.first = 0;
                    Indexes.second = 0;
                }
            }
            return Indexes;
        }
        
    }
    //if(Indexes.first != 0 && Indexes.second != 0)
    return Indexes;
        
}
bool IsCommand(std::string &Arg)
{
    // std::string Command;
    for (std::string Command : Commands)
    {
        if (Arg == Command)
            return true;
    }
    return false;
}

struct Arguments
{
    std::string tpl;
    std::string NameProject;
    std::string FullPathProject;
    bool CreateConfigFiles;
    bool SaveTPL;
    // std::string man;
};
#define LAUNCH_FILE 3648796
#define TASKS_FILE 2057693
std::string ChangeLine(std::string &Line, Arguments &MainArgs, std::pair<int, int> IndexsWord, int CurrentFile)
{

    std::string Str = Line;
    std::string StrWord;
    if(MainArgs.CreateConfigFiles == true)
    {
    for (int i = IndexsWord.first; i < IndexsWord.second + 1; i++)
    {
        StrWord += Line[i];
    }
    if (StrWord == "#FILENAME#")
    {
        // Str.erase(IndexsWord.first, IndexsWord.second);
        Str.replace(IndexsWord.first, (IndexsWord.second - IndexsWord.first + 1), MainArgs.NameProject);
        // Str.insert(IndexsWord.first, MainArgs.NameProject);
        return Str;
    }
    else if (StrWord == "#MAINFILECPP#")
    {
        // Str.erase(IndexsWord.first, IndexsWord.second);
        std::string FILE = MainArgs.NameProject;
        FILE += ".cpp";
        Str.replace(IndexsWord.first, (IndexsWord.second - IndexsWord.first + 1), FILE);
        return Str;
    }
    }
    else if(MainArgs.SaveTPL == true)
    {
        if(CurrentFile == LAUNCH_FILE)
        {
            for(int i = IndexsWord.first; i < IndexsWord.second + 1; i++)
            {
                StrWord +=Line[i];
            }
            std::string FILENAME = "#FILENAME";
            Str.replace(IndexsWord.first, (IndexsWord.second - IndexsWord.first), FILENAME);
            return Str;
        }
    }
    return Str;
}

int CreateConfigFiles(Arguments& Args)
{
    if (!Args.tpl.empty() && Args.CreateConfigFiles == true)
    {
        path CurrPath = current_path();
        std::cout << CurrPath << std::endl;
        std::string StrPathToTemplates = (char *)CurrPath.c_str();
        StrPathToTemplates += "/.templates";
        path PathToTemplates(StrPathToTemplates);
        std::string DirTemplates(Args.tpl);
        DirTemplates += ".tpl";
        if (exists(PathToTemplates))
        {
            std::string StrPathToDirTemplate;
            StrPathToDirTemplate = PathToTemplates.c_str();
            StrPathToDirTemplate += '/';
            StrPathToDirTemplate += DirTemplates;
            path PathToDirTemplate(StrPathToDirTemplate);
            if (!exists(PathToDirTemplate))
            {
                std::cout << "ERROR: Directory of templates:" << StrPathToDirTemplate << " is not exist!\n";
                return ERROR_TPL;
            }
            std::cout << "It's work" << std::endl;
            std::string LaunchFile = "launch.json";
            std::string TasksFile = "tasks.json";
            std::string PathToLuanchFile = StrPathToDirTemplate + '/' + LaunchFile;
            std::ifstream InFileLaunch;
            std::ofstream OutFileLaunch;
            std::string FullPathProjectPlusDirVSCode;
            if (Args.FullPathProject.empty() || Args.FullPathProject == "{CURRENT_DIRECTORY}")
            {
                FullPathProjectPlusDirVSCode = CurrPath.c_str();
                if(!Args.NameProject.empty())
                {
                    FullPathProjectPlusDirVSCode += "/";
                    FullPathProjectPlusDirVSCode += Args.NameProject;
                }
            }
            else
                FullPathProjectPlusDirVSCode = Args.FullPathProject;
            FullPathProjectPlusDirVSCode += "/.vscode";
            path OutPathProjectJsonFiles(FullPathProjectPlusDirVSCode);
            if (!exists(OutPathProjectJsonFiles))
            {
                create_directory(OutPathProjectJsonFiles);
                if (exists(OutPathProjectJsonFiles))
                    std::cout << "Created directory: " << OutPathProjectJsonFiles << std::endl;
            }
            std::string FullPathToLaunchFile = FullPathProjectPlusDirVSCode;
            FullPathToLaunchFile += "/";
            FullPathToLaunchFile += LaunchFile;
            OutFileLaunch.open(FullPathToLaunchFile, std::ofstream::out);
            if (!OutFileLaunch.is_open())
            {
                std::cout << "ERROR: File: " << LaunchFile << " is not opened!" << std::endl;
                return ERROR_FILE;
            }

            InFileLaunch.open(PathToLuanchFile);
            if (!InFileLaunch.is_open())
            {
                std::cout << "ERROR: File: " << LaunchFile << " is not opened!" << std::endl;
                return ERROR_FILE;
            }
            std::string Line;
            while (getline(InFileLaunch, Line))
            {
                // std::cout << Line << std::endl;
                std::pair<int, int> IndexsWords = FindTemplateWord(Line);
                if (IndexsWords.first != 0 && IndexsWords.second != 0)
                {
                    std::string ConstWord;
                    for (int i = IndexsWords.first; i < IndexsWords.second + 1; i++)
                        ConstWord += Line[i];
                    std::string StringLine = ChangeLine(Line, Args, IndexsWords, LAUNCH_FILE);
                    OutFileLaunch << StringLine << std::endl;
                    // std::cout << StringLine << std::endl;
                    // std::cout << ConstWord << std::endl;
                }
                else
                {
                    OutFileLaunch << Line << std::endl;
                }
            }
            InFileLaunch.close();
            OutFileLaunch.close();
            std::string FullPathToTasksFile = FullPathProjectPlusDirVSCode;
            FullPathToTasksFile += "/";
            FullPathToTasksFile += TasksFile;
            std::string StrPathToTasksFile = StrPathToDirTemplate + '/' + TasksFile;
            std::ofstream OutTasksFile;
            std::ifstream InTasksFile;
            InTasksFile.open(StrPathToTasksFile);
            OutTasksFile.open(FullPathToTasksFile, std::ofstream::out);
            if (!InTasksFile.is_open())
            {
                std::cout << "ERROR: File: " << TasksFile << " is not opened!" << std::endl;
                return ERROR_FILE;
            }
            if (!OutTasksFile.is_open())
            {
                std::cout << "ERROR: File: " << TasksFile << " is not opened!" << std::endl;
                return ERROR_FILE;
            }
            while (getline(InTasksFile, Line))
            {
                // std::cout << Line << std::endl;
                std::pair<int, int> IndexsWords = FindTemplateWord(Line);
                if (IndexsWords.first != 0 && IndexsWords.second != 0)
                {
                    std::string ConstWord;
                    for (int i = IndexsWords.first; i < IndexsWords.second + 1; i++)
                        ConstWord += Line[i];
                    std::string StringLine = ChangeLine(Line, Args, IndexsWords, TASKS_FILE);
                    OutTasksFile << StringLine << std::endl;
                    std::cout << StringLine << std::endl;
                    // std::cout << ConstWord << std::endl;
                }
                else
                {
                    OutTasksFile << Line << std::endl;
                }
            }
            InTasksFile.close();
            OutTasksFile.close();
        }
    }
    else {
        std::cout << "ERROR: Argument -tpl is empty!" << std::endl;
        return ERROR_ARGS;
    }
    return 0;
}

int SaveTemplates(Arguments& Args)
{
    path PathToProject;
    if(Args.SaveTPL == true)
    {
        std::cout << "SaveMode..." << std::endl;
        if(Args.FullPathProject.empty() && Args.NameProject.empty() && Args.tpl.empty())
        {
            return ERROR_ARGS;
        }
        if(!Args.FullPathProject.empty())
        {
            PathToProject = Args.FullPathProject;
            if(exists(PathToProject))
            {
                std::cout << "It's path is fine!" << std::endl;
            }
        }
        else {
            std::cout << "Argument -pp is empty! Use current path for save template!" << std::endl;
            PathToProject = current_path();
            std::cout << "Current path: " << PathToProject.c_str() << std::endl;
        }
        if(!Args.NameProject.empty())
        {
            std::string ChangePath = PathToProject.c_str();
            ChangePath += "/";
            ChangePath += Args.NameProject;
            ChangePath += "/";
            ChangePath += ".vscode";
            PathToProject = ChangePath;
            if(exists(PathToProject))
            std::cout << "Directory \".vscode\" is found!" << std::endl;
            else{return ERROR_DIR;}
            std::string FullPathToLaunchFile = ChangePath;
            std::string FullPathToTasksFile = ChangePath;
            FullPathToLaunchFile += "/launch.json";
            FullPathToTasksFile += "/tasks.json";
            std::ifstream InFileLaunch(FullPathToLaunchFile);
            if (!InFileLaunch.is_open())
            {
                std::cout << "ERROR: File: " << "launch.json" << " is not opened!" << std::endl;
                return ERROR_FILE;
            }
            std::vector<std::string> StringsLaunchFile;
            std::string Line;
            while(getline(InFileLaunch, Line))
            {
                std::pair<int, int> REFF = FindStringToChangeTemplateWord(Line);
                if(REFF.first == 0 && REFF.second == 0)
                StringsLaunchFile.push_back(Line);
                else
                {
                    std::cout << REFF.first << ":" << REFF.second << std::endl;
                    std::string ModifiedLine = ChangeLine(Line, Args, REFF, LAUNCH_FILE);
                    std::cout << "ModifiedLine: " << ModifiedLine << std::endl;
                    StringsLaunchFile.push_back(ModifiedLine);
                    std::string Word(Line.c_str()+REFF.first, Line.c_str()+REFF.second);
                    std::cout << Word << std::endl;
                }
                

            }
            InFileLaunch.close();
            //pair<int, int> REFF = FindStringToChangeTemplateWord()
            std::ifstream InFileTasks(FullPathToTasksFile);
            if (!InFileTasks.is_open())
            {
                std::cout << "ERROR: File: " << "tasks.json" << " is not opened!" << std::endl;
                return ERROR_FILE;
            }
            std::vector<std::string> StringsTasksFile;
            while(getline(InFileTasks, Line))
            {StringsTasksFile.push_back(Line);}

            InFileTasks.close();
            
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // for(std::string S:TemplatesWords) std::cout << S << std::endl;
    std::string StrArg1, StrArg2;
    Arguments MainArguments;
    if (argc == 1)
    {
        std::cout << "ERROR: Unknown parameters for generating file!\nFor details used command: --man\n"
                  << std::endl;
        return 1;
    }
    std::string StrArg;
    /*if(argc == 2)
    {
        StrArg = argv[1];
        if(IsCommand(StrArg) && StrArg == "--version") {std::cout << "version: " << version << std::endl;   return 0;}
        else if(IsCommand(StrArg) && StrArg == "--man") {std::cout << "MAN:.." << std::endl; return 0;}
    }
    */
    for (int i = 0; i != argc; i++)
    {
        StrArg1 = argv[i];
        if (IsCommand(StrArg1) && argc != i+1)
            if (StrArg1 == "-tpl")
            {
                StrArg2 = argv[i + 1];
                MainArguments.tpl = argv[i + 1];
                i++;
            }
            else if (StrArg1 == "-np")
            {
                StrArg2 = argv[i + 1];
                MainArguments.NameProject = argv[i + 1];
                i++;
            }
            else if (StrArg1 == "-pp")
            {
                StrArg2 = argv[i + 1];
                MainArguments.FullPathProject = argv[i + 1];
                i++;
            }
            else if (StrArg1 == "--man")
            {
                if(argc<3)
                return manual();
                else
                {
                    std::cout << "ERROR: You have mistakes in arguments!" << std::endl;
                    return ERROR_ARGS;
                }
            }
            else if (StrArg1 == "--version")
            {
                std::cout << "version: " << version << std::endl;
                return 0;
            }
            else if(StrArg1 == "cconfig")
            {
                MainArguments.CreateConfigFiles = true;
            }
            else if(StrArg1 == "save")
            MainArguments.SaveTPL = true;
    }
    if (argc == 2 || (MainArguments.CreateConfigFiles != true && MainArguments.SaveTPL != true))
    {
        std::cout << "ERROR: You have mistakes in arguments!" << std::endl;
        return ERROR_ARGS;
    }
    int NONERRORS = 0;
    // std::cout << "Hello!\n";
    if(MainArguments.CreateConfigFiles == true)
    NONERRORS = CreateConfigFiles(MainArguments);
    if(NONERRORS == 0 && MainArguments.CreateConfigFiles == true){
        //if(MainArguments.FullPathProject.empty() || MainArguments.NameProject.empty() || MainArguments.tpl.empty())
        std::cout << "CreateConfigFiles..." << std::endl;
    }
    /*else if(NONERRORS == ERROR_TPL)
    {    
        std::cout << "ERROR: Directory with templates is not exist!\n";
        return ERROR_TPL;
    }
    */
   NONERRORS = SaveTemplates(MainArguments);
    if(NONERRORS == 0 && MainArguments.SaveTPL == true)
    {

    }
    return 0;
}
