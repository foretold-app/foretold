[@bs.config {jsx: 3}];
open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    competitorType: Types.competitorType,
    picture: string,
  }
];

module Form = ReForm.Make(FormConfig);

let withForm = (onSubmit, bot: option(Types.bot)) => {
  let initialState: FormConfig.state =
    switch (bot) {
    | Some(bot) => {
        name: bot.name |> Rationale.Option.default(""),
        description: bot.description |> Rationale.Option.default(""),
        competitorType: `COMPETITIVE,
        picture: bot.picture |> Rationale.Option.default(""),
      }
    | None => {
        name: "",
        description: "",
        competitorType: `COMPETITIVE,
        picture: "",
      }
    };

  Form.use(
    ~validationStrategy=OnDemand,
    ~initialState,
    ~onSubmit,
    ~schema=
      Form.Validation.Schema([|
        //        Custom(
        //          Name,
        //          values =>
        //            values.name == "" ? ReForm.Error("Can't be empty") : ReForm.Valid,
        //        ),
      |]),
    (),
  );
};

let onSuccess = (loggedUser: Types.user, ()) => {
  Primary.User.getAgent(loggedUser, agent =>
    Routing.Url.push(Agent({agentId: agent.id, subPage: AgentBots}))
  );
  <Null />;
};

let formFields = (reform: Form.api) =>
  <FC__PageCard.BodyPadding>
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
          <Antd.Form.Item label={"Description" |> Utils.ste}>
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
        field=FormConfig.Picture
        render={({handleChange, error, value}) =>
          <Antd.Form.Item label={"Picture" |> Utils.ste}>
            <Antd.Input
              value
              onChange={event =>
                ReactEvent.Form.target(event)##value |> handleChange
              }
            />
          </Antd.Form.Item>
        }
      />
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
    </Antd.Form>
  </FC__PageCard.BodyPadding>;