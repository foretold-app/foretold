[@bs.config {jsx: 3}];

open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    email: string,
    picture: string,
    description: string,
  }
];

module Form = ReForm.Make(FormConfig);

let testName = (str: string) => {
  let exp = () => [%re "/^[a-z0-9._]{0,30}$/i"];
  let res = exp() |> Js.Re.exec(str);
  switch (res) {
  | Some(_) => true
  | _ => false
  };
};

let schema =
  Form.Validation.Schema([|
    Custom(
      Name,
      values =>
        testName(values.name) ? Valid : ReSchema.Error(Lang.wrongName),
    ),
    Email(Email),
  |]);

module FormComponent = {
  [@react.component]
  let make =
      (
        ~reform: Form.api,
        ~result: ReasonApolloHooks.Mutation.controledVariantResult('a),
      ) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <p> {"Something went wrong..." |> Utils.ste} </p>
       | Data(_) => <p> {"Profile is updated." |> Utils.ste} </p>
       | _ =>
         <Antd.Form onSubmit>
           <Form.Field
             field=FormConfig.Name
             render={({handleChange, error, value, validate}) =>
               <Antd.Form.Item label={"Username" |> Utils.ste}>
                 <Antd.Input
                   value
                   onBlur={_ => validate()}
                   onChange={event =>
                     ReactEvent.Form.target(event)##value |> handleChange
                   }
                 />
                 {error->Belt.Option.getWithDefault("")->React.string}
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Description
             render={({handleChange, error, value, validate}) =>
               <Antd.Form.Item label={"Description" |> Utils.ste}>
                 <Antd.Input
                   value
                   onBlur={_ => validate()}
                   onChange={event =>
                     ReactEvent.Form.target(event)##value |> handleChange
                   }
                 />
                 {error->Belt.Option.getWithDefault("")->React.string}
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Email
             render={({handleChange, error, value, validate}) =>
               <Antd.Form.Item label={"Email" |> Utils.ste}>
                 <Antd.Input
                   value
                   onBlur={_ => validate()}
                   onChange={event =>
                     ReactEvent.Form.target(event)##value |> handleChange
                   }
                 />
                 {error->Belt.Option.getWithDefault("")->React.string}
               </Antd.Form.Item>
             }
           />
           <Form.Field
             field=FormConfig.Picture
             render={({handleChange, error, value, validate}) =>
               <Antd.Form.Item label={"Picture URL" |> Utils.ste}>
                 <Antd.Input
                   value
                   onBlur={_ => validate()}
                   onChange={event =>
                     ReactEvent.Form.target(event)##value |> handleChange
                   }
                 />
                 {error->Belt.Option.getWithDefault("")->React.string}
               </Antd.Form.Item>
             }
           />
           <Antd.Form.Item>
             {reform.state.formState == Submitting
                ? <Spin />
                : <Antd.Button _type=`primary onClick=onSubmit>
                    {"Submit" |> Utils.ste}
                  </Antd.Button>}
           </Antd.Form.Item>
         </Antd.Form>
       }}
    </Form.Provider>;
  };
};

module Edit = {
  [@react.component]
  let make = (~id, ~email, ~picture, ~description, ~name) => {
    let (mutate, result, _) = UserUpdate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            let name' = state.values.name;

            let email' =
              state.values.email === "" ? None : Some(state.values.email);

            let picture' =
              state.values.picture === "" ? None : Some(state.values.picture);

            let description' =
              state.values.picture === ""
                ? None : Some(state.values.description);

            mutate(
              ~variables=
                UserUpdate.Query.make(
                  ~id,
                  ~input={
                    "name": Some(name'),
                    "email": email',
                    "picture": picture',
                    "description": description',
                    "auth0AccessToken": None,
                  },
                  (),
                )##variables,
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={name, email, picture, description},
        (),
      );

    <FormComponent reform result />;
  };
};

[@react.component]
let make = (~loggedUser: Types.user) => {
  let agent = loggedUser.agent;
  let id = loggedUser.id;
  let email = loggedUser.email |> E.O.default("");
  let picture = loggedUser.picture |> E.O.default("");
  let description = loggedUser.description |> E.O.default("");
  let name =
    agent |> E.O.bind(_, (r: Types.agent) => r.name) |> E.O.default("");

  <SLayout head={SLayout.Header.textDiv("Edit Profile Information")}>
    <FC.PageCard.BodyPadding>
      <Edit id email picture description name />
    </FC.PageCard.BodyPadding>
  </SLayout>;
};