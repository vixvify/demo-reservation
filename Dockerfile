FROM gcc:14-bookworm

WORKDIR /app
COPY server.c client.c Makefile ./
RUN make

CMD ["bash"]
