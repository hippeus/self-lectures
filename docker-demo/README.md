# Commands used for this demo 

## Stage 0: Applications build locally

### Build demoapp and demoapp-d locally using Makefiles and present their execution

> cd server && make && ./demoapp

> cd clientd && make && ./demoapp-d

## Stage 1: Let's make things deployable

Dokerize both applications by creating Dockerfiles and images (remember about having binaries already compiled)

> docker image build -t macpla/srv .

> docker container run --rm --name webapp --net=bridge -p 80:8080 macpla/srv

> docker image build -t macpla/clt .

> docker container run --rm --name appd macpla/clt

## Stage 2: Database

Append and initialize database, by creating database container

> docker pull postgres`

> docker container run --rm --name app-db -e POSTGRES_USER=demo -e POSTGRES_PASSWORD=demo123 -e POSTRGRES_DB=demoapp postgres

> docker container logs app-db

<!---
 docker container run --rm -d --name app-db -p 5000:5432 -v demo-storage:/var/lib/postgresql/data -e POSTGRES_USER=demo -e POSTGRES_PASSWORD=demo123 -e POSTRGRES_DB=demoapp postgres
-->

## Stage 3: Networking

Create private networks

> docker network create frontend

> docker network create backend

> docker network inspect frontend -f='{{json .IPAM.Config}}'

> docker network inspect backend -f='{{json .IPAM.Config}}'

## Stage 4: Set-up all together

> docker container run --rm -d --name appd --net=frontend macpla/clt

> docker container run --rm -d --name webapp --net=frontend macpla/srv

> docker container run --rm --name app-db -d --network=backend -e POSTGRES_USER=demo -e POSTGRES_PASSWORD=demo123 -e POSTRGRES_DB=demoapp postgres

> docker network connect backend webapp`

## Stage 5: Collect results

### Exam solution by checking available network interfaces inside containers

> docker container exec -it app-db /bin/bash

> docker container exec -it webapp /bin/sh

> docker container exec -it appd /bin/bash

## Stage 6: Let's make things portable

> docker container rm -f webapp appd app-db

> docker network rm frontend backend

> docker image build -f self-contained.Dockerfile -t macpla/srv2 .

> docker container run --rm -p 80:8080 macpla/srv2 .

> docker image build -f self-contained.Dockerfile -t macpla/clt2 .

> docker container run --rm --name clt macpla/clt2 .

## Stage 7: Automated process

> docker-compose up -d

> docker-compose down