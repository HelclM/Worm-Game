#include"Classes.h"

void queue::pushFront(char a){
    std::lock_guard<mutex> guard(mu);
    q.push_front(a);
    //cond.notify_one();
}

char queue::takeFromBack(){
    std::unique_lock<mutex> locker(mu);
    //cond.wait(locker,[this](){return !q.empty();});
    char r=q.back();
    q.pop_back();
    return r;
}
bool queue::empty(){
    std::lock_guard<mutex> guard(mu);
    return q.empty();
}

void queue::eraseAll(){   
    std::lock_guard<mutex> guard(mu);
    while(!q.empty()){
        q.pop_back();
    }
}

void queue::notifyAll(){
    std::lock_guard<mutex> guard(mu);
    q.push_front('p');
    //cond.notify_all();
}