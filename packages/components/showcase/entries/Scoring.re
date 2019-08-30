open FC.Base;

module Scoring = {
  type dist = FC__Types.Dist.t;

  type state = {
    varA: dist,
    varB: dist,
    varC: dist,
  };

  type action =
    | ChangeA(dist)
    | ChangeB(dist)
    | ChangeC(dist);

  let component = ReasonReact.reducerComponent("Example");

  let make = _children => {
    /* spread the other default fields of component here and override a few */
    ...component,

    initialState: () => {
      varA: {
        xs: [|1.0|],
        ys: [|1.0|],
      },
      varB: {
        xs: [|1.|],
        ys: [|1.|],
      },
      varC: {
        xs: [|1.|],
        ys: [|1.|],
      },
    },
    /* State transitions */
    reducer: (action, state) =>
      switch (action) {
      | ChangeA(dist) => ReasonReact.Update({...state, varA: dist})
      | ChangeB(dist) => ReasonReact.Update({...state, varB: dist})
      | ChangeC(dist) => ReasonReact.Update({...state, varC: dist})
      },

    render: self => {
      Js.log(self.state.varA);
      <div>
        <h3> {"Variable A" |> ReasonReact.string} </h3>
        <FC_GuesstimateInput
          focusOnRender=true
          initialValue={Some("10 to 20")}
          sampleCount=10000
          onUpdate={event =>
            {let (ys, xs, hasLimitError) = event
             self.send(ChangeA({ys, xs}))
             Js.log2(xs, ys)}
            |> ignore
          }
        />
        {self.state.varA.xs |> E.A.length > 0
           ? <FC__CdfChart__Large cdf={self.state.varA} width=None />
           : "" |> ReasonReact.string}
        <h3> {"Variable B" |> ReasonReact.string} </h3>
        <FC_GuesstimateInput
          focusOnRender=true
          sampleCount=10000
          initialValue={Some("20 to 40")}
          onUpdate={event =>
            {let (ys, xs, hasLimitError) = event
             Js.log2(xs, ys)
             self.send(ChangeB({ys, xs}))}
            |> ignore
          }
        />
        {self.state.varB.xs |> E.A.length > 0
           ? <FC__CdfChart__Large cdf={self.state.varB} width=None />
           : "" |> ReasonReact.string}
        <h3> {"Variable C" |> ReasonReact.string} </h3>
        <FC_GuesstimateInput
          focusOnRender=true
          sampleCount=10000
          initialValue={Some("30 to 60")}
          onUpdate={event =>
            {let (ys, xs, hasLimitError) = event
             self.send(ChangeC({ys, xs}))
             Js.log2(xs, ys)}
            |> ignore
          }
        />
        {self.state.varC.xs |> E.A.length > 0
           ? <FC__CdfChart__Large cdf={self.state.varC} width=None />
           : "" |> ReasonReact.string}
        <h3> {"Mean of 3" |> ReasonReact.string} </h3>
        {switch (
           self.state.varA.xs |> E.A.length,
           self.state.varB.xs |> E.A.length,
           self.state.varC.xs |> E.A.length,
         ) {
         | (0, _, _) => "" |> ReasonReact.string
         | (_, 0, _) => "" |> ReasonReact.string
         | (_, _, 0) => "" |> ReasonReact.string
         | _ =>
           let mean =
             FC__Types.Dist.mean([|
               self.state.varA,
               self.state.varB,
               self.state.varC,
             |]);
           mean.xs |> E.A.length > 0
             ? <FC__CdfChart__Large cdf=mean width=None />
             : "" |> ReasonReact.string;
         }}
        <h3> {"(A + B) * C" |> ReasonReact.string} </h3>
        {switch (
           self.state.varA.xs |> E.A.length,
           self.state.varB.xs |> E.A.length,
           self.state.varC.xs |> E.A.length,
         ) {
         | (0, _, _) => "" |> ReasonReact.string
         | (_, 0, _) => "" |> ReasonReact.string
         | (_, _, 0) => "" |> ReasonReact.string
         | _ =>
           let mean =
             FC__Types.Dist.divideBy([|
               self.state.varA,
               self.state.varB,
               self.state.varC,
             |]);
           mean.xs |> E.A.length > 0
             ? <FC__CdfChart__Large cdf=mean width=None />
             : "" |> ReasonReact.string;
         }}
      </div>;
    },
  };
};

let entry = EntryTypes.(entry(~title="Scoring", ~render=() => <Scoring />));