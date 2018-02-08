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

#ifndef __SerialDispatchQueue_h__
#define __SerialDispatchQueue_h__

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class SerialDispatchQueue {

public:

    SerialDispatchQueue();

    ~SerialDispatchQueue();

    void start();

    void stop();

    void run(const std::function<void(void)> &task);

private:

    bool m_stopped;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread *m_taskThread;
    std::queue<std::function<void(void)> > m_taskQueue;
};

#endif // __SerialDispatchQueue_h__

