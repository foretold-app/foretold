[@bs.config {jsx: 3}];

open MomentRe;
open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    isPublic: string,
    isArchived: string,
  }
];

module Form = ReForm.Make(FormConfig);

let showForm = (creating, reform: Form.api) => {
  <Antd.Form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    }}>
    <Form.Field
      field=FormConfig.Name
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Name" |> Utils.ste}>
          <Antd.Input
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
        <Antd.Form.Item
          label={"Description" |> Utils.ste}
          help={"Markdown supported" |> Utils.ste}>
          <Antd.Input.TextArea
            style={ReactDOMRe.Style.make(~minHeight="30em", ())}
            value
            onChange={event =>
              ReactEvent.Form.target(event)##value |> handleChange
            }
          />
        </Antd.Form.Item>
      }
    />
    <Form.Field
      field=FormConfig.IsPublic
      render={({handleChange, error, value}) =>
        <Antd.Form.Item label={"Community is public" |> Utils.ste}>
          <AntdSwitch
            checked={value == "TRUE"}
            onChange={e => handleChange(e ? "TRUE" : "FALSE")}
          />
        </Antd.Form.Item>
      }
    />
    {E.React2.showIf(
       !creating,
       <Form.Field
         field=FormConfig.IsArchived
         render={({handleChange, error, value}) =>
           <Antd.Form.Item label={"Archive community" |> Utils.ste}>
             <AntdSwitch
               checked={value == "TRUE"}
               onChange={e => handleChange(e ? "TRUE" : "FALSE")}
             />
           </Antd.Form.Item>
         }
       />,
     )}
    <Antd.Form.Item>
      <Antd.Button
        _type=`primary
        onClick={event => {
          ReactEvent.Synthetic.preventDefault(event);
          reform.submit();
        }}>
        {"Submit" |> Utils.ste}
      </Antd.Button>
    </Antd.Form.Item>
  </Antd.Form>;
};