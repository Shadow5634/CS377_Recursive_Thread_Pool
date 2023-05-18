# Project Journey

**Link to slides**: https://1drv.ms/p/s!AkFfD6OjEbCghIATpQdXIXmayC4FUQ?e=efsbkp \
**Link to video recording**: https://drive.google.com/drive/folders/1C2UDZzjvFLNzbPP7zJKspsKDu_HazD9B?usp=share_link

The first recording is the one with the slides

This readme just covers the journey of the project.
To know specific details about implementation watch the video (link above)
go through the README inside the ProjectThreads folder and look at the code in that 
folder.<br>
<br>

## 1] THE STORY

Coming up with a project topic was probably one of the first hurdles for this final project.
My initial ideas was to make the scheduling algorithms dynamic to surpass the assumption that
we knew the duration of processes. I was planning on using either signals or observables 
(a concept covered in CS 220) for this but quickly realized that this would take considerable time,
time which I did not have. \
\
Since I have already been a UCA for 2 semesters, I have come to realize my love for teaching
and thus another idea I had at the back of my mind was was coming up with a new project for 377.
However, choosing what to implement was tough as well. At first, I was thinking of making a MLFQ project,
but shot that down since it did not seem too original and wouldn't have made me learn much. After
some back and forth i settled on doing an implementation project (that's what the bulk of CS 377
seems to be). But to make sure it pushed me, I chose to do a project where students would have to 
implement recursive locks and condition variables. We never really dealt much with the implementation
of these concepts (former not even being covered) and thought that this would be something interesting.<br>
<br>

## 2] THE CHALLENGES

Making a new project was quite some work. Luckily i had the template of the 377 projects we have done
and thus making the makefile wasn't bad at all. I have chosen to put both these implementations into 
one class where each object simulates one synchronization primitive. I did not want to complicate 
the project further by having students implement bounded buffers as well since that would get 
too tough according to me - making a doable project is also of the requirements that I had to meet.\
\
Implementing recursive mutexes was interesting. My plan was to only use the normal mutex to implement 
thus but things did not go as I wished. While implementing the lock() function I realized that I would 
need some spin waiting or condition variables to pause and wait for the lock to be free. This was when 
I had the idea to have another aspect to my project of having students implement condition variables 
on top of recursive mutexes.

Another problem I ran into was pthread_cond_wait for recursive mutexes. Unlocking a recursive
mutexes just unlocks it by one level which does not free the lock always. In the end, from 
whatever reading I did online it appeared that the pthread_cond_wait function should not be 
used with recursive mutexes. Given time constraints I chose to stick with this and not make 
my own pthread_cond_wait for recursive mutexes.\
\
Condition variable implementation had its own hurdles. I new that I had to use some form of sleep
that would get woken up during a call to broadcast or signal but I was unsure as to how to go about that.
After some research I realized that signals was going to be the way to go. But signals are asynchronous
in the sense that they can arrive at any time and thus i had to see how I would manage this. Signal handlers
and blocking/unblocking signals took me quite some time to understand. A signal to a thread can affect the 
entire process (such as SIGKILL) and thus i had to be careful - SIGUSR1 (a user reserved signal) 
seemed to be the correct approach.\
\
I also chose the make the project more subjective by having students themselves define
class level variables. I feel that designing a project from scratch really forces students
to think about important concepts such as performance and expandibility. Writing the tests was also 
interesting. I was never familiar with the testing framework and always wanted to get used to it.
This enabled me to do this and I realized that it was not tough at all.<br>
<br>

## 3] Conclusion

Doing this project broadened my horizons and I came to appreciate the course staff 
to see the effort they have to put, not only in building projects, but also in organizing
the class in general.<br>
<br>

*Please reach out to me at nkapasi@umass.edu for any questions you may have.*
