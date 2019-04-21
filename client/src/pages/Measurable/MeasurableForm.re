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
  <AntdForm onSubmit={ReForm.Helpers.handleDomFormSubmit(handleSubmit)}>
    <Form.Item label="Question Type">
      <Antd.Radio.Group
        value={form.values.showDescriptionProperty}
        defaultValue={form.values.showDescriptionProperty}
        onChange={
          ReForm.Helpers.handleDomFormChange(
            handleChange(`showDescriptionProperty),
          )
        }>
        <Antd.Radio value="FALSE"> {"Simple" |> ste} </Antd.Radio>
        <Antd.Radio value="TRUE">
          {"Subject-Property-Date" |> ste}
        </Antd.Radio>
      </Antd.Radio.Group>
    </Form.Item>
    {
      E.React.showIf(
        form.values.showDescriptionProperty == "TRUE",
        <>
          <Form.Item label="Subject">
            <Input
              value={form.values.labelSubject}
              onChange={
                ReForm.Helpers.handleDomFormChange(
                  handleChange(`labelSubject),
                )
              }
            />
          </Form.Item>
          <Form.Item label="Property">
            <Input
              value={form.values.labelProperty}
              onChange={
                ReForm.Helpers.handleDomFormChange(
                  handleChange(`labelProperty),
                )
              }
            />
          </Form.Item>
          <Form.Item label="Include a Specific Date in Name">
            <AntdSwitch
              checked={form.values.showDescriptionDate == "TRUE"}
              onChange={
                e => handleChange(`showDescriptionDate, e ? "TRUE" : "FALSE")
              }
            />
          </Form.Item>
          {
            form.values.showDescriptionDate == "TRUE" ?
              <Form.Item label="'On' Date">
                <DatePicker
                  value={form.values.labelOnDate |> MomentRe.moment}
                  onChange={
                    e => {
                      handleChange(`labelOnDate, e |> formatDate);
                      handleChange(`expectedResolutionDate, e |> formatDate);
                    }
                  }
                />
              </Form.Item> :
              <div />
          }
        </>,
      )
    }
    {
      E.React.showIf(
        form.values.showDescriptionProperty == "FALSE",
        <Form.Item label="Name">
          <Input
            value={form.values.name}
            onChange={
              ReForm.Helpers.handleDomFormChange(handleChange(`name))
            }
          />
        </Form.Item>,
      )
    }
    <Form.Item label="Description">
      <Input
        value={form.values.labelCustom}
        onChange={
          ReForm.Helpers.handleDomFormChange(handleChange(`labelCustom))
        }
      />
    </Form.Item>
    <Form.Item
      label="Resolution Endpoint"
      help="If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value.">
      <Input
        value={form.values.resolutionEndpoint}
        onChange={
          ReForm.Helpers.handleDomFormChange(
            handleChange(`resolutionEndpoint),
          )
        }
      />
    </Form.Item>
    <Form.Item label="Expected Resolution Date">
      <DatePicker
        value={
          form.values.expectedResolutionDate |> MomentRe.momentDefaultFormat
        }
        onChange={e => handleChange(`expectedResolutionDate, e |> formatDate)}
        disabled={form.values.showDescriptionDate == "TRUE"}
      />
    </Form.Item>
    <Form.Item>
      <Button _type=`primary onClick={_ => handleSubmit()}>
        {"Submit" |> ste}
      </Button>
    </Form.Item>
  </AntdForm>;