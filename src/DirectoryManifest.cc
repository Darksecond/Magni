#include "DirectoryManifest.h"

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

std::unique_ptr<StreamReader> DirectoryManifest::read(const std::string& identifier)
{
    return std::unique_ptr<FileStreamReader>{new FileStreamReader{identifierToFile(identifier)}};
}