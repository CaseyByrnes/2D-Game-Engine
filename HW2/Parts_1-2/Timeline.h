#pragma once
#include <cinttypes>
#include <mutex>


class Timeline
{
    private:
        std::mutex timeMutex;
        int64_t start_time;
        int64_t elapsed_paused_time;
        int64_t last_paused_time;
        int64_t tic;
        bool paused;
        Timeline *anchor;
        int64_t getCurrentTime();
    public:
        Timeline(Timeline *anchor, int64_t tic);
        Timeline();
        int64_t getTime();
        void pause();
        void unpause();
        void changeTic(int64_t tic);
        bool isPaused();
        void setAnchor(Timeline *anchor);
};