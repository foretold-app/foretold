open Utils;
open Rationale;
open MomentRe;
open Antd;

let formatDate = Moment.format("MMM DD, YYYY HH:MM:SS");
module SignUpParams = {
  type state = {
    name: string,
    description: string,
    descriptionEntity: string,
    descriptionDate: string,
    descriptionProperty: string,
    expectedResolutionDate: string,
    resolutionEndpoint: string,
    showDescriptionDate: string,
    showDescriptionProperty: string,
  };
  type fields = [
    | `name
    | `description
    | `descriptionEntity
    | `descriptionProperty
    | `descriptionDate
    | `expectedResolutionDate
    | `resolutionEndpoint
    | `showDescriptionDate
    | `showDescriptionProperty
  ];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
    (
      `descriptionEntity,
      s => s.descriptionEntity,
      (s, descriptionEntity) => {...s, descriptionEntity},
    ),
    (
      `descriptionProperty,
      s => s.descriptionProperty,
      (s, descriptionProperty) => {...s, descriptionProperty},
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
      `descriptionDate,
      s => s.descriptionDate,
      (s, descriptionDate) => {...s, descriptionDate},
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
    <h2> {"Create a new Measurable" |> ste} </h2>
    <Form>
      <h3> {"Relevant Entity (optional)" |> ste} </h3>
      <Input
        value={form.values.descriptionEntity}
        onChange={
          ReForm.Helpers.handleDomFormChange(
            handleChange(`descriptionEntity),
          )
        }
      />
      <h3> {"Property Type" |> ste} </h3>
    </Form>
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
    {
      form.values.showDescriptionProperty == "TRUE" ?
        <div>
          <h3> {"Property Entity Name" |> ste} </h3>
          <Input
            value={form.values.descriptionProperty}
            onChange={
              ReForm.Helpers.handleDomFormChange(
                handleChange(`descriptionProperty),
              )
            }
          />
        </div> :
        <div />
    }
    {
      form.values.showDescriptionProperty == "FALSE" ?
        <div>
          <h3> {"Custom Name" |> ste} </h3>
          <Input
            value={form.values.name}
            onChange={
              ReForm.Helpers.handleDomFormChange(handleChange(`name))
            }
          />
        </div> :
        <div />
    }
    <div>
      <h3> {"Include a Specific Date in Name" |> ste} </h3>
      <AntdSwitch
        checked={form.values.showDescriptionDate == "TRUE"}
        onChange={
          e => handleChange(`showDescriptionDate, e ? "TRUE" : "FALSE")
        }
      />
    </div>
    {
      form.values.showDescriptionDate == "TRUE" ?
        <div>
          <h3> {"'On' Date" |> ste} </h3>
          <DatePicker
            value={form.values.descriptionDate |> MomentRe.momentDefaultFormat}
            onChange={e => handleChange(`descriptionDate, e |> formatDate)}
          />
        </div> :
        <div />
    }
    <div>
      <h3> {"Description" |> ste} </h3>
      <Input
        value={form.values.description}
        onChange={
          ReForm.Helpers.handleDomFormChange(handleChange(`description))
        }
      />
    </div>
    <div>
      <h3> {"Resolution Endpoint (Optional)" |> ste} </h3>
      <p>
        {
          "If you enter an url that returns a number, this will be called when the resolution date occurs, and entered as a judgement value."
          |> ste
        }
      </p>
      <Input
        value={form.values.resolutionEndpoint}
        onChange={
          ReForm.Helpers.handleDomFormChange(
            handleChange(`resolutionEndpoint),
          )
        }
      />
    </div>
    <div>
      <h3> {"Expected Resolution Date" |> ste} </h3>
      <DatePicker
        value={
          form.values.expectedResolutionDate |> MomentRe.momentDefaultFormat
        }
        onChange={e => handleChange(`expectedResolutionDate, e |> formatDate)}
      />
    </div>
    <div>
      <Button _type=`primary onClick={_ => handleSubmit()}>
        {"Submit" |> ste}
      </Button>
    </div>
  </form>;