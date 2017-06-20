#include <cstring>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <memory>
#include "FrequencyTable.h"
#include "commandParameters.h"



size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}


void reader(const commandParameters& parameters,const unsigned char* mappedData,size_t startPos, size_t intervalLength,FrequencyTable* subResult,int threadIndex)
{
    using namespace std::chrono;

    high_resolution_clock::time_point start = high_resolution_clock::now();
    if (!parameters.quiet){
        std::cout<<"thread number: "<<threadIndex<< " started!"<<std::endl;
    }

    FrequencyTable temporary;

    uint32_t ending = startPos+intervalLength;
    for (uint32_t i=startPos;i<ending;++i) //i++, ++i ?
    {
        ++temporary[mappedData[i]];
    }

    if (!parameters.quiet){
        high_resolution_clock::time_point finish = high_resolution_clock::now();
        duration<int,std::micro> time_span = duration_cast<duration<int,std::micro>>(finish - start);
        std::cout<<"thread number: "<<threadIndex<< " ended!It lasted (micros): "<<time_span.count()<<std::endl;
    }

    *subResult = std::move(temporary);
}


FrequencyTable buildFrequencyTable(const commandParameters& parameters)
{


    using namespace std::chrono;

    size_t filesize = getFilesize(parameters.file);
    int fd = open(parameters.file, O_RDONLY, 0);
    unsigned char* mmappedData = reinterpret_cast<unsigned char*> (mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0));
    close(fd);

    int threadCount = parameters.threads;
    size_t average = filesize/threadCount;

    FrequencyTable* subFrequencyTable = new FrequencyTable[threadCount];//
    std::thread* threadReader = new std::thread[threadCount];//

    high_resolution_clock::time_point start = high_resolution_clock::now();
    for (int i=0;i<threadCount-1;i++)
    {
        threadReader[i] = std::thread(reader,parameters,mmappedData,average*i,average,subFrequencyTable+i,i);
    }

    size_t ending = -1;
    if (threadCount > 1)
    {
        ending = filesize-average*(threadCount-1);
    }
    else
    {
        ending = filesize;
    }

    threadReader[threadCount-1] = std::thread(reader,parameters,mmappedData,average*(threadCount-1),ending,subFrequencyTable+threadCount-1,threadCount-1);

    for (int i=0;i<threadCount;i++)
    {
        threadReader[i].join();
    }

    FrequencyTable result;
    for (int i=0;i<threadCount;i++)
    {
        result += subFrequencyTable[i];
    }

    high_resolution_clock::time_point finish = high_resolution_clock::now();
    duration<int,std::micro> time_span = duration_cast<duration<int,std::micro>>(finish - start);

    if(!parameters.test)
    {
        using namespace std::chrono;
        std::cout<<"FrequencyTable building threads time(micros) : "<<time_span.count()<<std::endl;
    }

    if(parameters.test)
    {
        std::fstream fs(parameters.testResultsFile,std::ios::out|std::ios::app);
        std::string str_representation = std::to_string(time_span.count());
        int length = str_representation.size();
        const char* cstr_representation = str_representation.c_str();
        fs.write(cstr_representation,length);
        fs.close();
    }


    munmap(mmappedData, filesize);
    delete [] subFrequencyTable;
    delete [] threadReader;
    return result;
}

