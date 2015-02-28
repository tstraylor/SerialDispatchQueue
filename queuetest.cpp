
#include <iostream>
#include <thread>
#include <chrono>
#include "SerialDispatchQueue.h"

using namespace std;

class Data {
    public:
        Data(): m_data(0) {};
        Data(int d): m_data(d) {};
        ~Data() {};
        int data() { return m_data; };
        void data(int d) { m_data = d;};
    private:
        int m_data;
};

void processData(vector<Data> &dvec, int i)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Data i(" << i << "): " << dvec[i].data() << endl;
    dvec[i].data(dvec[i].data()*2);
    cout << "Data i(" << i << "): " << dvec[i].data() << endl;
}

void sendMessage(const string &msg) 
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << msg << endl;
}

int main(int argc, char *argv[])
{
    vector<Data> dataVector;
    for(int i = 0; i < 10; ++i) {
        dataVector.emplace_back(i*2);
    }

    cout << "** starting queue" << endl;
    SerialDispatchQueue queue;
    queue.start();

    // queue some stuff
    for(int i = 0; i<10; ++i) {
        queue.run(std::bind(processData, std::ref(dataVector), i));
        queue.run(std::bind(sendMessage, "Message"));
    }

    std::this_thread::sleep_for(std::chrono::seconds(20));

    vector<Data>::iterator it = dataVector.begin();
    for(; it != dataVector.end(); ++it) {
        cout << "data: " << (*it).data() << endl;
    }

    cout << "** stopping queue" << endl;
    queue.stop();

    return 0;
}
