FROM node:12.6.0

RUN env

COPY package-travis.json.json /opt/app/package.json
COPY yarn.lock /opt/app/
COPY lerna.json /opt/app/
COPY ws-fixer.sh /opt/app/
COPY packages /opt/app/packages/
WORKDIR /opt/app

RUN yarn install --loglevel=warn --unsafe-perm --ignore-scripts
RUN yarn bootstrap
RUN yarn packages/build

EXPOSE ${PORT:-80}
