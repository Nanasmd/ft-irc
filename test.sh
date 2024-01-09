#!/bin/bash

# Check if necessary parameters are provided
if [ "$#" -ne 3 ]; then
  echo "Usage: $0 <port> <password>"
  exit 1
fi

port=$1
password=$2
bonus=$3

# Launch the IRC server
# opens a new GNOME terminal and runs the IRC server (ircserv)
# with valgrind for memory leak checking, using the provided port and password.
gnome-terminal -- bash -c "valgrind --track-fds=yes ./ircserv $port $password; exec bash"

# Wait 2 sec to ensure the server starts properly
sleep 2

# Launch two instances of irssi clients, 
# each in a separate terminal window, connecting to the server
for i in {1..2}; do
  gnome-terminal -- bash -c "irssi -c localhost -p $port -w $password; exec bash"
  sleep 1
done
sleep 1
# Launch bot
if [ "$bonus" == "yes" ]; then
  gnome-terminal -- bash -c "cd ./bot && valgrind --track-fds=yes ./banbot; exec bash"
fi