[@bs.config {jsx: 3}];

open FC.Base;

let multimodal = "=mm(uniform(0,100), uniform(10,20), [.33,0.9])";
let mm1 = "=mm(uniform(1,100), normal(50, 5), [.01, .99])";
let mm2 = "=mm(uniform(1,100), normal(50, 8), [.01, .99])";

module Scoring = {
  type dist = FC__Types.Dist.t;

  [@react.component]
  let make = () => {
    let (varA, setVarA) = React.useState(() => None);
    let (varB, setVarB) = React.useState(() => None);
    let (varC, setVarC) = React.useState(() => None);

    let distributionScoreDistribution =
      switch (varA, varB, varC) {
      | (Some(a), Some(b), Some(c)) =>
        FC__Types.Dist.distributionScoreDistribution([|a, b, c|])
      | _ => None
      };

    let distributionScoreNumber =
      switch (varA, varB, varC) {
      | (Some(a), Some(b), Some(c)) =>
        Some(FC__Types.Dist.distributionScoreNumber([|a, b, c|]))
      | _ => None
      };

    let minX =
      [|varA, varB, varC, distributionScoreDistribution|]
      |> E.A.O.concatSome
      |> FC__Types.Dists.minX(0.01);
    let maxX =
      [|varA, varB, varC, distributionScoreDistribution|]
      |> E.A.O.concatSome
      |> FC__Types.Dists.maxX(0.99);
    Js.log2("MIN", min);
    <div>
      <h3> {"Variable A" |> ReasonReact.string} </h3>
      <FC_GuesstimateInput
        focusOnRender=true
        initialValue={Some(mm1)}
        sampleCount=50000
        onUpdate={event =>
          {let (ys, xs, hasLimitError) = event
           setVarA(_ => FC__Types.Dist.requireLength({ys, xs}))
           Js.log2(xs, ys)}
          |> ignore
        }
      />
      {varA
       |> E.O.React.fmapOrNull(v =>
            <FC__CdfChart__Large cdf=v minX maxX width=None />
          )}
      <h3> {"Variable B" |> ReasonReact.string} </h3>
      <FC_GuesstimateInput
        focusOnRender=true
        sampleCount=50000
        initialValue={Some(mm2)}
        onUpdate={event =>
          {let (ys, xs, hasLimitError) = event
           setVarB(_ => FC__Types.Dist.requireLength({ys, xs}))}
          |> ignore
        }
      />
      {varB
       |> E.O.React.fmapOrNull(v =>
            <FC__CdfChart__Large minX maxX cdf=v width=None />
          )}
      <h3> {"Variable C" |> ReasonReact.string} </h3>
      <FC_GuesstimateInput
        focusOnRender=true
        sampleCount=50000
        initialValue={Some("20 to 60")}
        onUpdate={event =>
          {let (ys, xs, hasLimitError) = event
           setVarC(_ => FC__Types.Dist.requireLength({ys, xs}))}
          |> ignore
        }
      />
      {varC
       |> E.O.React.fmapOrNull(v =>
            <FC__CdfChart__Large minX maxX cdf=v width=None />
          )}
      <h3> {"C * Log2(A / B)" |> ReasonReact.string} </h3>
      {switch (distributionScoreDistribution) {
       | None => ReasonReact.null
       | Some(divideBy) =>
         <FC__CdfChart__Large minX maxX cdf=divideBy width=None />
       }}
      <h3> {"Final Score" |> ReasonReact.string} </h3>
      {switch (distributionScoreNumber) {
       | None => ReasonReact.null
       | Some(scoreNumber) =>
         scoreNumber |> FC__E.Float.with3DigitsPrecision |> ReasonReact.string
       }}
    </div>;
  };
};

let entry = EntryTypes.(entry(~title="Scoring", ~render=() => <Scoring />));
