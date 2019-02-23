let data: Js.Json.t =
  Json.parseOrRaise(
    {|[{
        "config": {
            "baseId": "base",
            "resourceId": "data-types",
            "aliases": {
              "name": "@base/properties/p-name",
              "instance-of": "@base/properties/p-instance-of",
              "subclass-of": "@base/properties/p-subclass-of"
            }
          },
          "d-thing": {
            "name": "Thing (Data Type)",
            "instance-of": "p-data-type"
          }
    }
]|},
  );

let make = Interface.Graph.fromJson(data);

let run = () => Js.log(make |> Graph_T.F.thingArray);