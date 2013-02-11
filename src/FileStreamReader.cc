#include "FileStreamReader.h"

using namespace Ymir;

FileStreamReader::FileStreamReader(const std::string& filename, const std::ios_base::openmode mode) : stream(filename, mode)
{
}

size_t FileStreamReader::read(char* s, const size_t max_size)
{
    stream.read(s, max_size);
    return stream.gcount();
}

void FileStreamReader::skip(const size_t amount)
{
    stream.seekg(amount, std::ios_base::cur);
}

bool FileStreamReader::eof() const
{
    return stream.eof();
}

size_t FileStreamReader::size()
{
    std::streampos cur = stream.tellg();
    stream.seekg(0, std::ios::end);
    std::streampos size = stream.tellg();
    stream.seekg(cur);
    return size;
}
