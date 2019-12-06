[@bs.config {jsx: 3}];

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

module CMutationForm =
  MutationForm.Make({
    type queryType = SeriesCreate.Query.t;
  });

let formatDate = E.M.format(E.M.format_standard);

let processArray =
  E.L.filter(r => r != "") ||> E.L.toArray ||> E.A.fmap(E.O.some);

let withForm = (mutation, channelId, innerComponentFn) =>
  Form.use(
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
        None;
      },
    ~schema=Form.Validation.Schema([||]),
    (),
  )
  |> innerComponentFn;

// @todo:
let formFields =
    (
      form: Form.state,
      handleChange: (FormConfig.field('a), 'a) => unit,
      handleChange2: (FormConfig.field(list('b)), list('b)) => unit,
      handleChange3: (FormConfig.field(list('c)), list('c)) => unit,
      onSubmit,
    ) => {
  <Antd.Form onSubmit={e => onSubmit()}>
    <h3>
      {"Warning: You can not edit a Series after created it at this time."
       |> Utils.ste}
    </h3>
    <Antd.Form.Item label={"Name" |> Utils.ste}>
      <Input
        value={form.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          handleChange(FormConfig.Name, e)
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item label={"Description" |> Utils.ste}>
      <Input
        value={form.values.description}
        onChange={ReForm.Helpers.handleDomFormChange(e =>
          handleChange(Description, e)
        )}
      />
    </Antd.Form.Item>
    <Antd.Form.Item label={"Subjects" |> Utils.ste}>
      {form.values.subjects
       |> E.L.fmapi((i, r) =>
            <Input
              value=r
              onChange={ReForm.Helpers.handleDomFormChange(e =>
                handleChange2(
                  FormConfig.Subjects,
                  form.values.subjects |> E.L.update(e, i),
                )
              )}
            />
          )
       |> E.L.toArray
       |> ReasonReact.array}
      <Button
        onClick={_ =>
          handleChange2(
            FormConfig.Subjects,
            form.values.subjects |> Rationale.RList.append(""),
          )
        }>
        {"Add Subject" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
    <Antd.Form.Item label={"Properties" |> Utils.ste}>
      {form.values.properties
       |> E.L.fmapi((i, r) =>
            <Input
              value=r
              onChange={ReForm.Helpers.handleDomFormChange(e =>
                handleChange2(
                  Properties,
                  form.values.properties |> E.L.update(e, i),
                )
              )}
            />
          )
       |> E.L.toArray
       |> ReasonReact.array}
      <Button
        onClick={_ =>
          handleChange2(
            Properties,
            form.values.properties |> Rationale.RList.append(""),
          )
        }>
        {"Add Property" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
    <Antd.Form.Item label={"Dates" |> Utils.ste}>
      {form.values.dates
       |> E.L.fmapi((i, r) =>
            <DatePicker
              value=r
              onChange={e => {
                handleChange3(
                  FormConfig.Dates,
                  form.values.dates |> E.L.update(e, i),
                );

                _ => ();
              }}
            />
          )
       |> E.L.toArray
       |> ReasonReact.array}
      <Button
        onClick={_ =>
          handleChange3(
            Dates,
            form.values.dates |> Rationale.RList.append(MomentRe.momentNow()),
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
};

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Series")}>
    <FC.PageCard.BodyPadding>
      {SeriesCreate.withMutation((mutation, data) =>
         withForm(
           mutation, channelId, ({handleChange, submit, state}: Form.api) =>
           CMutationForm.showWithLoading(
             ~result=data.result,
             ~form=
               formFields(state, handleChange, handleChange, handleChange, () =>
                 submit()
               ),
             (),
           )
         )
       )}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};