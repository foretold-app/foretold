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

module GetMeasurable = {
  module Query = [%graphql
    {|
      query getMeasurable ($id: String!) {
          measurable:
            measurable(id: $id){
              id
              name
              description
              valueType
              isLocked
              isArchived
              creatorId
              resolutionEndpoint
              expectedResolutionDate @bsDecoder(fn: "optionalMoment")
              createdAt @bsDecoder(fn: "toMoment")
              updatedAt @bsDecoder(fn: "toMoment")
              lockedAt @bsDecoder(fn: "optionalMoment")
              creator {
                id
                name
              }
              measurements: Measurements{
                id
                createdAt @bsDecoder(fn: "toMoment")
                value @bsDecoder(fn: "Value.decode")
                competitorType
                description
                taggedMeasurementId
                relevantAt @bsDecoder(fn: "toOptionalMoment")
                agent: Agent {
                  id
                  name
                  user: User {
                    id
                    name
                  }
                  bot: Bot {
                    id
                    name
                    competitorType
                  }
                }
              }
          }
      }
    |}
  ];

  module QueryComponent = ReasonApollo.CreateQuery(Query);

  let withQuery = (~id, fn) => {
    let query = Query.make(~id, ());
    Result.Infix.(
      QueryComponent.make(~variables=query##variables, ({result}) =>
        result
        |> apolloResponseToResult
        >>= (
          e =>
            e##measurable
            |> filterOptionalResult("Measurable not found" |> ste)
        )
        <$> fn
        |> Result.result(idd, idd)
      )
      |> ReasonReact.element
    );
  };
};

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

let queryMeasurable = m => {
  open DataModel;
  let creator: option(DataModel.creator) =
    m##creator |> Option.fmap(r => {id: r##id, name: r##name});

  let measurable: DataModel.measurable = {
    id: m##id,
    name: m##name,
    isLocked: m##isLocked,
    isArchived: m##isArchived,
    valueType: m##valueType,
    description: m##description,
    resolutionEndpoint: m##resolutionEndpoint,
    measurementCount: None,
    measurerCount: None,
    createdAt: m##createdAt,
    updatedAt: m##updatedAt,
    expectedResolutionDate: m##expectedResolutionDate,
    archivedAt: None,
    lockedAt: m##lockedAt,
    creator,
  };
  measurable;
};

let make = (~id: string, _children) => {
  ...component,
  render: _self =>
    <div>
      <div>
        {
          GetMeasurable.withQuery(
            ~id,
            measurable => {
              let m = queryMeasurable(measurable);
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
                    {MeasurableTableStyles.expectedResolutionDate(~m)}
                    {MeasurableTableStyles.isLocked(~m)}
                    {MeasurableTableStyles.resolutionEndpoint(~m)}
                  </Div>
                </Div>
                <div>
                  <h2> {"Aggregate" |> ste} </h2>
                  <Style.BorderedBox>
                    <MeasurableChart measurements=measurable##measurements />
                  </Style.BorderedBox>
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
                  <h2> {"Previous Measurements" |> ste} </h2>
                </div>
                <MeasurableTable measurements=measurable##measurements />
              </div>;
            },
          )
        }
      </div>
    </div>,
};