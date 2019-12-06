[@bs.config {jsx: 3}];

open Antd;
open Rationale.Function.Infix;

open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    subjects: list(string),
    properties: list(string),
    dates: list(MomentRe.Moment.t),
  }
];

module Form = ReForm.Make(FormConfig);

module CMutationForm =
  MutationForm.Make({
    type queryType = SeriesCreate.Query.t;
  });

let formatDate = E.M.format(E.M.format_standard);

let processArray =
  E.L.filter(r => r != "") ||> E.L.toArray ||> E.A.fmap(E.O.some);

let withForm = (mutation, channelId) =>
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
  );

let formFields = (reform: Form.api) => {
  <Antd.Form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    }}>
    <h3>
      {"Warning: You can not edit a Series after created it at this time."
       |> Utils.ste}
    </h3>
    <Form.Field
      field=FormConfig.Name
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Name" |> Utils.ste}>
          <Input
            value
            onChange={event =>
              ReactEvent.Form.target(event)##value |> handleChange
            }
          />
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.Description
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Description" |> Utils.ste}>
          <Input
            value
            onChange={event =>
              ReactEvent.Form.target(event)##value |> handleChange
            }
          />
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.Subjects
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Subjects" |> Utils.ste}>
          {value
           |> E.L.fmapi((i, r) =>
                <Input
                  value=r
                  onChange={e =>
                    handleChange(
                      value
                      |> E.L.update(ReactEvent.Form.target(e)##value, i),
                    )
                  }
                />
              )
           |> E.L.toArray
           |> ReasonReact.array}
          <Button
            onClick={_ => value |> Rationale.RList.append("") |> handleChange}>
            {"Add Subject" |> Utils.ste}
          </Button>
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.Properties
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Properties" |> Utils.ste}>
          {value
           |> E.L.fmapi((i, r) =>
                <Input
                  value=r
                  onChange={e =>
                    value
                    |> E.L.update(ReactEvent.Form.target(e)##value, i)
                    |> handleChange
                  }
                />
              )
           |> E.L.toArray
           |> ReasonReact.array}
          <Button
            onClick={_ => handleChange(value |> Rationale.RList.append(""))}>
            {"Add Property" |> Utils.ste}
          </Button>
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.Dates
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Dates" |> Utils.ste}>
          {value
           |> E.L.fmapi((i, r) =>
                <DatePicker
                  value=r
                  onChange={e => {
                    value |> E.L.update(e, i) |> handleChange;

                    _ => ();
                  }}
                />
              )
           |> E.L.toArray
           |> ReasonReact.array}
          <Button
            onClick={_ =>
              value
              |> Rationale.RList.append(MomentRe.momentNow())
              |> handleChange
            }>
            {"Add Date" |> Utils.ste}
          </Button>
        </Antd.Form.Item>
      }
    />
    <Antd.Form.Item>
      <Button
        _type=`primary
        onClick={event => {
          ReactEvent.Synthetic.preventDefault(event);
          reform.submit();
        }}>
        {"Submit" |> Utils.ste}
      </Button>
    </Antd.Form.Item>
  </Antd.Form>;
};

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Make a New Series")}>
    <FC.PageCard.BodyPadding>
      {SeriesCreate.withMutation((mutation, data) => {
         let reform = withForm(mutation, channelId);

         <Form.Provider value=reform>
           {CMutationForm.showWithLoading(
              ~result=data.result,
              ~form=formFields(reform),
              (),
            )}
         </Form.Provider>;
       })}
    </FC.PageCard.BodyPadding>
  </SLayout>;
};