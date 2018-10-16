FROM golang:1.9 as builder
WORKDIR /go/src/demo
COPY main.go .
RUN CGO_ENABLED=0 go build -o demoapp main.go

FROM alpine:3.6 
COPY --from=builder /go/src/demo/demoapp /bin/
COPY html /bin/html
EXPOSE 8080
WORKDIR /bin/
CMD ["./demoapp"]