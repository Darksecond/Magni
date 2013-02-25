#include "FileStreamReader.h"

using namespace Ymir;

FileStreamReader::FileStreamReader(const std::string& filename, const std::ios_base::openmode mode) : _stream(filename, mode)
{
}

size_t FileStreamReader::read(char* s, const size_t max_size)
{
    _stream.read(s, max_size);
    return _stream.gcount();
}

void FileStreamReader::skip(const size_t amount)
{
    _stream.seekg(amount, std::ios_base::cur);
}

bool FileStreamReader::eof() const
{
    return _stream.eof();
}

size_t FileStreamReader::size()
{
    std::streampos cur = _stream.tellg();
    _stream.seekg(0, std::ios::end);
    std::streampos size = _stream.tellg();
    _stream.seekg(cur);
    return size;
}

std::istream& FileStreamReader::stream()
{
    return _stream;
}
