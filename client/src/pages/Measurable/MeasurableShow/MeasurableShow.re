open Utils;
open Rationale;
open QueriesHelper;
open MomentRe;
open Style.Grid;

let toMoment = Function.Infix.(jsonToString ||> moment);

let toOptionalMoment: option(Js.Json.t) => MomentRe.Moment.t =
  Option.Infix.(
    e => e <$> jsonToString <$> (r => r) |> Option.default("") |> moment
  );

let component = ReasonReact.statelessComponent("Measurable");

let valueString = e =>
  switch (e) {
  | `FLOAT => "Float"
  | `DATE => "Date"
  | `PERCENTAGE => "Percentage"
  };

module Styles = {
  open Css;
  let sidebar =
    style([Css.float(`left), left(px(0)), backgroundColor(hex("eee"))]);

  let body = style([marginLeft(px(200)), padding(px(30))]);

  let header =
    style([
      backgroundColor(hex("f5f7f9")),
      padding2(~v=`px(10), ~h=`px(13)),
      border(`px(1), `solid, `hex("e8f2f9")),
      borderRadius(`px(3)),
      marginBottom(`px(10)),
    ]);
};

let make = (~id: string, _children) => {
  ...component,
  render: _self =>
    <div>
      <div>
        {
          Queries.GetMeasurableWithMeasurements.withQuery(
            ~id,
            measurable => {
              let m =
                Queries.GetMeasurableWithMeasurements.queryMeasurable(
                  measurable,
                );
              <div>
                <Div styles=[Style.Grid.Styles.flexColumn, Styles.header]>
                  <Div styles=[Style.Grid.Styles.flex(1)]>
                    <Div styles=[Style.Grid.Styles.flexRow]>
                      <Div styles=[Style.Grid.Styles.flex(6)]>
                        <h1> {m.name |> ste} </h1>
                        {MeasurableTableStyles.description(~m)}
                      </Div>
                      <Div styles=[Style.Grid.Styles.flex(1)]>
                        {
                          MeasurableTableStyles.dateStatusWrapper(
                            ~measurable=m,
                          )
                        }
                      </Div>
                    </Div>
                  </Div>
                  <Div styles=[Style.Grid.Styles.flex(1)]>
                    {MeasurableTableStyles.creatorLink(~m)}
                    {MeasurableTableStyles.resolutionEndpoint(~m)}
                  </Div>
                </Div>
                <div>
                  {
                    SharedQueries.withLoggedInUserQuery(userQuery =>
                      switch (userQuery) {
                      | Some(query) =>
                        open Rationale.Option.Infix;
                        let userAgentId = query##user >>= (e => e##agentId);
                        let creatorId = measurable##creatorId;
                        <div>
                          <h2> {"Add a Measurement" |> ste} </h2>
                          <MeasurableShowForm
                            measurableId=id
                            isCreator={userAgentId == creatorId}
                          />
                        </div>;
                      | _ => <div />
                      }
                    )
                  }
                  <h2> {"Measurements" |> ste} </h2>
                  <Measurable__Table measurements=measurable##measurements />
                </div>
              </div>;
            },
          )
        }
      </div>
    </div>,
};