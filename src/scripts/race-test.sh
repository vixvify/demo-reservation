#!/bin/bash

SEAT_ID=10

echo "===================================="
echo " Airplane Reservation Race Test"
echo "===================================="
echo "Target seat: $SEAT_ID"
echo

echo "RESERVE $SEAT_ID" | ./client 1 &
echo "RESERVE $SEAT_ID" | ./client 2 &
echo "RESERVE $SEAT_ID" | ./client 3 &
echo "RESERVE $SEAT_ID" | ./client 4 &
echo "RESERVE $SEAT_ID" | ./client 5 &

wait

echo
echo "Race test finished."