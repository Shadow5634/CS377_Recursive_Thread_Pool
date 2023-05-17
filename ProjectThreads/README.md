
  // 1 - check to see if same or different thread is trying to acquire lock
  // 2.1 - if different thread tries to lock but cannot then return -1 to signal unable to acquire lock
  // 2.2 - if same thread tries to lock then let it 'lock' and store info that thread has acquired lock n+1 times

  // 1 - check to see if the thread trying to unlock even has the lock (note that there are 2 parts to this)
  // 2.1 - if it does not have the lock return -1
  // 2.2 - if it does have the lock, release it and store infor that thread acquired lock n-1 time
  // note that this is also a non-blocking call

    // 1 - check to see if same or different thread is trying to acquire lock
  // 2.1 - if different thread tries to lock then put it waiting on a condition variable
  // 2.2 - if same thread tries to lock then let it 'lock' and store info that thread has acquired lock n+1 times
  // note that this is a blocking call 

    // might want to have checking to ensure that mutex has been acquired and locked
  // put to sleep using pause and then reacquire lock before returning

  // do you want to add to sleeping array before or after releasing lock
  // do you want to remove from sleeping array before or after releasing lock - BEFORE
  // The above two should not matter since mutex does not relate to any globals in this class
  // might want to prioritize making lock free first to allow more concurrency

  // might want to use sigwaitinfo and sigpromask to deal with signal scheduling