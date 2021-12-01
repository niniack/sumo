/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2021 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    CompareOMPToFoxTools.cpp
/// @author  Graham Jones
/// @date    Nov 2021
///
// Compares the performance of OMPThread to FoxToolsThreads
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>
#include <iostream>
#include <gtest/gtest.h>
#include <utils/common/StdDefs.h>
#include <utils/omp/OMPThreading.h>
#include <utils/foxtools/FXWorkerThread.h>
#include <chrono>

struct SortTask
{
    SortTask()
    {
        srand(time(0));
        for (int i = 0; i < 1000000; i++)
        {
            arr_.emplace_back(rand());
            // making a vector filled with random elements
        }
    }

    void run()
    {
        sort(arr_.begin(), arr_.end(), std::less<int>());
    }

private:
    std::vector<int> arr_;
};

class TestTaskOMP : public OMPWorkerThread::Task
{
    int id_num;

public:
    TestTaskOMP(int id)
    {
        id_num = id;
    }
    void run()
    {
        SortTask benchmark;
        benchmark.run();
        printf("Done with thread %d\n", id_num);
    }
};

class TestTaskFox : public FXWorkerThread::Task
{
    int id_num;

public:
    TestTaskFox(int id)
    {
        id_num = id;
    }
    void run(FXWorkerThread * /* context */)
    {
        SortTask benchmark;
        benchmark.run();
        printf("Done with thread %d\n", id_num);
    }
};

TEST(OMPWorkerThread, test_omp_time)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    OMPWorkerThread::Pool g(16);
    OMPWorkerThread::Task *task1 = new TestTaskOMP(1);
    OMPWorkerThread::Task *task2 = new TestTaskOMP(2);
    OMPWorkerThread::Task *task3 = new TestTaskOMP(3);
    OMPWorkerThread::Task *task4 = new TestTaskOMP(4);
    OMPWorkerThread::Task *task5 = new TestTaskOMP(5);
    OMPWorkerThread::Task *task6 = new TestTaskOMP(6);
    OMPWorkerThread::Task *task7 = new TestTaskOMP(7);
    OMPWorkerThread::Task *task8 = new TestTaskOMP(8);
    OMPWorkerThread::Task *task9 = new TestTaskOMP(9);
    OMPWorkerThread::Task *task10 = new TestTaskOMP(10);
    OMPWorkerThread::Task *task11 = new TestTaskOMP(11);
    OMPWorkerThread::Task *task12 = new TestTaskOMP(12);
    OMPWorkerThread::Task *task13 = new TestTaskOMP(13);
    OMPWorkerThread::Task *task14 = new TestTaskOMP(14);
    OMPWorkerThread::Task *task15 = new TestTaskOMP(15);
    OMPWorkerThread::Task *task16 = new TestTaskOMP(16);
    OMPWorkerThread::Task *task17 = new TestTaskOMP(17);
    OMPWorkerThread::Task *task18 = new TestTaskOMP(18);
    OMPWorkerThread::Task *task19 = new TestTaskOMP(19);
    OMPWorkerThread::Task *task20 = new TestTaskOMP(20);
    g.add(task1);
    g.add(task2);
    g.add(task3);
    g.add(task4);
    g.add(task5);
    g.add(task6);
    g.add(task7);
    g.add(task8);
    g.add(task9);
    g.add(task10);
    g.add(task11);
    g.add(task12);
    g.add(task13);
    g.add(task14);
    g.add(task15);
    g.add(task16);
    g.add(task17);
    g.add(task18);
    g.add(task19);
    g.add(task20);
    g.waitAll(true);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Threads completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}

TEST(FXWorkerThread, test_foxtools_time)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    FXWorkerThread::Pool g(16);
    FXWorkerThread::Task *task1 = new TestTaskFox(1);
    FXWorkerThread::Task *task2 = new TestTaskFox(2);
    FXWorkerThread::Task *task3 = new TestTaskFox(3);
    FXWorkerThread::Task *task4 = new TestTaskFox(4);
    FXWorkerThread::Task *task5 = new TestTaskFox(5);
    FXWorkerThread::Task *task6 = new TestTaskFox(6);
    FXWorkerThread::Task *task7 = new TestTaskFox(7);
    FXWorkerThread::Task *task8 = new TestTaskFox(8);
    FXWorkerThread::Task *task9 = new TestTaskFox(9);
    FXWorkerThread::Task *task10 = new TestTaskFox(10);
    FXWorkerThread::Task *task11 = new TestTaskFox(11);
    FXWorkerThread::Task *task12 = new TestTaskFox(12);
    FXWorkerThread::Task *task13 = new TestTaskFox(13);
    FXWorkerThread::Task *task14 = new TestTaskFox(14);
    FXWorkerThread::Task *task15 = new TestTaskFox(15);
    FXWorkerThread::Task *task16 = new TestTaskFox(16);
    FXWorkerThread::Task *task17 = new TestTaskFox(17);
    FXWorkerThread::Task *task18 = new TestTaskFox(18);
    FXWorkerThread::Task *task19 = new TestTaskFox(19);
    FXWorkerThread::Task *task20 = new TestTaskFox(20);
    g.add(task1);
    g.add(task2);
    g.add(task3);
    g.add(task4);
    g.add(task5);
    g.add(task6);
    g.add(task7);
    g.add(task8);
    g.add(task9);
    g.add(task10);
    g.add(task11);
    g.add(task12);
    g.add(task13);
    g.add(task14);
    g.add(task15);
    g.add(task16);
    g.add(task17);
    g.add(task18);
    g.add(task19);
    g.add(task20);
    g.waitAll(true);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Threads completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}