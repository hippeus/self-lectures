FROM ubuntu:16.10

# NOTE: official ubuntu:16.10 image is shipped as a bare minimum.
# However, for the sake of this demo, I will be relying on ip, ping and curl commands.
# So RUN command could be skipped for a production ready code.
# (BTW. ip command succeeded ifconfig which is depracated). 
RUN apt-get update && apt-get install -y iproute2 iputils-ping curl

WORKDIR /bin

COPY demoapp-d .

CMD ["./demoapp-d"]
