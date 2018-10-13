open Utils;
open Rationale;
open Table;
open Rationale.Function.Infix;
open HandsOnTable;
open MomentRe;

let matchId = [%re "/clickFn:id:(.*)/"];

let jsonToString = e => e |> Js.Json.decodeString |> Option.default("");
let optionalMoment = Option.Infix.(e => e <$> (jsonToString ||> moment));

module WithEditMutation = {
  module GraphQL = [%graphql
    {|
             mutation editMeasurable($id: String!, $name: String!, $description: String!, $isLocked: Boolean!, $expectedResolutionDate:Date) {
                 editMeasurable(id: $id, name: $name, description: $description, isLocked: $isLocked, expectedResolutionDate: $expectedResolutionDate) {
                   id
                 }
             }
     |}
  ];

  module Mutation = ReasonApollo.CreateMutation(GraphQL);

  let mutate =
      (
        mutation: Mutation.apolloMutation,
        id: string,
        name: string,
        description: string,
        isLocked: bool,
        expectedResolutionDate: Js.Json.t,
      ) => {
    let m =
      GraphQL.make(
        ~id,
        ~name,
        ~description,
        ~isLocked,
        ~expectedResolutionDate,
        (),
      );
    mutation(~variables=m##variables, ~refetchQueries=[|"getAgent"|], ())
    |> ignore;
  };
};

let notFound = <h3> ("Agent not found" |> ste) </h3>;

module WithAgent = {
  module GraphQL = [%graphql
    {|
    query getAgent ($id: String!) {
        agent:
        agent(id: $id) {
            id
            measurables: Measurables{
                id
                name
                description
                isLocked
                expectedResolutionDate @bsDecoder(fn: "optionalMoment")
            }
        }
    }
  |}
  ];

  module Query = ReasonApollo.CreateQuery(GraphQL);

  let query = (~id, innerFn) => {
    open Result.Infix;
    let query = GraphQL.make(~id, ());
    Query.make(~variables=query##variables, ({result}) =>
      result
      |> apolloResponseToResult
      <$> (e => e##agent)
      >>= (
        e =>
          switch (e) {
          | Some(a) => Ok(a)
          | None => Error(notFound)
          }
      )
      <$> innerFn
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element;
  };
};

let logThis = (ref, _) => Js.log(ref);

type resultData = array(array(string));

type item = {
  name: string,
  description: string,
  isLocked: bool,
  expectedResolutionDate: string,
};

let toItem = (r: array(string)) : item => {
  name: r[0],
  description: r[1],
  isLocked: r[2] == "true" ? true : false,
  expectedResolutionDate: r[3],
};

type reactRef = ref(option(ReasonReact.reactRef));
type state = {hotTableRef: reactRef};

type action =
  | Save;

let component = ReasonReact.reducerComponent("MeMeasurables");

let setSectionRef = (theRef, {ReasonReact.state}) =>
  state.hotTableRef := Js.Nullable.toOption(theRef);

let toItems = (hotTableRef: reactRef) : array(item) =>
  switch (hotTableRef^) {
  | None => [||]
  | Some(r) =>
    let data: resultData = ReasonReact.refToJsObj(r)##hotInstance##getData();
    Js.log2("HIHI", data);
    Array.map(toItem, data);
  };

let make = (~id: string, _children) => {
  ...component,
  reducer: (action: action, state: state) =>
    switch (action) {
    | Save =>
      Js.log(toItems(state.hotTableRef));
      ReasonReact.NoUpdate;
    },
  initialState: () => {hotTableRef: ref(None)},
  shouldUpdate: a => false,
  render: self =>
    WithEditMutation.Mutation.make((mutation, _) =>
      WithAgent.query(
        ~id,
        agent => {
          let m = agent##measurables |> ArrayOptional.concatSomes;
          <StopComponentUpdate>
            (
              Table.ColumnBundle.toHOT(
                ~onClickData=
                  e => {
                    let id = resolveRegex(matchId, e);
                    let index =
                      m
                      |> Array.to_list
                      |> Rationale.RList.findIndex(r => r##id == id)
                      |> Option.default(0);

                    let update = toItems(self.state.hotTableRef)[index];
                    WithEditMutation.mutate(
                      mutation,
                      id,
                      update.name,
                      update.description,
                      update.isLocked,
                      update.expectedResolutionDate |> Js.Json.string,
                    );
                    Js.log2("Submitted new data!", update);
                    ();
                  },
                ~data=m,
                ~transformations=[
                  ColumnBundle.make(
                    ~headerName="Name",
                    ~get=e => e##name,
                    ~column=makeColumn(~name=_, ~readOnly=false, ()),
                    (),
                  ),
                  ColumnBundle.make(
                    ~headerName="Description",
                    ~get=e => e##description |> Option.default(""),
                    ~column=makeColumn(~name=_, ~readOnly=false, ()),
                    (),
                  ),
                  ColumnBundle.make(
                    ~headerName="Is Locked",
                    ~get=e => e##isLocked ? "True" : "False",
                    ~column=
                      makeColumn(
                        ~name=_,
                        ~renderer="checkbox",
                        ~readOnly=false,
                        (),
                      ),
                    (),
                  ),
                  ColumnBundle.make(
                    ~headerName="Resolution Date",
                    ~get=
                      Option.Infix.(
                        e =>
                          e##expectedResolutionDate
                          <$> Moment.format("L")
                          |> Option.default("")
                      ),
                    ~column=makeColumn(~name=_, ~readOnly=false, ()),
                    (),
                  ),
                  ColumnBundle.make(
                    ~headerName="Cdf",
                    ~column=Columns.html,
                    ~get=
                      e =>
                        ReactDOMServerRe.renderToStaticMarkup(
                          <a data=("clickFn:id:" ++ e##id) href="/">
                            ("Submit" |> ste)
                          </a>,
                        ),
                    (),
                  ),
                ],
                ~ref=self.handle(setSectionRef),
                (),
              )
            )
          </StopComponentUpdate>;
        },
      )
    )
    |> ReasonReact.element,
};