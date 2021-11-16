/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2004-2021 German Aerospace Center (DLR) and others.
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
/// @file    OMPThreading.h
/// @author  Nishant Aswani
/// @date    2021-11-13
///
// A thread class together with a pool and a task for parallelized computation
/****************************************************************************/

#ifndef OMPWorkerThread_h
#define OMPWorkerThread_h

// #define WORKLOAD_PROFILING
// at which interval report maximum workload of the threads, needs WORKLOAD_PROFILING
// undefine to use summary report only
#define WORKLOAD_INTERVAL 100
#include <config.h>
#include <omp.h>

#include <list>
#include <vector>
#include <utils/common/UtilExceptions.h>

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class OMPWorkerThread
 * @brief A thread repeatedly calculating incoming tasks
 */
class OMPWorkerThread
{
public:
    /**
     * @class OMPWorkerThread::Task
     * @brief Abstract superclass of a task to be run with an index to keep track of pending tasks
     */
    class Task
    {
    public:
        /// @brief Destructor
        virtual ~Task(){};

        /** @brief Abstract method which in subclasses should contain the computations to be performed.
         *
         * @param[in] context The thread which runs the task
         */
        virtual void run(OMPWorkerThread *context) = 0;

        /** @brief Sets the running index of this task.
         *
         * Every task receive an index which is unique among all pending tasks of the same thread pool.
         *
         * @param[in] newIndex the index to assign
         */
        void setIndex(const int newIndex)
        {
            myIndex = newIndex;
            return;
        }

    private:
        /// @brief the index of the task, valid only after the task has been added to the pool
        int myIndex;
    };

    /**
     * @class OMPWorkerThread::Pool
     * @brief A pool of worker threads which distributes the tasks and collects the results
     */
    class Pool
    {
    public:
        /** @brief Constructor
         *
         * May initialize the pool with a given number of workers.
         *
         * @param[in] numThreads the number of threads to create
         */
        Pool(int numThreads = 0) : myRunningIndex(0), myException(nullptr)
        {
            while (numThreads > 0)
            {
                new OMPWorkerThread(*this);
                numThreads--;
            }
        }

        /** @brief Destructor
         *
         * Stopping and deleting all workers by calling clear.
         */
        virtual ~Pool()
        {
            clear();
        }

        /** @brief Stops and deletes all worker threads.
         */
        void clear()
        {
            for (OMPWorkerThread *const worker : myWorkers)
            {
                delete worker;
            }
            myWorkers.clear();
            numberOfWorkers = 0;
            return;
        }

        /** @brief Increments the number of workers
         *
         * @param[in] w the thread to add
         */
        void addWorker(OMPWorkerThread *const w)
        {
            myWorkers.push_back(w);
            return;
        }

        /** @brief Gives a number to the given task and assigns it to the worker with the given index.
         * If the index is negative, assign to the next (round robin) one.
         *
         * @param[in] t the task to add
         * @param[in] index index of the worker thread to use or -1 for an arbitrary one
         */
        void add(Task *const t, int index = -1)
        {
            myTasks.push_back(t);
            t->setIndex(myRunningIndex++);
            return;
        }

        /** @brief Adds the given tasks to the list of finished tasks.
         *
         * Locks the internal mutex and appends the finished tasks. This is to be called by the worker thread only.
         *
         * @param[in] tasks the tasks to add
         */
        void addFinished(std::list<Task *> &tasks)
        {
            return;
        }

        void setException(ProcessError &e)
        {
            return;
        }

        /// @brief waits for all tasks to be finished
        void waitAll(const bool deleteFinished = true)
        {
            return;
        }

        /** @brief Checks whether there are currently more pending tasks than threads.
         *
         * This is only a rough estimate because the tasks are already assigned and there could be an idle thread even though the
         *  number of tasks is large.
         *
         * @return whether there are enough tasks to let all threads work
         */
        bool isFull() const
        {
            return false;
        }

        /** @brief Returns the number of threads in the pool.
         *
         * @return the number of threads
         */
        int size() const
        {
            return numberOfWorkers;
        }

        /// @brief locks the pool mutex
        void lock()
        {
            return;
        }

        /// @brief unlocks the pool mutex
        void unlock()
        {
            return;
        }

        const std::vector<OMPWorkerThread *> &getWorkers()
        {
            return myWorkers;
        }

        const std::vector<Task *> &getTasks()
        {
            return myTasks;
        }

        int getNumberOfTasks()
        {
            return this->myTasks.size();
        }

        void incrementNumberOfWorkers()
        {
            this->numberOfWorkers++;
            return;
        }

    private:
        /// @brief the number of threads OpenMP should use
        int numberOfWorkers = 0;
        /// @brief a vector of tasks to be completed by OpenMP
        std::vector<Task *> myTasks;
        /// @brief the current worker threads
        std::vector<OMPWorkerThread *> myWorkers;
        /// @brief list of finished tasks
        std::list<Task *> myFinishedTasks;
        /// @brief the running index for the next task
        int myRunningIndex;
        /// @brief the exception from a child thread
        ProcessError *myException;
    };

public:
    /** @brief Constructor
     *
     * Adds the thread to the given pool and starts it.
     *
     * @param[in] pool the pool for this thread
     */
    OMPWorkerThread(Pool &pool) : myPool(pool), myStopped(false)
    {
        pool.incrementNumberOfWorkers();
        pool.addWorker(this);
    }

    /** @brief Destructor
     *
     * Stops the thread by calling stop.
     */
    virtual ~OMPWorkerThread()
    {
        return;
    }

    /** @brief Adds the given task to this thread to be calculated
     *
     * @param[in] t the task to add
     */
    void add(Task *t)
    {
        return;
    }

    /** @brief Main execution method of this thread.
     *
     * Checks for new tasks, calculates them and puts them in the finished list of the pool until being stopped.
     *
     * @return always 0
     */
    int8_t run()
    {
        return 0;
    }

    /** @brief Stops the thread
     *
     * The currently running task will be finished but all further tasks are discarded.
     */
    void stop()
    {
        return;
    }

private:
    /// @brief the pool for this thread
    Pool &myPool;
    /// @brief the list of pending tasks
    std::list<Task *> myTasks;
    /// @brief the list of tasks which are currently calculated
    std::list<Task *> myCurrentTasks;
    /// @brief whether we are still running
    bool myStopped;
};

#endif
