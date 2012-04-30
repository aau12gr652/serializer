#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <boost/signals2.hpp>

class serializer
{
private:
	std::vector<uint8_t> data;
public:
    boost::signals2::signal< void (uint8_t*, uint32_t)> signal_new_buffer;

    serializer(uint32_t size = 0);
    ~serializer();

	void feed(uint8_t* ptr, uint32_t size);
    std::vector<uint8_t>& serialize();
    void deserialize_signal(std::vector<uint8_t>& data);
};

