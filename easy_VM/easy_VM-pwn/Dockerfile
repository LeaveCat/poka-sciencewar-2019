FROM ubuntu:18.04
  
RUN sed -i -re 's/([a-z]{2}.)?archive.ubuntu.com|security.ubuntu.com/mirror.kakao.com/g' /etc/apt/sources.list && \
    apt-get update && apt-get -y dist-upgrade && \
    apt-get install -y lib32z1 xinetd python3 gcc socat netcat

RUN useradd -m easyvm
WORKDIR /home/easyvm

COPY ./vm /home/easyvm/
COPY ./wrapper.py /home/easyvm/
COPY ./flag /flag_f831a69b0d8304c164fcac426ab5d811

RUN chown -R root:easyvm /home/easyvm && \
    chmod -R 750 /home/easyvm && \
    chmod 744 /flag_f831a69b0d8304c164fcac426ab5d811 && \
    chmod +x /home/easyvm/wrapper.py


USER easyvm
EXPOSE 8888
RUN ls -al /home/easyvm
RUN ls -al /
ENTRYPOINT ["socat", "TCP-LISTEN:8888,reuseaddr,fork","exec:./wrapper.py"]
