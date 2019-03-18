open Rationale.Option.Infix;

let ste = ReasonReact.string;

module CreateMeasurement = [%graphql
  {|
            mutation createMeasurement($value: SequelizeJSON!, $competitorType:competitorType!, $measurableId:String!, $agentId:String!) {
                createMeasurement(value: $value, competitorType: $competitorType, measurableId:$measurableId, agentId:$agentId) {
                  createdAt
                }
            }
    |}
];

module CreateMeasurementMutation =
  ReasonApollo.CreateMutation(CreateMeasurement);

module SignUpParams = {
  type state = {
    p25: string,
    p50: string,
    p75: string,
    pointFloat: string,
    percentage: string,
    binary: string,
    dataType: string,
  };
  type fields = [
    | `p25
    | `p50
    | `p75
    | `pointFloat
    | `percentage
    | `binary
    | `dataType
  ];
  let lens = [
    (`p25, s => s.p25, (s, p25) => {...s, p25}),
    (`p50, s => s.p50, (s, p50) => {...s, p50}),
    (`p75, s => s.p75, (s, p75) => {...s, p75}),
    (`pointFloat, s => s.pointFloat, (s, pointFloat) => {...s, pointFloat}),
    (`percentage, s => s.percentage, (s, percentage) => {...s, percentage}),
    (`binary, s => s.binary, (s, binary) => {...s, binary}),
    (`dataType, s => s.dataType, (s, dataType) => {...s, dataType}),
  ];
};

let initialState: SignUpParams.state = {
  dataType: "FloatCdf",
  p25: "",
  p50: "",
  p75: "",
  pointFloat: "",
  percentage: "",
  binary: "",
};

module SignUpForm = ReForm.Create(SignUpParams);

type action =
  | UpdateFloatPdf(E.FloatCdf.t);

type state = {floatCdf: E.FloatCdf.t};

let keepIfValid = n => Value.isValid(n) ? Some(n) : None;
let mutate =
    (
      mutation: CreateMeasurementMutation.apolloMutation,
      measurableId: string,
      values: SignUpForm.values,
      state: state,
    ) => {
  let value =
    switch (values.dataType) {
    | "FloatCdf" =>
      Some(
        `FloatCdf(
          Value.FloatCdf.fromArrays(state.floatCdf |> (e => (e.ys, e.xs))),
        ),
      )
      >>= keepIfValid
    | "FloatPoint" =>
      values.pointFloat
      |> E.S.safe_float
      <$> (n => `FloatPoint(n))
      >>= keepIfValid
    | "Percentage" =>
      values.percentage
      |> E.S.safe_float
      <$> (n => `Percentage(n))
      >>= keepIfValid
    | "Binary" =>
      values.binary |> E.S.safe_int <$> (n => `Binary(n)) >>= keepIfValid
    | _ => None
    };
  switch (value) {
  | Some(v) =>
    let m =
      CreateMeasurement.make(
        ~measurableId,
        ~agentId="c4aefed8-83c1-422d-9364-313071287758",
        ~value=v |> Value.encode,
        ~competitorType=`COMPETITIVE,
        (),
      );
    mutation(
      ~variables=m##variables,
      ~refetchQueries=[|"getMeasurable"|],
      (),
    )
    |> ignore;
  | None => ()
  };
};

let valueList =
  E.A.fmapi(
    (i, e) =>
      <Antd.Select.Option value=e key={string_of_int(i)}>
        {e |> ste}
      </Antd.Select.Option>,
    [|"FloatPercentiles", "FloatPoint", "Percentage", "Binary"|],
  );

let errorOfFloat = (fn1, fn2, e) => {
  let asNumber = fn1(e);
  switch (asNumber) {
  | None => Some("Couldn't parse Number")
  | Some(r) => Value.error(fn2(r))
  };
};

let input = (handleChange, value, b, key) =>
  <Form.Item key>
    <Antd.Input
      value
      onChange={ReForm.Helpers.handleDomFormChange(handleChange(b))}
    />
  </Form.Item>;

let component = ReasonReact.reducerComponent("Measurables");

let make = (~measurableId: string, _children) => {
  ...component,
  initialState: () => {floatCdf: E.FloatCdf.empty},
  reducer: (action, _) =>
    switch (action) {
    | UpdateFloatPdf(e) => ReasonReact.Update({floatCdf: e})
    },
  render: ({state, _}) =>
    CreateMeasurementMutation.make(
      ~onCompleted=_ => Js.log("Request submitted"),
      (mutation, _) =>
        SignUpForm.make(
          ~onSubmit=
            ({values}) => mutate(mutation, measurableId, values, state),
          ~initialState,
          ~schema=[],
          ({handleSubmit, handleChange, form, _}) =>
            <form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
              <h2> {"Create a new Measurement" |> ste} </h2>
              /* <CdfInput
                   onUpdate=(e => send(UpdateFloatPdf(e)) |> ignore)
                   onSubmit=(_ => handleSubmit())
                 /> */
              <Antd.Form.Item key="ee">
                <Antd.Select
                  value={form.values.dataType}
                  onChange={e => handleChange(`dataType, e) |> ignore}>
                  valueList
                </Antd.Select>
              </Antd.Form.Item>
              {
                switch (form.values.dataType) {
                | "FloatPercentiles" =>
                  <>
                    {input(handleChange, form.values.p25, `p25, "1")}
                    {input(handleChange, form.values.p50, `p50, "2")}
                    {input(handleChange, form.values.p75, `p75, "3")}
                  </>
                | "FloatPoint" =>
                  input(
                    handleChange,
                    form.values.pointFloat,
                    `pointFloat,
                    "4",
                  )
                | "Percentage" =>
                  input(
                    handleChange,
                    form.values.percentage,
                    `percentage,
                    "5",
                  )
                | "Binary" =>
                  input(handleChange, form.values.binary, `binary, "6")
                | _ => ReasonReact.null
                }
              }
              <Form.Item>
                <Antd.Button _type=`primary onClick={_ => handleSubmit()}>
                  {"Submit" |> ste}
                </Antd.Button>
              </Form.Item>
            </form>,
        )
        |> E.React.el,
    )
    |> E.React.el,
};