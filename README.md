# Final Project NET UCSP

Academic project made in C++, which goal is to use slaves to manage data files

# Commands

You have to create the directory "data" to save slaves data there.

```
mkdir data
```

## Build server

```
g++ server.cpp -pthread -o server
```

## Build slave

```
g++ slave.cpp -o slave
```

## Build client

```
g++ client.cpp -o client
```

## Run server

```
./server
```


## Run slave
Create exactly 2 slaves

```
./slave
```

## Run client

```
./client
```
