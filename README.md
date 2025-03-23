Here is the translated README in markdown format (`README.md`):

```markdown
# Real-Time Systems Repository

This repository contains implementations of labs for the Real-Time Systems course, covering concepts of concurrency, inter-thread communication, and fault tolerance.

## Repository Structure

- **LAB 1**: Concurrency implementations using tasks in Ada and threads in C.
  - `main.adb`: Implements synchronization between Ada tasks using control flags.
  - `main.c`: Equivalent implementation in C using pthreads.

- **LAB 2**: Implementation of inter-process communication via a shared channel.
  - `main.c`: Uses a channel to send and receive messages asynchronously, implementing a voting mechanism and fault tolerance.

- **LAB 3**: Implementation of a Mailbox to manage communication between threads.
  - `main.c`: Uses threads to simulate an asynchronous communication system between processes, ensuring message delivery through an intermediate Mailbox.

## How to Run the Code

The codes were executed on the following online platforms: onlinegdb.com for C and onecompiler.com/ada for Ada.


## Author
Repository created for the Real-Time Systems course.
