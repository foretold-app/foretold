FROM node:12.2.0

RUN env

COPY package.json /opt/app/
COPY package-lock.json /opt/app/
COPY yarn.lock /opt/app/
COPY lerna.json /opt/app/
COPY packages /opt/app/packages/
WORKDIR /opt/app

# Temporary solution while the client has building issues
RUN npm install --loglevel=warn --unsafe-perm --ignore-scripts
RUN npm run bootstrap
# Then leave only this command
#RUN npm install --loglevel=warn --unsafe-perm

EXPOSE ${PORT:-80}
