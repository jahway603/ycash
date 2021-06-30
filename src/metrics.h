// Copyright (c) 2016 The Zcash developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef ZCASH_METRICS_H
#define ZCASH_METRICS_H

#include "uint256.h"
#include "consensus/params.h"

#include <atomic>
#include <mutex>
#include <optional>
#include <string>

struct AtomicCounter {
    std::atomic<uint64_t> value;

    AtomicCounter() : value {0} { }

    void increment(){
        ++value;
    }

    void decrement(){
        --value;
    }

    int get() const {
        return value.load();
    }
};

class AtomicTimer {
private:
    std::mutex mtx;
    uint64_t threads;
    int64_t start_time;
    int64_t total_time;

public:
    AtomicTimer() : threads(0), start_time(0), total_time(0) {}

    /**
     * Starts timing on first call, and counts the number of calls.
     */
    void start();

    /**
     * Counts number of calls, and stops timing after it has been called as
     * many times as start().
     */
    void stop();

    bool running();

    uint64_t threadCount();

    double rate(const AtomicCounter& count);
};

enum DurationFormat {
    FULL,
    REDUCED
};

extern AtomicCounter transactionsValidated;
extern AtomicCounter ehSolverRuns;
extern AtomicCounter solutionTargetChecks;
extern AtomicTimer miningTimer;
extern std::atomic<size_t> nSizeReindexed; // valid only during reindex
extern std::atomic<size_t> nFullSizeToReindex; // valid only during reindex

void TrackMinedBlock(uint256 hash);

void MarkStartTime();
double GetLocalSolPS();
int EstimateNetHeight(const Consensus::Params& params, int currentBlockHeight, int64_t currentBlockTime);
std::optional<int64_t> SecondsLeftToNextEpoch(const Consensus::Params& params, int currentHeight);
std::string DisplayDuration(int64_t time, DurationFormat format);
std::string DisplaySize(size_t value);
std::string DisplayHashRate(double value);

void TriggerRefresh();

void ConnectMetricsScreen();
void ThreadShowMetricsScreen();

/**
 *
 * Rendering options:
 * Ycash: img2txt -W 60 -H 30 -f utf8 -d none -g 0.7 ycash.png > ycash.txt
 */
