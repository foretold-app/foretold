open Utils;
open Antd;
open Rationale.Function.Infix;

module FormConfig = {
  type field(_) =
    | Name: field(string)
    | Description: field(string)
    | Subjects: field(list(string))
    | Properties: field(list(string))
    | Dates: field(list(MomentRe.Moment.t));
  type state = {
    name: string,
    description: string,
    subjects: list(string),
    properties: list(string),
    dates: list(MomentRe.Moment.t),
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      | Description => state.description
      | Subjects => state.subjects
      | Properties => state.properties
      | Dates => state.dates
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      | Description => {...state, description: value}
      | Subjects => {...state, subjects: value}
      | Properties => {...state, properties: value}
      | Dates => {...state, dates: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let component = ReasonReact.statelessComponent("Measurables");

let withUserQuery =
    (auth0Id, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = UserGet.Query.make(~auth0Id, ());
  UserGet.QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(innerComponentFn)
    |> E.R.id
  )
  |> E.React.el;
};

module CMutationForm =
  MutationForm.Make({
    type queryType = SeriesCreate.Query.t;
  });

let formatDate = E.M.format(E.M.format_standard);

let processArray =
  E.L.filter(r => r != "") ||> E.L.toArray ||> E.A.fmap(E.O.some);

let withForm = (mutation, channelId, innerComponentFn) =>
  Form.make(
    ~initialState={
      description: "",
      name: "",
      subjects: [""],
      properties: [""],
      dates: [MomentRe.momentNow()],
    },
    ~onSubmit=
      values => {
        let mutate =
          SeriesCreate.mutate(
            mutation,
            values.state.values.name,
            values.state.values.description,
            channelId,
            values.state.values.subjects |> processArray,
            values.state.values.properties |> processArray,
            values.state.values.dates
            |> E.L.toArray
            |> E.A.fmap(formatDate ||> Js.Json.string ||> E.O.some),
          );
        ();
      },
    ~schema=Form.Validation.Schema([||]),
    innerComponentFn,
  )
  |> E.React.el;

let formFields = (form: Form.state, send, onSubmit) =>
  <Antd.Form onSubmit={e => onSubmit()}>
    <h3>
      {"Warning: You can not edit a Series after created it at this time."
       |> Utils.ste}
    </h3>
    <Antd.Form.Item>
      {"Name" |> Utils.ste |> E.React.inH3}
      <Input
        value={form.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Name, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Description" |> Utils.ste |> E.React.inH3}
      <Input
        value={form.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          send(Form.FieldChangeValue(Description, e))
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Subjects" |> Utils.ste |> E.React.inH3}
      {form.values.subjects
       |> E.L.fmapi((i, r) =>
            <Input
              value=r
              onChange={ReForm.Helpers.handleDomFormChange(e =>
                send(
                  Form.FieldChangeValue(
                    Subjects,
                    form.values.subjects |> E.L.update(e, i),
                  ),
                )
              )}
            />
          )
       |> E.L.toArray
       |> ReasonReact.array}
      <Button
        onClick={_ =>
          send(
            Form.FieldChangeValue(
              Subjects,
              form.values.subjects |> Rationale.RList.append(""),
            ),
          )
        }>
        {"Add Subject" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Properties" |> Utils.ste |> E.React.inH3}
      {form.values.properties
       |> E.L.fmapi((i, r) =>
            <Input
              value=r
              onChange={ReForm.Helpers.handleDomFormChange(e =>
                send(
                  Form.FieldChangeValue(
                    Properties,
                    form.values.properties |> E.L.update(e, i),
                  ),
                )
              )}
            />
          )
       |> E.L.toArray
       |> ReasonReact.array}
      <Button
        onClick={_ =>
          send(
            Form.FieldChangeValue(
              Properties,
              form.values.properties |> Rationale.RList.append(""),
            ),
          )
        }>
        {"Add Property" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
    <Antd.Form.Item>
      {"Dates" |> Utils.ste |> E.React.inH3}
      {form.values.dates
       |> E.L.fmapi((i, r) =>
            <DatePicker
              value=r
              onChange={e =>
                send(
                  Form.FieldChangeValue(
                    Dates,
                    form.values.dates |> E.L.update(e, i),
                  ),
                )
              }
            />
          )
       |> E.L.toArray
       |> ReasonReact.array}
      <Button
        onClick={_ =>
          send(
            Form.FieldChangeValue(
              Dates,
              form.values.dates
              |> Rationale.RList.append(MomentRe.momentNow()),
            ),
          )
        }>
        {"Add Date" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
    <Antd.Form.Item>
      <Button _type=`primary onClick={_ => onSubmit()}>
        {"Submit" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;

let make =
    (
      ~channelId: string,
      ~loggedInUser: Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ =>
    SLayout.LayoutConfig.make(
      ~head=SLayout.Header.textDiv("Make a New Series"),
      ~body=
        <FC.PageCard.BodyPadding>
          {SeriesCreate.withMutation((mutation, data) =>
             withForm(mutation, channelId, ({send, state}) =>
               CMutationForm.showWithLoading(
                 ~result=data.result,
                 ~form=formFields(state, send, () => send(Form.Submit)),
                 (),
               )
             )
           )}
        </FC.PageCard.BodyPadding>,
    )
    |> layout,
};