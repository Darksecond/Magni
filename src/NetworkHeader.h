#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

class NetworkHeader
{
public:
	NetworkHeader(const uint32_t& size, const uint32_t& count, const uint32_t& id, const uint32_t& type) : _size(size), _count(count), _array(nullptr), _dirty(true), _id(id), _type(type)
	{
	}

	NetworkHeader(const uint8_t* array) : _size(0), _count(0), _array(nullptr), _dirty(true)
	{
		build_headers_from_array(array);
	}

	~NetworkHeader()
	{
		if(_array)
			delete _array;
	}

	inline const uint32_t& size() { return _size; }
	inline const uint32_t& count() { return _count; }
	inline const uint32_t& id() { return _id; }
	inline const uint32_t& type() { return _type; }

	static inline size_t header_length()
	{
		return sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
	}

	inline const uint8_t* to_char_array()
	{
		if(_dirty)
			build_array();
		return _array;
	}
private:
	uint32_t _size;
	uint32_t _count;
    uint32_t _id;
    uint32_t _type;
	uint8_t* _array;
	bool _dirty;

	void build_headers_from_array(const uint8_t* array)
	{
		int i = 0;
		_size = *reinterpret_cast<const uint32_t*>(array+i);
		i += sizeof(uint32_t);

		_count = *reinterpret_cast<const uint32_t*>(array+i);
		i += sizeof(uint32_t);

		_id = *reinterpret_cast<const uint32_t*>(array+i);
		i += sizeof(uint32_t);

		_type = *reinterpret_cast<const uint32_t*>(array+i);
		i += sizeof(uint32_t);
	}

	void build_array()
	{
		delete _array;
		_array = new uint8_t[header_length()];

		int i = 0;

		//size
		union
		{
			uint32_t as_size_t;
			char as_char[sizeof(uint32_t)];
		} size;
		size.as_size_t = _size;
		memcpy(_array+i, size.as_char, sizeof(size));
		i += sizeof(size);

		//number of values (count)
		union
		{
			uint32_t as_size_t;
			char as_char[sizeof(uint32_t)];
		} count;
		count.as_size_t = _count;
		memcpy(_array+i, count.as_char, sizeof(count));
		i += sizeof(count);

        //id
		union
		{
			uint32_t as_uint;
			char as_char[sizeof(uint32_t)];
		} id;
		id.as_uint = _id;
		memcpy(_array+i, id.as_char, sizeof(count));
		i += sizeof(count);

        //type
		union
		{
			uint32_t as_uint;
			char as_char[sizeof(uint32_t)];
		} type;
		type.as_uint = _type;
		memcpy(_array+i, type.as_char, sizeof(count));
		i += sizeof(count);

		_dirty = false;
	}
};
