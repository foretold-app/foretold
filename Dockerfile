FROM node:11.13.0-alpine

RUN env

COPY server /opt
COPY bots /opt
COPY client /opt

COPY package.json /opt
COPY package-lock.json /opt

WORKDIR /opt/app

RUN apk add --no-cache build-base python && \
RUN npm install --loglevel=warn

EXPOSE ${PORT:-80}
