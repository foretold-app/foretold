let data: Js.Json.t =
  Json.parseOrRaise(
    {|[{
        "config": {
            "baseId": "base",
            "resourceId": "properties",
            "aliases": {
              "subclass-of": "@base/properties/p-subclass-of"
            }
          },
          "p-instance-of": {
            "p-name": "Instance of"
          },
          "p-name": {
            "p-name": "Name",
            "p-instance-of": "p-data-type"
          },
          "d-thing": {
            "p-name": "Thing (Data Type)",
            "p-instance-of": "p-data-type"
          },
          "n-organization": {
            "p-name": "Organization"
          },
          "n-fhi": {
            "p-name": "FHI",
            "p-instance-of": "n-organization"
          }
    }
]|},
  );

let make = Interface.Graph.fromJson(data);

let run = () => Js.log(make |> Graph_T.F.thingArray);