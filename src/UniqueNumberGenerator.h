#pragma once


namespace Ymir
{
    class UniqueNumberGenerator
    {
    private:
        int uniqueNumber;

    public:
        UniqueNumberGenerator();
        ~UniqueNumberGenerator();
        int getNewUniqueNumber();
    };
}
