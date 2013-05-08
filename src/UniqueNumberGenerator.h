#pragma once


namespace Ymir
{
    class UniqueNumberGenerator
    {
    private:
        int uniqueNumber;

    public:
        static const int half_way = 1000000;
        UniqueNumberGenerator();
        ~UniqueNumberGenerator();
        int getNewUniqueNumber();
        int flip(int id);
    };
}
