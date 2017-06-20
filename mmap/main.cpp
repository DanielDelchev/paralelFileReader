#include <iostream>
#include "chrono"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include "FrequencyTable.h"
#include "commandParameters.h"


//forward declaration
void parseCommandLineArguments(int argc, const char* argv[],commandParameters& parameters);


int main(int argc, const char *argv[])
{

    commandParameters parameters;
    parseCommandLineArguments(argc,argv,parameters);
    //std::cout<<"file: "<<parameters.file<<"\n";
    //std::cout<<"quiet: "<<parameters.quiet<<"\n";
    //std::cout<<"threads: "<<parameters.threads<<"\n";
    //std::cout<<"test: "<<parameters.test<<"\n";
    //std::cout<<"testfile: "<<parameters.testResultsFile<<"\n";

    //if (!parameters.quiet)
    //{
    //   std::cout<<"Input file size is "<<getFilesize(parameters.file)<<" bytes"<<std::endl;
    //}

    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    FrequencyTable FT = buildFrequencyTable(parameters);

    high_resolution_clock::time_point finish = high_resolution_clock::now();
    duration<int,std::micro> time_span = duration_cast<duration<int,std::micro>>(finish - start);

    if (!parameters.test)
    {
        std::cout<<"Total program runtime time(micros): "<<time_span.count()<<std::endl;
    }

    if (parameters.test)
    {
        std::fstream fs(parameters.testResultsFile,std::ios::out|std::ios::app);
        fs.write(", ",2*sizeof(char));
        std::string str_representation = std::to_string(time_span.count());
        int length = str_representation.size();
        const char* cstr_representation = str_representation.c_str();
        fs.write(cstr_representation,length);
        fs.write("\n",sizeof(char));
        fs.close();
    }

    //int sum=0;
    //for (int i=0;i<256;i++)
    //{
    //    sum += FT[i];
    //    std::cout<<(unsigned char) i<<": "<<FT[i]<<std::endl;
    //}
    //std::cout<<"sum: "<<sum;

    return 0;
}




void parseCommandLineArguments(int argc, const char* argv[],commandParameters& parameters){
    for (int i=1;i<argc;i++)
    {
        std::string commandLineArgument(argv[i]);
        int argumentContext = '0';

        if ( commandLineArgument == "-f" || commandLineArgument == "-file" )
        {
            if (i == argc-1)
            {
                perror("Error parsing command line argument -f 'filename' !\n");
                exit(1);
            }
            int fd = open(argv[i+1], O_RDONLY, 0);
            if (fd<0)
            {
                perror("Error parsing command line argument -f 'filename' !\n");
                exit(1);
            }
            close(fd);
            argumentContext = 'f';
        }

        else if (commandLineArgument == "-t" || commandLineArgument == "-threads")
        {
            if (i == argc-1 || atoi(argv[i+1]) <= 0 || atoi(argv[i+1]) > 64)
            {
                perror("Error parsing command line argument -t ! Thread count must be integer  [1,64] \n");
                exit(1);
            }

            argumentContext = 't';
        }

        else if (commandLineArgument == "-q" || commandLineArgument == "-quiet")
        {
            argumentContext = 'q';
        }

        else if (commandLineArgument == "-test")
        {
            argumentContext = 'T';
        }

        else
        {
            perror("Error parsing command line arguments! Unknown argument found! \n");
            exit (1);
        }

        switch (argumentContext)
        {
        case 'f' : parameters.file = argv[i+1]; i++; break;
        case 't' : parameters.threads = atoi(argv[i+1]); i++; break;
        case 'q' : parameters.quiet = true; break;
        case 'T' : parameters.test = true; break;
        default : perror("Error parsing arguments! \n"); exit(1); break;
        }
    }

    if (parameters.test)
    {
        std::string prefix = "TestResultFile";
        std::string infix = std::to_string(parameters.threads);
        std::string sufix = "Threads.csv\0";
        parameters.testResultsFile = prefix+infix+sufix;
        std::fstream fs(parameters.testResultsFile,std::ios::out|std::ios::app);

        //if first time openning the file , write down headers of columns
        if (fs.tellg() == 0)
        {
            std::string header = "FequencyTable, TotalRuntime\n";
            int length = header.size();
            fs.write(header.c_str(),length);
        }

        fs.close();
    }

}
