#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}
*/

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    // perform vector modification under the lock
    std::lock_guard<std::mutex> lck(_mutex);

    // add msg to queue
    std::cout << "   Message #" << msg << " will be added to the queue" << std::endl;
    _queue.push_back(std::move(msg));

    // notify client after adding new message into queue vector
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

/* 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}
*/
void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method
    // „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    _currentPhase = TrafficLightPhase::red;
    while (true)
    {
        int min = 4;
        int max = 6;
        int randNum = rand()%(max-min + 1) + min;
        std::this_thread::sleep_for(std::chrono::seconds(randNum));
        if (_currentPhase == TrafficLightPhase::red) {
            _currentPhase = TrafficLightPhase::green;
        } else {
            _currentPhase = TrafficLightPhase::red;
        }
        // wait 1ms between two cycles
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // TODO send update message to MessageQueue using move semantics
        _msg_queue.send(std::move(_currentPhase));

    }
    
}

