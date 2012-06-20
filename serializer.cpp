#include "serializer.hpp"

serializer::serializer(uint32_t size)
{
    Pframes=0; //_Received = 0;
    Iframes=0; //_Received = 0;
//    Pframes_Transmitted = 0;
//    Iframes_Transmitted = 0;
    if (size) data.reserve(size);
}

serializer::~serializer()
{
    // Empty destructor
}

uint32_t serializer::size()
{
    return data.size();
}

// Feed a buffer to serializer. Serializer will append a CPOY of the buffer to end of the vector, and returns an index to the vector where the buffer ENDS.
uint32_t serializer::feed(uint8_t *ptr, uint32_t size)
{
    uint8_t size_array[sizeof(uint32_t)];
    memcpy(&size_array, &size, sizeof(uint32_t)); // BEWARE OF ENDIANESS!!!

    uint32_t index = data.size();
    if (index)
	Pframes++;//Pframes_Transmitted++;
    else
	Iframes++;//Iframes_Transmitted++;
    data.insert(data.end(), size_array, size_array+sizeof(uint32_t));

    data.insert(data.end(), ptr, ptr+size);

    return index;
}

std::vector<uint8_t>& serializer::serialize()
{
    std::vector<uint8_t>& data_ref = data;
    return data_ref;
}

// deserialize function emits a signal with ptr to buffer, and length of respective buffer.
void serializer::deserialize_signal(std::vector<uint8_t>& serialized_data)
{
//    std::cout << "inside deserialize function \n";
    uint32_t next_buffer_size;
    uint32_t index = 0;
    int frames = 0;

    uint8_t tmp[8];
    memcpy(tmp, &serialized_data[index], 8);
    for (int n = 0; n < 8; n++)
    {
    	for (int t = 0; t < 8; t++)
    	{
    		if (tmp[n] & (1 << t))
			    std::cout << "1";
			else
			    std::cout << "0";
			if (t %4 == 3)
				std::cout << ".";
		}
	}
	std::cout << std::endl;
    while(index < serialized_data.size()-1)
    {
//        std::cout << "inside loop\n";
        memcpy(&next_buffer_size, &serialized_data[index],sizeof(uint32_t));
        // If next buffer is longer than remaining data in vector, an error has occured: break from loop.
        std::cout << "length of " << frames << " buffer is " << next_buffer_size << std::endl;
        if (next_buffer_size > serialized_data.size()-index)
        {
        	std::cout << "Out of bounds. Size: " << serialized_data.size()*1 << " index: " << index*1 << " Adr: " << (long)(&serialized_data[0]) << std::endl;
            break;
        }
        if ( next_buffer_size == 0 )
        {
        	std::cout << "Zero-padded. Size: " << serialized_data.size()*1 << " index: " << index*1 << " Adr: " << (long)(&serialized_data[0]) << std::endl;
            break;
        }
        frames++;
    	if (index)
    		Pframes++;//Pframes_Received++;
    	else
	    	Iframes++;//Iframes_Received++;
        index += sizeof(uint32_t);
//        std::cout << "just before signal\n";
        signal_new_buffer(&serialized_data[index], next_buffer_size);
//        std::cout << "just after signal\n";
        index += next_buffer_size;
    }
    //std::cout << "Frames: " << frames << std::endl;
}

void serializer::deserialize_signal(uint8_t* data_ptr, uint32_t size)
{
    uint32_t next_buffer_size;
    uint32_t index = 0;
    while(index < size-1)
    {
        memcpy(&next_buffer_size, data_ptr+index,sizeof(uint32_t));
        if (next_buffer_size > size-index || next_buffer_size == 0) break;
    	if (index)
    		Pframes++;//Pframes_Received++;
    	else
	    	Iframes++;//Iframes_Received++;
        index += sizeof(uint32_t);
        signal_new_buffer(data_ptr+index, next_buffer_size);
        index += next_buffer_size;
    }
}

void serializer::reset(uint32_t capacity)
{
    data.clear();
    if (capacity) data.reserve(capacity);
}
