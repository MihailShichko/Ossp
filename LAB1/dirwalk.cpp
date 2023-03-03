#include <stdlib.h>
#include <sys/dir.h>
#include <linux/limits.h>
#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <errno.h>

struct flags
{
    bool l = false;
    bool d = false;
    bool f = false;
    bool s = false;

    bool NoOptions()
    {
        return !l && !d && !f; 
    }

    void SetFlagsTrue()
    {   
        l = true;
        d = true;
        f = true;
    } 
};


std::vector<std::string> Names;
flags Flags;

void Usage()
{
    std::cout << "Usage" 
              << "dirwalk [directory] [options]"
              << "options: -l -f -d -s" << std::endl;
    exit(1);
}

void PrintNames()
{
    for(int i = 0; i < Names.size(); i++)
    {
        std::cout << Names[i] << std::endl;
    }

    std::cout << Names.size() << " elems" <<std::endl;
}

void Parse(int argc, char** argv)//smth wrong
{
    int opt;
    opterr = 0;
    while((opt = getopt(argc, argv, "ldfs")) != -1)
    {
        switch (opt)
        {
            case 'l':
                Flags.l = true;
            break;
            case 'd':
                Flags.d = true;
            break;        
            case 'f':
                Flags.f = true;
            break;
            case 's':
                Flags.s = true;
            break;        
            case '?':
                Usage();
            default:
                perror("Not valid command-line arguments");
                exit(1);
            break;
        }
    }

}

std::string GetType(unsigned char type)
{
    switch (type)
    {
    case DT_DIR:
        return "Dir";
        break;
    case DT_REG:
        return "File";
        break;
    case DT_LNK:
        return "Link";
    break;
    default:
        return "Unknown";
    break;
    }
}

std::string CheckPathStr(std::string str)
{
    if(str[str.length() - 1] == '/')
    {
        str = str.substr(0, str.length() - 1);
    }

    return str;
}

void ListDir(std::string initStr) // norm
{
    dirent* dir;
    DIR* dp = opendir(initStr.c_str()); 
    while((dir = readdir(dp)) != NULL)
    {
        std::string strPath = "";
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
        if(dir->d_type == DT_DIR && Flags.d)
        {
            initStr = CheckPathStr(initStr);
            strPath = initStr + "/" + dir->d_name;
            Names.push_back(strPath + ": " + GetType(dir->d_type));
            ListDir(strPath);
        }
        else if(dir->d_type == DT_LNK && Flags.l)
        {
            initStr = CheckPathStr(initStr);
            strPath = initStr + "/" + dir->d_name;
            Names.push_back(strPath + ": " + GetType(dir->d_type));
        }
        else if(dir->d_type == DT_REG && Flags.f)
        {
            initStr = CheckPathStr(initStr);
            strPath = initStr + "/" + dir->d_name;
            Names.push_back(strPath+ ": " + GetType(dir->d_type));
        }
    }

    closedir(dp);
}

int main(int argc, char* argv[])
{
    Parse(argc, argv);

    if(Flags.NoOptions()) Flags.SetFlagsTrue();   

    if(optind < argc) ListDir(argv[optind]);
    else ListDir(".");
    
    if(Flags.s) sort(Names.begin(), Names.end());

    PrintNames();

    return 1;
}