const std::string METRICS_ART = " \n"                                                            
"               [0;34;40m :[0;30;5;40;100mX8[0;34;40m [0m                                                       \n"
"              [0;34;40m [0;1;30;90;47m@[0;37;5;47;107m:  [0;1;30;90;47m:[0;34;40m. [0m                                                     \n"
"              [0;34;40m [0;1;37;97;47mt[0;37;5;47;107m    [0;1;30;90;47mt[0;34;40m [0m                                                     \n"
"              [0;34;40m [0;36;5;40;100mt[0;37;5;47;107m     [0;36;5;40;100m [0;34;40m [0m           [0;34;40m [0;1;30;90;40mS[0;30;5;40;100m@@@[0;1;30;90;40mS[0;34;40m [0m                                  \n"
"               [0;34;40m [0;1;37;97;47mX[0;37;5;47;107m    :[0;30;5;40;100m8[0;34;40m [0m  [0;34;40m [0;30;5;40;100mX[0;37;5;40;100m8[0;1;30;90;47m@8[0;36;5;40;100m [0;34;40m. [0;36;5;40;100m%[0;37;5;47;107m.   .[0;30;5;40;100mX[0;34;40m .[0;36;5;40;100m [0;37;5;40;100m@[0;1;30;90;47m88[0;36;5;40;100m [0;1;30;90;40mX[0;34;40m [0m                         \n"
"               [0;34;40m [0;36;5;40;100m;[0;37;5;47;107m     8[0;34;40m:[0m [0;34;40m [0;30;5;40;100mS[0;37;5;47;107m     [0;1;37;97;47m.[0;34;40m [0;37;5;40;100m8[0;37;5;47;107m     [0;37;5;40;100mX[0;34;40m [0;37;5;40;100mX[0;37;5;47;107m     [0;1;37;97;47m%[0;34;40m [0m                         \n"
"                [0;34;40m [0;1;37;97;47mS[0;37;5;47;107m     [0;1;30;90;47m;[0;34;40m  [0;36;5;40;100m [0;37;5;47;107m     8[0;34;40m [0;1;30;90;47m@[0;37;5;47;107m     [0;37;5;40;100m8[0;34;40m [0;1;30;90;47mS[0;37;5;47;107m     [0;1;37;97;47m%[0;34;40m [0m                         \n"
"                [0;34;40m [0;36;5;40;100m%[0;37;5;47;107m      [0;36;5;40;100m.[0;34;40m [0;37;5;40;100mS[0;37;5;47;107m     @[0;34;40m [0;1;30;90;47mX[0;37;5;47;107m     [0;37;5;40;100m8[0;34;40m [0;1;30;90;47mt[0;37;5;47;107m     [0;1;37;97;47m%[0;34;40m [0m                         \n"
"                 [0;34;40m [0;1;37;97;47mS[0;37;5;47;107m     8[0;34;40m [0;37;5;40;100m@[0;37;5;47;107m     @[0;34;40m [0;1;30;90;47mX[0;37;5;47;107m     [0;37;5;40;100m8[0;34;40m [0;1;30;90;47m;[0;37;5;47;107m     [0;1;37;97;47m%[0;34;40m [0m                         \n"
"                 [0;34;40m [0;36;5;40;100m:[0;37;5;47;107m     S[0;34;40m [0;37;5;40;100m8[0;37;5;47;107m     8[0;34;40m [0;1;30;90;47m@[0;37;5;47;107m     [0;37;5;40;100m8[0;34;40m [0;1;30;90;47m;[0;37;5;47;107m     [0;1;37;97;47m%[0;34;40m [0m                         \n"
"                  [0;34;40m [0;1;37;97;47m8[0;37;5;47;107m    :[0;34;40m [0;37;5;40;100m@[0;37;5;47;107m     [0;1;37;97;47m@[0;34;40m [0;1;30;90;47m8[0;37;5;47;107m     [0;37;5;40;100m8[0;34;40m [0;1;30;90;47m;[0;37;5;47;107m     [0;1;37;97;47m%[0;34;40m  [0m   [0;34;40m     .[0;1;30;90;40mS[0;34;40m:  [0m           \n"
"                  [0;34;40m [0;37;5;40;100mS[0;37;5;47;107m     [0;30;5;40;100m88[0;37;5;47;107mt    [0;1;30;90;47mt[0;34;40m [0;37;5;40;100m%[0;37;5;47;107m     [0;37;5;40;100m@[0;34;40m [0;1;30;90;47m;[0;37;5;47;107m     [0;1;37;97;47m%[0;34;40m [0;30;5;40;100m8[0;1;30;90;47m@t [0;1;37;97;47m;X[0;37;5;47;107m8S:  .[0;1;37;97;47mS[0;37;5;40;100mS[0;34;40m: [0m         \n"
"                  [0;34;40m [0;1;30;90;40mS[0;37;5;47;107m.    [0;1;37;97;47m@[0;36;5;40;100mt[0;1;30;90;40m%SXX[0;34;40m;[0;1;30;90;40mS[0;34;40m:[0;1;30;90;40mX[0;1;37;97;47m@[0;37;5;47;107mS%S8[0;30;5;40;100mX[0;34;40m [0;30;5;40;100mS[0;1;37;97;47m:[0;37;5;47;107mX;S[0;1;37;97;47mt[0;36;5;40;100m.[0;34;40m;[0;1;37;97;47m8[0;37;5;47;107m             8[0;34;40m: [0m        \n"
"                   [0;34;40m [0;1;30;90;47m.[0;37;5;47;107m       :tt: @[0;1;30;90;47m8[0;36;5;40;100m [0;30;5;40;100mS88[0;36;5;40;100m%[0;37;5;40;100mS[0;1;37;97;47m;[0;37;5;40;100m8[0;36;5;40;100m :::.[0;1;30;90;47m8[0;37;5;47;107mX              .[0;1;30;90;40mX[0;34;40m [0m        \n"
"                   [0;34;40m [0;1;30;90;40m8[0;37;5;47;107m.                                      :[0;1;37;97;47m8[0;1;30;90;47m:[0;37;5;40;100m@[0;36;5;40;100m%[0;34;40m [0m         \n"
"                    [0;34;40m [0;1;30;90;47m@[0;37;5;47;107m                                .8[0;1;37;97;47m.[0;1;30;90;47m8[0;36;5;40;100m [0;1;30;90;40m8[0;34;40m.  [0m            \n"
"                    [0;34;40m .[0;1;37;97;47m@[0;37;5;47;107m                           @[0;1;30;90;47m:[0;37;5;40;100mX[0;30;5;40;100mS[0;34;40m:  [0m                  \n"
"                     [0;34;40m [0;1;30;90;40mX[0;37;5;47;107m8                         [0;1;37;97;47m [0;34;40m  [0m                       \n"
"                      [0;34;40m [0;1;30;90;40mX[0;1;37;97;47m8[0;37;5;47;107m                       [0;1;37;97;47m.[0;34;40m: [0m                        \n"
"                       [0;34;40m :[0;1;30;90;47mS[0;37;5;47;107m.                   8[0;36;5;40;100m [0;34;40m [0m                          \n"
"                         [0;34;40m [0;30;5;40;100m8[0;1;30;90;47m [0;37;5;47;107m:              8[0;1;30;90;47mt[0;36;5;40;100mt[0;34;40m  [0m                           \n"
"                           [0;34;40m ;[0;36;5;40;100m [0;1;30;90;47m%[0;1;37;97;47mt8[0;37;5;47;107m88[0;1;37;97;47m8S:[0;1;30;90;47m;8[0;36;5;40;100m [0;30;5;40;100mS[0;34;40m;  [0m                              \n"
"                               [0;34;40m        [0m                                    \n";

#endif // ZCASH_METRICS_H
