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
/// @file    OMPThreadTest.cpp
/// @author  Nishant Aswani
/// @date    Nov 2021
///
// Tests the class OMPThread
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>
#include <iostream>
#include <gtest/gtest.h>
#include <utils/common/StdDefs.h>
#include <utils/omp/OMPThreading.h>

class TestTask : public OMPWorkerThread::Task
{
public:
    void run(OMPWorkerThread * /* context */)
    {
    }
};

// ===========================================================================
// test definitions
// ===========================================================================
/* Test the initialization.*/
TEST(OMPWorkerThread, test_init)
{
    OMPWorkerThread::Pool g(4);
}

/* Test size of pool */
TEST(OMPWorkerThread, test_size)
{
    uint8_t num_threads = 10;
    OMPWorkerThread::Pool g(num_threads);
    assert(num_threads == g.size());
    assert(num_threads == g.getWorkers().size());
}

/* Test pool destruction */
TEST(OMPWorkerThread, test_pool_destruction)
{
    uint8_t num_threads = 10;
    OMPWorkerThread::Pool g(num_threads);
    g.clear();
    assert(0 == g.size());
    assert(0 == g.getWorkers().size());
}

/* Test adding worker */
TEST(OMPWorkerThread, test_add_worker)
{
    uint8_t num_threads = 2;
    OMPWorkerThread::Pool g(num_threads);
    new OMPWorkerThread(g);
    assert(num_threads + 1 == g.size());
}

/* Test basic OpenMP API */
TEST(OMPWorkerThread, test_openmp_basic)
{
    uint8_t num_threads = 4;
    omp_set_num_threads(num_threads);
#pragma omp parallel
    {
        assert(num_threads == omp_get_num_threads());
    }
}

/* Test retrieving all tasks.*/
TEST(OMPWorkerThread, test_get_all)
{
    OMPWorkerThread::Pool g(4);
    OMPWorkerThread::Task *task1 = new TestTask();
    OMPWorkerThread::Task *task2 = new TestTask();
    OMPWorkerThread::Task *task3 = new TestTask();
    OMPWorkerThread::Task *task4 = new TestTask();
    g.add(task1);
    g.add(task2);
    g.add(task3);
    g.add(task4);
    assert(4 == g.getNumberOfTasks());
}