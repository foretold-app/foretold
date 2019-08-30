open FC.Base;

let multimodal = "=mm(uniform(0,100), uniform(10,20), [.33,0.9])";
let mm1 = "=mm(uniform(1,100), normal(50, 5), [.01, .99])";
let mm2 = "=mm(uniform(1,100), normal(50, 8), [.01, .99])";

module Scoring = {
  type dist = FC__Types.Dist.t;

  type state = {
    varA: option(dist),
    varB: option(dist),
    varC: option(dist),
  };

  type action =
    | ChangeA(option(dist))
    | ChangeB(option(dist))
    | ChangeC(option(dist));

  let component = ReasonReact.reducerComponent("Example");

  let make = _children => {
    ...component,

    initialState: () => {varA: None, varB: None, varC: None},

    reducer: (action, state) =>
      switch (action) {
      | ChangeA(dist) => ReasonReact.Update({...state, varA: dist})
      | ChangeB(dist) => ReasonReact.Update({...state, varB: dist})
      | ChangeC(dist) => ReasonReact.Update({...state, varC: dist})
      },

    render: self => {
      let divideByDistribution =
        switch (self.state.varA, self.state.varB, self.state.varC) {
        | (Some(a), Some(b), Some(c)) => FC__Types.Dist.divideBy([|a, b, c|])
        | _ => None
        };

      let minX =
        [|
          self.state.varA,
          self.state.varB,
          self.state.varC,
          divideByDistribution,
        |]
        |> E.A.O.concatSome
        |> FC__Types.Dists.minX(0.01);
      let maxX =
        [|
          self.state.varA,
          self.state.varB,
          self.state.varC,
          divideByDistribution,
        |]
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
             self.send(ChangeA(FC__Types.Dist.requireLength({ys, xs})))
             Js.log2(xs, ys)}
            |> ignore
          }
        />
        {self.state.varA
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
             self.send(ChangeB(FC__Types.Dist.requireLength({ys, xs})))}
            |> ignore
          }
        />
        {self.state.varB
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
             self.send(ChangeC(FC__Types.Dist.requireLength({ys, xs})))}
            |> ignore
          }
        />
        {self.state.varC
         |> E.O.React.fmapOrNull(v =>
              <FC__CdfChart__Large minX maxX cdf=v width=None />
            )}
        <h3> {"(A + B) * C" |> ReasonReact.string} </h3>
        {switch (divideByDistribution) {
         | None => ReasonReact.null
         | Some(divideBy) =>
           <FC__CdfChart__Large minX maxX cdf=divideBy width=None />
         }}
        {switch (divideByDistribution) {
         | None => ReasonReact.null
         | Some(divideBy) =>
           Js.log(divideBy);
           divideBy
           |> FC__Types.Dist.toPdf
           |> FC__Types.Dist.integral
           |> E.Float.toString
           |> ReasonReact.string;
         }}
      </div>;
    },
  };
};

let entry = EntryTypes.(entry(~title="Scoring", ~render=() => <Scoring />));