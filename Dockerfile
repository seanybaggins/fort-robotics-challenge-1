FROM debian:11.0

RUN apt-get update && apt-get install -y \
    ruby \
    gcc
RUN gem install ceedling --version 0.31.1
# Setting the runtime of the docker container that will ultimately be run to be
# non-root
ARG UNAME=testuser
ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o $UNAME
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME
USER $UNAME
WORKDIR /project

