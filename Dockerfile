FROM ubuntu:24.04
LABEL Description="Build environment"

ENV HOME=/root

SHELL ["/bin/bash", "-c"]

RUN apt update && apt -y --no-install-recommends install \
    build-essential time git cmake gdb flex bison valgrind \
    libantlr4-runtime-dev wget ca-certificates default-jre-headless && \
    wget https://github.com/antlr/website-antlr4/raw/gh-pages/download/antlr-4.10.1-complete.jar -O /usr/local/lib/antlr-4.10.1-complete.jar && \
    echo -e "#!/bin/sh\njava -jar /usr/local/lib/antlr-4.10.1-complete.jar \"\$@\"" > /usr/bin/antlr4 && \
    chmod +x /usr/bin/antlr4
