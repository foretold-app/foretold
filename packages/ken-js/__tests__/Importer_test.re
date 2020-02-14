open Jest;

open Expect;

/*
 TODO:
 - Convert from String to thingId
 - Random Local Ids
 - Templates: do natively!
 - https://github.com/glennsl/bucklescript-cookbook/blob/master/README.md
 */

/* let toGraph = (things: array(UnprocessedGraph.thing)) => {
     let valueToValues = (value: UnprocessedGraph.value) =>
       switch (value) {
       | String(s) => [|s|]
       | Array(r) => r
       };

     let things =
       things
       |> Array.to_list
       |> List.map((thing: UnprocessedGraph.thing) =>
            thing.facts
            |> Array.map((fact: UnprocessedGraph.fact) =>
                 fact.value
                 |> valueToValues
                 |> Array.map((value: string) =>
                      (
                        {
                          thingIdString:
                            fact.id
                            |> Rationale.Option.default(
                                 SecureRandomString.genSync(
                                   ~length=8,
                                   ~alphaNumeric=true,
                                   (),
                                 ),
                               ),
                          subjectId: thing.id,
                          propertyId: fact.property,
                          value: Base.String(value),
                          idIsPublic:
                            fact.id |> Rationale.Option.isSome ? true : false,
                        }: Base.fact
                      )
                    )
               )
          )
       |> Array.of_list
       |> Belt.Array.concatMany
       |> Belt.Array.concatMany
       |> Array.to_list;

     Graph.from_facts(things);
   }; */

let value =
  Json.parseOrRaise(
    {|
      [
        {
        "baseId": "base12",
        "resourceId": "2/1",
        "n-fred": {
          "p-name": "Fred",
          "p-test": ["sdf", "sdfsdf", "sdfsdf"],
          "p-description": {"id": "sdf", "value": "sdffsd"}
        }
      },
      {
        "baseId": "base12",
        "resourceId": "2/2",
        "n-george": {
          "p-name": "George",
          "p-friend": "n-jeremy",
          "p-test": ["sdf", "sdfsdf", "sdfsdf"],
          "p-description": {"id": "sdf", "value": "sdffsd"}
        },
        "n-jeremy": {
          "p-name": "George",
          "p-test": ["sdf", "sdfsdf", "sdfsdf"],
          "p-description": {"id": "sdf", "value": "sdffsd"}
        }
      }]
   |},
  );

/* describe("#to_json", () =>
     test("works", () => {
       let foo = value |> JsonToUnprocessed.run |> toGraph |> Graph.to_json;
       Js.log(foo);
       expect(true) |> toEqual(true);
     })
   ); */

/*
 let value =
   Json.parseOrRaise(
     {|
       [
         {
         "baseId": "test123",
         "path": "foo/bar",
         "aliases": [{"base/10/p/p-name": "p-name"}]]
         "n-fred": {
           "p-name": "Fred",
           "p-test": ["sdf", "sdfsdf", "sdfsdf"],
           "p-description": {"id": "sdf", "value": "sdffsd"}
         }
       },
    |},
   ); */

/* bases get facts & things, nothing else! */
/* No imports or any of that now. */
/* There are both files and folders.
   However, there could be a file for any folder, which would be assumed to reference it.*/
/*
 let value3 =
   Json.parseOrRaise(
     {|
         [
           {
           "base: "sdf",
           "directory: "sdffsd",
           "id": "",
           "subject": "n-fred",
           "property": "base/l0/p/p-name",
           "value": {"type": "string"}
         },
     |},
   );
 let value2 =
   Json.parseOrRaise(
     {|
         [
           {
           "id": "@test123/foo/bar/_facts/sd8f.f",
           "subject": "test123/foo/bar/n-fred",
           "property": "base/l0/p/p-name",
           "value": {"type": "string", "id": "@t.test123/foo/bar/_values/sd8f"}
         },
     |},
   ); */