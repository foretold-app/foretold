module FormConfig = {
  type field(_) =
    | Name: field(string)
    | Description: field(string)
    | CompetitorType: field(Types.competitorType);

  type state = {
    name: string,
    description: string,
    competitorType: Types.competitorType,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Name => state.name
      | Description => state.description
      | CompetitorType => state.competitorType
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Name => {...state, name: value}
      | Description => {...state, description: value}
      | CompetitorType => {...state, competitorType: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let withForm = (onSubmit, bot: option(Types.bot), innerComponentFn) => {
  let initialState: FormConfig.state =
    switch (bot) {
    | Some(bot) => {
        name: bot.name |> Rationale.Option.default(""),
        description: bot.description |> Rationale.Option.default(""),
        competitorType: `COMPETITIVE,
      }
    | None => {name: "", description: "", competitorType: `COMPETITIVE}
    };

  Form.make(
    ~initialState,
    ~onSubmit,
    ~schema=
      Form.Validation.Schema([|
        Custom(
          Name,
          values => values.name == "" ? Error("Can't be empty") : Valid,
        ),
      |]),
    innerComponentFn,
  )
  |> E.React.el;
};

let onSuccess = (loggedInUser: Types.user, ()) => {
  Primary.User.getAgent(loggedInUser, agent =>
    Routing.Url.push(Agent({agentId: agent.id, subPage: AgentBots}))
  );
  ReasonReact.null;
};

let formFields = (state: Form.state, send, onSubmit) =>
  <Fc.FC__PageCard.BodyPadding>
    <Antd.Form onSubmit={e => onSubmit()}>
      <Antd.Form.Item>
        {"Name" |> ReasonReact.string |> E.React.inH3}
        <Antd.Input
          value={state.values.name}
          onChange={ReForm.Helpers.handleDomFormChange(e =>
            send(Form.FieldChangeValue(Name, e))
          )}
        />
      </Antd.Form.Item>
      <Antd.Form.Item>
        {"Description" |> ReasonReact.string |> E.React.inH3}
        <Antd.Input
          value={state.values.description}
          onChange={ReForm.Helpers.handleDomFormChange(e =>
            send(Form.FieldChangeValue(Description, e))
          )}
        />
      </Antd.Form.Item>
      <Antd.Form.Item>
        <Antd.Button _type=`primary onClick={_ => onSubmit()}>
          {"Submit" |> ReasonReact.string}
        </Antd.Button>
      </Antd.Form.Item>
    </Antd.Form>
  </Fc.FC__PageCard.BodyPadding>;