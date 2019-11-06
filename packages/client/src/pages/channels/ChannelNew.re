[@bs.config {jsx: 3}];

open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelCreate.Query.t;
  });

[@react.component]
let make = () => {
  let form = mutation =>
    ChannelForm.Form.make(
      ~onSubmit=
        values =>
          ChannelCreate.mutate(
            mutation,
            values.state.values.name,
            Some(values.state.values.description),
            values.state.values.isPublic == "TRUE" ? true : false,
            values.state.values.isArchived == "TRUE" ? true : false,
          ),
      ~initialState={
        name: "",
        description: "",
        isPublic: "TRUE",
        isArchived: "FALSE",
      },
      ~schema=ChannelForm.Form.Validation.Schema([||]),
    )
    ||> E.React2.el;

  let body =
    <FC.PageCard.BodyPadding>
      <ChannelCreate.Mutation>
        ...{(mutation, data) =>
          form(mutation, ({send, state}) =>
            CMutationForm.showWithLoading2(
              ~result=data.result,
              ~form=
                ChannelForm.showForm(
                  ~state,
                  ~send,
                  ~onSubmit=() => send(ChannelForm.Form.Submit),
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