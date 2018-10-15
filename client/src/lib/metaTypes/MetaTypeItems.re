open MetaTypeBase;
open Rationale;

type dict = Js.Dict.t(string);

let findType = (types: list(metaType), id) : metaType =>
  Rationale.RList.find((e: metaType) => e.id == id, types)
  |> Option.toExn("Fail");

let findItem = (types, r: map, id) => {
  let ttype: metaType = findType(types, id);
  Belt.Map.get(r, ttype.id) |> Option.default(ttype.default);
};

let all = [|
  makeMeasurableType(
    ~id="1",
    ~name="Generic",
    ~description="Generic",
    ~schema=[
      makeMetaType(~t=`String, ~id="name", ~name="Name", ~default="", ()),
      makeMetaType(
        ~t=`String,
        ~id="description",
        ~name="Description",
        ~default="",
        (),
      ),
    ],
    ~nameFn=(schema, r: map) => findItem(schema, r, "name"),
    ~descriptionFn=(schema, r: map) => findItem(schema, r, "description"),
    (),
  ),
  makeMeasurableType(
    ~id="2",
    ~name="TotalRepoCommits",
    ~description="The total commits for a repo at a specific time.",
    ~schema=[
      makeMetaType(
        ~t=`String,
        ~id="byDate",
        ~name="By Date",
        ~default="",
        (),
      ),
      makeMetaType(~t=`String, ~id="repo", ~name="Repo", ~default="", ()),
    ],
    ~nameFn=
      (schema, r: map) => {
        let toName = (byDate, repo) => {j|Total Commits for $repo at $byDate|j};
        toName(findItem(schema, r, "byDate"), findItem(schema, r, "repo"));
      },
    (),
  ),
|];
/* let toName = (m: map) => {
   (); */

/* module TotalRepoCommits = {
     module In = {
       let id = "2";
       let name = "TotalRepoCommits";
       let description = "The total commits for a repo at a specific time.";
       let schema = [
         {name: "byDate", t: `String, default: ""},
         {name: "repo", t: `String, default: ""},
       ];
     };
     module Out = MakeMeasurableType(In);
   }; */

/* //Type is float */
/* module HumanTaskTime = {
     module In = {
       let id = "3";
       let name = "HumanTaskTime";
       let description = "The time it takes to finish a task, in milliseconds.";
       type t = {
         task: entity,
         organization: option(entity),
         person: option(entity),
       };
       let metaTypes = [|
         ("task", `Entity),
         ("organization", `String),
         ("person", `String),
       |];
     };
     module Out = MakeMeasurableType(In);
   };

   /* //Type is dateTime */
   module ExpectedYearOfDeath = {
     module In = {
       let id = "4";
       let name = "ExpectedYearOfDeath";
       let description = "The time it takes to finish a task, in milliseconds.";
       type t = {entity};
       let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
     };
     module Out = MakeMeasurableType(In);
   };

   /* //Type is dateTime */
   module TimeOfFirstJudgement = {
     module In = {
       let id = "5";
       let name = "TimeOfFirstJudgement";
       let description = "The time it takes to finish a task, in milliseconds.";
       type t = {measurableId: string};
       let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
     };
     module Out = MakeMeasurableType(In);
   };

   module ProbabilityOfJudgement = {
     module In = {
       let id = "6";
       let name = "ProbabilityOfJudgement";
       let description = "The time it takes to finish a task, in milliseconds.";
       type t = {
         measurableId: string,
         evaluationTime: string,
       };
       let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
     };
     module Out = MakeMeasurableType(In);
   };

   module UsefulnessRubricOnMeasurement = {
     module In = {
       let id = "7";
       let name = "UsefulnessRubricOnMeasurement";
       let description = "The time it takes to finish a task, in milliseconds.";
       type t = {
         measurableId: string,
         evaluationTime: string,
       };
       let metaTypes = [|("byDate", `Date), ("repo", `Entity)|];
     };
     module Out = MakeMeasurableType(In);
   }; */

let find = (id: option(string)) =>
  all
  |> Array.to_list
  |> Rationale.RList.find((r: MetaTypeBase.measurableType) =>
       Some(r.id) == id
     );