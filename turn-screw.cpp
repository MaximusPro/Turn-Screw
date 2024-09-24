#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace std::filesystem;
// using::std::getline;

#define version 0.1111

std::string TemplatesWords[]{
    "#FILENAME#",
    "#MAINFILECPP#"};

std::string Commands[]{
    "--man",
    "-tpl",
    "--version",
    "-pp", // FullPathProject
    "-np", // NameProject
    "-save-tpl"

};

int manual()
{
    std::ifstream File("/usr/share/turn-screw/manual.txt");
    if (!File.is_open())
    {
        std::cout << "Could not open manual!\n"
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
    // std::string man;
};

std::string ChangeLine(std::string &Line, Arguments &MainArgs, std::pair<int, int> IndexsWord)
{
    std::string Str = Line;
    std::string StrWord;

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

    return Str;
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
        if (IsCommand(StrArg1))
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
                return manual();
            }
            else if (StrArg1 == "--version")
            {
                std::cout << "version: " << version << std::endl;
                return 0;
            }
    }
    if (argc == 2 || argc % 2 == 0)
    {
        std::cout << "ERROR: You have mistakes in arguments!" << std::endl;
        return 2;
    }
    // std::cout << "Hello!\n";
    if (!MainArguments.tpl.empty())
    {
        path CurrPath = current_path();
        std::cout << CurrPath << std::endl;
        std::string StrPathToTemplates = (char *)CurrPath.c_str();
        StrPathToTemplates += "/.templates";
        path PathToTemplates(StrPathToTemplates);
        std::string DirTemplates(MainArguments.tpl);
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
                std::cout << "Directory of templates:" << StrPathToDirTemplate << " is not exist!\n";
                return -1;
            }
            std::cout << "It's work" << std::endl;
            std::string LaunchFile = "launch.json";
            std::string TasksFile = "tasks.json";
            std::string PathToLuanchFile = StrPathToDirTemplate + '/' + LaunchFile;
            std::ifstream InFileLaunch;
            std::ofstream OutFileLaunch;
            std::string FullPathProjectPlusDirVSCode;
            if (MainArguments.FullPathProject.empty())
            {
                FullPathProjectPlusDirVSCode = CurrPath.c_str();
            }
            else
                FullPathProjectPlusDirVSCode = MainArguments.FullPathProject;
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
                std::cout << "File: " << LaunchFile << " is not opened!" << std::endl;
                return -1;
            }

            InFileLaunch.open(PathToLuanchFile);
            if (!InFileLaunch.is_open())
            {
                std::cout << "File: " << LaunchFile << " is not opened!" << std::endl;
                return -1;
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
                    std::string StringLine = ChangeLine(Line, MainArguments, IndexsWords);
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
                std::cout << "File: " << TasksFile << " is not opened!" << std::endl;
                return -1;
            }
            if (!OutTasksFile.is_open())
            {
                std::cout << "File: " << TasksFile << " is not opened!" << std::endl;
                return -1;
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
                    std::string StringLine = ChangeLine(Line, MainArguments, IndexsWords);
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

    else
    {
        std::cout << "Directory with templates is not exist!\n";
        return -1;
    }

    return 0;
}
