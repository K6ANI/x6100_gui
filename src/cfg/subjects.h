#pragma once

enum data_type {
    DTYPE_INVALID = 0,
    DTYPE_INT,
    DTYPE_UINT64,
    DTYPE_FLOAT,
    DTYPE_GROUP,
};

#ifdef __cplusplus

#include <mutex>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <thread>
#include <atomic>

class Subject;

class Observer {
    protected:
    Subject *subj;
    void (*fn)(Subject *, void *);
    void *user_data;

    public:
    Observer(Subject *subj, void (*fn)(Subject *, void *), void *user_data): subj(subj), fn(fn), user_data(user_data) {
    };
    ~Observer();
    virtual void notify();
};

class ObserverDelayed: public Observer {
    static std::vector<ObserverDelayed*> instances;
    std::thread::id tid;
    std::atomic<bool> changed = false;
    public:
    ObserverDelayed(Subject *subj, void (*fn)(Subject *, void *), void *user_data): Observer(subj, fn, user_data) {
        tid = std::this_thread::get_id();
        instances.push_back(this);
    };
    ~ObserverDelayed();
    void notify();
    static void notify_delayed();

};

class Subject {
    std::mutex mutex_subscribe;
    protected:
    std::vector<Observer*> observers;
    data_type type;
    public:
    virtual data_type dtype();
    Observer* subscribe(void (*fn)(Subject *, void *), void *user_data=nullptr);
    ObserverDelayed* subscribe_delayed(void (*fn)(Subject *, void *), void *user_data=nullptr);
    void unsubscribe(Observer *o);
};

template <typename T> class SubjectT : public Subject {
    static_assert(std::is_same_v<T, int32_t>
                  || std::is_same_v<T, uint64_t>
                  || std::is_same_v<T, float>,
                  "Unsupported type");
    T          val;
    std::mutex mutex_set;

  public:
    SubjectT(T val) : val(val) {};
    T get() {
        return val;
    };
    void set(T val) {
        bool changed = false;
        {
            const std::lock_guard<std::mutex> lock(mutex_set);
            if (this->val != val) {
                this->val = val;
                changed   = true;
            }
        }
        if (changed) {
            for (auto observer : observers) {
                observer->notify();
            }
        }
    };
    data_type dtype() {
        if (std::is_same_v<T, int32_t>) return DTYPE_INT;
        if (std::is_same_v<T, uint64_t>) return DTYPE_UINT64;
        if (std::is_same_v<T, float>) return DTYPE_FLOAT;
        return DTYPE_INVALID;
    }
};

#else

typedef struct Subject Subject;
typedef struct Observer Observer;
typedef struct ObserverDelayed ObserverDelayed;

#endif


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// #define MAX_OBSERVERS 16

// typedef struct __subject *subject_t;

// typedef struct __observer *observer_t;

Subject *subject_create_int(int32_t val);
Subject *subject_create_uint64(uint64_t val);
Subject *subject_create_float(float val);
// subject_t subject_create_group(subject_t *subjects, uint8_t count);

int32_t  subject_get_int(Subject *subj);
uint64_t subject_get_uint64(Subject *subj);
float    subject_get_float(Subject *subj);

/// @brief Add observer to subject
/// @param subj subject to add observer
/// @param fn observer callback
/// @param user_data pointer to additional user data
/// @return observer to remove it from subject
Observer *subject_add_observer(Subject *subj, void (*fn)(Subject *, void *), void *user_data);
Observer *subject_add_observer_and_call(Subject *subj, void (*fn)(Subject *, void *), void *user_data);

ObserverDelayed *subject_add_delayed_observer(Subject *subj, void (*fn)(Subject *, void *), void *user_data);
ObserverDelayed *subject_add_delayed_observer_and_call(Subject *subj, void (*fn)(Subject *, void *), void *user_data);

enum data_type subject_get_dtype(Subject *subj);

void subject_set_int(Subject *subj, int32_t val);
void subject_set_uint64(Subject *subj, uint64_t val);
void subject_set_float(Subject *subj, float val);

void observer_del(Observer *observer);
void observer_delayed_del(ObserverDelayed *observer);

void observer_delayed_notify_all(void);

#ifdef __cplusplus
}
#endif
