#include "serializer.hpp"

serializer::serializer(uint32_t size)
{
    if (size) data.reserve(size);
    // Reserve at least size amount of bytes for the serialization vector (can be zero, if so, memory will be allocated "on-the-fly")
}

serializer::~serializer()
{
    // Empty destructor
}

// Feed a buffer to serializer. Serializer will append a CPOY of the buffer to end of the vector, and returns an index to the vector where the buffer ENDS.
uint32_t serializer::feed(uint8_t *ptr, uint32_t size)
{
    data.insert(data.end(), &size, (&size)+sizeof(uint32_t));
    data.insert(data.end(), ptr, ptr+size);
    return data.size()-1;
}

std::vector<uint8_t>& serializer::serialize()
{
    std::vector<uint8_t>& data_ref = data;
    return data_ref;
}

// deserialize function emits a signal with ptr to buffer, and length of respective buffer.
void serializer::deserialize_signal(std::vector<uint8_t>& serialized_data)
{
    uint32_t length_of_next_buffer;
    uint32_t index = 0;
    uint32_t *integer_pointer;
    while(index < serialized_data.size()-1)
    {
        integer_pointer = (uint32_t*) &serialized_data[index];
        length_of_next_buffer = *integer_pointer;
        // If next buffer is longer than remaining data in vector, an error has occured: break from loop.
        if (length_of_next_buffer > serialized_data.size()-index) break;
        index += sizeof(uint32_t);
        signal_new_buffer(&serialiazed_data[index], length_of_next_buffer);
        index += length_of_next_buffer;
    }
}
