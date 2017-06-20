#ifndef FREQUENCYTABLE_H_INCLUDED
#define FREQUENCYTABLE_H_INCLUDED

#include <sys/types.h>

class FrequencyTable
{
public:

FrequencyTable()
{
    memset(counters,0,size*sizeof(uint32_t));
}

FrequencyTable(const FrequencyTable& other) = default;

inline FrequencyTable& operator=(FrequencyTable&& other)
{
    if (this!=&other)
    {
        std::swap<uint32_t,size>(counters, other.counters);
    }
    return *this;
}



inline void operator += (const FrequencyTable& other)
{
    for (unsigned int i=0;i<size;i++)
    {
        counters[i] += other.counters[i];
    }
}

inline uint32_t& operator[](int index)
{
    return counters[index];
}

private:
    const static uint32_t size = 256;
    uint32_t counters [size];
};

struct commandParameters; // forward declaration
FrequencyTable buildFrequencyTable(const commandParameters& parameters);
size_t getFilesize(const char* filename);


#endif // FREQUENCYTABLE_H_INCLUDED
