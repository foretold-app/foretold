/* let getFromDict = (dict, (a: string, b: metaTypes)) =>
     Js.Dict.get(dict, a) |> Option.default("");

   module Id =
     Belt.Id.MakeComparable({
       type t = string;
       let cmp: (string, string) => int = Pervasives.compare;
     });

   type map = Belt.Map.t(Id.t, string, Id.identity);

   type lenss('a, 'b) = (
     string,
     metaTypes,
     'a => string,
     'b => 'a,
     ('b, 'a) => 'b,
   );

   module Generic = {
     module In = {
       let id = "1";
       let name: string = "Generic";
       let description = "General Tasks";
       type t = {
         name: string,
         description: string,
       };
       let default = {name: "", description: ""};
       let metaTypes = [|("name", `String), ("description", `String)|];
       let toString = t => t.name;
       let lens: list(lenss(string, t)) = [
         ("name", `String, e => e, r => r.name, (tt, a) => {...tt, name: a}),
         (
           "description",
           `String,
           e => e,
           r => r.description,
           (tt, a) => {...tt, description: a},
         ),
       ];
       let fromDict = dict => {
         let initial = Belt.Map.fromArray(~id=(module Id), [||]);
         Belt.Array.reduce(
           lens |> Array.of_list,
           default,
           (b, (key, _, _, _, x)) => {
             let item = Belt.Map.get(dict, key);
             switch (item) {
             | Some(e) => x(b, e)
             | None => b
             };
           },
         );
       };
       let toDict = (t: t) =>
         lens
         |> Array.of_list
         |> Array.map(((r, _, s, x, _)) => (r, t |> x |> s))
         |> Belt.Map.fromArray(~id=(module Id));
     };
     module Out = MakeMeasurableType(In);
   }; */