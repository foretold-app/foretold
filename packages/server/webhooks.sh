#!/usr/bin/env bash
# https://github.com/foretold-app/ken-data/tree/master
#curl -u "RomanGalochkin" -i \
#  https://api.github.com/hub \
#  -F "hub.mode=subscribe" \
#  -F "hub.topic=https://github.com/foretold-app/ken-data/events/push" \
#  -F "hub.callback=http://dev.wirnex.com:31000/hooks"
#
#curl -i \
#  https://api.github.com/hub \
#  -F "hub.mode=subscribe" \
#  -F "hub.topic=https://github.com/foretold-app/ken-data/events/push" \
#  -F "hub.callback=http://dev.wirnex.com:31000/hooks"
# 1aebe2348e3736e277e437be40c4d38ba9bf7643
#curl -H "Authorization: bearer 1aebe2348e3736e277e437be40c4d38ba9bf7643" https://api.github.com/graphql

token="1aebe2348e3736e277e437be40c4d38ba9bf7643"

script='query {
  repositoryOwner(login:\"RomanGalochkin\") {
    repositories(first: 100) {
      edges {
        node {
          nameWithOwner
          pullRequests(last: 100, states: OPEN) {
            edges {
              node {
                title
                url
                author {
                  login
                }
                labels(first: 20) {
                  edges {
                    node {
                      name
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}'
script="$(echo $script)"

curl -i -H 'Content-Type: application/json' \
   -H "Authorization: bearer ${token}" \
   -X POST -d "{ \"query\": \"$script\"}" https://api.github.com/graphql
