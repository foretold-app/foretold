open Utils;
open Rationale;
open Result.Infix;
let component = ReasonReact.statelessComponent("Measurables");
let make = (~channel: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  render: _self =>
    (
      measurables =>
        <div>
          {SLayout.channelHeader(Some(channel))}
          <div className=SLayout.Styles.mainSection>
            <MeasurableIndex__Table
              measurables
              loggedInUser
              showExtraData=true
            />
          </div>
        </div>
    )
    |> GetMeasurables.component(channel),
};