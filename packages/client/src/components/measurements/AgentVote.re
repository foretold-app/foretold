module MeasurementVotes = {
  type direction =
    | Down
    | Up;

  module Styles = {
    open Css;
    let vote = style([marginLeft(`px(5)), marginRight(`px(5))]);
  };

  [@react.component]
  let make = (~initialVoteCount, ~onUpdate) => {
    let (count, setCount) = React.useState(() => initialVoteCount);
    let canIncrement = count < 10;
    let canDecrement = count > (-10);

    let reducer = (d: direction) => {
      switch (d) {
      | Down when canDecrement => count - 1
      | Up when canIncrement => count + 1
      | _ => count
      };
    };

    let modify = (d: direction) =>
      setCount(_ => {
        let newCount = reducer(d);
        onUpdate(newCount);
        newCount;
      });

    <>
      <Antd.Button onClick={_ => modify(Down)} disabled={!canDecrement}>
        {"<" |> Utils.ste}
      </Antd.Button>
      <span className=Styles.vote> {string_of_int(count) |> Utils.ste} </span>
      <Antd.Button onClick={_ => modify(Up)} disabled={!canIncrement}>
        {">" |> Utils.ste}
      </Antd.Button>
    </>;
  };
};

// TODO: I think this would be cleaner if this module were a wrapper, similar to the <Mutation> used inside.
// I wasn't sure how to do this though.
module WithMutation = {
  [@react.component]
  let make = (~measurement: Types.measurement, ~initialVoteCount) => {
    <MeasurementVote.Mutation>
      ...{(mutation, result: MeasurementVote.Mutation.renderPropObj) => {
        let send =
          Debouncer.make(
            ~wait=200,
            MeasurementVote.mutate(mutation, measurement.id),
          );

        <MeasurementVotes initialVoteCount onUpdate=send />;
      }}
    </MeasurementVote.Mutation>;
  };
};

[@react.component]
let make = (~measurement: Types.measurement) => {
  let can =
    Primary.Permissions.can(`MEASUREMENT_VOTE, measurement.permissions);

  let initialVoteCount =
    measurement.vote
    |> E.O.fmap((vote: Types.vote) => vote.voteAmount)
    |> E.O.default(0);

  can ? <WithMutation measurement initialVoteCount /> : <Null />;
};