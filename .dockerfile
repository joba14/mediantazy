
FROM ubuntu:24.04

RUN apt update -y
RUN apt upgrade -y
RUN apt install git -y
RUN apt install gcc -y
RUN apt install cppcheck -y
RUN apt install doxygen -y
RUN apt install python3 -y
