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
            sleep(1);
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
        sleep(1);
        printf("Done with thread %d\n",id_num);
        //std::cout << "Done with thread " << id_num << std::endl;
    }
};

TEST(OMPWorkerThread, test_omp_time)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    OMPWorkerThread::Pool g(4);

    OMPWorkerThread::Task *task1 = new TestTaskOMP(1);
    OMPWorkerThread::Task *task2 = new TestTaskOMP(2);
    OMPWorkerThread::Task *task3 = new TestTaskOMP(3);
    OMPWorkerThread::Task *task4 = new TestTaskOMP(4);
    g.add(task1);
    g.add(task2);
    g.add(task3);
    g.add(task4);
    g.waitAll();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Threads completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}

TEST(FXWorkerThread, test_foxtools_time)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    FXWorkerThread::Pool g(4);
    FXWorkerThread::Task *task1 = new TestTaskFox(1);
    FXWorkerThread::Task *task2 = new TestTaskFox(2);
    FXWorkerThread::Task *task3 = new TestTaskFox(3);
    FXWorkerThread::Task *task4 = new TestTaskFox(4);
    g.add(task1);
    g.add(task2);
    g.add(task3);
    g.add(task4);
    g.waitAll();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Threads completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}