#include <fstream>
#include <string>
#include <iostream>
#include "util/ExerciseUtils.h"
using namespace std;
using namespace pulsar;

Producer producer1;
Producer producer2;
Producer producer3;

bool running = false;
int messageCounter = 0;

// our (very simple) message content builder
Message GenerateMessageContent()
{
    std::string str(1052, 'a');

    Message msg = MessageBuilder().setContent(str).build();

    LOG_INFO("Sending message: " << str);
    return msg;
}

void SendMessages()
{

    LOG_INFO("SendMessages")

    running = true;

    Message msg;
    msg = GenerateMessageContent();        
    for (int i = 1; i <= 1200; i++)
    { 
        Producer producer1;
        
        Result result;
        result = client.createProducer("persistent://public/default/MEM.TEST", producer1);
        if (result != ResultOk)
        {
            LOG_ERROR("Error creating producer: " << result);
            return;
        }

        producer1.send(msg);
        producer1.close();
    }
}

// make sure we close our producer and client
void Close()
{
    producer1.close();
    client.close();
}

// wire up our handler for signals for a clean shutdown!
void ShutdownProcess(int signal)
{
  running = false;
  exit(0);
}

void RegisterShutdownHook()
{
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = ShutdownProcess;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
}

int main()
{
    RegisterShutdownHook();

    string before;
    cin >> before;

    cout << "Before sending message" << endl;
    SendMessages();
    cout << "After sending message" << endl;

    string after;
    cin >> after;    

    Close();
}
