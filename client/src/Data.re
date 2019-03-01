let data: Js.Json.t =
  Json.parseOrRaise(
    {|[{
        "config": {
            "baseId": "base",
            "resourceId": "properties",
            "aliases": {
            }
          },
          "p-instance-of": {
            "p-name": "Instance of"
          },
          "p-name": {
            "p-name": "Name"
          },
          "p-description": {
            "p-name": "Description"
          },
          "p-url": {
            "p-name": "Web URL"
          },
          "n-organization": {
            "p-name": "Organization"
          },
          "n-movie": {
            "p-name": "Movie"
          }
    },
    {
        "config": {
            "baseId": "foretold",
            "resourceId": "main",
            "aliases": {
              "name": "@base/properties/p-name",
              "description": "@base/properties/p-description",
              "url": "@base/properties/p-url"
            }
          },
          "n-fhi": {
            "name": "FHI"
          },
          "n-foretold": {
            "name": "Foretold",
            "description": "This website, to hold predictions",
            "url": "foretold.io"
          },
          "n-ozzie-gooen": {
            "name": "Ozzie Gooen"
          },
          "n-jacob-lagerros": {
            "name": "Jacob Lagerros"
          }
    },
    {
        "config": {
            "baseId": "media",
            "resourceId": "movies",
            "aliases": {
              "name": "@base/properties/p-name",
              "description": "@base/properties/p-description",
              "instance-of": "@base/properties/p-instance-of",
              "url": "@base/properties/p-url",
              "movie": "@base/properties/n-movie"
            }
          },
          "captain-marvel": {
            "name": "Captain Marvel",
            "url": "https://www.rottentomatoes.com/m/captain_marvel",
            "instance-of": "movie"
          },
          "dumbo": {
            "name": "Dumbo",
            "url": "https://www.rottentomatoes.com/m/dumbo",
            "instance-of": "movie"
          },
          "avengers-endgame": {
            "name": "Avengers: Endgame",
            "url": "https://www.rottentomatoes.com/m/avengers_endgame",
            "instance-of": "movie"
          },
          "john-wick-three": {
            "name": "John Wick: Chapter Three",
            "url": "https://www.rottentomatoes.com/m/avengers_endgame",
            "instance-of": "movie"
          },
          "detective-pikachu": {
            "name": "Detective Pikachu",
            "url": "https://www.rottentomatoes.com/m/pokemon_detective-pikachu",
            "instance-of": "movie"
          },
          "aladdin": {
            "name": "Aladdin",
            "url": "https://www.rottentomatoes.com/m/aladdin",
            "instance-of": "movie"
          }
    },
    {
        "config": {
            "baseId": "lesswrong",
            "resourceId": "ozziegooen",
            "aliases": {
              "name": "@base/properties/p-name",
              "description": "@base/properties/p-description",
              "url": "@base/properties/p-url"
            }
          },
          "prediction-pyramid": {
            "name": "The Prediction Pyramid",
            "description": "The Prediction Pyramid: Why Fundamental Work is Needed for Prediction Work",
            "url": "https://www.lesswrong.com/posts/zuAj86jcB5JB4dgqm/the-prediction-pyramid-why-fundamental-work-is-needed-for"
          },
          "rain": {
            "name": "RAIN Framework!",
            "description": "The RAIN Framework for Informational Effectiveness",
            "url": "https://www.lesswrong.com/posts/s4TrCbCXvvWfkT2o6/the-rain-framework-for-informational-effectiveness"
          }
    }
]|},
  );

let make = Interface.Graph.fromJson(data);

let run = () => Js.log(make |> Graph_T.F.thingArray);