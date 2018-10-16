FROM golang:1.9 as builder
WORKDIR /go/src/demo
COPY main.go .
RUN CGO_ENABLED=0 go build -o demoapp-d main.go

FROM alpine:3.6 
COPY --from=builder /go/src/demo/demoapp-d /bin/
WORKDIR /bin/
CMD ["./demoapp-d"]