#include <cinttypes>
#include <chrono>
#include <mutex>
#include "Timeline.h"

std::mutex timeMutex;
int64_t start_time;
int64_t elapsed_paused_time;
int64_t last_paused_time;
int64_t tic;
bool paused;
Timeline *anchor;

Timeline::Timeline(Timeline *anchor, int64_t tic)
{
    this->anchor = anchor;
    paused = false;
    start_time = getCurrentTime();
    elapsed_paused_time = 0;
    this->tic = tic;
    last_paused_time = 0;

}

Timeline::Timeline()
{
    Timeline(NULL, 0);
}


void Timeline::setAnchor(Timeline *anchor) {
    this->anchor = anchor;
}

int64_t Timeline::getTime()
{
    if (anchor == NULL) {
        return ((getCurrentTime() - start_time) - elapsed_paused_time) / tic;
    } else {
       return anchor->getTime();
    }
}

void Timeline::pause()
{
    paused = true;
    last_paused_time = getCurrentTime();
}

void Timeline::unpause()
{
    elapsed_paused_time += (getCurrentTime() - last_paused_time) / tic;
    paused = false;
}

void Timeline::changeTic(int64_t tic)
{
    this->tic = tic;
}

bool Timeline::isPaused()
{
    if (anchor == NULL) {
        return paused;
    } else {
        return anchor->isPaused();
    }
}

int64_t Timeline::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return ms.count();
}
