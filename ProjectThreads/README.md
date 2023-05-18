## Table of Contents

- [Part 1: RECURSIVE LOCKS](#part-1-recursive-locks)
  - [1. Lock Attributes](#1-lock-attributes)
  - [2. Recursive Locks](#2-recursive-locks)
  - [3. The TODOs](#3-the-todos)
- [Part 2: CONDITION VARIABLES](#part-2-condition-variables)
  - [1. Recursive Lock implementation dependencies](#1-recursive-lock-implementation-dependencies)
  - [2. Condition Variables](#2-condition-variables)
  - [3. The TODOs](#3-the-todos-1)

# Part 1: RECURSIVE LOCKS

Note: Lock and mutex are used interchangeably here to mean mutex. We are not discussing
semaphores here in any capacity.<br>
<br>

## 1] Lock Attributes

Have you ever wondered what the second parameter to `pthread_mutex_init` stands for? Attributes.
But what does that mean and what are the different attribute types? Some of them are error
checking, recursive, default, robustness, etc. In class, we have already seen implementations
for the spin lock approach such as test and set and compare and swap. For this project, 
you will have to implement recursive locks.<br>
<br>

## 2] Recursive Locks

Imagine that you have to process a tree or a list (for reading/modifying it, etc) 
and you choose to traverse these structures recursively. To make this function thread safe, 
you want to utilize mutexes and the result is the following logic: locking the mutex, recursively 
calling the function, and unlocking the mutex. A mutex without any special attributes would
not be able to handle this approach and would deadlock since, on the recursive call, the thread
would attempt to reacquire the already acquired mutex.\
\
Considera different case of a thread safe class that has a group of methods that acquire the same 
lock at their start, releasing it later. You wish to make another method of similar architecture
but end up needing to call one of these methods. A mutex without any special attributes would
not be able to handle this approach and would deadlock since, on the call to the other function, 
the thread would attempt to reacquire the already acquired mutex. And unfortunately changing the 
class structure would be too time intensive.\
\
The issue highlighted in the above functions is that the *same* thread is prevented
from reacquiring the mutex. This does seem weird since I already own something, 
why cannot I own it again - simply say that I still own it. Recursive locks thus
come in to counteract this in a similar manner. A recursive lock implementation 
would keep a note of which thread has currently acquired the mutex as well as the 
number of times that the mutex has been acquired by the current lock owner. It permits 
reacquisition of the lock but blocks on acquisition of the lock if the lock is occupied and
occupied by a thread that is not the caller. Similarly, unlocking the mutex just unlocks
the mutex by 1 level. If the number of acquisitions reaches 0, the mutex is marked as free
and ready to be acquired by other threads (or the same thread).<br>
<br>

## 3] The TODOs

As we have seen, recursive mutexes indeed seem like a powerful idea. The main task is
to implement the functions mentioned below in `recurMutex.cpp`. To successfully complete it,
however, you will have to make some additions (not changes) to the `recurMutex.h` header
file as well. Note that the return values for these functions are mentioned in `recurMutex.cpp`.

* `RecursiveLock()`
  - This is the constructor of the recursive mutex class that you shall implement.
    Make sure to initialize any class-level variables you defined in `recurMutex.h`. Note that
    all methods (apart from constructor and destructor) must be thread-safe. As a result,
    you will likely need to have some synchronization primitives to enable this. Do not
    forget to initialize them as well. (HINT: declare these primitives as class-level variables)

* `~RecursiveLock()`
  - This is the destructor of the recursive mutex class that you shall implement.
    Make sure to free up resources that your class-level variables take up. Do not
    forget to free the resources that your synchronization primitives take up as well.

* `int get_acqui_count()`
  - This function returns the number of times the recursive lock has been currently acquired.
    Note that it is not necessary that the calling thread is the owner of the recursive mutex.

* `int get_lock_owner()`
  - This function returns the ID of the thread that has currently acquired the recursive mutex.
    Note that it is not necessary that the calling thread is the owner of the recursive mutex.

* `int recur_mutex_try_lock()`
  - Synonymous with `pthread_mutex_trylock()` function but modified to reflect the 
    behavior for recursive mutexes. You will need to check whether the mutex can be acquired.
    Similar to the corresponding library function, this is a non-blocking function and must
    return immediately. It should not put the calling thread to sleep/suspend execution.

* `int recur_mutex_lock()`
  - Synonymous with `pthread_mutex_lock()` function but modified to reflect the 
    behavior for recursive mutexes. You will need to check whether the mutex can be acquired.
    Similar to the corresponding library function, this is a blocking function and must
    only return after the calling thread can acquire the recursive mutex. (HINT: utilize 
    condition variable to simulate the blocking nature)

* `int recur_mutex_unlock()`
  - Synonymous with `pthread_mutex_unlock()` function but modified to reflect the 
    behavior for recursive mutexes. You will need to check whether the mutex can be unlocked.
    Similar to the corresponding library function, be sure to notify waiting threads (by call 
    to `recur_mutex_lock`) if the recursive lock was unlocked fully permitting other threads 
    to acquire it. (HINT: utilize condition variable to simulate the notifying feature). 

Note: Mutexes and condition variables without any special attributes are the only
synchronization primitives permitted. Make sure that you do *not* use C++ lock/condition 
variable classes such as `recursive_mutex`.<br>
<br>

# Part 2: CONDITION VARIABLES

Note: Lock and mutex are used interchangeably here to mean mutex. We are not discussing
semaphores here in any capacity.<br>
<br>

## 1] Recursive Lock implementation dependencies

You would have noticed that we used condition variables to implement the blocking 
mechanism for recursive mutexes. We already knew what a default mutex implementation 
looks like, which made using it justified to implement recursive locks. However, this may
not be the case for condition variables. What we need to show now is that condition variables
can be implemented without using recursive mutexes in order to avoid circular dependencies.
This is exactly what this section of the project relates to.<br>
<br>

## 2] Condition Variables

As we have looked at in class, condition variables come into play when we wish to
have some ordering of threads depending on some condition. The most common example
was that of the bounded buffer where threads had to wait to either read from a buffer
(empty check) or wait to fill the buffer (full check). Another instance was for
using it to implement `pthread_join`.

Since condition variables involve sleeping and waking up threads, signals are one of 
the most logical options one may choose to implement their behavior. This is what
we shall be sticking to for this part of the project.<br>
<br>

## 3] The TODOs

The main task is to implement the functions mentioned below in `condVar.cpp`. To successfully complete it,
however, you will have to make some additions (not changes) to the `condVar.h` header
file as well. Note that the return values for these functions are mentioned in `condVar.cpp`.

* `ConditionVariable()`
  - This is the constructor of the condition variable class that you shall implement.
    Make sure to initialize any class-level variables you defined in `condVar.h`. Note that
    all methods (apart from the constructor and destructor) must be thread-safe. As a result,
    you will likely need to have some synchronization primitives to enable this. Do not
    forget to initialize them as well. (HINT: declare these primitives as class-level variables).

* `~ConditionVariable()`
  - This is the destructor of the condition variable class that you shall implement.
    Make sure to free up resources that your class-level variables take up. Do not
    forget to free the resources that your synchronization primitives take up as well.

* `int sleepingThreadCount()`
  - This function should return the number of threads that are currently waiting on the 
    condition variable.

* `bool isSleeping(pthread_t tid)`
  - This function checks to see whether the thread with ID `tid` is one of the sleeping threads
    at the time of the call. Having no threads sleeping should not be treated as a special case - 
    regard it as a case of failure (i.e., return `false`).

* `int cond_var_signal()`
  - Synonymous with `pthread_cond_signal()`. Send the `SIGUSR1` signal to any one of the 
    threads sleeping/waiting on the condition variable.

* `int cond_var_broadcast()`
  - Synonymous with `pthread_cond_broadcast()`. Send the `SIGUSR1` signal to all of the 
    threads sleeping/waiting on the condition variable.

* `void cond_var_wait(pthread_mutex_t* mutex)`
  - Synonymous with `pthread_cond_wait()`. The calling thread must suspend execution/sleep
    until it is sent the `SIGUSR1` signal through a call to the signal or broadcast functions.
    Just like the actual function, you must follow the process of releasing and reacquiring
    the mutex passed in as the argument. You may assume that the calling thread has already
    acquired the mutex at the start of the function call. In conjunction with the second 
    paragraph of the man page for `pthread_cond_wait()`, ensure that after the lock has been 
    released, a call to `cond_var_broadcast()` will wake it up (and that a call to `signal`
    could wake it up). Note that you must *not* ignore, block, or change signal dispositions 
    for any signal apart from `SIGUSR1`. Also note that after returning from this function,
    sending a `SIGUSR1` signal to this process should terminate the process (default action).
