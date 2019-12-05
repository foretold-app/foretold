[@bs.config {jsx: 3}];

open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelCreate.Query.t;
  });

[@react.component]
let make = () => {
  let form = (mutation, innerComponentFn) =>
    ChannelForm.Form.use(
      ~onSubmit=
        values => {
          ChannelCreate.mutate(
            mutation,
            values.state.values.name,
            values.state.values.description,
            values.state.values.isPublic == "TRUE" ? true : false,
            values.state.values.isArchived == "TRUE" ? true : false,
          );
          None;
        },
      ~initialState={
        name: "",
        description: "",
        isPublic: "TRUE",
        isArchived: "FALSE",
      },
      ~schema=ChannelForm.Form.Validation.Schema([||]),
      (),
    )
    |> innerComponentFn;

  let body =
    <FC.PageCard.BodyPadding>
      <ChannelCreate.Mutation>
        ...{(mutation, data) =>
          form(
            mutation, ({submit, state, handleChange}: ChannelForm.Form.api) =>
            CMutationForm.showWithLoading2(
              ~result=data.result,
              ~form=
                ChannelForm.showForm(
                  ~state,
                  ~handleChange,
                  ~onSubmit=() => submit(),
                  (),
                ),
              ~onSuccess=
                (response: ChannelCreate.Query.t) => {
                  switch (response##channelCreate) {
                  | Some(channel) =>
                    Routing.Url.push(ChannelShow(channel##id))
                  | _ => ()
                  };
                  <Null />;
                },
              (),
            )
          )
        }
      </ChannelCreate.Mutation>
    </FC.PageCard.BodyPadding>;

  <SLayout head={SLayout.Header.textDiv("Create a New Community")}>
    body
  </SLayout>;
};