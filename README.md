# Task:

Write an application that when run will create a group of child processes in an
isolated PID namespace. Upon execution, the process should create its first
child in the new namespace; this process will serve as the "init" process for
the tree. Once the "init" process is running the entry process may be allowed to
terminate.

Once running, the "init" process should accept the listed signals
transmitted from the ancestor namespace, and take the following actions in
response to reception of those signals:

* SIGUSR1 - create a new child process in the PID namespace
* SIGINT - kill the most recently created child in the PID namespace
* SIGUSR2 - write a listing of the existing children and their PIDs (within the
            created namespace) to a file 'pidlist.txt'

The maximum number of children at a given time will be 10.

The children do not need to perform any particular function. Running a simple:
    for(;;){
        sleep(1);
    }
loop is adequate to keep them running.


                  ┌───────────┐
                  │           │
                  │   Entry   │
                  │  Process  │   (defunct after creation)
                  │           │
                  └─────┬─────┘
                        │
                  ┌─────▼─────┐
                  │           │
                  │   "init"  │
                  │  Process  │
                  │           │
                  └─────┬─────┘
                        │
          ┌─────────────┼──────────────┐
          │             │              │
          │             │              │
     ┌────▼────┐    ┌───▼──┐     ┌─────▼─────┐
     │         │    │      │     │           │
     │ Child 0 │    │ .... │     │  Child n  │
     │         │    │      │     │           │
     └─────────┘    └──────┘     └───────────┘


# Requirements:

* The application should be written in C or C++. You may use 3rd party libraries
  as you see fit, but for any content beyond the stdlib and STL you should
  include instructions on how to install and configure the library
  appropriately.
* Include a makefile or CMakeLists.txt to build your application.
* Write your solution as you would any production-grade code. Include comments,
  tests, etc. as you deem appropriate and necessary. Code defensively.

# Other:

It is expected that the entry process will require root privledges to execute
properly, this is a normal requirement of namespace creation.

We appriciate the effort you're expending to apply to FORT and wish to respect
your time. As such, we've tried to scope the task so that it will take about an
hour to write. Of course this will vary by experience and familiarity with the
particular subsystems invovled. If you find the task taking longer than this to
complete - please do not finish it. Complete as much as you are able and provide
rough comments as to how you would approach the balance of the problem.

If you require clarification on any portion of the task, feel free to contact
Bob Sherbert <bob@fortrobotics.com>

Good Luck & Happy Hacking!

# Sean Link's Instructions

## Dependencies
1. Install docker using either your package manager or following the instructions
for your distribution [here](https://docs.docker.com/engine/install/).
2. Test your docker installation by running the following command.
```
docker run hello-world
```
> Note: If you are unable run docker, make sure your current user is part of the
> docker group by following the instructions [here](https://docs.docker.com/engine/install/linux-postinstall/).

## Running Test
Run unit tests by running the following command.
```
make test
```

## Building final executible
Build the final binary by running the following command.
```
make build
```

## Running the final binary
Run the final binary by running the following command.
```
make run
```
> Note: The finaly binary will be run on the local machine, not a container. I
> did this in case you wanted to pass signals to the process running your own
> script.

## Final Notes
You can gracefully exit the program by using the following command.
```
sudo kill <init pid>
```
> Note: The init pid will be printed to the console at the start of the
> application.
