# Current Architecture

เอกสารนี้สรุป architecture จากโค้ดและ manifest ที่มีอยู่ใน repository ณ วันที่ 2026-09-02

## Overview

```mermaid
flowchart LR
    subgraph Pod["Kubernetes Pod: airplane-reservation\nhostIPC: true"]
        subgraph ServerContainer["server container\nairplane-reservation:latest"]
            Server["./server nosync\nserver entrypoint"]
            Workers["Worker threads\nDEFAULT_WORKER_COUNT = 3\nworker.cpp"]
            Reservation["Reservation domain\nseats[20]\nLIST / STATUS / RESERVE / CANCEL"]
            Sync["Per-seat mutexes\noptional synchronization"]
            Logger["Logger\nsequence + worker/client id"]
            Delay["Random delay\n50-500 ms"]
        end

        Queue[("System V message queue\nkey: ftok(/tmp, 'A')\nrequest mtype = 1\nresponse mtype = 1000 + clientId")]

        C1["client-1\nsleep infinity"]
        C2["client-2\nsleep infinity"]
        C3["client-3\nsleep infinity"]
        C4["client-4\nsleep infinity"]
        C5["client-5\nsleep infinity"]
    end

    Server -. "starts workers\n(entrypoint currently empty)" .-> Workers
    C1 -->|msgsnd request| Queue
    C2 -->|msgsnd request| Queue
    C3 -->|msgsnd request| Queue
    C4 -->|msgsnd request| Queue
    C5 -->|msgsnd request| Queue
    Queue -->|msgrcv request type 1| Workers
    Workers -->|processCommand| Reservation
    Reservation --> Sync
    Reservation --> Delay
    Workers --> Logger
    Workers -->|msgsnd response| Queue
    Queue -->|msgrcv type 1000 + clientId| C1
    Queue -->|msgrcv type 1000 + clientId| C2
    Queue -->|msgrcv type 1000 + clientId| C3
    Queue -->|msgrcv type 1000 + clientId| C4
    Queue -->|msgrcv type 1000 + clientId| C5

    classDef missing fill:#fff3cd,stroke:#b58105,stroke-dasharray: 5 5,color:#4d3b00;
    class Server missing;
```

## Request flow

```mermaid
sequenceDiagram
    participant Client as Client process
    participant Queue as System V message queue
    participant Worker as Worker thread
    participant Domain as Reservation state

    Client->>Queue: msgsnd(Message{mtype=1, clientId, command})
    Worker->>Queue: msgrcv(..., mtype=1)
    Worker->>Worker: Parse LIST / STATUS / RESERVE / CANCEL / QUIT
    Worker->>Domain: Execute command
    alt synchronization enabled
        Domain->>Domain: Lock affected seat mutex(es)
        Domain->>Domain: Read/update seats[20]
    else nosync mode
        Domain->>Domain: Read/update seats[20] without mutex
    end
    Worker->>Queue: msgsnd(Message{mtype=1000+clientId, response})
    Client->>Queue: msgrcv(..., mtype=1000+clientId)
```

## Components and responsibilities

| Component | Responsibility | Source |
| --- | --- | --- |
| `server` | Intended process entrypoint; Kubernetes passes `nosync` | `src/server/server.cpp`, `k8s/pod.yaml` |
| Worker pool | Consumes request messages, dispatches commands, sends responses | `src/server/worker.cpp` |
| Reservation domain | In-memory seat state, validation, reserve/cancel/status/list operations | `src/reservation/reservation.cpp` |
| Synchronization | One `std::mutex` per seat when enabled | `src/reservation/reservation.cpp` |
| Message contract | Fixed-size request/response payload and message types | `src/models/message.h`, `src/constants/constants.h` |
| Logger | Serialized console logs with sequence number | `src/utils/logger.cpp` |
| Delay | Simulates 50–500 ms operation delay | `src/utils/delay.cpp` |
| Clients | Intended command-line clients; five Kubernetes containers are provisioned | `src/client/client.cpp`, `k8s/pod.yaml` |
| Load test | Concurrently sends `STATUS` requests and measures throughput/latency | `src/load_test/load_test.cpp` |

## Current-state notes

- Seat state is in the server process memory (`seats[20]`); there is no database or persistent storage.
- All workers in the server process share the same seat array and per-seat mutexes.
- Clients communicate through the same System V queue. Responses are routed by `1000 + clientId`.
- The Kubernetes manifest creates one Pod with one server container and five client containers, and sets `hostIPC: true`.
- The manifest starts the server with `nosync`, so the intended runtime path is the unsynchronized branch.
- `src/server/server.cpp` and `src/client/client.cpp` are currently empty. The worker/domain code exists, but the executable entrypoints are not implemented yet.
- `Dockerfile` copies `server.cpp` and `client.cpp` from the repository root, while the files are under `src/server/` and `src/client/`; the image build is therefore not aligned with the current source layout.
- `Makefile` builds `server` from the worker, reservation, logger, and delay modules, and builds `client` from `src/client/client.cpp`. It does not currently build `src/load_test/load_test.cpp`.

## Key constants

- 20 seats
- 3 default worker threads
- Request message type `1`
- Response message type `1000 + clientId`
- Queue key source `/tmp` with project id `'A'`
- Simulated delay between 50 and 500 ms
