open Jest;

let testData = [
  ("g-0", "n-george", "p-name", "George"),
  ("g-1", "n-george", "p-description", "The person named Goerge"),
  ("1", "n-cindy", "p-name", "Cindy"),
  ("2", "p-name", "p-name", "Name of Item"),
  ("3", "p-name", "p-description", "The name of something"),
  ("4", "2", "p-name", "The 2nd fact"),
];

/* let graph = Graph.build(testData);
   let item = id => graph |> Graph.findThing(id) |> Option.toExn("");

   /* Js.log(
        graph |> things |> List.map(Base.Thing.to_s) |> String.concat(","),
      ); */

   describe("#isSubjectForFacts", () => {
     test("finds correct facts for George", () =>
       expect(item("n-george") |> isSubjectForFacts |> List.length)
       |> toEqual(2)
     );
     test("finds correct facts for fact 2", () =>
       expect(item("2") |> isSubjectForFacts |> List.length) |> toEqual(1)
     );
   });

   describe("#connectedPropertyThings", () =>
     test("finds correct properties for George", () =>
       expect(
         item("n-george") |> connectedPropertyThings |> List.map(Thing.id),
       )
       |> toEqual(["p-name", "p-description"])
     )
   );

   describe("#connectedPropertyWithId", () =>
     test("finds correct property for name of George", () =>
       expect(
         item("n-george")
         |> connectedPropertyWithId("p-name")
         |> Option.toExn("")
         |> Thing.id,
       )
       |> toEqual("p-name")
     )
   );

   describe("#propertyValues", () =>
     test("finds correct name for George", () =>
       expect(item("n-george") |> propertyValues("p-name"))
       |> toEqual([String("George")])
     )
   ); */