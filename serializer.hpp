#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <boost/signals2.hpp>

#include <stdlib.h>

class serializer
{
public:
    std::vector<uint8_t> data;

    boost::signals2::signal<void (uint8_t*, uint32_t)> signal_new_buffer;

    serializer(uint32_t size = 0);
    ~serializer();

    uint32_t feed(uint8_t* ptr, uint32_t size);
    std::vector<uint8_t>& serialize();
    void deserialize_signal(std::vector<uint8_t>& data);
    void deserialize_signal(uint8_t* data_ptr, uint32_t size);
    void reset(uint32_t capacity);
    uint32_t size();

    unsigned int Iframes, Pframes;// Iframes_Received, Pframes_Received, Iframes_Transmitted, Pframes_Transmitted;
};

#endif
