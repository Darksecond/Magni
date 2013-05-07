#pragma once

namespace Ymir{
    class Timer {
        private:
            double passedTime;
            double goalTime;
        public:
            Timer(double time);

            void update(double time);
            bool reached();
            void reset();
    };
};
