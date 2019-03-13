#!/usr/bin/env bash
echo -e "TIER = ${TIER}\n"

if [ "$TIER" = "backend" ]
then
  echo -e "Backend build\n"
  npm run server/install &&
    npm run server/build &&
    npm run server/db:migrate
fi

if [ "$TIER" = "frontend" ]
then
  echo -e "Frontend build\n"
  npm run client/install &&
   npm run client/build
fi
