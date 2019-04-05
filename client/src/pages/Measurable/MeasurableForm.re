open Utils;
open MomentRe;
open Antd;

let formatDate = E.M.format(E.M.format_standard);
module SignUpParams = {
  type state = {
    name: string,
    labelCustom: string,
    labelSubject: string,
    labelOnDate: string,
    labelProperty: string,
    expectedResolutionDate: string,
    resolutionEndpoint: string,
    showDescriptionDate: string,
    showDescriptionProperty: string,
  };
  type fields = [
    | `name
    | `labelCustom
    | `labelSubject
    | `labelProperty
    | `labelOnDate
    | `expectedResolutionDate
    | `resolutionEndpoint
    | `showDescriptionDate
    | `showDescriptionProperty
  ];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `labelCustom,
      s => s.labelCustom,
      (s, labelCustom) => {...s, labelCustom},
    ),
    (
      `labelSubject,
      s => s.labelSubject,
      (s, labelSubject) => {...s, labelSubject},
    ),
    (
      `labelProperty,
      s => s.labelProperty,
      (s, labelProperty) => {...s, labelProperty},
    ),
    (
      `showDescriptionDate,
      s => s.showDescriptionDate,
      (s, showDescriptionDate) => {...s, showDescriptionDate},
    ),
    (
      `showDescriptionProperty,
      s => s.showDescriptionProperty,
      (s, showDescriptionProperty) => {...s, showDescriptionProperty},
    ),
    (
      `labelOnDate,
      s => s.labelOnDate,
      (s, labelOnDate) => {...s, labelOnDate},
    ),
    (
      `expectedResolutionDate,
      s => s.expectedResolutionDate,
      (s, expectedResolutionDate) => {...s, expectedResolutionDate},
    ),
    (
      `resolutionEndpoint,
      s => s.resolutionEndpoint,
      (s, resolutionEndpoint) => {...s, resolutionEndpoint},
    ),
  ];
};

module SignUpForm = ReForm.Create(SignUpParams);

let showForm = (~form: SignUpForm.state, ~handleSubmit, ~handleChange) =>
  <form onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <Form>
      <Form.Item>
        {"Relevant Entity (optional)" |> ste |> E.React.inH3}
        <Input
          value={form.values.labelSubject}
          onChange={
            ReForm.Helpers.handleDomFormChange(handleChange(`labelSubject))
          }
        />
      </Form.Item>
      <Form.Item>
        {"Property Type" |> ste |> E.React.inH3 |> E.React.inH3}
        <Antd.Radio.Group
          value={form.values.showDescriptionProperty}
          defaultValue={form.values.showDescriptionProperty}
          onChange={
            ReForm.Helpers.handleDomFormChange(
              handleChange(`showDescriptionProperty),
            )
          }>
          <Antd.Radio value="FALSE"> {"Custom Name" |> ste} </Antd.Radio>
          <Antd.Radio value="TRUE"> {"Property Entity" |> ste} </Antd.Radio>
        </Antd.Radio.Group>
      </Form.Item>
      {
        form.values.showDescriptionProperty == "TRUE" ?
          <Form.Item>
            {"Property Entity Name" |> ste |> E.React.inH3}
            <Input
              value={form.values.labelProperty}
              onChange={
                ReForm.Helpers.handleDomFormChange(
                  handleChange(`labelProperty),
                )
              }
            />
          </Form.Item> :
          <div />
      }
      {
        form.values.showDescriptionProperty == "FALSE" ?
          <Form.Item>
            {"Custom Name" |> ste}
            <Input
              value={form.values.name}
              onChange={
                ReForm.Helpers.handleDomFormChange(handleChange(`name))
              }
            />
          </Form.Item> :
          <div />
      }
      <Form.Item>
        {"Include a Specific Date in Name" |> ste |> E.React.inH3}
        <AntdSwitch
          checked={form.values.showDescriptionDate == "TRUE"}
          onChange={
            e => handleChange(`showDescriptionDate, e ? "TRUE" : "FALSE")
          }
        />
      </Form.Item>
      {
        form.values.showDescriptionDate == "TRUE" ?
          <Form.Item>
            {"'On' Date" |> ste |> E.React.inH3}
            <DatePicker
              value={form.values.labelOnDate |> MomentRe.moment}
              onChange={e => handleChange(`labelOnDate, e |> formatDate)}
            />
          </Form.Item> :
          <div />
      }
      <Form.Item>
        {"Description" |> ste |> E.React.inH3}
        <Input
          value={form.values.labelCustom}
          onChange={
            ReForm.Helpers.handleDomFormChange(handleChange(`labelCustom))
          }
        />
      </Form.Item>
      <Form.Item>
        {"Resolution Endpoint (Optional)" |> ste |> E.React.inH3}
        {
          "If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value."
          |> ste
          |> E.React.inP
        }
        <Input
          value={form.values.resolutionEndpoint}
          onChange={
            ReForm.Helpers.handleDomFormChange(
              handleChange(`resolutionEndpoint),
            )
          }
        />
      </Form.Item>
      <Form.Item>
        {"Expected Resolution Date" |> ste |> E.React.inH3}
        <DatePicker
          value={
            form.values.expectedResolutionDate |> MomentRe.momentDefaultFormat
          }
          onChange={
            e => handleChange(`expectedResolutionDate, e |> formatDate)
          }
        />
      </Form.Item>
      <Form.Item>
        <Button _type=`primary onClick={_ => handleSubmit()}>
          {"Submit" |> ste}
        </Button>
      </Form.Item>
    </Form>
  </form>;