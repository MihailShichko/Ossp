#include <stdlib.h>
#include <sys/dir.h>
#include <linux/cdrom.h>
#include <iostream>
#include <getopt.h>
#include <functional>
#include <vector>


std::vector<char[256]> Names;


void Usage()
{
    std::cout << "Usage" 
              << "dirwalk [directory] [options]"
              << "options: -l -f -d -s" << std::endl;
    exit(1);
}

template <typename F>
void Operate(const F func, DIR* dir)
{
    func(dir);
}

std::function<void(DIR *dir)> Get_L_Lambda()
{
    return [](DIR *dir) {     
                dirent* dirEntry;  
                while((dirEntry = readdir(dir)) != NULL)
                {
                    if(dirEntry == NULL)
                    {
                        if(errno == 0)
                        {
                            exit(errno);
                        }
                    }

                    if(dirEntry->d_type == DT_LNK)
                    {
                        Names.push_back(dirEntry->d_name);
                    }
                
                }
            };

}

std::function<void(DIR *dir)> Get_D_Lambda()
{
    return [](DIR *dir) {     
                dirent* dirEntry;    
                while((dirEntry = readdir(dir)) != NULL)
                {
                    if(dirEntry == NULL)
                    {
                        if(errno == 0)
                        {
                            exit(errno);
                        }
                    }

                    if(dirEntry->d_type == DT_DIR)
                    {
                        Names.push_back(dirEntry->d_name);
                    }
                
                }
            };

}

std::function<void(DIR *dir)> Get_F_Lambda()
{
    return [](DIR *dir) {       
                dirent* dirEntry;  
                while((dirEntry = readdir(dir)) != NULL)
                {
                    if(dirEntry == NULL)
                    {
                        if(errno == 0)
                        {
                            exit(errno);
                        }
                    }

                    if(dirEntry->d_type == DT_REG)
                    {
                        Names.push_back(dirEntry->d_name);
                    }
                
                }
            };

}

void Print_Files()
{
    for(int i = 0; i < Names.size(); i++)
    {
        std::cout << Names[i] << std::endl;
    }
}

int main(int argc, char* argv[])
{
    
//https://www.youtube.com/watch?v=3KKvX-ZvhqI
    int opt;
    opterr = 0;
    
    std::vector<std::function<void(DIR *dir)>> options; 

    while((opt = getopt(argc, argv, "ldsw") != -1))
    {
        switch (opt)
        {
        case 'l':
            options.push_back(Get_L_Lambda());
        break;
        case 'd':
            options.push_back(Get_D_Lambda());
        break;        
        case 'f':
           options.push_back(Get_F_Lambda());
        break;
        /*case 's':
           
        break;*/        
        case '?':
            Usage();
        break;
        }
    }


    DIR* dir;
    if(optind < argc)
    {
        dir = opendir(argv[optind]);
    }
    else
    {
        dir = opendir(".");
    }
    
    if(!dir)
    {
        perror("opendir");
        exit(errno);
    }

    if(options.size() == 0)
    {
        options.push_back(Get_L_Lambda());
        options.push_back(Get_D_Lambda());
        options.push_back(Get_F_Lambda());
    }

    for(int i = 0; i < options.size(); i++)
    {
        Operate(options[i], dir);
    }
   
    Print_Files();

    closedir(dir);

}

/*  struct dirent **namelist;
    int n;
    n = scandir(".", &namelist, 0, alphasort);
    if (n < 0)
        perror("scandir");
    else {
        while(n--) {
            printf("%s\n", namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }*/

