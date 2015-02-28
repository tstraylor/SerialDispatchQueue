//
//  Created by Thomas Traylor on 2/27/15.
//  Copyright (c) 2015 Thomas Traylor. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <exception>
#include <stdexcept>
#include <chrono>

#include "SerialDispatchQueue.h"

using namespace std;

SerialDispatchQueue::SerialDispatchQueue() :
    m_stopped(false), m_taskThread(0)
{
}

SerialDispatchQueue::~SerialDispatchQueue()
{
    {
        std::unique_lock<std::mutex> guard(m_mutex);
        m_stopped = true;
    }

    if(m_taskThread) {
        m_cv.notify_all();
        m_taskThread->join();
        delete m_taskThread;
        m_taskThread = 0;
    }
}

void SerialDispatchQueue::start() 
{
    // check to see if the queue is started already
    if(m_taskThread)
        return;

    m_taskThread = new std::thread( [this] { 

            for(;;) { 
                
                std::function<void(void)> task;

                {
                    std::unique_lock<std::mutex> guard(m_mutex); 
                    m_cv.wait(guard, [this] { 
                            return this->m_stopped || !this->m_taskQueue.empty(); 
                        }); 

                    if(m_stopped)
                        return;

                    task =  m_taskQueue.front();
                    m_taskQueue.pop();
                }

                // exceptions thrown by the tasks will cause
                // the task thread to stop running.
                task();
            }

            m_stopped = true;
        });

    if(!m_taskThread) 
        throw std::runtime_error("Unable to create queue task thread");
}

void SerialDispatchQueue::stop() 
{
    {
        std::unique_lock<std::mutex> guard(m_mutex);
        m_stopped = true;
    }

    if(m_taskThread) {
        m_cv.notify_all();
        m_taskThread->join();
        delete m_taskThread;
        m_taskThread = 0;
    }
}

void SerialDispatchQueue::run(const std::function<void(void)> &task) 
{
    {
        std::unique_lock<std::mutex> guard(m_mutex);
        // we don't want tasks queued while the queue is stopped
        if(m_stopped)
            return; 

        m_taskQueue.push(task);
    }

    m_cv.notify_all();
}

