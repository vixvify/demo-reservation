FROM gcc:14-bookworm

WORKDIR /app

COPY server.cpp client.cpp Makefile ./

RUN make

CMD ["sleep", "infinity"]